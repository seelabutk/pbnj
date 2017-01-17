#include "Camera.h"

#include <cmath>

#include <ospray/ospray.h>

namespace pbnj {

Camera::Camera(int width, int height) :
    imageWidth(width), imageHeight(height), xPos(0.0), yPos(0.0), zPos(0.0),
    viewX(0.0), viewY(0.0), viewZ(0.0), orbitRadius(0.0)
{
    this->oCamera = ospNewCamera("perspective");
    this->updateOSPRayPosition();
    ospSet3fv(this->oCamera, "up",  (float[]){0.0, 1.0, 0.0});
    ospSetf(this->oCamera, "aspect", (float)this->imageWidth/imageHeight);
    ospCommit(this->oCamera);
}

void Camera::setOrbitRadius(float radius)
{
    this->orbitRadius = radius;
    this->updateOSPRayPosition();
}

void Camera::setPosition(float x, float y, float z)
{
    this->xPos = x;
    this->yPos = y;
    this->zPos = z;
    this->updateOSPRayPosition();
}

void Camera::updateOSPRayPosition()
{
    float deltaX = (this->xPos-this->viewX);
    float deltaY = (this->yPos-this->viewY);
    float deltaZ = (this->zPos-this->viewZ);
    float mag = std::sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
    this->viewX = -deltaX/mag;
    this->viewY = -deltaY/mag;
    this->viewZ = -deltaZ/mag;

    ospSet3fv(this->oCamera, "pos",
            (float[]){this->xPos, this->yPos, this->zPos});
    ospSet3fv(this->oCamera, "dir",
            (float[]){this->viewX, this->viewY, this->viewZ});
    ospCommit(this->oCamera);
}

OSPCamera Camera::asOSPRayObject()
{
    return this->oCamera;
}

}
