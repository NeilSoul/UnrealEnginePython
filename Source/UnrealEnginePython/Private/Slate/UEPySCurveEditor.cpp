#include "UEPySCurveEditor.h"
#include "ScopedTransaction.h"


#if WITH_EDITOR 
namespace
{
#define LOCTEXT_NAMESPACE "SCurveEditor"
	class SPythonCurveEditor : public SCurveEditor
	{
		/**
		 * custom paint: always paint background full window
		 * additional edit capability:
		 *		extend/shrink selected keys time interval
		 */
	public:
		SLATE_BEGIN_ARGS(SPythonCurveEditor)
			: _ViewMinInput(0.0f)
			, _ViewMaxInput(10.0f)
			, _ViewMinOutput(0.0f)
			, _ViewMaxOutput(1.0f)
			, _InputSnap(0.1f)
			, _OutputSnap(0.05f)
			, _InputSnappingEnabled(false)
			, _OutputSnappingEnabled(false)
			, _ShowTimeInFrames(false)
			, _TimelineLength(0.f)
			, _DesiredSize(FVector2D::ZeroVector)
			, _DrawCurve(true)
			, _HideUI(true)
			, _AllowZoomOutput(true)
			, _AlwaysDisplayColorCurves(false)
			, _ZoomToFitVertical(true)
			, _ZoomToFitHorizontal(true)
			, _ShowZoomButtons(true)
			, _XAxisName()
			, _YAxisName()
			, _ShowInputGridNumbers(true)
			, _ShowOutputGridNumbers(true)
			, _ShowCurveSelector(true)
			, _GridColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.3f))
		{
			_Clipping = EWidgetClipping::ClipToBounds;
		}

			SLATE_ATTRIBUTE(float, ViewMinInput)
			SLATE_ATTRIBUTE(float, ViewMaxInput)
			SLATE_ATTRIBUTE(TOptional<float>, DataMinInput)
			SLATE_ATTRIBUTE(TOptional<float>, DataMaxInput)
			SLATE_ATTRIBUTE(float, ViewMinOutput)
			SLATE_ATTRIBUTE(float, ViewMaxOutput)
			SLATE_ATTRIBUTE(float, InputSnap)
			SLATE_ATTRIBUTE(float, OutputSnap)
			SLATE_ATTRIBUTE(bool, InputSnappingEnabled)
			SLATE_ATTRIBUTE(bool, OutputSnappingEnabled)
			SLATE_ATTRIBUTE(bool, ShowTimeInFrames)
			SLATE_ATTRIBUTE(float, TimelineLength)
			SLATE_ATTRIBUTE(FVector2D, DesiredSize)
			SLATE_ATTRIBUTE(bool, AreCurvesVisible)
			SLATE_ARGUMENT(bool, DrawCurve)
			SLATE_ARGUMENT(bool, HideUI)
			SLATE_ARGUMENT(bool, AllowZoomOutput)
			SLATE_ARGUMENT(bool, AlwaysDisplayColorCurves)
			SLATE_ARGUMENT(bool, ZoomToFitVertical)
			SLATE_ARGUMENT(bool, ZoomToFitHorizontal)
			SLATE_ARGUMENT(bool, ShowZoomButtons)
			SLATE_ARGUMENT(TOptional<FString>, XAxisName)
			SLATE_ARGUMENT(TOptional<FString>, YAxisName)
			SLATE_ARGUMENT(bool, ShowInputGridNumbers)
			SLATE_ARGUMENT(bool, ShowOutputGridNumbers)
			SLATE_ARGUMENT(bool, ShowCurveSelector)
			SLATE_ARGUMENT(FLinearColor, GridColor)
			SLATE_EVENT(FOnSetInputViewRange, OnSetInputViewRange)
			SLATE_EVENT(FOnSetOutputViewRange, OnSetOutputViewRange)
			SLATE_EVENT(FOnSetAreCurvesVisible, OnSetAreCurvesVisible)
			SLATE_EVENT(FSimpleDelegate, OnCreateAsset)
		SLATE_END_ARGS()

	public:

		enum class EDuplicateKeysMode : uint8
		{
			SameValue = 0,
			SameDeltaValue = 1,
			ModeNum,
		};

	public:
		/** construct the widget */
		void Construct(const FArguments& inArgs);

		/** duplicate selected keys via specific mode */
		void DuplicateSelectedKeys(EDuplicateKeysMode inMode);

		/** scale selected keys via specific value */
		void ScaleSelectedKeys(float inTimeScale, float inValueScale = 1.0f);

	protected:
		/** override paint */
		int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

		/** get the sections of selected key, each element of the result at least have two element */
		TArray<TArray<FSelectedCurveKey>> GetSelectedKeysSections() const;

		/** move all keys whose time greater than a specific time, according to a given time and value offset */
		void SetCurveKeysOffset(FRealCurve* inCurve, float inStartTime, float inTimeOffset, float inValueOffset = 0.0f);

		/** scale the time and range in a specific time range */
		void ScaleSelectedKeysSection(const TArray<FSelectedCurveKey>& inSection, float inTimeScale, float inValueScale);

		/** do the real work */
		void ScaleSelectedKeysSectionImpl(const TArray<FSelectedCurveKey>& inSection, const TArray<TPair<float, float>>& inTimeValueArray, float inTimeScale, float inValueScale);

	protected:

		/** get the sorted keys of a specific selected curve keys array */
		static TArray<TPair<float, float>> GetSortedSelectedKeysTimeValue(const TArray<FSelectedCurveKey>& inKeys);

		/** get the selected key information, if the queried key is not valid, return the fallback */
		static float GetSelectedCurveKeyTime(FSelectedCurveKey inKey, float inFallback = 0.0f);
		static float GetSelectedCurveKeyValue(FSelectedCurveKey inKey, float inFallback = 0.0f);
		static TPair<float, float> GetSelectedCurveKeyTimeValue(FSelectedCurveKey inKey, TPair<float, float> inFallback = TPair<float, float>{ 0.0f, 0.0f });

		/** set the selected key attribute, return if this key is valid */
		static bool SetSelectedCurveKeyTime(FSelectedCurveKey inKey, float inTime);
		static bool SetSelectedCurveKeyValue(FSelectedCurveKey inKey, float inValue);
		static bool SetSelectedCurveKeyTimeValue(FSelectedCurveKey inKey, float inTime, float inValue);
	};

	FORCEINLINE float SPythonCurveEditor::GetSelectedCurveKeyTime(FSelectedCurveKey inKey, float inFallback /* = 0.0f */)
	{
		if (!inKey.IsValid())
		{
			return inFallback;
		}
		return inKey.Curve->GetKeyTime(inKey.KeyHandle);
	}

	FORCEINLINE float SPythonCurveEditor::GetSelectedCurveKeyValue(FSelectedCurveKey inKey, float inFallback /* = 0.0f */)
	{
		if (!inKey.IsValid())
		{
			return inFallback;
		}

		return inKey.Curve->GetKeyValue(inKey.KeyHandle);
	}

	FORCEINLINE TPair<float, float> SPythonCurveEditor::GetSelectedCurveKeyTimeValue(FSelectedCurveKey inKey, TPair<float, float> inFallback)
	{
		if (!inKey.IsValid())
		{
			return inFallback;
		}

		return inKey.Curve->GetKeyTimeValuePair(inKey.KeyHandle);
	}

	FORCEINLINE bool SPythonCurveEditor::SetSelectedCurveKeyTime(FSelectedCurveKey inKey, float inTime)
	{
		if (!inKey.IsValid())
		{
			return false;
		}
		inKey.Curve->SetKeyTime(inKey.KeyHandle, inTime);
		return true;
	}

	FORCEINLINE bool SPythonCurveEditor::SetSelectedCurveKeyValue(FSelectedCurveKey inKey, float inValue)
	{
		if (!inKey.IsValid())
		{
			return false;
		}
		inKey.Curve->SetKeyValue(inKey.KeyHandle, inValue);
		return true;
	}

	FORCEINLINE bool SPythonCurveEditor::SetSelectedCurveKeyTimeValue(FSelectedCurveKey inKey, float inTime, float inValue)
	{
		if (!inKey.IsValid())
		{
			return false;
		}
		inKey.Curve->SetKeyTime(inKey.KeyHandle, inTime);
		inKey.Curve->SetKeyValue(inKey.KeyHandle, inValue);
		return true;
	}


	void SPythonCurveEditor::Construct(const FArguments& inArgs)
	{
		SCurveEditor::Construct(
			SCurveEditor::FArguments()
			.ViewMinInput(inArgs._ViewMinInput)
			.ViewMaxInput(inArgs._ViewMaxInput)
			.ViewMinOutput(inArgs._ViewMinOutput)
			.ViewMaxOutput(inArgs._ViewMaxInput)
			.InputSnap(inArgs._InputSnap)
			.OutputSnap(inArgs._OutputSnap)
			.InputSnappingEnabled(inArgs._InputSnappingEnabled)
			.OutputSnappingEnabled(inArgs._OutputSnappingEnabled)
			.ShowTimeInFrames(inArgs._ShowTimeInFrames)
			.TimelineLength(0)
			.DesiredSize(inArgs._DesiredSize)
			.AreCurvesVisible(inArgs._AreCurvesVisible)
			.DrawCurve(inArgs._DrawCurve)
			.HideUI(inArgs._HideUI)
			.AllowZoomOutput(inArgs._AllowZoomOutput)
			.AlwaysDisplayColorCurves(inArgs._AlwaysDisplayColorCurves)
			.ZoomToFitVertical(inArgs._ZoomToFitVertical)
			.ZoomToFitHorizontal(inArgs._ZoomToFitHorizontal)
			.ShowZoomButtons(inArgs._ShowZoomButtons)
			.XAxisName(inArgs._XAxisName)
			.YAxisName(inArgs._YAxisName)
			.ShowInputGridNumbers(inArgs._ShowInputGridNumbers)
			.ShowOutputGridNumbers(inArgs._ShowOutputGridNumbers)
			.ShowCurveSelector(inArgs._ShowCurveSelector)
			.GridColor(inArgs._GridColor)
			.OnSetInputViewRange(inArgs._OnSetInputViewRange)
			.OnSetOutputViewRange(inArgs._OnSetOutputViewRange)
			.OnSetAreCurvesVisible(inArgs._OnSetAreCurvesVisible)
			.OnCreateAsset(inArgs._OnCreateAsset)
		);
	}

	int32 SPythonCurveEditor::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
	{
		// Rendering info
		bool bEnabled = ShouldBeEnabled(bParentEnabled);
		ESlateDrawEffect DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
		const FSlateBrush* TimelineAreaBrush = FEditorStyle::GetBrush("CurveEd.TimelineArea");
	
		FGeometry CurveAreaGeometry = AllottedGeometry;

		// Positioning info
		FTrackScaleInfo ScaleInfo(ViewMinInput.Get(), ViewMaxInput.Get(), ViewMinOutput.Get(), ViewMaxOutput.Get(), CurveAreaGeometry.GetLocalSize());

		if (FMath::IsNearlyEqual(ViewMinInput.Get(), ViewMaxInput.Get()) || FMath::IsNearlyEqual(ViewMinOutput.Get(), ViewMaxOutput.Get()))
		{
			return 0;
		}

		// timeline background
		int32 BackgroundLayerId = LayerId;
		float TimelineMaxX = ScaleInfo.InputToLocalX(TimelineLength.Get());
		FSlateDrawElement::MakeBox
		(
			OutDrawElements,
			BackgroundLayerId,
			CurveAreaGeometry.ToPaintGeometry(FVector2D(0, 0), CurveAreaGeometry.GetLocalSize()),
			TimelineAreaBrush,
			DrawEffects,
			TimelineAreaBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
		);

		return SCurveEditor::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}

	void SPythonCurveEditor::DuplicateSelectedKeys(EDuplicateKeysMode inMode)
	{
		if (!AreKeysSelected())
		{
			return;
		}

		auto SelectedKeysSections = GetSelectedKeysSections();
		if (SelectedKeysSections.Num() == 0)
		{
			return;
		}

		const FScopedTransaction Transition{ LOCTEXT("CurveEditor_Duplicate", "Duplicate Keys") };
		CurveOwner->ModifyOwner();
		TMap<FRealCurve*, TArray<TPair<float, float>>> KeysToInsert;
		TArray<FRichCurveEditInfo> ChangedCurveEditInfos;
		for (auto& Section : SelectedKeysSections)
		{
			/** record the keys to insert map */
			auto TimeValueArray = GetSortedSelectedKeysTimeValue(Section);
			auto& InsertKeys = KeysToInsert.FindOrAdd(Section[0].Curve);
			for (int32 i = 1, len = TimeValueArray.Num(); i != len; i++)
			{//	we don't insert the first selected element
				InsertKeys.Add(TimeValueArray[i]);
			}
			ChangedCurveEditInfos.Add(GetViewModelForCurve(Section[0].Curve)->CurveInfo);
	
			/** move keys */
			float StartTime = TimeValueArray[0].Key;
			float TimeOffset = TimeValueArray.Last().Key - StartTime;
			
			switch (inMode)
			{
			case EDuplicateKeysMode::SameValue:
				SetCurveKeysOffset(Section[0].Curve, StartTime, TimeOffset);
				break;
			case EDuplicateKeysMode::SameDeltaValue:
			{
				float ValueOffset = TimeValueArray.Last().Value - TimeValueArray[0].Value;
				SetCurveKeysOffset(Section[0].Curve, StartTime, TimeOffset, ValueOffset);
			}
				break;
			default:
				break;
			}
		}

		/** insert needed keys */
		EmptyAllSelection();
		for (auto& CurveInsertKeys : KeysToInsert)
		{
			FRealCurve* Curve = CurveInsertKeys.Key;
			for (const auto& TimeValue : CurveInsertKeys.Value)
			{
				Curve->AddKey(TimeValue.Key, TimeValue.Value);
			}
		}
		

		if (ChangedCurveEditInfos.Num() > 0)
		{
			CurveOwner->OnCurveChanged(ChangedCurveEditInfos);
		}
	}

	void SPythonCurveEditor::ScaleSelectedKeys(float inTimeScale, float inValueScale /* = 1.0f */)
	{
		bool bInvalidTimeScale = inTimeScale <= 0.0f;
		bool bRedundantScale = inTimeScale == 1.0f && inValueScale == 1.0f;
		if (!AreKeysSelected() || bInvalidTimeScale || bRedundantScale)
		{
			return;
		}

		auto SelectedKeysSections = GetSelectedKeysSections();
		if (SelectedKeysSections.Num() == 0)
		{
			return;
		}

		const FScopedTransaction Transition{ LOCTEXT("CurveEditor_Scale", "Scale Keys") };
		CurveOwner->ModifyOwner();
		TArray<FRichCurveEditInfo> ChangedCurveEditInfos;
		for (auto& Section : SelectedKeysSections)
		{
			ChangedCurveEditInfos.Add(GetViewModelForCurve(Section[0].Curve)->CurveInfo);
			ScaleSelectedKeysSection(Section, inTimeScale, inValueScale);
		}

		if (ChangedCurveEditInfos.Num() > 0)
		{
			CurveOwner->OnCurveChanged(ChangedCurveEditInfos);
		}
	}

	auto SPythonCurveEditor::GetSelectedKeysSections() const ->TArray<TArray<FSelectedCurveKey>>
	{
		TArray<TArray<FSelectedCurveKey>> Result;

		TArray<FSelectedCurveKey> Section;
		for (auto CurveModel : CurveViewModels)
		{
			if (!CurveModel.IsValid())
			{
				continue;
			}

			FRealCurve* Curve = CurveModel->CurveInfo.CurveToEdit;
			for (auto KeyIterator = Curve->GetKeyHandleIterator(); KeyIterator; KeyIterator++)
			{
				FSelectedCurveKey Key{Curve, *KeyIterator};
				if (IsKeySelected(Key))
				{
					Section.Add(Key);
				}
				else
				{
					if (Section.Num() > 1)
					{
						Result.Add(MoveTemp(Section));
					}
					Section.Empty();
				}
			}
			if (Section.Num() > 1)
			{
				Result.Add(MoveTemp(Section));
			}
			Section.Empty();
		}

		return Result;
	}

	void SPythonCurveEditor::SetCurveKeysOffset(FRealCurve* inCurve, float inStartTime, float inTimeOffset, float inValueOffset /* = 0 */)
	{
		if (inCurve == nullptr)
		{
			return;
		}
		if (inTimeOffset == 0.0f && inValueOffset == 0.0f)
		{
			return;
		}

		if (inTimeOffset > 0)
		{
			// to avoid move a key to an exist place, we move from backward
			auto Key = inCurve->GetLastKeyHandle();
			while (inCurve->IsKeyHandleValid(Key) && inCurve->GetKeyTime(Key) > inStartTime)
			{
				auto OldTimeValue = inCurve->GetKeyTimeValuePair(Key);
				inCurve->SetKeyTime(Key, OldTimeValue.Key + inTimeOffset);
				inCurve->SetKeyValue(Key, OldTimeValue.Value + inValueOffset);
				Key = inCurve->GetPreviousKey(Key);
			}
		}
		else
		{
			// this branch may failed since the we never know if collision could trigger
			auto Key = inCurve->GetFirstKeyHandle();
			while (inCurve->IsKeyHandleValid(Key) && inCurve->GetKeyTime(Key) <= inStartTime)
			{
				Key = inCurve->GetNextKey(Key);
			}
			while (inCurve->IsKeyHandleValid(Key))
			{
				auto OldTimeValue = inCurve->GetKeyTimeValuePair(Key);
				inCurve->SetKeyTime(Key, OldTimeValue.Key + inTimeOffset);
				inCurve->SetKeyValue(Key, OldTimeValue.Value + inValueOffset);
				Key = inCurve->GetNextKey(Key);
			}
		}
	}

	void SPythonCurveEditor::ScaleSelectedKeysSection(const TArray<FSelectedCurveKey>& inSection, float inTimeScale, float inValueScale)
	{
		check(inTimeScale != 0.0f);
		auto TimeValueArray = GetSortedSelectedKeysTimeValue(inSection);

		float StartTime = TimeValueArray.Last().Key;
		float TimeRange = StartTime - TimeValueArray[0].Key;
		float TimeOffset = TimeRange * inTimeScale - TimeRange;

		if (inTimeScale > 1.0f)
		{
			SetCurveKeysOffset(inSection[0].Curve, StartTime, TimeOffset);
			ScaleSelectedKeysSectionImpl(inSection, TimeValueArray, inTimeScale, inValueScale);
		}
		else
		{
			ScaleSelectedKeysSectionImpl(inSection, TimeValueArray, inTimeScale, inValueScale);
			SetCurveKeysOffset(inSection[0].Curve, StartTime, TimeOffset);
		}
	}

	void SPythonCurveEditor::ScaleSelectedKeysSectionImpl(const TArray<FSelectedCurveKey>& inSection, const TArray<TPair<float, float>>& inTimeValueArray, float inTimeScale, float inValueScale)
	{
		check(inSection.Num() > 1 && inSection.Num() == inTimeValueArray.Num() && inTimeScale != 0.0f);

		// generate the time value offset
		static const auto GenerateTimeValueOffsetImpl = [](const TArray<TPair<float, float>>& inArray, float inTScale, float inVScale, float& outStartTime, float& outStartValue) -> TArray<TPair<float, float>>
		{
			TArray<TPair<float, float>> Result;
			for (int32 i = 0, len = inArray.Num(); i != len; i++)
			{
				if (i > 0)
				{
					float DeltaTime = inArray[i].Key - inArray[i - 1].Key;
					float DeltaValue = inArray[i].Value - inArray[i - 1].Value;
					Result.Emplace(DeltaTime * inTScale + Result[i - 1].Key, DeltaValue * inVScale + Result[i - 1].Value);
				}
				else
				{
					outStartTime = inArray[i].Key;
					outStartValue = inArray[i].Value;
					Result.Emplace(0.0f, 0.0f);
				}
			}
			return Result;
		};

		// function body
		float StartTime;
		float StartValue;
		TArray<TPair<float, float>> TimeValueOffsetPairs = Invoke(GenerateTimeValueOffsetImpl, inTimeValueArray, inTimeScale, inValueScale, StartTime, StartValue);

		if (inTimeScale < 1.0f)
		{	// update forward for shrink
			for (int32 i = 0, len = inSection.Num(); i != len; i++)
			{
				SetSelectedCurveKeyTimeValue(inSection[i], StartTime + TimeValueOffsetPairs[i].Key, StartValue + TimeValueOffsetPairs[i].Value);
			}
		}
		else
		{	// update backward for expand
			for (int32 i = inSection.Num() - 1; i >= 0; i--)
			{
				SetSelectedCurveKeyTimeValue(inSection[i], StartTime + TimeValueOffsetPairs[i].Key, StartValue + TimeValueOffsetPairs[i].Value);
			}
		}
		
	}

	TArray<TPair<float, float>> SPythonCurveEditor::GetSortedSelectedKeysTimeValue(const TArray<FSelectedCurveKey>& inKeys)
	{
		TArray<TPair<float, float>> Result;
		for (auto Key : inKeys)
		{
			Result.Emplace(GetSelectedCurveKeyTimeValue(Key));
		}
		Result.Sort([](TPair<float, float> inLhs, TPair<float, float> inRhs) {return inLhs.Key < inRhs.Key; });
		return Result;
	}

