#ifndef PBNJ_CAMERA_H
#define PBNJ_CAMERA_H

#include <pbnj.h>
#include <Volume.h>

#include <ospray/ospray.h>

namespace pbnj {

    class Camera {
        public:
            Camera(int width, int height);
            ~Camera();

            void setPosition(float x, float y, float z);
            void setUpVector(float x, float y, float z);
            void setOrbitRadius(float radius);
            // no longer needed as the volume is centered automatically
            void centerView();

            OSPCamera asOSPRayObject();

            //some sort of setPath function that takes an enum type for the path
            //and a ... for path parameters

            int imageWidth;
            int imageHeight;

        private:
            float xPos;
            float yPos;
            float zPos;
            float viewX;
            float viewY;
            float viewZ;
            float upX;
            float upY;
            float upZ;
            float orbitRadius;

            OSPCamera oCamera;

            void updateOSPRayPosition();
    };
}

#endif
