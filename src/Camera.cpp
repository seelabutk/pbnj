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
    ospRemoveParam(this->oCamera, "aspect");
    ospRemoveParam(this->oCamera, "pos");
    ospRemoveParam(this->oCamera, "dir");
    ospRemoveParam(this->oCamera, "up");
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
    this->setView(-this->xPos, -this->yPos, -this->zPos);
}

void Camera::setImageSize(int width, int height)
{
    this->imageWidth  = width;
    this->imageHeight = height;
}

int Camera::getImageWidth()
{
    return this->imageWidth;
}

int Camera::getImageHeight()
{
    return this->imageHeight;
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

void Camera::setRegion(float top, float right, float bottom, float left)
{
    // render only a defined region of the current view (set clip space)
    // to be used with tiles
    float upperRight[] = {top, right};
    float lowerLeft[] = {bottom, left};
    ospSet2fv(this->oCamera, "imageStart", lowerLeft);
    ospSet2fv(this->oCamera, "imageEnd", upperRight);
}

OSPCamera Camera::asOSPRayObject()
{
    return this->oCamera;
}

}