#undef LOCTEXT_NAMESPACE
}


static PyObject* py_ue_scurve_editor_set_curve(ue_PySCurveEditor* self, PyObject* args)
{
	ue_py_slate_cast(SPythonCurveEditor);
	PyObject* py_curve;
	if (!PyArg_ParseTuple(args, "O:set_curve", &py_curve))
	{
		return nullptr;
	}

	UCurveBase* curve = ue_py_check_type<UCurveBase>(py_curve);
	if (!curve)
	{
		return PyErr_Format(PyExc_Exception, "argument is not a UCurveBase");
	}
	curve->SetFlags(RF_Transactional);
	py_SPythonCurveEditor->SetCurveOwner(curve, true);

	Py_RETURN_SLATE_SELF;
}

static PyObject* py_ue_scurve_editor_duplicate_selected_keys(ue_PySCurveEditor* self, PyObject* args)
{
	ue_py_slate_cast(SPythonCurveEditor);
	int py_duplicate_type = 0;
	if (!PyArg_ParseTuple(args, "i:duplicate_selected_keys", &py_duplicate_type))
	{
		return nullptr;
	}

	if (py_duplicate_type >= static_cast<int>(SPythonCurveEditor::EDuplicateKeysMode::ModeNum))
	{
		return PyErr_Format(PyExc_Exception, "argument is not a valid duplicate mode");
	}
	py_SPythonCurveEditor->DuplicateSelectedKeys(static_cast<SPythonCurveEditor::EDuplicateKeysMode>(py_duplicate_type));
	Py_RETURN_SLATE_SELF;
}

