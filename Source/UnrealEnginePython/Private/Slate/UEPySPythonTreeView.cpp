
#include "UEPySPythonTreeView.h"

#include "Runtime/Slate/Public/Widgets/Views/STreeView.h"

//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#include "UEPySHeaderRow.h"
// DK End
//////////////////////////////////////////////////////////////////////////


static PyObject *py_ue_spython_tree_view_set_item_expansion(ue_PySPythonTreeView *self, PyObject * args)
{
	ue_py_slate_cast(SPythonTreeView);
	PyObject *py_item;
	PyObject *py_bool;
	if (!PyArg_ParseTuple(args, "OO:set_item_expansion", &py_item, &py_bool))
	{
		return nullptr;
	}
	py_SPythonTreeView->SetPythonItemExpansion(py_item, PyObject_IsTrue(py_bool) ? true : false);
	Py_RETURN_NONE;
}

void SPythonTreeView::SetPythonItemExpansion(PyObject *item, bool InShouldExpandItem)
{
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
	for (TSharedPtr<struct FPythonTreeItem> PythonItem : *ItemsSource)
	// DK End
	//////////////////////////////////////////////////////////////////////////
	{
		if (PythonItem->py_object == item)
		{
			SetItemExpansion(PythonItem, InShouldExpandItem);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
static PyObject *py_spython_tree_view_update_item_source_list(ue_PySPythonTreeView *self, PyObject * args)
{
	PyObject *values;
	if (!PyArg_ParseTuple(args, "O:update_item_source_list", &values))
	{
		return NULL;
	}

	values = PyObject_GetIter(values);
	if (!values)
	{
		return PyErr_Format(PyExc_Exception, "argument is not an iterable");
	}

	//NOTE: ikrimae: Increment first so we don't decrement and destroy python objects that 
	//we're passing in e.g. if you pass the same item source array into update_items(). 
	//Might not be necessary but I'm not too familiar with python's GC
	TArray<TSharedPtr<FPythonTreeItem>> tempNewArray;
	while (PyObject *item = PyIter_Next(values))
	{
		Py_INCREF(item);
		tempNewArray.Add(TSharedPtr<FPythonTreeItem>(new FPythonTreeItem(item)));
	}

	for (TSharedPtr<struct FPythonTreeItem>& item : self->item_source_list)
	{
		Py_XDECREF(item->py_object);
		item->children.Empty();
	}
	self->item_source_list.Empty();

	Move<TArray<TSharedPtr<FPythonTreeItem>>>(self->item_source_list, tempNewArray);

	ue_py_slate_cast(SPythonTreeView);
	py_SPythonTreeView->SetListItemsSource(self->item_source_list);
	Py_RETURN_NONE;
}
// DK End
//////////////////////////////////////////////////////////////////////////

static PyMethodDef ue_PySPythonTreeView_methods[] = {
	{ "set_item_expansion", (PyCFunction)py_ue_spython_tree_view_set_item_expansion, METH_VARARGS, "" },
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
	{ "update_item_source_list", (PyCFunction)py_spython_tree_view_update_item_source_list, METH_VARARGS, "" },
	// DK End
	//////////////////////////////////////////////////////////////////////////
	{ NULL }  /* Sentinel */
};

PyTypeObject ue_PySPythonTreeViewType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.SPythonTreeView", /* tp_name */
	sizeof(ue_PySPythonTreeView), /* tp_basicsize */
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
	"Unreal Engine SPythonTreeView",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	ue_PySPythonTreeView_methods,             /* tp_methods */
};

static int ue_py_spython_tree_view_init(ue_PySPythonTreeView *self, PyObject *args, PyObject *kwargs)
{

	ue_py_slate_setup_farguments(SPythonTreeView);

	// first of all check for values
	PyObject *values = ue_py_dict_get_item(kwargs, "tree_items_source");
	if (!values)
	{
		PyErr_SetString(PyExc_Exception, "you must specify tree items");
		return -1;
	}

	values = PyObject_GetIter(values);
	if (!values)
	{
		PyErr_SetString(PyExc_Exception, "values field is not an iterable");
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#if 0
	TArray<TSharedPtr<FPythonItem>> *items = new TArray<TSharedPtr<FPythonItem>>();
	while (PyObject *item = PyIter_Next(values))
	{
		Py_INCREF(item);
		items->Add(TSharedPtr<FPythonItem>(new FPythonItem(item)));
	}
	Py_DECREF(values);

	arguments.TreeItemsSource(items);
#else
	self->item_source_list.Empty();
	while (PyObject *item = PyIter_Next(values))
	{
		Py_INCREF(item);
		self->item_source_list.Add(TSharedPtr<FPythonTreeItem>(new FPythonTreeItem(item)));
	}

	arguments.TreeItemsSource(&self->item_source_list);
#endif
	// DK End
	//////////////////////////////////////////////////////////////////////////

	ue_py_slate_farguments_optional_enum("allow_overscroll", AllowOverscroll, EAllowOverscroll);
	ue_py_slate_farguments_optional_bool("clear_selection_on_click", ClearSelectionOnClick);
	ue_py_slate_farguments_optional_enum("consume_mouse_wheel", ConsumeMouseWheel, EConsumeMouseWheel);
	ue_py_slate_farguments_float("item_height", ItemHeight);
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#if 0
	ue_py_slate_farguments_event("on_generate_row", OnGenerateRow, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnGenerateRow, GenerateRow);
	ue_py_slate_farguments_event("on_selection_changed", OnSelectionChanged, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnSelectionChanged, OnSelectionChanged);
#else
	ue_py_slate_farguments_event("on_generate_row", OnGenerateRow, TSlateDelegates<TSharedPtr<FPythonTreeItem>>::FOnGenerateRow, GenerateRow);
	ue_py_slate_farguments_event("on_selection_changed", OnSelectionChanged, TSlateDelegates<TSharedPtr<FPythonTreeItem>>::FOnSelectionChanged, OnSelectionChanged);
#endif
	// DK End
	//////////////////////////////////////////////////////////////////////////
	ue_py_slate_farguments_enum("selection_mode", SelectionMode, ESelectionMode::Type);
#if ENGINE_MINOR_VERSION > 12
	ue_py_slate_farguments_optional_float("wheel_scroll_multiplier", WheelScrollMultiplier);
#endif
	//////////////////////////////////////////////////////////////////////////
	// DK Begin: ID(#DK_PyTreeView) modifier:(xingtongli)
#if 0
	ue_py_slate_farguments_event("on_get_children", OnGetChildren, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnGetChildren, GetChildren);
#else
	ue_py_slate_farguments_event("on_get_children", OnGetChildren, TSlateDelegates<TSharedPtr<FPythonTreeItem>>::FOnGetChildren, GetChildren);
#endif
	// DK End
	//////////////////////////////////////////////////////////////////////////
	
	ue_py_snew(SPythonTreeView);
	return 0;
}

void ue_python_init_spython_tree_view(PyObject *ue_module)
{

	ue_PySPythonTreeViewType.tp_init = (initproc)ue_py_spython_tree_view_init;

	ue_PySPythonTreeViewType.tp_base = &ue_PySTreeViewType;

	if (PyType_Ready(&ue_PySPythonTreeViewType) < 0)
		return;

	Py_INCREF(&ue_PySPythonTreeViewType);
	PyModule_AddObject(ue_module, "SPythonTreeView", (PyObject *)&ue_PySPythonTreeViewType);
}
