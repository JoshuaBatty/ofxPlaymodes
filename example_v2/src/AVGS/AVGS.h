//
//  AVGS.h
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 17/07/14.
//
//

#pragma once

#include "ofMain.h"
#include "AVRingBuffer.h"

#include "PlayModes.h"
#include "GrainPlayer.h"
#include "GrainReorder.h"
#include "PostProcess.h"

class AVGS{
    
public:
    AVGS();
    
    void setup(AVRingBuffer *avRingBuffer);
    void update(DestinationState &state, AVRingBuffer &avRingBuffer);
    void draw(int _x, int _y, int _w, int _h);
    void drawPostProcessing(int _x, int _y, int _w, int _h);
    void drawOscillators();
    
    PlayModes     playModes;
    GrainPlayer   grainPlayer;
    GrainReorder  grainReorder;

    // Post Processing
    ofFbo    playModesFbo;
    ofShader bloomShader;
    ofFbo    bloomFbo;
    float    bloom_amount;
    
    PostProcess post;
    
    //AVGS
    float randomPosition;
    float randomPitch;
    float randomGrainSize;
    
    //Events
    void bNewGrain(bool & b);

    bool bDrawOscillators;

    int width;
};
