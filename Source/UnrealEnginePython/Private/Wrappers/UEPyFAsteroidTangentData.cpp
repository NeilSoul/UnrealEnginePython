#if WITH_EDITOR

#include "UEPyFAsteroidTangentData.h"

/**
 * method, since this is a pure data structure, there are no methods at all
 */


/**
 * property
 */

/** arrive tangent */
//	getter
static PyObject* py_ue_fasteroid_tangent_data_get_arrive_tangent(ue_PyFAsteroidTangentData* self, void* closure)
{
	return PyFloat_FromDouble(self->data.ArriveTangent);
}
//	setter
static int py_ue_fasteroid_tangent_data_set_arrive_tangent(ue_PyFAsteroidTangentData* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* f_value = PyNumber_Float(value);
		self->data.ArriveTangent = PyFloat_AsDouble(f_value);
		Py_DECREF(f_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}


/** leave tangent */
//	getter
static PyObject* py_ue_fasteroid_tangent_data_get_leave_tangent(ue_PyFAsteroidTangentData* self, void* closure)
{
	return PyFloat_FromDouble(self->data.LeaveTangent);
}
//	setter
static int py_ue_fasteroid_tangent_data_set_leave_tangent(ue_PyFAsteroidTangentData* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* f_value = PyNumber_Float(value);
		self->data.LeaveTangent = PyFloat_AsDouble(f_value);
		Py_DECREF(f_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

/** tangent weight mode */
//	getter
static PyObject* py_ue_fasteroid_tangent_data_get_tangent_weight_mode(ue_PyFAsteroidTangentData* self, void* closure)
{
	return PyLong_FromLong(self->data.TangentWeightMode);
}
//	setter
static int py_ue_fasteroid_tangent_data_set_tangent_weight_mode(ue_PyFAsteroidTangentData* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* l_value = PyNumber_Long(value);
		self->data.TangentWeightMode = (ERichCurveTangentWeightMode)PyLong_AsLong(l_value);
		Py_DECREF(l_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

/** arrive tangent weight */
//	getter
static PyObject* py_ue_fasteroid_tangent_data_get_arrive_tangent_weight(ue_PyFAsteroidTangentData* self, void* closure)
{
	return PyFloat_FromDouble(self->data.ArriveTangentWeight);
}
//	setter
static int py_ue_fasteroid_tangent_data_set_arrive_tangent_weight(ue_PyFAsteroidTangentData* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* f_value = PyNumber_Float(value);
		self->data.ArriveTangentWeight = PyFloat_AsDouble(f_value);
		Py_DECREF(f_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

/** leave tangent weight */
//	getter
static PyObject* py_ue_fasteroid_tangent_data_get_leave_tangent_weight(ue_PyFAsteroidTangentData* self, void* closure)
{
	return PyFloat_FromDouble(self->data.LeaveTangentWeight);
}
//	setter
static int py_ue_fasteroid_tangent_data_set_leave_tangent_weight(ue_PyFAsteroidTangentData* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* f_value = PyNumber_Float(value);
		self->data.LeaveTangentWeight = PyFloat_AsDouble(f_value);
		Py_DECREF(f_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

//	the property
static PyGetSetDef ue_PyFAsteroidTangentData_properties[] =
{
	{(char*)"arrive_tangent", (getter)py_ue_fasteroid_tangent_data_get_arrive_tangent, (setter)py_ue_fasteroid_tangent_data_set_arrive_tangent, (char*)"", NULL},
	{(char*)"leave_tangent", (getter)py_ue_fasteroid_tangent_data_get_leave_tangent, (setter)py_ue_fasteroid_tangent_data_set_leave_tangent, (char*)"", NULL},
	{(char*)"tangent_weight_mode", (getter)py_ue_fasteroid_tangent_data_get_tangent_weight_mode, (setter)py_ue_fasteroid_tangent_data_set_tangent_weight_mode, (char*)"", NULL},
	{(char*)"arrive_tangent_weight", (getter)py_ue_fasteroid_tangent_data_get_arrive_tangent_weight, (setter)py_ue_fasteroid_tangent_data_set_arrive_tangent_weight, (char*)"", NULL},  
	{(char*)"leave_tangent_weight", (getter)py_ue_fasteroid_tangent_data_get_leave_tangent_weight, (setter)py_ue_fasteroid_tangent_data_set_leave_tangent_weight, (char*)"", NULL}, 
	{NULL}
};

//	the str method
static PyObject* ue_PyFAsteroidTangentData_str(ue_PyFAsteroidTangentData* self)
{
	return PyUnicode_FromFormat(
		"<unreal_engine.FAsteroidTangentData {'arrive tangent': %S, 'leave tangent': %S, 'tangent weight mode': %s, 'arrive tangent weight': %S,  'leave tangent weight': %S}>",
			PyFloat_FromDouble(self->data.ArriveTangent),
			PyFloat_FromDouble(self->data.LeaveTangent),
			TCHAR_TO_UTF8(*UEnum::GetValueAsString(self->data.TangentWeightMode.GetValue())),
			PyFloat_FromDouble(self->data.ArriveTangentWeight),
			PyFloat_FromDouble(self->data.LeaveTangentWeight)
		);
}

PyTypeObject ue_PyFAsteroidTangentDataType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.FAsteroidTangentData", /* tp_name */
	sizeof(ue_PyFAsteroidTangentData), /* tp_basicsize */
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
	(reprfunc)ue_PyFAsteroidTangentData_str,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
#if PY_MAJOR_VERSION < 3
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES,        /* tp_flags */
#else
	Py_TPFLAGS_DEFAULT,        /* tp_flags */
#endif
	"Unreal Engine FAsteroidTangentData",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	0,             /* tp_methods */
	0,
	ue_PyFAsteroidTangentData_properties,
};


PyObject* py_ue_new_fasteroid_tangent_data(FAsteroidTangentData data)
{
	ue_PyFAsteroidTangentData* ret = (ue_PyFAsteroidTangentData*)PyObject_New(ue_PyFAsteroidTangentData, &ue_PyFAsteroidTangentDataType);
	ret->data = data;
	return (PyObject*)ret;
}

ue_PyFAsteroidTangentData* py_ue_is_fasteroid_tangent_data(PyObject* obj)
{
	if (!PyObject_IsInstance(obj, (PyObject*)&ue_PyFAsteroidTangentDataType))
	{
		return nullptr;
	}
	return (ue_PyFAsteroidTangentData*)obj;
}

void ue_python_init_fasteroid_tangent_data(PyObject* ue_module)
{
	ue_PyFAsteroidTangentDataType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&ue_PyFAsteroidTangentDataType) < 0)
	{
		return;
	}

	Py_INCREF(&ue_PyFAsteroidTangentDataType);
	PyModule_AddObject(ue_module, "FAsteroidTangentData", (PyObject*)&ue_PyFAsteroidTangentDataType);
}

#endif