//
//  VisualRingBuffer.h
//  example
//
//  Created by Joshua Batty on 6/05/2016.
//
//

#pragma once

#include "ofMain.h"
#include "VideoBuffer.h"
#include "VideoHeader.h"
#include "TextureGrabber.h"
#include "VideoRenderer.h"
#include "VideoRate.h"
#include "VideoMixerGPU.h"
#include "BasicVideoRenderer.h"

class VisualRingBuffer{
    
public:
    void setup(int w, int h);
    void setTexture(ofVideoGrabber grabber);
    void setTexture(ofVideoPlayer player);
    void setTexture(ofTexture texture);
    void setTexture(ofFbo fbo);
    
    void draw(int x, int y, int w, int h);
    void toggleRecord();
    
    void begin();
    void end();
    
    void setFps(int fps);
    void setDelayMs(int delay);
    void setWidth(int width);
    void setHeight(int height);
    
    ofxPm::TextureGrabber tGrabber;
    ofxPm::VideoBuffer vBuffer;
    ofxPm::VideoHeader vHeader;
    ofxPm::BasicVideoRenderer vRenderer;
    ofxPm::VideoRate vRate;
    ofxPm::VideoMixerGPU vMixer;
    
private:
    int width;
    int height;
};