static PyObject* py_ue_scurve_editor_scale_selected_keys(ue_PySCurveEditor* self, PyObject* args)
{
	ue_py_slate_cast(SPythonCurveEditor);
	float py_scale = 1.0f;
	if (!PyArg_ParseTuple(args, "f:scale_selected_keys", &py_scale))
	{
		return nullptr;
	}

	py_SPythonCurveEditor->ScaleSelectedKeys(py_scale);

	Py_RETURN_SLATE_SELF;
}

static PyMethodDef ue_PySCurveEditor_methods[] =
{
	{ "set_curve", (PyCFunction)py_ue_scurve_editor_set_curve, METH_VARARGS, ""},
	{ "duplicate_selected_keys", (PyCFunction)py_ue_scurve_editor_duplicate_selected_keys, METH_VARARGS, ""},
	{ "scale_selected_keys", (PyCFunction)py_ue_scurve_editor_scale_selected_keys, METH_VARARGS, ""},
	{NULL}
};

PyTypeObject ue_PySCurveEditorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.SCurveEditor", /* tp_name */
	sizeof(ue_PySCurveEditor), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,       /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
	"Unreal Engine SCurveEditor",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	ue_PySCurveEditor_methods,             /* tp_methods */
};

static int ue_py_scurve_editor_init(ue_PySCurveEditor* self, PyObject* args, PyObject* kwargs)
{
	ue_py_slate_setup_farguments(SPythonCurveEditor);

	ue_py_slate_farguments_float("timeline_length", TimelineLength);
	ue_py_slate_farguments_fvector2d("desired_size", DesiredSize);

	ue_py_snew(SPythonCurveEditor);
	return 0;
}

void ue_python_init_scurve_editor(PyObject* ue_module)
{
	ue_PySCurveEditorType.tp_init = (initproc)ue_py_scurve_editor_init;
	ue_PySCurveEditorType.tp_base = &ue_PySCompoundWidgetType;

	if (PyType_Ready(&ue_PySCurveEditorType) < 0)
	{
		return;
	}

	Py_INCREF(&ue_PySCurveEditorType);
	PyModule_AddObject(ue_module, "SCurveEditor", (PyObject*)&ue_PySCurveEditorType);
}

#endif