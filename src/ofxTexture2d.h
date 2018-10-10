#pragma once

#include "ofTexture.h"
#include "ofMain.h"

class ofxTexture2d : public ofTexture
{
    public:
		void loadData(const unsigned char * data, int w, int h, int xOffset, int yOffset, int glFormat);
		void loadData(const float* data, int w, int h, int xOffset, int yOffset, int glFormat);
		void loadData(const unsigned short* data, int w, int h, int xOffset, int yOffset, int glFormat);
		void loadData(const ofPixels & pix, int xOffset, int yOffset);
		void loadData(const ofShortPixels & pix, int xOffset, int yOffset);
		void loadData(const ofFloatPixels & pix, int xOffset, int yOffset);
		void loadData(const void * data, int w, int h, int xOffset, int yOffset, int glFormat, int glType);
};
