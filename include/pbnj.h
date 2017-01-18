#ifndef PBNJ_H
#define PBNJ_H

#include <ospray/ospray.h>

namespace pbnj {
    /* contains metadata about the data being requested
     * This includes things like:
     *  - filename
     *  - file type
     *  - data type
     *  - dimensions
     *  - min/max
     *  etc.
     */
    class DataFile;

    /* abstraction wrapper around OSPRay volumes */
    class Volume;

    /* abstraction wrapper around OSPRay transfer functions
     * combines color and opacity tfs into a single object
     */
    class TransferFunction;

    /* abstraction wrapper around OSPRay renderer */
    class Renderer;

    /* abstraction wrapper around OSPRay camera
     * Provides simplified camera movement
     */
    class Camera;

    /* configuration class, uses rapidjson to parse JSON
     * config files
     */
    class ConfigReader;

    class Configuration;

    void pbnjInit(int *argc, const char **argv);
}

#endif
