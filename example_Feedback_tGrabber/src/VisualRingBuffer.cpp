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
    vRate.setup(vMixer,60);
    vBuffer.setup(vRate,140,true,false);
    vHeader.setDelayMs(300);
    vRenderer.setup(vMixer);
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
void VisualRingBuffer::begin(){
    tGrabber.begin();
    ofClear(0,0,0,0);
}
//--------------------------------------------------------------
void VisualRingBuffer::end(){
    tGrabber.end();
    tGrabber.update();
}

//--------------------------------------------------------------
void VisualRingBuffer::setDelayMs(int delay){
    vHeader.setDelayMs(delay);
}

//--------------------------------------------------------------
void VisualRingBuffer::setFps(int fps){
    vRate.setFps(fps);
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
    vRenderer.draw(x,y,w,h);
}

//--------------------------------------------------------------
void VisualRingBuffer::toggleRecord(){
    if(vBuffer.isStopped()){
        vBuffer.resume();
    }else{
        vBuffer.stop();
    }
}

