``Configuration`` class
=======================

This class reads from a provided JSON configuration file. An instance of this
class will hold values from valid keywords in the config file that can be used
to set values in or instantiate other PBNJ classes. By configuring the
parameters of a render, a single application can quickly create images for a
preconfigured visualization.

.. cpp:class:: pbnj::Configuration

    .. cpp:function:: Configuration(std::string filename)

       The ``Configuration`` class constructor. The filename given should
       point to a valid JSON file. This JSON file is parsed for various
       keywords. Any invalid keywords are ignored, and the order in which
       the keywords appear does not matter. There are four required
       keywords, shown in bold below. All others are optional.

       +-----------------------------+-----------------------------------------+-----------------------------+
       | Keyword                     | Possible values                         | Default value               |
       +=============================+=========================================+=============================+
       | **filename**                | A valid absolute path to a single file  | N/A                         |
       |                             | or an absolute path with wildcards for  |                             |
       |                             | multiple volumes. See the Volume class  |                             |
       |                             | for data types that can be read         |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | **dimensions**              | A 3-element array containing the        | N/A                         |
       |                             | dataset's extent                        |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | **imageSize**               | A 2-element array containing the image  | N/A                         |
       |                             | width and height                        |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | **outputImageFilename**     | An absolute path to where the rendered  | N/A                         |
       |                             | image(s) should be saved                |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | backgroundColor             | A 4-element array describing a color in | [0, 0, 0, 1] (black)        |
       |                             | RGBA format                             |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | cameraPosition              | A 3-element array describing where the  | [0, 0, 0] (origin)          |
       |                             | camera should be placed in world space  |                             |
       |                             | coordinates                             |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | cameraUpVector              | A 3-element array describing the        | [0, 1, 0] (+y-axis)         |
       |                             | camera's "up" direction                 |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | colorMap                    | One of the provided color maps listed   | grayscale                   |
       |                             | below                                   |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | dataVariable                | A valid variable name from the dataset  | the first available         |
       |                             | (for NetCDF files)                      | variable in the NetCDF file |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | isosurfaceValues            | An array of values within the dataset's | none, will do a volume      |
       |                             | min and max values (inclusive) for which| render                      |
       |                             | to render isocontours. This should be an|                             |
       |                             | array even if there is only one isovalue|                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | opacityAttenuation          | A single float value in [0, 1] used to  | 1.0                         |
       |                             | dampen the opacity map                  |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | opacityMap                  | One of the provided opacity maps listed | ramp                        |
       |                             | below, or an array of float values in   |                             |
       |                             | [0, 1]                                  |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | samplesPerPixel             | A single positive integer describing how| 1                           |
       |                             | many rays OSPRay should trace through   |                             |
       |                             | each pixel of the output image          |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+
       | specular                    | A single float value in [0, 1] to set   | 0.1                         |
       |                             | how much specular highlights the surface|                             |
       |                             | material will have. This will only have |                             |
       |                             | an affect if ``isosurfaceValues`` is    |                             |
       |                             | also used                               |                             |
       +-----------------------------+-----------------------------------------+-----------------------------+

       Some color maps have synonymous names that you can use in a
       configuration file. This is done to avoid localized spelling
       differences and to avoid remembering the specific phrasing of
       a name.

       +----------------------+
       | Color map            |
       +======================+
       | "grayscale"          |
       | ("greyscale,         |
       | "xray",              |
       | "x-ray",             |
       | "blackToWhite")      |
       +----------------------+
       | "cool to warm"       |
       | ("coolToWarm")       |
       +----------------------+
       | "reverse spectral"   |
       | ("spectral reverse", |
       | "reverseSpectral",   |
       | "spectralReverse")   |
       +----------------------+
       | "magma"              |
       +----------------------+
       | "viridis"            |
       +----------------------+

       Similarly, some opacity maps have synonymous names. Note that
       custom arrays of float values can be used instead of the
       provided maps.

       +------------------------+
       | Opacity map            |
       +========================+
       | "ramp"                 |
       +------------------------+
       | "reverse ramp"         |
       | ("reverseRamp")        |
       +------------------------+
       | "tents"                |
       | ("tent",               |
       | "teeth")               |
       +------------------------+
       | "exponential"          |
       +------------------------+
       | "reverse exponential"  |
       | ("reverseExponential") |
       +------------------------+
       | "flat"                 |
       +------------------------+

    .. cpp:member:: std::string dataFilename

       The path to the dataset to render. This should be passed to Volume.
       This is set by ``filename`` in the configuration file.

    .. cpp:member:: std::vector<std::string> globbedFilenames

       A vector of paths to multiple datasets, usually for time steps.
       This should be passed to TimeSeries.
       This is set by ``filename`` in the configuration file, if the
       provided path contains a wildcard.

    .. cpp:member:: std::string dataVariable

       Which variable (in a NetCDF dataset) to use.
       This is set by ``dataVariable`` in the configuration file.

    .. cpp:member:: int dataXDim

       The x-axis extent of the dataset. This should be passed to either
       Volume or TimeSeries. The Configuration's state can be used to
       determine which to use.
       This is set by ``dimensions`` in the configuration file.

    .. cpp:member:: int dataYDim

       The y-axis extent of the dataset. This should be passed to either
       Volume or TimeSeries. The Configuration's state can be used to
       determine which to use.
       This is set by ``dimensions`` in the configuration file.

    .. cpp:member:: int dataZDim

       The z-axis extent of the dataset. This should be passed to either
       Volume or TimeSeries. The Configuration's state can be used to
       determine which to use.
       This is set by ``dimensions`` in the configuration file.

    .. cpp:member:: int imageWidth

       The width of the output image. This should be passed to the Camera.
       This is set by ``imageSize`` in the configuration file

    .. cpp:member:: int imageHeight

       The height of the output image. This should be passed to the Camera.
       This is set by ``imageSize`` in the configuration file

    .. cpp:member:: std::string imageFilename

       The path to save the output image. This should be passed to the
       Renderer's ``render()`` function.
       This is set by ``outputImageFilename`` in the configuration file.

    .. cpp:member:: std::vector<unsigned char> bgColor

       A 3-element vector containing the background color. This should be
       passed to the Renderer's ``setBackgroundColor()`` function.
       This is set by ``backgroundColor`` in the configuration file.

    .. cpp:member:: std::vector<float> colorMap

       A vector containing a sequence of RGB values. This should be passed
       to the Volume's ``setColorMap()`` function.
       This is set by ``colorMap`` in the configuration file.

    .. cpp:member:: std::vector<float> opacityMap

       A vector containing a sequence of values. This should be passed
       to the Volume's ``setOpacityMap()`` function.
       This is set by ``opacityMap`` in the configuration file.

    .. cpp:member:: float opacityAttenuation

       The attenuation value. This should be passed to the Volume's
       ``attenuateOpacity()`` function.
       This is set by ``opacityAttenuation`` in the configuration file.

    .. cpp:member:: unsigned int samples

       The number of samples OSPRay should take at each pixel. This should be
       passed to the Renderer's ``setSamples()`` function.
       This is set by ``samplesPerPixel`` in the configuration file.

    .. cpp:member:: float cameraX

       The camera's x-position in world space. This should be passed to the
       Camera's ``setPosition`` function.
       This is set by ``cameraPosition`` in the configuration file.

    .. cpp:member:: float cameraY

       The camera's y-position in world space. This should be passed to the
       Camera's ``setPosition`` function.
       This is set by ``cameraPosition`` in the configuration file.

    .. cpp:member:: float cameraZ

       The camera's z-position in world space. This should be passed to the
       Camera's ``setPosition`` function.
       This is set by ``cameraPosition`` in the configuration file.

    .. cpp:member:: float cameraUpX

       The x-component of the camera's up vector. This should be passed to the
       Camera's ``setUpVector`` function.
       This is set by ``cameraUpVector`` in the configuration file.

    .. cpp:member:: float cameraUpY

       The y-component of the camera's up vector. This should be passed to the
       Camera's ``setUpVector`` function.
       This is set by ``cameraUpVector`` in the configuration file.

    .. cpp:member:: float cameraUpZ

       The z-component of the camera's up vector. This should be passed to the
       Camera's ``setUpVector`` function.
       This is set by ``cameraUpVector`` in the configuration file.

    .. cpp:member:: std::vector<float> isosurfaceValues

       The value(s) for which to render isocontours. This should be passed to
       the Renderer's ``setIsosurface`` function.
       This is set by ``isosurfaceValues`` in the configuration file.

    .. cpp:member:: float specularity

       The amount of specular reflection that the isosurface material will
       have. Note that the amount of diffuse reflection is equal to one
       minus this value. This should be passed to the Renderer's
       ``setIsosurface`` function.
       This is set by ``specular`` in the configuration file.

    .. cpp:function:: pbnj::CONFSTATE getConfigState()

       This function will return one of six values depending on the combination
       of parameters found in the configuration file.
       The possible ``CONFSTATE`` values are:

       ``SINGLE_NOVAR``
            A single dataset was given with no variable name specified

       ``SINGLE_VAR``
            A single dataset was given with a specified variable name

       ``MULTI_NOVAR``
            Multiple datasets were given (i.e. a path with wildcards was
            provided) with no variable name specified

       ``MULTI_VAR``
            Multiple datasets were given with a specified variable name

       ``ERROR_NODATA``
            No filename(s) were given

       ``ERROR_MULTISET``
            Both a single dataset and multiple datasets were provided
            (i.e. the ``filename`` keyword was duplicated with different
            values)

Example
-------

A simple JSON file may look like the following:

.. code-block:: javascript

    {
        "filename" : "/some/path/to/a/dataset.nc",
        "dimensions" : [100, 200, 300],
        "imageSize" : [512, 512],
        "outputImageFilename" : "/some/path/for/the/image.png",
        "colorMap" : "viridis",
        "cameraPosition" : [100, -600, 400]
    }


