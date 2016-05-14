//
//  PlayModes.h
//  
//
//  Created by Joshua Batty on 5/05/14.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "VideoBuffer.h"
#include "VideoHeader.h"
#include "VideoRenderer.h"
#include "BasicVideoRenderer.h"

#include "GrainEnvelope.h"

class PlayModes{
    
public:
    void setup(ofxPm::VideoBuffer *_vBuffer);
    void assignVideoBuffer(ofxPm::VideoBuffer *_vBuffer);
    
    void update();
    void draw(int _x, int _y, int _w, int _h);
    void drawData();


    void setNumPlayers(int _numPlayers);
    void setVideoGrainAmount(float _grainAmount);
    void setWindowShape(int _window);
    void setBlendMode(ofBlendMode _blendMode);
    
    int getBlendMode();
    int getWindowShape();
    
    void setDelay(float _delay);
    void setSpeed(float _speed);
    
    //Register for Events
    ofEvent<bool> newGrain;
    bool bNewGrain;

    GrainEnvelope grainEnvelope[NUM_PLAYERS];
    int grainEnv[NUM_PLAYERS];
    
    ofxPm::VideoBuffer *vBuffer;
    ofxPm::VideoHeader vHeader[NUM_PLAYERS];
    ofxPm::BasicVideoRenderer vRenderer[NUM_PLAYERS];

    
    int delay;
    int numPlayers;
    int grainAmount;
    ofBlendMode blendMode;
    
    bool bSetPosition;
    
    bool bGrainSwitch[NUM_PLAYERS];
    int grainCounter[NUM_PLAYERS];
    
    float framePosPerc;
    
    int window;

};
