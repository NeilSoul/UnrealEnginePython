//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyPropertyButton) modifier:(xingtongli)
#include "PyButtonPropertyDetails.h"
#include "SButton.h"
//#include "Slate.h"
#include "IDetailChildrenBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

//#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "PyPropertyButton.h"
#include "UnrealEnginePython.h"

void FPyButtonPropertyDetails::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	ScriptPathHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPyPropertyButton, ScriptPath));
	FunctionHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPyPropertyButton, Function));
	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Run")))
			.OnClicked(this, &FPyButtonPropertyDetails::OnClicked)
		];
}

void FPyButtonPropertyDetails::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	
	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects = ChildBuilder.GetParentCategory().GetParentLayout().GetSelectedObjects();
	SelectObject = SelectedObjects[0];

	//if (SelectedObject)
	//{
	//	//UObject* ButtonHandle = nullptr;
	//	//StructPropertyHandle.Get().GetValue(ButtonHandle);
	//	//UPyPropertyButton* Button = Cast<UPyPropertyButton>(ButtonHandle);
	//	//if (Button)
	//	{
		ChildBuilder.AddProperty(ScriptPathHandle.ToSharedRef());
		ChildBuilder.AddProperty(FunctionHandle.ToSharedRef());
		/*

			ChildBuilder.AddCustomRow(FText::GetEmpty())
				[

				];*/
	//	}
	//	
	//}
}

FReply FPyButtonPropertyDetails::OnClicked()
{
	FString ScriptPath;
	FString FunctionName;
	ScriptPathHandle->GetValue(ScriptPath);
	FunctionHandle->GetValue(FunctionName);
	RunFile(ScriptPath, FunctionName, SelectObject.Get());
	return FReply::Handled();
}

void FPyButtonPropertyDetails::RunFile(const FString& FilePath, const FString& FunctionName, UObject* ParamObj)
{
	FString FullPath = FilePath;
	FString OriPath = FullPath;
	bool bFoundFile = false;
	FUnrealEnginePythonModule &PythonModule = FModuleManager::GetModuleChecked<FUnrealEnginePythonModule>("UnrealEnginePython");

	if (!FilePath.IsEmpty() && !FPaths::FileExists(FilePath))
	{
		for (FString SreachPath : PythonModule.ScriptsPaths)
		{
			FullPath = FPaths::Combine(*SreachPath, OriPath);
			if (FPaths::FileExists(FullPath))
			{
				bFoundFile = true;
				break;
			}
		}
	}
	else
	{
		bFoundFile = true;
	}

	if (!bFoundFile)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find file %s"), *FilePath);
		return;
	}

#if PY_MAJOR_VERSION >= 3
	FILE *FP = nullptr;

#if PLATFORM_WINDOWS
	if (fopen_s(&FP, TCHAR_TO_UTF8(*FullPath), "r") != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to open file %s"), *FullPath);
		return;
	}
#else
	FP = fopen(TCHAR_TO_UTF8(*FullPath), "r");
	if (!FP)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to open file %s"), *FullPath);
		return;
	}
#endif

	FScopePythonGIL GIL;

	PyObject *EvalRet = PyRun_FileExFlags(FP, TCHAR_TO_UTF8(*FullPath), Py_file_input, PythonModule.GetGlobalDict(), (PyObject*)PythonModule.GetGlobalDict(), 0, nullptr);
	fclose(FP);
	if (!EvalRet)
	{
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
		{
			PyErr_Clear();
			return;
		}
		unreal_engine_py_log_error();
		return;
	}
	Py_DECREF(EvalRet);
#else
	// damn, this is horrible, but it is the only way i found to avoid the CRT error :(
	FString command = FString::Printf(TEXT("execfile(\"%s\")"), *FullPath);
	PyObject *EvalRet = PyRun_String(TCHAR_TO_UTF8(*command), Py_file_input, (PyObject *)main_dict, (PyObject *)local_dict);
	if (!EvalRet)
	{
		if (PyErr_ExceptionMatches(PyExc_SystemExit))
		{
			PyErr_Clear();
			return;
		}
		unreal_engine_py_log_error();
		return;
	}
#endif

	if (!FunctionName.IsEmpty())
	{
		PyObject *FuncToCall = PyDict_GetItemString((PyObject*)PythonModule.GetGlobalDict(), TCHAR_TO_UTF8(*FunctionName));
		if (!FuncToCall)
		{
			UE_LOG(LogTemp, Error, TEXT("unable to find function %s"), *FunctionName);
			return;
		}
		PyObject *PyArgs = nullptr;
		if (SelectObject.IsValid())
		{
			PyObject *PyParamObj = (PyObject *)ue_get_python_uobject(ParamObj);
			if (PyParamObj)
			{
				Py_INCREF(PyParamObj);
				PyArgs = PyTuple_New(1);
				PyTuple_SetItem(PyArgs, 0, PyParamObj);
			}
			else
			{
				unreal_engine_py_log_error();
			}
		}

		PyObject *Ret = PyObject_CallObject(FuncToCall, PyArgs);
		Py_DECREF(PyArgs);
		if (!Ret)
		{
			unreal_engine_py_log_error();
		}
		else
		{
			Py_DECREF(Ret);
		}
	}
}
// DK End
//////////////////////////////////////////////////////////////////////////
