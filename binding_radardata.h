#include "Python.h"
#include "../OpenStorm/Source/OpenStorm/Radar/RadarData.h"
#include "../OpenStorm/Source/OpenStorm/Radar/Globe.h"

typedef struct {
	PyObject_HEAD
	RadarData* radarData;
} RadarBufferObject;

static PyTypeObject RadarBufferObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
};



static PyObject* test(PyObject* self, PyObject* args) {
	printf("Get Beaned\n");
	return PyLong_FromLong(1);
}

static PyObject* radarDataAllocate(PyObject* self, PyObject* args) {
	// max sweeps
	int maxSweeps = 0;
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0 != NULL){
		maxSweeps = PyLong_AsLong(arg0);
	}
	// max thetas
	int maxThetas = 0;
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1 != NULL){
		maxThetas = PyLong_AsLong(arg1);
	}
	// max radius
	int maxRadius = 0;
	PyObject* arg2=PyTuple_GetItem(args, 2);
	if (arg2 != NULL){
		maxRadius = PyLong_AsLong(arg2);
	}
	
	RadarData* radarData = new RadarData();
	radarData->sweepBufferCount = maxSweeps;
	radarData->thetaBufferCount = maxThetas;
	radarData->radiusBufferCount = maxRadius;
	return PyLong_FromVoidPtr(radarData);
}

static PyObject* radarDataDeallocate(PyObject* self, PyObject* args) {
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL)
		return NULL;
	delete (RadarData*)PyLong_AsVoidPtr(arg0);
	return PyBool_FromLong(1);
}

static PyObject* radarDataLoadFile(PyObject* self, PyObject* args) {
	// radar data
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	// file name
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1==NULL) return NULL;
	// volume type
	PyObject* arg2=PyTuple_GetItem(args, 2);
	if (arg2==NULL) return NULL;
	RadarData* radarData = (RadarData*)PyLong_AsVoidPtr(arg0);
	void* nexradData = RadarData::ReadNexradData(PyUnicode_AsUTF8(arg1));
	radarData->LoadNexradVolume(nexradData, (RadarData::VolumeType)PyLong_AsLong(arg2));
	RadarData::FreeNexradData(nexradData);
	return PyBool_FromLong(1);
}

static PyObject* radarDataUpdateProperties(PyObject* self, PyObject* args) {
	// radar data
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	// python class
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1==NULL) return NULL;
	RadarData* radarData = (RadarData*)PyLong_AsVoidPtr(arg0);
	
	PyObject_SetAttr(arg1, PyUnicode_FromString("test_prop"), PyBool_FromLong(1));
	if(radarData->buffer != NULL){
		
		RadarBufferObject *radarBufferObject;
    	radarBufferObject = (RadarBufferObject *) RadarBufferObject_Type.tp_alloc(&RadarBufferObject_Type, 0);
		radarBufferObject->radarData = radarData;
		//PyBuffer_FillInfo(bufferObj, arg1, radarData->buffer, radarData->fullBufferSize * sizeof(float), PyBUF_WRITABLE, 0);
		//PyObject_SetAttr(arg1, PyUnicode_FromString("buffer"), PyBytes_FromStringAndSize((char*)radarData->buffer, radarData->fullBufferSize * sizeof(float)));
		PyObject_SetAttr(arg1, PyUnicode_FromString("buffer"), (PyObject*)radarBufferObject);
	}else{
		PyObject_SetAttr(arg1, PyUnicode_FromString("buffer"), Py_None);
	}
	
	return PyBool_FromLong(1);
}

