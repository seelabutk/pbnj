#include "Camera.h"

#include <cmath>
#include <iostream>

#include <ospray/ospray.h>

namespace pbnj {

Camera::Camera(int width, int height) :
    imageWidth(width), imageHeight(height), xPos(0.0), yPos(0.0), zPos(0.0),
    viewX(0.0), viewY(0.0), viewZ(0.0), orbitRadius(0.0)
{
    //setup OSPRay camera with basic parameters
    this->oCamera = ospNewCamera("perspective");
    this->updateOSPRayPosition();
    float temp[] = {0.0, 1.0, 0.0};
    ospSet3fv(this->oCamera, "up",  temp);
    ospSetf(this->oCamera, "aspect", (float)this->imageWidth/imageHeight);
    ospCommit(this->oCamera);
}

void Camera::setOrbitRadius(float radius)
{
    //for use with paths
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

void Camera::centerView(Volume *v)
{
    //center the camera's viewpoint on the center of a volume
    std::vector<int> bounds = v->getBounds();
    this->viewX = (float)(bounds[0])/2.0;
    this->viewY = (float)(bounds[1])/2.0;
    this->viewZ = (float)(bounds[2])/2.0;
}

void Camera::updateOSPRayPosition()
{
    //calculate the components for the view vector
    float deltaX = (this->viewX-this->xPos);
    float deltaY = (this->viewY-this->yPos);
    float deltaZ = (this->viewZ-this->zPos);

    //update OSPRay camera
    float temp[] = {this->xPos, this->yPos, this->zPos};
    ospSet3fv(this->oCamera, "pos", temp);
    float temp2[] = {deltaX, deltaY, deltaZ};
    ospSet3fv(this->oCamera, "dir", temp2);
    ospCommit(this->oCamera);
}

OSPCamera Camera::asOSPRayObject()
{
    return this->oCamera;
}

}
