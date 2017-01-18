#ifndef PBNJ_RENDERER_H
#define PBNJ_RENDERER_H

#include <pbnj.h>

#include <string>

#include <ospray/ospray.h>

namespace pbnj {

    enum IMAGETYPE {INVALID, PIXMAP};

    class Renderer {
        public:
            Renderer();

            void setBackgroundColor(char r, char g, char b);
            void setVolume(Volume *v);
            void setCamera(Camera *c);

            void renderImage(std::string imageFilename);

        private:
            char backgroundColor[3];

            int cameraWidth;
            int cameraHeight;

            //these NEED to be pointers soon
            OSPRenderer oRenderer;
            OSPFrameBuffer oFrameBuffer;
            OSPModel oModel;
            OSPCamera oCamera;

            IMAGETYPE getFiletype(std::string filename);
            void saveImage(std::string filename, IMAGETYPE imageType);

            void saveAsPPM(std::string filename);
    };
}

#endif
