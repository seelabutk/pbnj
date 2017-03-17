# Peanut Butter N Jelly

Thin abstraction layer on top of OSPRay 

Provides the following:

* Data/volume abstraction object
    * we're focusing on volume rendering
    * have one point of contact for data, a Volume object
    * Volume objects hold:
        * DataFile - underlying representation of the loaded data, metadata,
        statistics, etc. Can read binary and NetCDF files
        * TransferFunction - container for color and opacity maps, attenuation
* Camera abstraction
    * easier movement of camera
    * direct placement of camera with one function, `setPosition()`
* Renderer abstraction
    * can directly create images (PPM or PNG)
    * can save to given buffer
* JSON-based config file
    * communicate with web applications, e.g. Enchiladas

Each PBNJ object (apart from the Renderer at the moment) is designed to be
initialized once and reused. This is the base for Enchiladas, as a Volume,
Camera, and Renderer can be initialized once. Then a stream of images can be
sent to a client.

The example application `simpleVolumeRender` demonstrates using PBNJ with a
JSON config file.