static PyObject* radarDataGetStats(PyObject* self, PyObject* args) {
	// radar data
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarData* radarData = (RadarData*)PyLong_AsVoidPtr(arg0);
	
	PyObject* statsDict = PyDict_New();
	PyObject_SetItem(statsDict, PyUnicode_FromString("inner_distance"), PyFloat_FromDouble(radarData->stats.innerDistance));
	PyObject_SetItem(statsDict, PyUnicode_FromString("min_value"), PyFloat_FromDouble(radarData->stats.minValue));
	PyObject_SetItem(statsDict, PyUnicode_FromString("max_value"), PyFloat_FromDouble(radarData->stats.maxValue));
	PyObject_SetItem(statsDict, PyUnicode_FromString("pixel_size"), PyFloat_FromDouble(radarData->stats.pixelSize));
	PyObject_SetItem(statsDict, PyUnicode_FromString("no_data_value"), PyFloat_FromDouble(radarData->stats.noDataValue));
	PyObject_SetItem(statsDict, PyUnicode_FromString("bound_radius"), PyFloat_FromDouble(radarData->stats.boundRadius));
	PyObject_SetItem(statsDict, PyUnicode_FromString("bound_upper"), PyFloat_FromDouble(radarData->stats.boundUpper));
	PyObject_SetItem(statsDict, PyUnicode_FromString("bound_lower"), PyFloat_FromDouble(radarData->stats.boundLower));
	PyObject_SetItem(statsDict, PyUnicode_FromString("latitude"), PyFloat_FromDouble(radarData->stats.latitude));
	PyObject_SetItem(statsDict, PyUnicode_FromString("longitude"), PyFloat_FromDouble(radarData->stats.longitude));
	PyObject_SetItem(statsDict, PyUnicode_FromString("altitude"), PyFloat_FromDouble(radarData->stats.altitude));
	PyObject_SetItem(statsDict, PyUnicode_FromString("volume_type"), PyLong_FromLong(radarData->stats.volumeType));
	
	return statsDict;
}


static PyObject* radarDataGetPixelForLocation(PyObject* self, PyObject* args) {
	// radar data
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarData* radarData = (RadarData*)PyLong_AsVoidPtr(arg0);
	// latitude
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1==NULL) return NULL;
	// longitude
	PyObject* arg2=PyTuple_GetItem(args, 2);
	if (arg2==NULL) return NULL;
	// altitude
	PyObject* arg3=PyTuple_GetItem(args, 3);
	if (arg3==NULL) return NULL;
	
	// radar data not loaded
	if(radarData->sweepInfo == NULL){
		return Py_None;
	}
	
	double latitude = PyFloat_AsDouble(arg1);
	double longitude = PyFloat_AsDouble(arg2);
	double altitude = PyFloat_AsDouble(arg3);
	
	Globe globe = {};
	globe.SetCenter(0, 0, -globe.surfaceRadius - radarData->stats.altitude);
	globe.SetTopCoordinates(radarData->stats.latitude, radarData->stats.longitude);
	
	auto radarSpaceLocation = globe.GetPointDegrees(latitude, longitude, altitude);
	
	float radius = sqrt(radarSpaceLocation.x * radarSpaceLocation.x + radarSpaceLocation.y * radarSpaceLocation.y + radarSpaceLocation.z * radarSpaceLocation.z);
	float theta = atan2(radarSpaceLocation.y, radarSpaceLocation.x);
	float phi = acos(radarSpaceLocation.z / radius);
	
	// normalize
	radius = radius;
	theta = (theta / (M_PI * 2.0)) + 0.25;
	theta = theta < 0 ? theta + 1 : theta;
	phi = (0.5 - phi / M_PI);
	
	// convert to volume texture coordinates
	radius = radius / radarData->stats.pixelSize - radarData->stats.innerDistance;
	theta = theta * radarData->thetaBufferCount/* + 1*/;
	// convert phi to volume sweep index
	phi = phi * 180;
	int firstIndex = radarData->sweepBufferCount;
	int lastIndex = -1;
	for (int sweepIndex = 0; sweepIndex < radarData->sweepBufferCount; sweepIndex++) {
		RadarData::SweepInfo &info = radarData->sweepInfo[sweepIndex];
		if(info.id != -1){
			firstIndex = std::min(firstIndex, sweepIndex);
			lastIndex = std::max(lastIndex, sweepIndex);
		}
	}
	fprintf(stderr, "phi %f %f %f\n",phi, radarData->sweepInfo[firstIndex].elevation, radarData->sweepInfo[lastIndex].elevation);
	if(phi < radarData->sweepInfo[firstIndex].elevation){
		phi = -INFINITY;
	}else if (phi > radarData->sweepInfo[lastIndex].elevation){
		phi = INFINITY;
	}else{
		for (int sweepIndex = firstIndex + 1; sweepIndex < radarData->sweepBufferCount; sweepIndex++) {
			RadarData::SweepInfo &info2 = radarData->sweepInfo[sweepIndex];
			if(phi <= info2.elevation){
				RadarData::SweepInfo &info1 = radarData->sweepInfo[sweepIndex - 1];
				// set phi to sweep index
				phi = (phi - info1.elevation) / (info2.elevation - info1.elevation) + 0.5f + sweepIndex;
				break;
			}
		}
	}
	bool isInVolume = radius >= 0 && radius < radarData->radiusBufferCount && isfinite(phi);
	float pixelThetaWidth = (radius + radarData->stats.innerDistance) * radarData->stats.pixelSize * 2 * M_PI / (float)radarData->thetaBufferCount;
	
	
	PyObject* outDict = PyDict_New();
	// is the input location in the volume
	PyObject_SetItem(outDict, PyUnicode_FromString("is_in_volume"), PyBool_FromLong(isInVolume));
	// index of sweep, take floor for pixel in buffer
	PyObject_SetItem(outDict, PyUnicode_FromString("sweep"), PyFloat_FromDouble(phi));
	// index of theta, take floor for pixel in buffer
	PyObject_SetItem(outDict, PyUnicode_FromString("theta"), PyFloat_FromDouble(theta));
	// index of radius, take floor for pixel in buffer
	PyObject_SetItem(outDict, PyUnicode_FromString("radius"), PyFloat_FromDouble(radius));
	// length of the pixel along the radius in meters
	PyObject_SetItem(outDict, PyUnicode_FromString("pixel_radius_length"), PyFloat_FromDouble(radarData->stats.pixelSize));
	// approximate width of the pixel along the theta in meters
	PyObject_SetItem(outDict, PyUnicode_FromString("pixel_theta_width"), PyFloat_FromDouble(pixelThetaWidth));
	
	return outDict;
}

