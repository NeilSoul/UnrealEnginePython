#if WITH_EDITOR

#include "UEPyFAsteroidFloatValue.h"
#include "UEPyFAsteroidTangentData.h"

/**
 * define the methods need for python, currently we don't need any method now
 */


/**
 * define the property of this structure 
 */

/** value */
//	getter
static PyObject* py_ue_fasteroid_float_value_get_value(ue_PyFAsteroidFloatValue* self, void* closure)
{
	return PyFloat_FromDouble(self->value.Value);
}

//	setter
static int py_ue_fasteroid_float_value_set_value(ue_PyFAsteroidFloatValue* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* f_value = PyNumber_Float(value);
		self->value.Value = PyFloat_AsDouble(f_value);
		Py_DECREF(f_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is no numeric");
	return -1;
}

/** interpolation mode */
//	getter
static PyObject* py_ue_fasteroid_float_value_get_interp_mode(ue_PyFAsteroidFloatValue* self, void* closure)
{
	return PyLong_FromLong(self->value.InterpMode);
}
//	setter
static int py_ue_fasteroid_float_value_set_interp_mode(ue_PyFAsteroidFloatValue* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* l_value = PyNumber_Long(value);
		self->value.InterpMode = (ERichCurveInterpMode)PyLong_AsLong(l_value);
		Py_DECREF(l_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

/** tangent mode */
//	getter
static PyObject* py_ue_fasteroid_float_value_get_tangent_mode(ue_PyFAsteroidFloatValue* self, void* closure)
{
	return PyLong_FromLong(self->value.TangentMode);
}
// setter
static int32 py_ue_fasteroid_float_value_set_tangent_mode(ue_PyFAsteroidFloatValue* self, PyObject* value, void* closure)
{
	if (value && PyNumber_Check(value))
	{
		PyObject* l_value = PyNumber_Long(value);
		self->value.TangentMode = (ERichCurveTangentMode)PyLong_AsLong(l_value);
		Py_DECREF(l_value);
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

/** tangent */
//	getter
static PyObject* py_ue_fasteroid_float_value_get_tangent(ue_PyFAsteroidFloatValue* self, void* closure)
{
	return py_ue_new_fasteroid_tangent_data(self->value.Tangent);
}
//	setter
static int32 py_ue_fasteroid_float_value_set_tangent(ue_PyFAsteroidFloatValue* self, PyObject* value, void* closure)
{
	if (auto tangent_data = py_ue_is_fasteroid_tangent_data(value))
	{
		self->value.Tangent = tangent_data->data;
		return 0;
	}

	PyErr_SetString(PyExc_TypeError, "value is not numeric");
	return -1;
}

//	the property definition
static PyGetSetDef ue_PyFAsteroidFloatValue_properties[] =
{
	{(char*)"value", (getter)py_ue_fasteroid_float_value_get_value, (setter)py_ue_fasteroid_float_value_set_value, (char*)"", NULL},
	{(char*)"interp_mode", (getter)py_ue_fasteroid_float_value_get_interp_mode, (setter)py_ue_fasteroid_float_value_set_interp_mode, (char*)"", NULL},	
	{(char*)"tangent_mode", (getter)py_ue_fasteroid_float_value_get_tangent_mode, (setter)py_ue_fasteroid_float_value_set_tangent_mode, (char*)"", NULL},
	{(char*)"tangent", (getter)py_ue_fasteroid_float_value_get_tangent, (setter)py_ue_fasteroid_float_value_set_tangent, (char*)"", NULL},
	{NULL}
};

//	the str method
static PyObject* ue_PyFAsteroidFloatValue_str(ue_PyFAsteroidFloatValue* self)
{
	return PyUnicode_FromFormat(
		"unreal_engine.FAsteroidFloatValue {'value': %S, 'interp mode': %s, 'tangent mode': %s, 'tangent': %S}>",
		PyFloat_FromDouble(self->value.Value),
		TCHAR_TO_UTF8(*UEnum::GetValueAsString(self->value.InterpMode.GetValue())),
		TCHAR_TO_UTF8(*UEnum::GetValueAsString(self->value.TangentMode.GetValue())),
		py_ue_new_fasteroid_tangent_data(self->value.Tangent)
	);
}

//	the python type
PyTypeObject ue_PyFAsteroidFloatValueType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.FAsteroidFloatValue", /* tp_name */
	sizeof(ue_PyFAsteroidFloatValue), /* tp_basicsize */
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
	(reprfunc)ue_PyFAsteroidFloatValue_str,                         /* tp_str */
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
	ue_PyFAsteroidFloatValue_properties,
};

PyObject* py_ue_new_fasteroid_float_value(FAsteroidFloatValue data)
{
	ue_PyFAsteroidFloatValue* ret = (ue_PyFAsteroidFloatValue*)PyObject_New(ue_PyFAsteroidFloatValue, &ue_PyFAsteroidFloatValueType);
	ret->value = data;
	return (PyObject*)ret;
}

ue_PyFAsteroidFloatValue* py_ue_is_fasteroid_float_value(PyObject* obj)
{
	if (!PyObject_IsInstance(obj, (PyObject*)&ue_PyFAsteroidFloatValueType))
	{
		return nullptr;
	}
	return (ue_PyFAsteroidFloatValue*)obj;
}

void ue_python_init_fasteroid_float_value(PyObject* ue_module)
{
	ue_PyFAsteroidFloatValueType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&ue_PyFAsteroidFloatValueType) < 0)
	{
		return;
	}

	Py_INCREF(&ue_PyFAsteroidFloatValueType);
	PyModule_AddObject(ue_module, "FAsteroidFloatValue", (PyObject*)&ue_PyFAsteroidFloatValueType);
}

#endif