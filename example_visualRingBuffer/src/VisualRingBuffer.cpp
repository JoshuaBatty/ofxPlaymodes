//
//  VisualRingBuffer.cpp
//  example
//
//  Created by Joshua Batty on 6/05/2016.
//
//

#include "VisualRingBuffer.h"

using namespace ofxPm;

//--------------------------------------------------------------
void VisualRingBuffer::setup(int w, int h){
    width = w;
    height = h;
    
    tGrabber.initGrabber(width,height);
    vHeader.setup(vBuffer);
    vMixer.setup(tGrabber,vHeader);
    vBuffer.setup(tGrabber, 140, true,false);
    vHeader.setDelayMs(300);
}

//--------------------------------------------------------------
void VisualRingBuffer::stepForwardOneFrame(){
    ofxPm::VideoFrame tGrabberFrame = tGrabber.getNextVideoFrame();
    ofxPm::VideoFrame vHeaderFrame = vHeader.getNextVideoFrame();

    ofxPm::VideoFrame mixedFrame;
    if (vHeaderFrame == NULL) {
        mixedFrame = vMixer.mixVideoFrames(tGrabberFrame, tGrabberFrame);
    } else {
        mixedFrame = vMixer.mixVideoFrames(tGrabberFrame, vHeaderFrame);
    }
    
    vBuffer.pushNewVideoFrameTracer(mixedFrame);
    
    
    // Steps
    // 1. tGrabber 'VideoFrame' is written to
    // 2. Get a 'VideoFrame' from vHeader
    // 3. Pass in the tGrabber and vHeaders 'VideoFrames' in vMixers draw shader
    // 4. vMixer calls new video fram with back Frame //still unsure about this
    // 5. vMixer sends its VideoFrame to vRate which.
    // 6. ON A SEPARATE THREAD, vRate sends its frame to vBuffer.
    // 7. vBuffer sends its VideoFrame to
}

//--------------------------------------------------------------
void VisualRingBuffer::begin(){
    tGrabber.begin();
    ofClear(0,0,0,0);
}
//--------------------------------------------------------------
void VisualRingBuffer::end(){
    tGrabber.end();
}

//--------------------------------------------------------------
void VisualRingBuffer::setDelayMs(int delay){
    vHeader.setDelayMs(delay);
}

//--------------------------------------------------------------
void VisualRingBuffer::setFps(int fps){
    vHeader.setFps(fps);
}

//--------------------------------------------------------------
void VisualRingBuffer::setWidth(int width){
    this->width = width;
}
//--------------------------------------------------------------
void VisualRingBuffer::setHeight(int height){
    this->height = height;
}

//--------------------------------------------------------------
void VisualRingBuffer::draw(int x, int y, int w, int h){
//    vMixer.getNextVideoFrame().getTextureRef().draw(x, y, w, h);
    vHeader.getNextVideoFrame().getTextureRef().draw(x, y, w, h);
}

//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofVideoGrabber grabber){
    begin();
    grabber.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofVideoPlayer player){
    begin();
    player.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofTexture texture){
    begin();
    texture.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofFbo fbo){
    begin();
    fbo.draw(0,0,width,height);
    end();
}

//--------------------------------------------------------------
void VisualRingBuffer::toggleRecord(){
    if(vBuffer.isStopped()){
        vBuffer.resume();
    }else{
        vBuffer.stop();
    }
}

