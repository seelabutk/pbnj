#include "Camera.h"
#include "Renderer.h"
#include "Volume.h"

#include <iostream>
#include <sstream>
#include <string>

#include <stdlib.h>
#include <stdio.h>

#include <ospray/ospray.h>

namespace pbnj {

Renderer::Renderer() :
    backgroundColor()
{
    this->oRenderer = ospNewRenderer("scivis");
    //ospSet1i(this->oRenderer, "spp", 4);
    //ospSet1i(this->oRenderer, "maxDepth", 5); // maybe later
    this->setBackgroundColor(0, 0, 0);
    this->oCamera = NULL;
    this->oModel = NULL;
}

void Renderer::setBackgroundColor(char r, char g, char b)
{
    this->backgroundColor[0] = r;
    this->backgroundColor[1] = g;
    this->backgroundColor[2] = b;
    float asVec[] = {r/(float)255.0, g/(float)255.0, b/(float)255.0};
    ospSet3fv(this->oRenderer, "bgColor", asVec);
    ospCommit(this->oRenderer);
}

void Renderer::setVolume(Volume *v)
{
    if(this->oModel != NULL) {
        std::cerr << "Volume already set!" << std::endl;
        return;
    }

    this->oModel = ospNewModel();
    ospAddVolume(this->oModel, v->asOSPRayObject());
    ospCommit(this->oModel);
}

void Renderer::setCamera(Camera *c)
{
    if(this->oCamera != NULL) {
        std::cerr << "Camera already set!" << std::endl;
        return;
    }

    this->cameraWidth = c->imageWidth;
    this->cameraHeight = c->imageHeight;
    this->oCamera = c->asOSPRayObject();
}

void Renderer::setSamples(unsigned int spp)
{
    ospSet1i(this->oRenderer, "spp", spp);
    ospCommit(this->oRenderer);
}

void Renderer::renderImage(std::string imageFilename)
{
    //check if everything is ready for rendering
    bool exit = false;
    IMAGETYPE imageType = this->getFiletype(imageFilename);
    if(imageType == INVALID) {
        std::cerr << "Invalid image filetype requested!" << std::endl;
        exit = true;
    }
    if(this->oModel == NULL) {
        std::cerr << "No volume set to render!" << std::endl;
        exit = true;
    }
    if(this->oCamera == NULL) {
        std::cerr << "No camera set to render with!" << std::endl;
        exit = true;
    }
    if(exit)
        return;

    //finalize the OSPRay renderer
    ospSetObject(this->oRenderer, "model", this->oModel);
    ospSetObject(this->oRenderer, "camera", this->oCamera);
    ospCommit(this->oRenderer);

    //set up framebuffer
    osp::vec2i imageSize;
    imageSize.x = this->cameraWidth;
    imageSize.y = this->cameraHeight;
    //this framebuffer will be released after a single frame
    this->oFrameBuffer = ospNewFrameBuffer(imageSize, OSP_FB_SRGBA,
                                           OSP_FB_COLOR | OSP_FB_ACCUM);
    ospRenderFrame(this->oFrameBuffer, this->oRenderer,
            OSP_FB_COLOR | OSP_FB_ACCUM);

    this->saveImage(imageFilename, imageType);
}

IMAGETYPE Renderer::getFiletype(std::string filename)
{
    std::stringstream ss;
    ss.str(filename);
    std::string token;
    char delim = '.';
    while(std::getline(ss, token, delim)) {
    }

    //only supporting PPM for now because
    //good god using libpng looks annoying
    if(token.compare("ppm") == 0) {
        return PIXMAP;
    }
    else {
        return INVALID;
    }
}

void Renderer::saveImage(std::string filename, IMAGETYPE imageType)
{
    if(imageType == PIXMAP)
        this->saveAsPPM(filename);
}

void Renderer::saveAsPPM(std::string filename)
{
    int width = this->cameraWidth, height = this->cameraHeight;
    uint32_t *colorBuffer = (uint32_t *)ospMapFrameBuffer(this->oFrameBuffer,
            OSP_FB_COLOR);
    //do a binary file so the PPM isn't quite so large
    FILE *file = fopen(filename.c_str(), "wb");
    unsigned char *rowOut = (unsigned char *)malloc(3*width);
    fprintf(file, "P6\n%i %i\n255\n", width, height);

    //the OSPRay framebuffer uses RGBA, but PPM only supports RGB
    for(int j = 0; j < height; j++) {
        unsigned char *rowIn = (unsigned char*)&colorBuffer[(height-1-j)*width];
        for(int i = 0; i < width; i++) {
            rowOut[3*i + 0] = rowIn[4*i + 0];
            rowOut[3*i + 1] = rowIn[4*i + 1];
            rowOut[3*i + 2] = rowIn[4*i + 2];
        }
        fwrite(rowOut, 3*width, sizeof(char), file);
    }

    fprintf(file, "\n");
    fclose(file);

    //unmap and release so OSPRay will deallocate the memory
    //used by the framebuffer
    ospUnmapFrameBuffer(colorBuffer, this->oFrameBuffer);
    ospRelease(this->oFrameBuffer);
}

}
