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

To enable basic functionality:
- read a JSON file using the Configuration class
- create a Volume object and populate its required information
  with parameters from the Configuration
- similarly create Camera and Renderer object populated with
  Configuration parameters
- call Renderer::render()
