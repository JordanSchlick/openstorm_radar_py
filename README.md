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
See [libtest_output.ipynb](./libtest_output.ipynb) for examples. [libtest.py](./libtest.py) also has some examples but it is not commented.  
See [__init\__.py](./__init__.py) for documentation of classes and methods.  
The python api mirrors the c++ one closely except for the variable naming conversion.
The [OpenStorm .h files](https://github.com/JordanSchlick/OpenStorm/tree/main/Source/OpenStorm/Radar) are well documented. [RadarData.h](https://github.com/JordanSchlick/OpenStorm/tree/main/Source/OpenStorm/Radar/RadarData.h) is especially helpfully.

## Building
The OpenStorm source code is a dependency and is required for building. The OpenStorm repository needs to be placed in the same directory as this module is located.  
Ex:  
```
Folder  
  ├OpenStorm  
  └openstorm_radar_py  
```
Run `git clone https://github.com/JordanSchlick/openstorm_radar_py` and `git clone https://github.com/JordanSchlick/OpenStorm` in the same directory do download them both.  
Your system needs to be have an environment capable of building native python modules.  
Run `python setup.py build` inside openstorm_radar_py to build the module.

If the build completes successfully you should be able to import and use the module in python with `import openstorm_radar_py`  

You may need to add the containing folder to your include path before you can import it depending on where it is located.
```
import sys
sys.path.insert(0, './path/to/Folder')
import openstorm_radar_py
```