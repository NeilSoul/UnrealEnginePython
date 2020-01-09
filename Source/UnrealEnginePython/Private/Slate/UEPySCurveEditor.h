#pragma once

#include "UnrealEnginePython.h"

#if WITH_EDITOR
#include "UEPySCompoundWidget.h"
#include "Editor/UnrealEd/Public/SCurveEditor.h"

extern PyTypeObject ue_PySCurveEditorType;

typedef struct
{
	ue_PySCompoundWidget s_compound_widget;
	/* Type-specific fields go here. */
} ue_PySCurveEditor;

void ue_python_init_scurve_editor(PyObject *);
#endif