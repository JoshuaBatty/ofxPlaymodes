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


#define LENGTH 294000

class GrainPlayer : public ofSoundSource{
    
public:
    void setup();
    void draw();

    void drawWaveform();
    
    float getRecordPostion();
    float getPlayPosition();
    
    void setGrainEnvelope(int _grainEnv);
    void setPosition(float _pos);
    
    void setPositionRandom(float _val);
    void setPitchRandom(float _val);
    void setGrainSizeRandom(float _val);
    
	void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested (float * output, int numFrames, int nChannels);
    void setSampleRate( int rate );
	string getName() { return "soundSourceMaxiGrains"; }
    
    //Bool event for Recording Toggle/ resets play position relative to current Rec position
    void updatePlayHead();
    bool bUpdatePlayheadEvent;
    
    int bufferSize;
	int	sampleRate;
	float volume;
    
    //////////// ofxMaxim ////////////
    maxiPitchStretch<gaussianWinFunctor> *ps1;
    maxiPitchStretch<cosineWinFunctor> *ps2;
    maxiPitchStretch<triangleWinFunctor> *ps3;
    maxiPitchStretch<rectWinFunctor> *ps4;
    maxiPitchStretch<hannWinFunctor> *ps5;
    
    maxiMix mymix;
    maxiSample samp;
    maxiSample sampFile;
    double wave;
    double outputs[2];
    double windowAmp;
    
    //Recording
    bool bSetPosition;
    bool bRecLiveInput;
    float recMix;
    float recordPosition;
    
    
    //Grain Variables
    float speed;
    float grainSize;
    float pitch;
    float playHead;
    float overlaps;
    float loopSize;
    
    //Master
    float grainPosition;
    float grainPitch;
    float grainLength;
    
    int grainEnv;
    
    
    //Drawing
    int curXpos, curYpos;
    int prevXpos, prevYpos;
    
};
