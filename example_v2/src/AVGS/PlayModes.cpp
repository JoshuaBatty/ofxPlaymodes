//
//  PlayModes.cpp
// 
//
//  Created by Joshua Batty on 5/05/14.
//
//

#include "PlayModes.h"
using namespace ofxPm;

//--------------------------------------------------------------
void PlayModes::setup(ofxPm::VideoBuffer *_vBuffer){
    
    assignVideoBuffer(_vBuffer);
    
    framePosPerc = 0.0f;
    
    window = 2; //"Triangle";
    delay = 1000;
    numPlayers = 5;
    grainAmount = 5;
    blendMode = OF_BLENDMODE_ALPHA;
    
    for(int i=0; i < NUM_PLAYERS; i++){
        vHeader[i].setDelayMs(0);
        bGrainSwitch[i] = true;
        grainCounter[i] = 1;
    }
    
    //Register for Grain Events
    bNewGrain = false;
}

//--------------------------------------------------------------
void PlayModes::assignVideoBuffer(ofxPm::VideoBuffer *_vBuffer){
    vBuffer = _vBuffer;
    
    for(int i=0; i < NUM_PLAYERS; i++){
        vHeader[i].setup(*vBuffer);
        vRenderer[i].setup(vHeader[i]);
    }
}

//--------------------------------------------------------------
int PlayModes::getWindowShape(){
    return window;
}

//--------------------------------------------------------------
int PlayModes::getBlendMode(){
    int mode;
    if(blendMode == OF_BLENDMODE_ALPHA) mode = 1;
    else if(blendMode == OF_BLENDMODE_ADD) mode = 2;
    else if(blendMode == OF_BLENDMODE_SCREEN) mode = 3;
    else if(blendMode == OF_BLENDMODE_MULTIPLY) mode = 4;
    else if(blendMode == OF_BLENDMODE_SUBTRACT) mode = 5;
    
    return mode;
}

//--------------------------------------------------------------
void PlayModes::setWindowShape(int _window){
    window = _window;
}

//--------------------------------------------------------------
void PlayModes::setBlendMode(ofBlendMode _blendMode){
    blendMode = _blendMode;
}

//--------------------------------------------------------------
void PlayModes::setNumPlayers(int _numPlayers){
    numPlayers = _numPlayers;
}

//--------------------------------------------------------------
void PlayModes::setVideoGrainAmount(float _grainAmount){
    grainAmount = ofMap(_grainAmount,0.025, MAX_GRAIN_SIZE,1,26);
}

//--------------------------------------------------------------
void PlayModes::setSpeed(float _speed){
    for(int i=0;i<numPlayers;i++){
        vHeader[i].setSpeed(_speed);
    }
}

//--------------------------------------------------------------
void PlayModes::setDelay(float _delay){
    delay = ofMap(_delay, 1.0, 0.0, 0, vBuffer->size());
    
    for(int i = numPlayers-1; i>=0; i--){
        // int env = grain[i].getEnvelope(i, numPlayers, grainAmount);
        grainEnv[i] = grainEnvelope[i].getGrainFrame(i, numPlayers, ofClamp(grainAmount,1,26));
        grainEnvelope[i].setEnvelope(window, grainAmount, grainEnv[i]);
        vHeader[i].setDelayFrames(ofWrap(delay+grainEnv[i], 0, vBuffer->size()));
        
        // Check to see if the grain is at the start for each one and send
        // an event to trigger a new grain event
        if(grainEnv[i] == 1 && grainCounter[i] == 1){
            if(bGrainSwitch[i] == true){
               // cout << grainEnv[i] <<  " -- Grain Env = NEW GRAIN" << endl;
                bNewGrain = true;
                ofNotifyEvent(newGrain, bNewGrain, this);
                
                bGrainSwitch[i] == false;
            }
            grainCounter[i]++;
        } else {
            bNewGrain = false;
            bGrainSwitch[i] == true;
            grainCounter[i] = 1;
        }
    }
}




//--------------------------------------------------------------
void PlayModes::update(){

}

//--------------------------------------------------------------
void PlayModes::draw(int _x, int _y, int _w, int _h){
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(blendMode);
    for(int i = numPlayers-1; i>=0; i--){
        if(grainAmount <= 2){
            ofSetColor(255,255);
        } else {
            /// This looks great on screen blend mode.
            /*
            float progress =(float)i/(float)(numPlayers-1);
            ofColor c;
            c.setHsb(progress*255, 255, 255);
            ofSetColor(c,grainEnvelope[i].getEnvelope()*255);
            */
            ofSetColor(255,grainEnvelope[i].getEnvelope()*255);
        }
        
        vRenderer[i].draw(_w,_y,_x-_w,_h);
        //grain[i].draw();
    }
    ofDisableAlphaBlending();
    
    //    float val = grainEnvelope[0].triangleWinFunctor(grainAmount, ofMap(ofGetMouseX(),0,ofGetWidth(),0,grainAmount));
    //    ofDrawBitmapString(ofToString(val), ofGetWidth()/2,50);
}

//--------------------------------------------------------------
void PlayModes::drawData(){
    ofSetColor(255,255);
	//vBuffer.draw();
    
    ofSetColor(255,255);
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate()))
                       + " || cameraBuffer FPS " + ofToString(vBuffer->getRealFPS())
                       + " || videoframes pool size: " + ofToString(VideoFrame::getPoolSize(VideoFormat(640,480,3)))
                       + " || total frames: " +ofToString(VideoFrame::getTotalNumFrames()),20,ofGetHeight()-40);
    
    
    /// WAVEFROM
    float vhPos = ofMap(vHeader[0].getDelayFrames(),0,vBuffer->size(),0.0,1.0);
    
    const float waveformWidth  = ((1920)/3) - 14;
    const float waveformHeight = 124;
    
    float top = 532 - waveformHeight;
    float left = 8;
    
    ////////// Video Header Play Pos ///////////////////////
    ofSetColor(255,0,0,255);
    //  ofDrawBitmapString("Video Header Play Pos", left, top-10);
    //  ofLine(left, top, waveformWidth, top);
    
    // frame pos
    ofSetColor(0,0,255,255);
    framePosPerc = (float)vBuffer->getFramePos() / (float)vBuffer->size();
    ofDrawLine(left+ (framePosPerc * (waveformWidth-left)), top, left+ (framePosPerc * (waveformWidth-left)), top+waveformHeight);
    ofDrawBitmapString("FramePos", left + framePosPerc * waveformWidth-76, top+45);
    
    ofDrawCircle(left+(framePosPerc*(waveformWidth-left)), top, 2);
}

