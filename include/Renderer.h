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

            void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b);
            void setBackgroundColor(std::vector<unsigned char> bgColor);
            void setVolume(Volume *v);
            void setIsosurface(Volume *v, std::vector<float> &isoValues);
            void setCamera(Camera *c);
            void setSamples(unsigned int spp);

            void render();
            void renderToBuffer(unsigned char **buffer);
            void renderToPNGObject(std::vector<unsigned char> &png);
            void renderImage(std::string imageFilename);

            int cameraWidth;
            int cameraHeight;
        private:
            unsigned char backgroundColor[3];

            OSPRenderer oRenderer;
            OSPFrameBuffer oFrameBuffer;
            OSPModel oModel;
            OSPCamera oCamera;
            OSPGeometry oSurface;
            OSPMaterial oMaterial;

            IMAGETYPE getFiletype(std::string filename);
            void saveImage(std::string filename, IMAGETYPE imageType);

            void saveAsPPM(std::string filename);
            void saveAsPNG(std::string filename);
            void bufferToPNG(std::vector<unsigned char> &png);

            std::string lastVolumeID;
            std::string lastCameraID;
            std::string lastRenderType;

            std::vector<OSPLight> lights;
            unsigned int samples;
    };
}

#endif
