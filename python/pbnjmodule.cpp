#include <Python.h>
#include <iostream>
#include "pbnj.h"
#include "Configuration.h"
#include "Camera.h"

extern "C" {

/*******************************************************************************
 * START pbnj.Configuration
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    pbnj::Configuration *object;
} Configuration;


static void
Configuration_dealloc(Configuration *self) {
    delete self->object;
    self->object = NULL;
}


static PyObject *
Configuration_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Configuration *self;

    self = (Configuration *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->object = NULL;
    }

    return (PyObject *)self;
}


static int
Configuration_init(Configuration *self, PyObject *args) {
    const char *filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return -1;
    }

    self->object = new pbnj::Configuration(std::string(filename));

    return 0;
}

static char *ConfigurationDoc = NULL;


static PyTypeObject ConfigurationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_pbnj.Configuration",      /* tp_name */
    sizeof(Configuration),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Configuration_dealloc, /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    ConfigurationDoc,          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Configuration_init, /* tp_init */
    0,                         /* tp_alloc */
    Configuration_new,         /* tp_new */
};


static PyObject *
Configuration_get_image_width(PyObject *self, PyObject *args) {
    Configuration *config;

    if (!PyArg_ParseTuple(args, "O!", &ConfigurationType, &config)) {
        return NULL;
    }

    return PyLong_FromLong((long)config->object->imageWidth);
}


static PyMethodDef Configuration_get_image_width_method = {
    "Configuration_get_image_width",
    (PyCFunction)Configuration_get_image_width,
    METH_VARARGS,
    "Configuration_get_image_width documentation",
};


static PyObject *
Configuration_get_image_height(PyObject *self, PyObject *args) {
    Configuration *config;

    if (!PyArg_ParseTuple(args, "O!", &ConfigurationType, &config)) {
        return NULL;
    }

    return PyLong_FromLong((long)config->object->imageHeight);
}


static PyMethodDef Configuration_get_image_height_method = {
    "Configuration_get_image_height",
    (PyCFunction)Configuration_get_image_height,
    METH_VARARGS,
    "Configuration_get_image_height documentation",
};


/*******************************************************************************
 * END pbnj.Configuration
 ******************************************************************************/

/*******************************************************************************
 * START pbnj.Camera
 ******************************************************************************/

typedef struct {
    PyObject_HEAD
    pbnj::Camera *object;
} Camera;


static void
Camera_dealloc(Camera *self) {
    delete self->object;
    self->object = NULL;
}


static PyObject *
Camera_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  Camera *self;

  self = (Camera *)type->tp_alloc(type, 0);
  if (self != NULL) {
      self->object = NULL;
  }

  return (PyObject *)self;
}


static int
Camera_init(Camera *self, PyObject *args) {
    int width, height;

    if (!PyArg_ParseTuple(args, "ii", &width, &height)) {
        return -1;
    }

    self->object = new pbnj::Camera(width, height);

    return 0;
}


static char *CameraDoc = NULL;


static PyTypeObject CameraType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_pbnj.Camera",      /* tp_name */
    sizeof(Camera),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Camera_dealloc, /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    CameraDoc,          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Camera_init, /* tp_init */
    0,                         /* tp_alloc */
    Camera_new,         /* tp_new */
};


static PyObject *
Camera_get_image_width(PyObject *self, PyObject *args) {
    Camera *camera;

    if (!PyArg_ParseTuple(args, "O!", &CameraType, &camera)) {
        return NULL;
    }

    return PyLong_FromLong((long)camera->object->imageWidth);
}


static PyMethodDef Camera_get_image_width_method = {
    "Camera_get_image_width",
    (PyCFunction)Camera_get_image_width,
    METH_VARARGS,
    "Camera_get_image_width documentation",
};


static PyObject *
Camera_get_image_height(PyObject *self, PyObject *args) {
    Camera *camera;

    if (!PyArg_ParseTuple(args, "O!", &CameraType, &camera)) {
        return NULL;
    }

    return PyLong_FromLong((long)camera->object->imageHeight);
}


static PyMethodDef Camera_get_image_height_method = {
    "Camera_get_image_height",
    (PyCFunction)Camera_get_image_height,
    METH_VARARGS,
    "Camera_get_image_height documentation",
};


/*******************************************************************************
 * END pbnj.Camera
 ******************************************************************************/

/*******************************************************************************
 * START pbnj
 ******************************************************************************/

static PyObject *
pbnj_init(PyObject *self) {
    int argc;
    const char *argv[1];
    const std::string dummy = "dummy";

    argc = 1;
    argv[0] = dummy.c_str();

    pbnj::pbnjInit(&argc, argv);

    Py_RETURN_NONE;
}


static PyMethodDef pbnj_init_method = {
    "init",
    (PyCFunction)pbnj_init,
    METH_NOARGS,
    "Initialize the PBNJ subsystem.",
};


static PyMethodDef PbnjMethods[] = {
    Configuration_get_image_width_method,
    Configuration_get_image_height_method,
    Camera_get_image_width_method,
    Camera_get_image_height_method,
    pbnj_init_method,
    {NULL, NULL, 0, NULL},
};


static char *pbnj_doc = NULL;


static struct PyModuleDef pbnjmodule = {
    PyModuleDef_HEAD_INIT,
    "_pbnj",
    pbnj_doc,
    -1, /* module keeps state in global variables */
    PbnjMethods,
};


PyMODINIT_FUNC
PyInit__pbnj(void) {
    PyObject *m;

    ConfigurationType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&ConfigurationType) < 0)
        return NULL;

    CameraType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&CameraType) < 0)
        return NULL;

    m = PyModule_Create(&pbnjmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ConfigurationType);
    PyModule_AddObject(m, "Configuration", (PyObject *)&ConfigurationType);

    Py_INCREF(&CameraType);
    PyModule_AddObject(m, "Camera", (PyObject *)&CameraType);

    return m;
}

} /* extern "C" */
