#include "ofxTexture.h"

// BEGIN Copied from ofTexture.cpp
// TODO: Figure out best way to use these ofTexture functions

static map<GLuint, int> & getTexturesIndex() {
	static map<GLuint, int> * textureReferences = new map<GLuint, int>;
	return *textureReferences;
}

static void retain(GLuint id) {
	if (id != 0) {
		if (getTexturesIndex().find(id) != getTexturesIndex().end()) {
			getTexturesIndex()[id]++;
		}
		else {
			getTexturesIndex()[id] = 1;
		}
	}
}

static void release(GLuint id) {
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (id != 0) {
		if (getTexturesIndex().find(id) != getTexturesIndex().end()) {
			getTexturesIndex()[id]--;
			if (getTexturesIndex()[id] == 0) {

#ifdef TARGET_ANDROID
				if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
					glDeleteTextures(1, (GLuint *)&id);

				getTexturesIndex().erase(id);
			}
		}
		else {
			ofLogError("ofTexture") << "release(): something's wrong here, releasing unknown texture id " << id;

#ifdef TARGET_ANDROID
			if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
				glDeleteTextures(1, (GLuint *)&id);
		}
	}
}

// END Copied from ofTexture.cpp

//----------------------------------------------------------
ofxTexture::ofxTexture()
{}

//----------------------------------------------------------
ofxTexture::~ofxTexture()
{
	clear();
}

//----------------------------------------------------------
ofxTexture::ofxTexture(ofxTexture && mom)
:texData(std::move(mom.texData)){
    mom.texData.bAllocated = false;
    mom.texData.textureID = 0;
}

//----------------------------------------------------------
ofxTexture & ofxTexture::operator=(ofxTexture && mom){
    texData = std::move(mom.texData);
    mom.texData.bAllocated = false;
    mom.texData.textureID = 0;
    return *this;
}

//----------------------------------------------------------
void ofxTexture::ofRetain()
{
	//retain(texData.textureID);
}

//----------------------------------------------------------
void ofxTexture::ofRelease()
{
	//release(texData.textureID);
}

//----------------------------------------------------------
void ofxTexture::clear()
{
	ofRelease();
	texData.textureID = 0;
	texData.bAllocated = false;
}

//----------------------------------------------------------
void ofxTexture::loadData(const unsigned char * data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const float* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const uint16_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const int16_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const uint32_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const int32_t* data, int w, int h, int d, int xOffset, int yOffset, int zOffset, int glFormat)
{
	loadData((const void*)data, w, h, d, xOffset, yOffset, zOffset, glFormat);
}

//----------------------------------------------------------
void ofxTexture::loadData(const ofPixels & pix, int d, int xOffset, int yOffset, int zOffset)
{
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), d, xOffset, yOffset, zOffset, ofGetGlFormat(pix));
}

//----------------------------------------------------------
void ofxTexture::loadData(const ofShortPixels & pix, int d, int xOffset, int yOffset, int zOffset)
{
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), d, xOffset, yOffset, zOffset, ofGetGlFormat(pix));
}

//----------------------------------------------------------
void ofxTexture::loadData(const ofFloatPixels & pix, int d, int xOffset, int yOffset, int zOffset)
{
	loadData(pix.getData(), pix.getWidth(), pix.getHeight(), d, xOffset, yOffset, zOffset, ofGetGlFormat(pix));
}

//----------------------------------------------------------
void ofxTexture::loadData(const ofBufferObject & buffer, int glFormat)
{
	buffer.bind(GL_PIXEL_UNPACK_BUFFER);
	loadData((const void*)nullptr,texData.width,texData.height,texData.depth,0,0,0,glFormat);
	buffer.unbind(GL_PIXEL_UNPACK_BUFFER);
}

//----------------------------------------------------------
void ofxTexture::clearData()
{
	glClearTexImage(texData.textureID, 0, ofGetGLFormatFromInternal(texData.glInternalFormat), texData.pixelType, nullptr);
}

//----------------------------------------------------------
void ofxTexture::bind()
{
	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
}

//----------------------------------------------------------
void ofxTexture::unbind()
{
	glBindTexture(texData.textureTarget, 0);
}

//----------------------------------------------------------
void ofxTexture::bindAsImage(GLuint unit, GLenum access, GLint level, GLboolean layered, GLint layer){
	glBindImageTexture(unit,texData.textureID,level,layered,layer,access,texData.glInternalFormat);
}

//----------------------------------------------------------
void ofxTexture::copyTo(ofBufferObject & buffer) const{
	ofSetPixelStoreiAlignment(GL_PACK_ALIGNMENT,texData.width,ofGetBytesPerChannelFromGLType(ofGetGlTypeFromInternal(texData.glInternalFormat)),ofGetNumChannelsFromGLFormat(ofGetGLFormatFromInternal(texData.glInternalFormat)));
	buffer.bind(GL_PIXEL_PACK_BUFFER);
	glBindTexture(texData.textureTarget,texData.textureID);
	glGetTexImage(texData.textureTarget,0,ofGetGLFormatFromInternal(texData.glInternalFormat),ofGetGlTypeFromInternal(texData.glInternalFormat),0);
	glBindTexture(texData.textureTarget,0);
	buffer.unbind(GL_PIXEL_PACK_BUFFER);

}

//----------------------------------------------------------
void ofxTexture::generateMipmaps(){
	bind();
	glGenerateMipmap(texData.textureTarget);
	unbind();
}

//----------------------------------------------------------
void ofxTexture::setMinMagFilters(GLenum min, GLenum mag){
	texData.magFilter = mag;
	texData.minFilter = min;
	bind();
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, texData.magFilter);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, texData.minFilter);
	unbind();
}
