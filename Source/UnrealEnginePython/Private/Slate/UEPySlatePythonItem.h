#pragma once

#include "UnrealEnginePython.h"

struct FPythonItem
{
	PyObject *py_object = nullptr;

	FPythonItem(PyObject *item)
	{
		py_object = item;
	}
};

//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
struct FPythonTreeItem
{
	PyObject *py_object = nullptr;
	TArray<TSharedPtr<FPythonTreeItem> > children;

	FPythonTreeItem(PyObject *item)
	{
		py_object = item;
	}
};
// DK End
//////////////////////////////////////////////////////////////////////////