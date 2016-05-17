#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofBackground(0);
    
    gui.setup();
    gui.add(fps.setup("fps",60,1,60));
    gui.add(delay.setup("delay",10,0,200));
    
    w = 640;
    h = 480;
    
    grabber.setDeviceID(0);
    grabber.setDesiredFrameRate(30);
    grabber.initGrabber(w,h);
    
    fbo.allocate(w,h,GL_RGBA);
    
    bTrailsOrChaser = false; // Toggle between video trails and buffer chase examples
    
    if(bTrailsOrChaser == true){
        ringBuffer.setupBufferTrails(w,h);
    } else {
        ringBuffer.setupBufferChaser(w,h,100);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    grabber.update();
    
    fbo.begin();
    ofClear(0,0,0,0);
    grabber.draw(0,0,w,h);
    fbo.end();

    if(grabber.isFrameNew()){
        ringBuffer.setTexture(fbo);
    }
    
    ringBuffer.setDelayMs(delay);
    ringBuffer.setFps(fps);
    
    if(bTrailsOrChaser == true){
        ringBuffer.stepTrailsForwardOneFrame();
    } else {
        ringBuffer.stepChaserForwardOneFrame();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(bTrailsOrChaser == true){
        ringBuffer.drawTrails(0,0,ofGetWidth(),ofGetHeight());
    }
    else {
        int w  = 320*0.5;
        int h  = 240*0.5;
        
        float position = 0;
        
        for( int x=0; (x+w)<ofGetWidth(); x+=w ) {
            for( int y=0; (y+h)<ofGetHeight(); y += h ) {
                position = (x / (float)ofGetWidth()) + (y / (float)ofGetHeight()) * 0.25;
                int i = ofMap(position,0.0,1.0,0,ringBuffer.getNumHeaders()-10);
                ringBuffer.drawChaserFrame(i,x,y,w,h);
            }
        }
    }
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==' '){
        ringBuffer.toggleRecord();
    }
}
