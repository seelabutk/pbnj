``Renderer`` class
==================

This class wraps around the OSPRay renderer class and abstracts its
functionality. The details of setting up the underlying OSPRay renderer
are handled for the user so that only high-level concepts are needed to
generate images. Currently this class always uses the OSPRay ``scivis``
renderer.

.. cpp:class:: pbnj::Renderer

    .. cpp:function:: Renderer()

       Constructor, creates a new ``scivis`` OSPRay renderer. It takes no
       arguments. By default, the background color is set to black and the
       number of samples per pixel is set to 1

    .. cpp:function:: void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b)

       Set the background color of the rendered image. ``r``, ``g``, and ``b``
       should all be in [0, 255]

    .. cpp:function:: void setBackgroundColor(std::vector<unsigned char> bgColor)

       Set the background color of the renderer image. ``bgColor`` should have
       three elements, each in [0, 255]

    .. cpp:function:: void setVolume(pbnj::Volume *v)

       Give the Renderer a ``Volume`` to render. Only one ``Volume`` object can be set
       at a time. Each subsequent call to this function will overwrite the
       previously set ``Volume`` object. If the ``Volume`` object being set is the same
       as the current one, this function will do nothing. This function
       **must** be called before rendering an image

    .. cpp:function:: void addLight()

       Add a light to the scene. This is only used for isosurface renders at
       the moment. A single directional light will be created. The direction of
       the light will be set to the ``Camera`` object's view direction. If a light
       already exists in the scene, this function will do nothing

    .. cpp:function:: void setIsosurface(pbnj::Volume *v, std::vector<float> &isoValues)

       Set one or more isosurface values for the ``Volume`` ``v``. ``isoValues``
       should contain one or more scalar values that exist within the range of
       the dataset's minimum and maximum values. This should be used instead of
       ``setVolume()`` to do an isosurface render. The rendered surface will
       have a specular reflection factor of 0.1 and diffuse reflection of 0.9

    .. cpp:function:: void setIsosurface(pbnj::Volume *v, std::vector<float> &isoValues, float specular)

       Set one or more isosurface values for the ``Volume`` ``v``. ``isoValues``
       should contain one or more scalar values that exist within the range of
       the dataset's minimum and maximum values. This should be used instead of
       ``setVolume()`` to do an isosurface render. ``specular`` should be in
       [0, 1] and will be used as the specular reflection factor. The diffuse
       reflection factor will be one minus ``specular``.

    .. cpp:function:: void setCamera(pbnj::Camera *c)

       Give the Renderer a ``Camera`` to render from. Only one ``Camera`` object can be
       set at a time. Each subsequent call to this function will overwrite the
       previously set ``Camera`` object. If the ``Camera`` object being set is the same
       as the current one, this function will do nothing. This function
       **must** be called before rendering and image

    .. cpp:function:: void setSamples(unsigned int spp)

       Set the number of samples per pixel that OSPRay will use when rendering
       the image. This is by default 1, though ``spp`` can be any positive
       integer. The higher the value, the better quality the render will be at
       the cost of render time. There are diminishing returns for increasingly
       higher samples per pixel

    .. cpp:function:: void render()

       Render an image to the OSPRay framebuffer. Both ``setVolume()`` and
       ``setCamera()`` **must** be called before calling this function

    .. cpp:function:: void renderToBuffer(unsigned char **buffer)

       Render an image to the OSPRay framebuffer and copy it to the given
       buffer. This function will composite the rendered image onto the
       background color before copying to the buffer. The buffer should be
       ``height``x``width`` elements long, where ``height`` and ``width`` are
       the dimensions given to the ``Camera`` object. Both ``setVolume()`` and
       ``setCamera()`` **must** be called before calling this function

    .. cpp:function:: void renderToPNGObject(std::vector<unsigned char> &png)

       Render an image to the OSPRay framebuffer and copy it to a PNG-formatted
       vector. ``png`` will contain the binary data necessary to dump to a PNG
       file or to transfer as data (as done in Tapestry). PNG encoding is done
       with the `LodePNG library <http://lodev.org/lodepng>`_. This function
       will composite the rendered image onto the background color before
       encoding to PNG. Both ``setVolume()`` and ``setCamera()`` **must** be
       called before calling this function

    .. cpp:function:: void renderImage(std::string imageFilename)

       Render an image to the OSPRay framebuffer and save it to disk at the
       path provided. PBNJ will read the file extension provided in
       ``imageFilename`` to determine which filetype to use. Currently only PNG
       and PPM files are supported. If the filetype is PNG, encoding is done
       with the `LodePNG library <http://lodev.org/lodepng>`_. This function
       will composite the rendered image onto the background color before
       saving. Both ``setVolume()`` and ``setCamera()`` **must** be called
       before calling this function

    .. cpp:member:: int cameraWidth

       The width of the image that will be rendered, as provided by a
       ``Camera`` object provided in ``setCamera()``

    .. cpp:member:: int cameraHeight

       The height of the image that will be rendered, as provided by a
       ``Camera`` object provided in ``setCamera()``
