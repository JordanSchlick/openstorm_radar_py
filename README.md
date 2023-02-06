# OpenStorm Radar Python Library
This library provides OpenStorm's radar processing capabilities to python.
It uses the c++ files in the Radar source directory of OpenStorm.
The RadarData and RadarDataHolder classes are exposed to python. RadarData is the class that contains the actual data.
The data can be accessed by converting radardata.buffer into a numpy array.
The rays are automatically interpolated to provide a uniform dense array of radar data.
The first dimension is the sweep the second is the rotation and the third is the distance away from the radar.
You will see the dimensions refereed to as sweep, theta, and radius in code.
There are 2 duplicated rays in each sweep that are used for padding for interpolation. In most cases they should be ignored or removed.
RadarDataHolder is a more advanced container for RadarData objects and supports loading multiple products as well as computing derived products. 
RadarDataHolder is multi threaded and asynchronous.

## Documentation
See [libtest.ipynb](./libtest.ipynb) for examples. [libtest.py](./libtest.py) also has some examples but it is not commented.  
See [__init\__.py](./__init__.py) for docmuntation of classes and methods.  
The python api mirror the c++ one closely except for the variable naming convesion.
[OpenSource .h files](https://github.com/JordanSchlick/OpenStorm/tree/main/Source/OpenStorm/Radar) are well documented. [RadarData.h](https://github.com/JordanSchlick/OpenStorm/tree/main/Source/OpenStorm/Radar/RadarData.h) is especially helpfully.

## Building
OpenStorm is a dependency and is required for building. It needs to be placed in the same directory as this module is located. Ex:  
```
Folder  
  ├OpenStorm  
  └openstorm_radar_py  
```
Your system needs to be have an environment capable of build native python modules.  
Run `python setup.py build` to build the module.