//
//  PlayModes.h
//  
//
//  Created by Joshua Batty on 5/05/14.
//
//

#pragma once

#include "ofMain.h"
#include "VideoBuffer.h"
#include "VideoHeader.h"
#include "VideoGrabber.h"
#include "TextureGrabber.h"
#include "VideoRenderer.h"
#include "VideoRate.h"
#include "BasicVideoRenderer.h"
#include "ofxHapPlayer.h"


#include "GrainEnvelope.h"

#define NUM_FRAMES 400
#define NUM_PLAYERS 5


struct Grain{
    int grainPosition = 0;
    int grainLength = 26;
    int numGrains = 3;
    
    void draw(){
        for(int i=0; i < numGrains; i++){
            grainPosition = ofGetSeconds() % grainLength + ofMap(i,0,numGrains,1,grainLength);
            grainPosition = ofWrap(grainPosition, 1, grainLength);
            
            ofSetColor(255);
            ofDrawBitmapString("grain Envelope = " + ofToString(grainPosition), ofGetWidth()/2, ofGetHeight()/2+(i*40)-100);
        }
    }
    
    int getEnvelope(int _grainNumber, int _maxGrains, int _grainLength){
        grainPosition = ofGetFrameNum() % _grainLength + ofMap(_grainNumber,0,_maxGrains,1,_grainLength);
        grainPosition = ofWrap(grainPosition, 1, grainLength);
        return grainPosition;
    }
};

class PlayModes{
    
public:
    void setup();
    void update();
    void draw(int _x, int _y, int _w, int _h);
    void drawData();
    void drawIntoFbo();
    void loadVideo(string _path);

    void setNumPlayers(int _numPlayers);
    void setVideoGrainAmount(float _grainAmount);
    void setWindowShape(string _window);
    void setFramePos(float _framePos);
    void setBlendMode(ofBlendMode _blendMode);
        
    void recordingStatus();
    
    void setFps(int _fps);
    void setDelay(float _delay);
    void setSpeed(float _speed);

    //Register for Events
    ofEvent<bool> newGrain;
    bool bNewGrain;

    Grain grain[NUM_PLAYERS];
    GrainEnvelope grainEnvelope[NUM_PLAYERS];
    int grainEnv[NUM_PLAYERS];
    
    
    ofVideoGrabber vGrabber;
    ofxHapPlayer player;
    
    ofxPm::TextureGrabber tGrabber;
    ofxPm::VideoBuffer vBuffer;
    ofxPm::VideoRate vRate;
    
    ofxPm::VideoHeader vHeader[NUM_PLAYERS];
    ofxPm::BasicVideoRenderer vRenderer[NUM_PLAYERS];


    int fps;
    int delay;
    int numPlayers;
    int grainAmount;
    ofBlendMode blendMode;
    
    bool bRecord;
    bool bRecordSwitch;
    bool bSetPosition;
    
    bool bGrainSwitch[NUM_PLAYERS];
    int grainCounter[NUM_PLAYERS];
    
    float framePosPerc;
    
    string window;
    

};
