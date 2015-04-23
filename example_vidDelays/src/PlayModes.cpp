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
void PlayModes::setup(){

    framePosPerc = 0.0f;
    
    bRecord = true;
    bRecordSwitch = true;
    
    window = "Triangle";
    fps = 60;
    delay = 1000;
    numPlayers = 5;
    grainAmount = 5;
    blendMode = OF_BLENDMODE_ALPHA;
    
	vGrabber.setDeviceID(0);
	vGrabber.setDesiredFrameRate(30);
	vGrabber.initGrabber(640,480);

    player.loadMovie("/Users/josh/Documents/Software/openFrameworks/2012/of_v0.8.0_osx_release/addons/ofxPlaymodes/example_PlayModes_Maxim_vidDelays/bin/data/Video/Josh_AVGS_demo.mov");
    player.play();
    player.setVolume(0.0);

    tGrabber.initGrabber(512, 512);
    vRate.setup(tGrabber,fps);
	vBuffer.setup(vRate,NUM_FRAMES,true);
    for(int i=0; i < NUM_PLAYERS; i++){
        vHeader[i].setup(vBuffer);
        vHeader[i].setDelayMs(i);
        vRenderer[i].setup(vHeader[i]);
        
        bGrainSwitch[i] = true;
        grainCounter[i] = 1;
    }
    
    //Register for Grain Events
    bNewGrain = false;
}

//--------------------------------------------------------------
void PlayModes::loadVideo(string _path){
    player.close();
    player.loadMovie(_path);
    player.play();
//    player.setPosition(ofRandomuf());
    player.setVolume(0.0);
}

//--------------------------------------------------------------
void PlayModes::setWindowShape(string _window){
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
    grainAmount = ofMap(_grainAmount,0.025, 0.45,1,26);
}

//--------------------------------------------------------------
void PlayModes::setSpeed(float _speed){
    for(int i=0;i<numPlayers;i++){
        vHeader[i].setSpeed(_speed);
    }
}

//--------------------------------------------------------------
void PlayModes::setFps(int _fps){
    vRate.setFps(_fps);
    for(int i=0;i<numPlayers;i++){
        vHeader[i].setFps(_fps);
    }
}

//--------------------------------------------------------------
void PlayModes::setDelay(float _delay){
    delay = ofMap(_delay, 1.0, 0.0, 0, NUM_FRAMES);
    
    for(int i = numPlayers-1; i>=0; i--){
       // int env = grain[i].getEnvelope(i, numPlayers, grainAmount);
        grainEnv[i] = grainEnvelope[i].getGrainFrame(i, numPlayers, grainAmount);
        grainEnvelope[i].setEnvelope(window, grainAmount, grainEnv[i]);
        vHeader[i].setDelayFrames(ofWrap(delay+grainEnv[i], 0, NUM_FRAMES));
        
        // Check to see if the grain is at the start for each one and send
        // an event to trigger a new grain event
        if(grainEnv[i] == 1 && grainCounter[i] == 1){
            if(bGrainSwitch[i] == true){
                cout << grainEnv[i] <<  " -- Grain Env = NEW GRAIN" << endl;
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
void PlayModes::setFramePos(float _framePos){
    vBuffer.setFramePos(_framePos);
}

//--------------------------------------------------------------
void PlayModes::recordingStatus(){
    if(bRecord) {
        if(bRecordSwitch==true){
            vBuffer.resume();
        }
        bRecordSwitch = false;
    } else if(bRecord==false){
        if(bRecordSwitch==false){
            vBuffer.stop();
        }
        bRecordSwitch = true;
    }
}

//--------------------------------------------------------------
void PlayModes::update(){
    recordingStatus();
	vGrabber.update();
    tGrabber.update();
    player.update();
    
    drawIntoFbo();
    
	vRate.setFps(fps);

}

//--------------------------------------------------------------
void PlayModes::drawIntoFbo(){
    tGrabber.begin();
    ofClear(0,0,0,0);
//    player.draw(0,0,512,512);
    vGrabber.draw(0,0,512,512);
    tGrabber.end();
}

//--------------------------------------------------------------
void PlayModes::draw(int _x, int _y, int _w, int _h){
    
//	vHeader1.draw();


    ofEnableAlphaBlending();
    ofEnableBlendMode(blendMode);
    for(int i = numPlayers-1; i>=0; i--){
        if(grainAmount <= 2){
            ofSetColor(255,255);
            vRenderer[i].draw(_w,_y,_x-_w,_h);
        } else {
            ofSetColor(255,grainEnvelope[i].getEnvelope()*255);
            vRenderer[i].draw(_w,_y,_x-_w,_h);
            //grain[i].draw();
        }
    }
    ofDisableAlphaBlending();
    
//    float val = grainEnvelope[0].triangleWinFunctor(grainAmount, ofMap(ofGetMouseX(),0,ofGetWidth(),0,grainAmount));
//    ofDrawBitmapString(ofToString(val), ofGetWidth()/2,50);
}

//--------------------------------------------------------------
void PlayModes::drawData(){
    
    ofSetColor(255,255);
	vBuffer.draw();
    
    ofSetColor(255,255);
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate()))
                       + " || cameraBuffer FPS " + ofToString(vBuffer.getRealFPS())
                       + " || videoframes pool size: " + ofToString(VideoFrame::getPoolSize(VideoFormat(512,512,3)))
                       + " || total frames: " +ofToString(VideoFrame::getTotalNumFrames()),20,ofGetHeight()-40);
    
    
    /// WAVEFROM
    float vhPos = ofMap(vHeader[0].getDelayFrames(),0,NUM_FRAMES,0.0,1.0);

    const float waveformWidth  = ofGetWidth() - 40;
    const float waveformHeight = 220;
    
    float top = ofGetHeight() - waveformHeight - 20;
    float left = 20;
    
    ////////// Video Header Play Pos ///////////////////////
    ofSetColor(255,0,0,255);
  //  ofDrawBitmapString("Video Header Play Pos", left, top-10);
  //  ofLine(left, top, waveformWidth, top);
    
    // frame pos
    ofSetColor(0,0,255,255);
    framePosPerc = (float)vBuffer.getFramePos() / (float)NUM_FRAMES;
    ofLine(left+ (framePosPerc * (waveformWidth-left)), top, left+ (framePosPerc * (waveformWidth-left)), top+waveformHeight);
    ofDrawBitmapString("FramePos", left + framePosPerc * waveformWidth-76, top+45);

    ofCircle(left+(framePosPerc*(waveformWidth-left)), top, 10);
    
    ofSetColor(255);
}

