#pragma once

#include "ofxTexture.h"

class ofxTextureArray
	: public ofxTexture
{
public:
	ofxTextureArray();

	void allocate(int w, int h, int d, int internalGlDataType) override;

	using ofxTexture::loadData;

protected:
	void loadData(const void * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat) override;
};
