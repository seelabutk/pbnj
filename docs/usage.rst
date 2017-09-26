PBNJ Usage
==========

Description
-----------

PBNJ (Peanut Butter N Jelly) is a library that wraps around the OSPRay 
ray tracing rendering engine to quickly produce images using simple
configuration files.

PBNJ is a fairly simple library with which to build applications. The
core functionality is provided by the Volume, Camera, and Renderer
classes. These abstract away a lot of details that are not needed in
most cases, instead providing "sane" defaults. By using the Configuration
class and a simple JSON config file, you can render images of a dataset
quickly.

PBNJ forms the rendering component of the Tapestry project.

Usage
-----

To enable basic functionality, you will need to use these four classes:

* Configuration
* Volume
* Camera
* Renderer

Possibly the simplest code to create an image would be the following:

.. code-block:: cpp

    #include "pbnj.h"
    #include "Camera.h"
    #include "Configuration.h"
    #include "Renderer.h"
    #include "Volume.h"

    int main(int argc, char **argv)
    {
        // read a valid JSON configuration file passed as an argument
        pbnj::Configuration *config = new pbnj::Configuration(argv[1]);
        // initialize PBNJ (this initializes OSPRay as well)
        pbnj::pbnjInit(&argc, argv);

        // use parameters read by the Configuration object to initialize the volume
        pbnj::Volume *volume = new pbnj::Volume(config->dataFilename,
                config->dataXDim, config->dataYDim, config->dataZDim);

        // similarly, use the Configuration object to help initialize the camera
        pbnj::Camera *camera = new pbnj::Camera(config->imageWidth, 
                config->imageHeight);
        // set the camera position in world space
        camera->setPosition(config->cameraX, config->cameraY, config->cameraZ);
        // center the view on the dataset's centroid
        camera->centerView();

        // create a new Renderer object
        pbnj::Renderer *renderer = new pbnj::Renderer();
        // and give it the camera and volume
        renderer->setCamera(camera);
        renderer->setVolume(volume);

        // render an image!
        renderer->renderImage(config->imageFilename);

        return 0;
    }

You will additionally need a JSON configuration file to pass to the example
program. A basic config file would look like the following:

.. code-block:: javascript

    {
        "filename" : "/absolute/path/to/dataset",
        "dimensions" : [100, 200, 300],
        "imageSize" : [512, 512],
        "outputImageFilename" : "/absolute/path/to/save/image.png"
    }

This will create a grayscale volume render of the dataset at the location
given in the ``filename`` field.

The ``simpleVolumeRender`` example application contains more comprehensive
code for checking and setting all possible parameters. The full list of valid
configuration parameters are listed in the Configuration class documentation.
