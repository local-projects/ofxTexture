#pragma once

#include "ofMain.h"

class ofxTextureData
	: public ofTextureData
{
public:
	ofxTextureData() 
	{
		textureID = 0;

		glInternalFormat = GL_RGB8;
		pixelType = GL_UNSIGNED_BYTE;

		tex_t = 0;
		tex_u = 0;
		tex_v = 0;

		tex_w = 0;
		tex_h = 0;
		tex_d = 0;
		
		width = 0;
		height = 0;
		depth = 0;

		bFlipTexture = false;
		compressionType = OF_COMPRESS_NONE;
		bAllocated = false;
	}

	float tex_v;
	float tex_d;
	float depth;
	int pixelType;
};

class ofxTexture
{
public:
	ofxTexture();
	virtual ~ofxTexture();

    ofxTexture(ofxTexture && mom);
    ofxTexture & operator=(ofxTexture && mom);

	virtual void allocate(int w, int h, int d, int internalGlDataType) = 0;
	void clear();

	void loadData(const unsigned char * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const float* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const uint16_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const int16_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const uint32_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const int32_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat);
	void loadData(const ofPixels & pix, int d, int xOffset, int yOffset, int zOffset);
	void loadData(const ofShortPixels & pix, int d, int xOffset, int yOffset, int zOffset);
	void loadData(const ofFloatPixels & pix, int d, int xOffset, int yOffset, int zOffset);
	void loadData(const ofBufferObject & buffer, int glformat);
	void clearData();

	void bind();
	void unbind();

	void bindAsImage(GLuint unit, GLenum access, GLint level=0, GLboolean layered=0, GLint layer=0);
	void copyTo(ofBufferObject & buffer) const;

	void generateMipmaps();
	void setMinMagFilters(GLenum min, GLenum mag);

	ofxTextureData texData;

protected:
	virtual void loadData(const void * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat) = 0;

	void ofRetain();
	void ofRelease();
};
namespace std{
inline void swap(ofxTexture & t1, ofxTexture & t2){
    swap(t1.texData, t2.texData);
}
}
