#include "ofxTextureArray.h"

//----------------------------------------------------------
ofxTextureArray::ofxTextureArray()
	: ofxTexture()
{
	texData.textureTarget = GL_TEXTURE_2D_ARRAY;
}

//----------------------------------------------------------
void ofxTextureArray::allocate(int w, int h, int d, int internalGlDataType)
{
	if (ofGetLogLevel() >= OF_LOG_VERBOSE)
	{
		int gl_maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_maxTexSize);
		int gl_maxTexLayers;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &gl_maxTexLayers);
		ofLogVerbose(__FUNCTION__) << "Max size is " << gl_maxTexSize << "x" << gl_maxTexSize << "x" << gl_maxTexLayers;
	}
	
	texData.tex_w = w;
	texData.tex_h = h;
	texData.tex_d = d;
	texData.tex_t = w;
	texData.tex_u = h;
	texData.tex_v = d;
	texData.textureTarget = GL_TEXTURE_2D_ARRAY;

	texData.glInternalFormat = internalGlDataType;
	// Get the glType (format) and pixelType (type) corresponding to the glTypeInteral (internalFormat)
	texData.pixelType = ofGetGlTypeFromInternal(texData.glInternalFormat);

	// Attempt to free the previous bound texture.
	clear();

	// Initialize the new texture.
	glGenTextures(1, (GLuint *)&texData.textureID);
	ofRetain();

	bind();
	glTexStorage3D(texData.textureTarget, 1, texData.glInternalFormat, (GLint)texData.tex_w, (GLint)texData.tex_h, (GLint)texData.tex_d);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);

	unbind();

	texData.width = w;
	texData.height = h;
	texData.depth = d;
	texData.bFlipTexture = false;
	texData.bAllocated = true;
}

//----------------------------------------------------------
void ofxTextureArray::loadData(const void * data, int w, int h, int d, int xOffset, int yOffset, int layerOffset, int glFormat)
{
	if (w > texData.tex_w || h > texData.tex_h || d > texData.tex_d)
	{
		ofLogError(__FUNCTION__) << "Failed to upload " << w << "x" << h << "x" << d << " data to " << texData.tex_w << "x" << texData.tex_h << "x" << texData.tex_d << " texture";
		return;
	}

	ofSetPixelStoreiAlignment(GL_UNPACK_ALIGNMENT, w, 1, ofGetNumChannelsFromGLFormat(glFormat));
	bind();
	glTexSubImage3D(texData.textureTarget, 0, xOffset, yOffset, layerOffset, w, h, d, glFormat, texData.pixelType, data);
	unbind();
}
