PBNJ Classes
============

Camera
------
A camera object that encapsulates an OSPRay camera.

ConfigReader
------------
A file reader that extracts JSON data from a given filename. This
is an internal class used by Configuration.

Configuration
-------------
A JSON config file parser. JSON parsing is done using the rapidjson
library.

DataFile
--------
A basic dataset abstraction class that handles reading data from
a file. This currently supports reading from single-variable float
binary files and from NetCDF files. This is an internal class used
by Volume.

Renderer
--------
A rendering object that encapsulates an OSPRay renderer and handles
framebuffers/images.

TimeSeries
----------
A time-varying dataset management class. These objects will attempt
to maintain memory usage within a configurable threshold when working
with multiple time steps.

TransferFunction
----------------
An abstraction layer on top of OSPRay's transfer function data types.
This class handles both color and opacity maps. Some basic maps are
provided.

Volume
------
An abstraction class encapsulating OSPRay's volume. This class holds
rendering attributs for the data, and uses DataFile to handle I/O.
