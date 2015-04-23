//
//  AVGS.h
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 17/07/14.
//
//

#pragma once

#include "ofMain.h"
#include "PlayModes.h"
#include "GrainPlayer.h"

class AVGS : public ofBaseApp{
    
public:
    AVGS();
    void setup();
    void update();
    void draw();
    void drawPostProcessing();
    
    PlayModes playModes;
    GrainPlayer grainPlayer;
    
    //Events
    void bNewGrain(bool & b);
    
    //AUDIO
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested (float * output, int numFrames, int nChannels);
    
    int sampleRate;
    int bufferSize;
    ofSoundStream soundStream;
    ofSoundMixer mixer;
    

    // Post Processing
    ofFbo playModesFbo;
    ofShader bloomShader;
    ofFbo    bloomFbo;
    float    bloom_amount;
    
    float randomPosition;
    float randomPitch;
    float randomGrainSize;

};