int radarBufferObjectGetBuffer(PyObject* exporter, Py_buffer* view, int flags){
	
	Py_buffer* bufferObj = view;//new Py_buffer();
	RadarBufferObject *radarBufferObject = (RadarBufferObject *)exporter;
	RadarData* radarData = radarBufferObject->radarData;
	if(radarData == NULL){
		PyErr_SetString(PyExc_BufferError, "radarData is NULL");
		return -1;
	}
	if(radarData->buffer == NULL){
		PyErr_SetString(PyExc_BufferError, "buffer is NULL");
		return -1;
	}
	bufferObj->obj = exporter;
	bufferObj->obj->ob_refcnt++;
	// skip padding rays
	bufferObj->buf = radarData->buffer;
	bufferObj->len = radarData->fullBufferSize * sizeof(float);
	bufferObj->ndim = 3;
	bufferObj->readonly = PyBUF_WRITABLE;
	bufferObj->itemsize = sizeof(float);
	bufferObj->format = "f";
	bufferObj->shape = new Py_ssize_t[3]{};
	bufferObj->shape[0] = radarData->sweepBufferCount;
	bufferObj->shape[1] = radarData->thetaBufferCount + 2;
	bufferObj->shape[2] = radarData->radiusBufferCount;
	bufferObj->strides = new Py_ssize_t[3]{};
	// skip padding rays
	bufferObj->strides[0] = radarData->sweepBufferSize * sizeof(float);
	bufferObj->strides[1] = radarData->thetaBufferSize * sizeof(float);
	bufferObj->strides[2] = 1 * sizeof(float);
	bufferObj->suboffsets = NULL;
	return 0;
}
void radarBufferObjectBufferFree(PyObject* exporter, Py_buffer* view){
	delete view->shape;
	delete view->strides;
}
static PyBufferProcs RadarBuffer_BufferProcs = {
	radarBufferObjectGetBuffer,
	radarBufferObjectBufferFree
};