#ifndef PBNJ_RENDERER_H
#define PBNJ_RENDERER_H

#include <pbnj.h>

#include <string>
#include <vector>

#include <ospray/ospray.h>

namespace pbnj {

    enum IMAGETYPE {INVALID, PIXMAP, PNG};

    class Renderer {
        public:
            Renderer();
            ~Renderer();

            void setBackgroundColor(char r, char g, char b);
            void setVolume(Volume *v);
            void setCamera(Camera *c);
            void setSamples(unsigned int spp);

            void render();
            void renderToBuffer(unsigned char **buffer);
            void renderToPNGObject(std::vector<unsigned char> &png);
            void renderImage(std::string imageFilename);

            int cameraWidth;
            int cameraHeight;
        private:
            char backgroundColor[3];

            OSPRenderer oRenderer;
            OSPFrameBuffer oFrameBuffer;
            OSPModel oModel;
            OSPCamera oCamera;

            IMAGETYPE getFiletype(std::string filename);
            void saveImage(std::string filename, IMAGETYPE imageType);

            void saveAsPPM(std::string filename);
            void saveAsPNG(std::string filename);
            void bufferToPNG(std::vector<unsigned char> &png);

            std::string lastVolumeID;
            std::string lastCameraID;
    };
}

#endif
