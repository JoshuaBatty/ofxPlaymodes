//
//  TextureGrabber.cpp
//  example-player
//
//  Created by Joshua Batty on 22/07/2014.
//
//

#include "TextureGrabber.h"

namespace ofxPm{
    TextureGrabber::TextureGrabber() {
        
    }
    
    TextureGrabber::~TextureGrabber() {
    }
    
    bool TextureGrabber::initGrabber(int w, int h){
        //-- Because all of Playmodes seems to want ARB textures I enable it here
        //-- Just incase it has been disabled globally before we setup playmodes.
        ofEnableArbTex();
        allocate(w, h, GL_RGB);
    }
    
    VideoFrame TextureGrabber::getNextVideoFrame(){
        return VideoFrame::newVideoFrame(getTexture());
    }
    
    void TextureGrabber::update(){
    
        frame = getNextVideoFrame();
        newFrameEvent.notify(this,frame);
    }
    
    float TextureGrabber::getFps(){
        return 25;
    }
}
