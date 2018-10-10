#include "ofxTexture3d.h"

//----------------------------------------------------------
ofxTexture3d::ofxTexture3d()
	: ofxTexture()
{
	texData.textureTarget = GL_TEXTURE_3D;
}


//----------------------------------------------------------
ofxTexture3d::ofxTexture3d(ofxTexture3d && mom)
:ofxTexture(std::move(mom)){}

//----------------------------------------------------------
ofxTexture3d & ofxTexture3d::operator=(ofxTexture3d && mom){
    texData = std::move(mom.texData);
    mom.texData.bAllocated = false;
    mom.texData.textureID = 0;
    return *this;
}

//----------------------------------------------------------
void ofxTexture3d::allocate(int w, int h, int d, int internalGlDataType)
{
	if (ofGetLogLevel() >= OF_LOG_VERBOSE)
	{
		int gl_maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_maxTexSize);
		int gl_max3DTexSize;
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &gl_max3DTexSize);
		ofLogVerbose(__FUNCTION__) << "Max size is " << gl_maxTexSize << "x" << gl_maxTexSize << "x" << gl_max3DTexSize;
	}
	
	texData.tex_w = w;
    texData.tex_h = h;
    texData.tex_d = d;
    texData.tex_t = w;
    texData.tex_u = h;
    texData.tex_v = d;
    texData.textureTarget = GL_TEXTURE_3D;

    texData.glInternalFormat = internalGlDataType;
	// Get the pixelType (type) corresponding to the glTypeInteral (internalFormat)
    texData.pixelType = ofGetGlTypeFromInternal(texData.glInternalFormat);

	// Attempt to free the previous bound texture.
	clear();

	// Initialize the new texture.
	glGenTextures(1, (GLuint *)&texData.textureID);
	ofRetain();

	bind();
	auto glFormat = ofGetGLFormatFromInternal(texData.glInternalFormat);
	glTexImage3D(texData.textureTarget, 0, texData.glInternalFormat, (GLint)texData.tex_w, (GLint)texData.tex_h, (GLint)texData.tex_d, 0, glFormat, texData.pixelType, 0);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	unbind();

    texData.width = w;
    texData.height = h;
    texData.depth = d;
    texData.bFlipTexture = false;
    texData.bAllocated = true;
}

//----------------------------------------------------------
void ofxTexture3d::loadData(const void * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
    if (w > texData.tex_w || h > texData.tex_h || d > texData.tex_d)
    {
		ofLogError(__FUNCTION__) << "Failed to upload " << w << "x" << h << "x" << d << " data to " << texData.tex_w << "x" << texData.tex_h << "x" << texData.tex_d << " texture";
		return;
    }

	int bpc = 1;
	switch(texData.pixelType){
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
			bpc = 1;
		break;
		case GL_UNSIGNED_SHORT:
		case GL_SHORT:
		case GL_HALF_FLOAT:
			bpc = 2;
		break;
		case GL_UNSIGNED_INT:
		case GL_INT:
		case GL_FLOAT:
			bpc = 4;
		break;
	}

	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, w, 1, ofGetNumChannelsFromGLFormat(glFormat));
	bind();
	glTexSubImage3D(texData.textureTarget, 0, xOffset, yOffset, zOffset, w, h, d, glFormat, texData.pixelType, data);
	unbind();
}
