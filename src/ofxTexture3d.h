#pragma once

#include "ofxTexture.h"

class ofxTexture3d
	: public ofxTexture
{
    public:
        ofxTexture3d();

        ofxTexture3d(ofxTexture3d && mom);
        ofxTexture3d & operator=(ofxTexture3d && mom);

		void allocate(int w, int h, int d, int internalGlDataType) override;

		using ofxTexture::loadData;

protected:
	void loadData(const void * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat) override;
};

