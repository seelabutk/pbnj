# Peanut Butter N Jelly

Thin abstraction layer on top of OSPRay for use with
Rendering as a Service project

Will provide the following:

* Data abstraction object
    * holds metadata of data file being requested
    * holds various statistics, such as min/max/mean/stdev/etc.
* Volume abstraction
    * simpler access to OSPRay volumes
* Transfer function abstraction
    * easier creation/editing of transfer functions
    * combines color and opacity functions into one object
* Camera abstraction
    * easier movement of camera
* Renderer abstraction
* Image writer
* Config file reader, probably YAML via yaml-cpp

