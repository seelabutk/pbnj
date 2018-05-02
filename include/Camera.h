#ifndef PBNJ_CAMERA_H
#define PBNJ_CAMERA_H

#include <pbnj.h>
#include <Volume.h>

#include <ospray/ospray.h>

#include <string>

namespace pbnj {

    class Camera {
        public:
            Camera(int width, int height);
            ~Camera();

            void setPosition(float x, float y, float z);
            void setUpVector(float x, float y, float z);
            void setOrbitRadius(float radius);
            void setView(float x, float y, float z);
            void setImageSize(int width, int height);
            int getImageWidth();
            int getImageHeight();
            // no longer needed as the volume is centered automatically
            void centerView();

            OSPCamera asOSPRayObject();

            //some sort of setPath function that takes an enum type for the path
            //and a ... for path parameters

            float viewX;
            float viewY;
            float viewZ;

            std::string ID;

        private:
            float xPos;
            float yPos;
            float zPos;
            float upX;
            float upY;
            float upZ;
            float orbitRadius;
            int imageWidth;
            int imageHeight;

            OSPCamera oCamera;

            void updateOSPRayPosition();
    };
}

#endif
