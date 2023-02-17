#include "Python.h"
#define _USE_MATH_DEFINES 
#include "math.h"
#include <algorithm>


#include "binding_radardata.h"
#include "binding_radardataholder.h"





static PyMethodDef pyMethods[] = {
	{"test",  test, METH_VARARGS,"test"},
	{"radarDataAllocate",  radarDataAllocate, METH_VARARGS, ""},
	{"radarDataDeallocate",  radarDataDeallocate, METH_VARARGS, ""},
	{"radarDataLoadFile",  radarDataLoadFile, METH_VARARGS, ""},
	{"radarDataUpdateProperties",  radarDataUpdateProperties, METH_VARARGS, ""},
	{"radarDataGetStats",  radarDataGetStats, METH_VARARGS, ""},
	{"radarDataRadarSpaceForLocation",  radarDataRadarSpaceForLocation, METH_VARARGS, ""},
	{"radarDataGetPixelForRadarSpace",  radarDataGetPixelForRadarSpace, METH_VARARGS, ""},
	
	{"radarDataHolderAllocate",  radarDataHolderAllocate, METH_VARARGS, ""},
	{"radarDataHolderDeallocate",  radarDataHolderDeallocate, METH_VARARGS, ""},
	{"radarDataHolderLoad",  radarDataHolderLoad, METH_VARARGS, ""},
	{"radarDataHolderUnload",  radarDataHolderUnload, METH_VARARGS, ""},
	{"radarDataHolderGetState",  radarDataHolderGetState, METH_VARARGS, ""},
	{"radarDataHolderGetProduct",  radarDataHolderGetProduct, METH_VARARGS, ""},
	{"radarDataHolderProductGetRadarData",  radarDataHolderProductGetRadarData, METH_VARARGS, ""},
	{"radarDataHolderProductIsLoaded",  radarDataHolderProductIsLoaded, METH_VARARGS, ""},
	{"radarDataHolderProductStartUsing",  radarDataHolderProductStartUsing, METH_VARARGS, ""},
	{"radarDataHolderProductStopUsing",  radarDataHolderProductStopUsing, METH_VARARGS, ""},
	//{"recover",  recover, METH_VARARGS, "Do stuff."},
	//{"stft",  stftFunc, METH_VARARGS, "Do stuff."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef pyModule = {
	PyModuleDef_HEAD_INIT,
	"openstorm_radar_native",   /* name of module */
	"", /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
				 or -1 if the module keeps state in global variables. */
	pyMethods
};



PyMODINIT_FUNC
PyInit_openstorm_radar_native(void)
{
	//printf("Initialized\n");
	Py_Initialize();
	
	
	
	
	PyObject* m;
	m = PyModule_Create(&pyModule);
	if (m == NULL)
		return NULL;
	
	
	RadarBufferObject_Type.tp_name = "openstorm_radar_native.RadarBufferType";
	RadarBufferObject_Type.tp_basicsize = sizeof(RadarBufferObject);
	RadarBufferObject_Type.tp_doc = PyDoc_STR("Radar Buffer Data");
	RadarBufferObject_Type.tp_new = PyType_GenericNew,
	RadarBufferObject_Type.tp_as_buffer = &RadarBuffer_BufferProcs;
    RadarBufferObject_Type.tp_itemsize = 0;
	//RadarBufferObject_Type.tp_dealloc = (destructor)myobj_dealloc,
	//RadarBufferObject_Type.tp_repr = (reprfunc)myobj_repr,
	RadarBufferObject_Type.tp_flags = Py_TPFLAGS_DEFAULT;
	
    if (PyType_Ready(&RadarBufferObject_Type) < 0)
        return NULL;
	
	Py_INCREF(&RadarBufferObject_Type);
    if (PyModule_AddObject(m, "RadarBufferType", (PyObject *) &RadarBufferObject_Type) < 0) {
        Py_DECREF(&RadarBufferObject_Type);
        Py_DECREF(m);
        return NULL;
    }
	
	
	return m;
}
