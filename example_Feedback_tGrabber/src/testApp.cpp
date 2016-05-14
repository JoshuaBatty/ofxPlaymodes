#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    gui.setup();
    gui.add(fps.setup("fps",60,1,60));
    gui.add(delay.setup("delay",100,0,2000));
    
    grabber.setDeviceID(0);
    grabber.setDesiredFrameRate(30);
    grabber.initGrabber(640,480);
    
    ringBuffer.setup(640, 480);
    
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
    grabber.update();
    
    if(grabber.isFrameNew()){
        ringBuffer.setTexture(grabber);
    }
    
    ringBuffer.setDelayMs(delay);
    ringBuffer.setFps(fps);
}

//--------------------------------------------------------------
void testApp::draw(){
    ringBuffer.draw(0,0,ofGetWidth(),ofGetHeight());
    gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key==' '){
        ringBuffer.toggleRecord();
    }
}
