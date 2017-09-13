#include "Camera.h"

#include <cmath>
#include <iostream>

#include <ospray/ospray.h>

namespace pbnj {

Camera::Camera(int width, int height) :
    imageWidth(width), imageHeight(height), xPos(0.0), yPos(0.0), zPos(0.0),
    viewX(0.0), viewY(0.0), viewZ(0.0), orbitRadius(0.0)
{
    this->ID = createID();
    //setup OSPRay camera with basic parameters
    this->oCamera = ospNewCamera("perspective");
    this->updateOSPRayPosition();
    ospSetf(this->oCamera, "aspect", (float)this->imageWidth/imageHeight);
    ospCommit(this->oCamera);
}

Camera::~Camera()
{
    ospRelease(this->oCamera);
}

void Camera::setOrbitRadius(float radius)
{
    //for use with paths
    this->orbitRadius = radius;
    this->updateOSPRayPosition();
}

void Camera::setUpVector(float x, float y, float z)
{
    this->upX = x;
    this->upY = y;
    this->upZ = z;    
    this->updateOSPRayPosition();
}

void Camera::setPosition(float x, float y, float z)
{
    this->xPos = x;
    this->yPos = y;
    this->zPos = z;
    this->updateOSPRayPosition();
}

void Camera::setView(float x, float y, float z)
{
    this->viewX = x;
    this->viewY = y;
    this->viewZ = z;
    this->updateOSPRayPosition();
}

void Camera::centerView()
{
    //center the camera's viewpoint on the center of a volume
    //std::vector<int> bounds = v->getBounds();
    this->viewX = -this->xPos; //(float)(bounds[0])/2.0;
    this->viewY = -this->yPos; //(float)(bounds[1])/2.0;
    this->viewZ = -this->zPos; //(float)(bounds[2])/2.0;
}

void Camera::updateOSPRayPosition()
{
    //update OSPRay camera
    float position[] = {this->xPos, this->yPos, this->zPos};
    ospSet3fv(this->oCamera, "pos", position);

    float direction[] = {this->viewX, this->viewY, this->viewZ};
    ospSet3fv(this->oCamera, "dir", direction);

    float up[] = {this->upX, this->upY, this->upZ};
    ospSet3fv(this->oCamera, "up",  up);
    ospCommit(this->oCamera);
}

OSPCamera Camera::asOSPRayObject()
{
    return this->oCamera;
}

}
