//
//  GrainPlayer.h
//  testCpp11
//
//  Created by Joshua Batty on 4/06/14.
//
//

#pragma once


#include "ofSoundUnit.h"
#include "ofMain.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include <sys/time.h>

#include "AVRingBuffer.h"
#include "DestinationState.h"

class GrainPlayer : public ofSoundSource{
    
public:
    ~GrainPlayer();
    void setup(AVRingBuffer *_avRingBuffer);
    void assignAudioBuffer(AVRingBuffer *_avRingBuffer);
    
    void update(DestinationState &state);
    
    float getPlayPosition();
    void setPlayheadPosition(float _pos);
    void setGrainReordering(float _pos);
    
    void setGrainEnvelope(int _grainEnv);
    void setPosition(float _pos);
    
    void setPositionRandom(float _val);
    void setPitchRandom(float _val);
    void setGrainSizeRandom(float _val);

	void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested (float * output, int numFrames, int nChannels);
	string getName() { return "soundSourceMaxiGrains"; }
    
    //Bool event for Recording Toggle/ resets play position relative to current Rec position
    float getPlayhead();
    void updatePlayHead();
    bool bUpdatePlayheadEvent;
    
    void resetPlayheadToRecordPosition();

	float volume;
        
    //////////// ofxMaxim ////////////
    
    // Grain Windows = 1. gaussian
    //                 2. triangle
    //                 3. rect
    //                 4. pulse
    //                 5. rev pulse
    
    maxiTimePitchStretch<gaussianWinFunctor, maxiSample> *ps1;
    maxiTimePitchStretch<triangleWinFunctor, maxiSample> *ps2;
    maxiTimePitchStretch<rectWinFunctor, maxiSample> *ps3;
    maxiTimePitchStretch<pulseExpWinFunctor, maxiSample> *ps4;
    maxiTimePitchStretch<revPulseExpWinFunctor, maxiSample> *ps5;
    
    maxiMix mymix;
    maxiSample *samp;
    double wave;
    double outputs[2];
        
    //Grain Variables
    float speed;
    float multiplier;
    float grainSize;
    float pitch;
    float overlaps;
    
    float grainPattern;

    long LENGTH_IN_SAMPLES;
    
    //Master
    float playHead;
    float grainPosition;
    float grainPitch;
    float grainLength;
    
    float playPosModifier;
    
    bool bSetPosition;

    int grainEnv;
    
    //Speed Sherrif
    bool bSpeedSherrif;
    int speedFrameCount;
    
};
