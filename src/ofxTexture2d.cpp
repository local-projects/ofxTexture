#include "ofxTexture2d.h"

void ofxTexture2d::loadData(const unsigned char * data, int w, int h, int xOffset, int yOffset, int glFormat)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, w, 1, ofGetNumChannelsFromGLFormat(glFormat));
	loadData((const void *)data, w, h, xOffset, yOffset, glFormat, GL_UNSIGNED_BYTE);
}
void ofxTexture2d::loadData(const float * data, int w, int h, int xOffset, int yOffset, int glFormat)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, w, 4, ofGetNumChannelsFromGLFormat(glFormat));
	loadData((const void *)data, w, h, xOffset, yOffset, glFormat, GL_FLOAT);
}
void ofxTexture2d::loadData(const unsigned short* data, int w, int h, int xOffset, int yOffset, int glFormat)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, w, 2, ofGetNumChannelsFromGLFormat(glFormat));
	loadData((const void *)data, w, h, xOffset, yOffset, glFormat, GL_UNSIGNED_SHORT);
}
void ofxTexture2d::loadData(const ofPixels & pix, int xOffset, int yOffset)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, pix.getWidth(), pix.getBytesPerChannel(), pix.getNumChannels());
    loadData(pix.getData(), pix.getWidth(), pix.getHeight(), xOffset, yOffset, ofGetGlFormat(pix), ofGetGlType(pix));
}
void ofxTexture2d::loadData(const ofShortPixels & pix, int xOffset, int yOffset)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, pix.getWidth(), pix.getBytesPerChannel(), pix.getNumChannels());
    loadData(pix.getData(), pix.getWidth(), pix.getHeight(), xOffset, yOffset, ofGetGlFormat(pix), ofGetGlType(pix));
}
void ofxTexture2d::loadData(const ofFloatPixels & pix, int xOffset, int yOffset)
{
    ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, pix.getWidth(), pix.getBytesPerChannel(), pix.getNumChannels());
    loadData(pix.getData(), pix.getWidth(), pix.getHeight(), xOffset, yOffset, ofGetGlFormat(pix), ofGetGlType(pix));
}

void ofxTexture2d::loadData(const void * data, int w, int h, int xOffset, int yOffset, int glFormat, int glType)
{
    if(glFormat!=texData.glInternalFormat)
    {
        ofLogError() << "ofxTexture2d::loadData() failed to upload format " <<  ofGetGlInternalFormatName(glFormat) << " data to " << ofGetGlInternalFormatName(texData.glInternalFormat) << " texture" <<endl;
        return;
    }

    if(w > texData.tex_w || h > texData.tex_h )
    {
        ofLogError() << "ofxTexture2d::loadData() failed to upload " <<  w << "x" << h << " data to " << texData.tex_w << "x" << texData.tex_h << "x" << " texture";
        return;
    }
    
        //update the texture image: 
        glEnable(texData.textureTarget);

        glBindTexture(texData.textureTarget, (GLuint) texData.textureID);

        glTexSubImage2D(texData.textureTarget, 0, xOffset, yOffset, w, h, glFormat, glType, data);

        glDisable(texData.textureTarget);
}
