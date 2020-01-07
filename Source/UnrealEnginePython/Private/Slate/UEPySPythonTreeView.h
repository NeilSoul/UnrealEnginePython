#pragma once

#include "UnrealEnginePython.h"

#include "UEPySTreeView.h"

extern PyTypeObject ue_PySPythonTreeViewType;

//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#if 0
class SPythonTreeView : public STreeView<TSharedPtr<struct FPythonItem>>
#else
class SPythonTreeView : public STreeView<TSharedPtr<struct FPythonTreeItem>>
#endif
// DK End
//////////////////////////////////////////////////////////////////////////
{
public:
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#if 0
	~SPythonTreeView()
	{
		delete(ItemsSource);
	}
#endif
	// DK End
	//////////////////////////////////////////////////////////////////////////
	void SetPythonItemExpansion(PyObject *item, bool InShouldExpandItem);
};

typedef struct
{
	ue_PySTreeView s_tree_view;
	/* Type-specific fields go here. */
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
	TArray<TSharedPtr<struct FPythonTreeItem>> item_source_list;
	// DK End
	//////////////////////////////////////////////////////////////////////////
} ue_PySPythonTreeView;

void ue_python_init_spython_tree_view(PyObject *);
