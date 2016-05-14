//
//  KortexState.h
//  Kortex_v3
//
//  Created by Joshua Batty on 12/11/2014.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

static int DEST_AVGS = 0;
static int DEST_DSP = 1;
static int DEST_FM = 2;
static int DEST_SHADER = 3;
static int DEST_LASER = 4;
static int DEST_REORDER = 5;

static int BITCRUSH_FREQ = 0;
static int BITCRUSH_STEP = 1;
static int BITCRUSH_MIX = 2;
static int FLANGE_DELAY = 3;
static int FLANGE_FREQ = 4;
static int FLANGE_MOD = 5;
static int FLANGE_TYPE = 6;
static int FLANGE_MIX = 7;
static int FILTER_CUTOFF = 8;
static int FILTER_RESONANCE = 9;
static int FILTER_TYPE = 10;
static int FILTER_MIX = 11;
static int ECHO_TIME = 12;
static int ECHO_FEEDBACK = 13;
static int ECHO_MIX = 14;
static int DISTORTION_AMOUNT = 15;
static int DISTORTION_MIX = 16;
static int REVERB_SIZE = 17;
static int REVERB_DAMP = 18;
static int REVERB_WIDTH = 19;
static int REVERB_MIX = 20;

class Destination {
public:
    vector<float> params;
};

class DspState: public Destination
{
public:
    DspState(){
        DspState::init();
    }
    void init();
    
    /* -- Linked State Vars -- */
    float bitCrushFreq;
    float bitCrushMix;
    float flangeDelay;           
    float flangeFreq;            
    float flangeMod;             
    float flangeMix;             
    
    float filterType;            
    float filterCutoff;          
    float filterMix;             
    
    float echoTime;              
    float echoFeedback;          
    float echoMix;               
    
    float distortionAmount;      
    float distortionMix;         
    
    float reverbSize;            
    float reverbMix;             
    
    /* -- Audio State Vars -- */
    float bitCrushStep;          
    float filterResonance;       
    float reverbDamp;            
    float reverbWidth;           
    float flangeType;       
};

static int PLAYPOS = 0;
static int SPEED = 1;
static int MULTIPLIER = 2;
static int PITCH = 3;
static int GRAINSIZE = 4;
static int OVERLAPS = 5;
static int RANDOMPITCH = 6;
static int RANDOMGRAINSIZE = 7;
static int RANDOMPOSITION = 8;

class AVGSState: public Destination
{
public:
    AVGSState(){
        AVGSState::init();
    }
    void init();
    
    /* -- Linked State Vars -- */
    float playPosition;          
    float speed;                 
    float multiplier;
    float pitch;
    float grainSize;             
    float overlaps;              
    // ----------------------
    float randomPitch;           
    float randomGrainSize;       
    float randomPosition;        
};

//static int PLAYPOS = 0;
static int GR_CARRIER_AMP = 1;
static int GR_CARRIER_FREQ = 2;
static int GR_MODULATOR_AMP = 3;
static int GR_MODULATOR_FREQ = 4;
static int GR_AMP_AMP = 5;
static int GR_AMP_FREQ = 6;

class REORDERState: public Destination
{
public:
    REORDERState(){
        REORDERState::init();
    }
    void init();
    
    /* -- Reordering Vars Vars -- */
    float carrierAmp;            
    float carrierFreq;           
    float modulatorAmp;          
    float modulatorFreq;         
    float ampAmp;                
    float ampFreq;               
    float playPosition;          
};

static int FM_AMPLITUDE = 0;
static int FM_CARRIER_FREQ = 1;
static int FM_MOD_INDEX = 2;
static int FM_MOD_FREQ = 3;
static int FM_CUTOFF = 4;
static int FM_RESONANCE = 5;
static int FM_LFO1_SPEED = 6;
static int FM_LFO1_DEPTH = 7;
static int FM_LFO2_SPEED = 8;
static int FM_LFO2_DEPTH = 9;
static int FM_LFO3_SPEED = 10;
static int FM_LFO3_DEPTH = 11;

class FMState: public Destination
{
public:
    FMState(){
        FMState::init();
    }
    void init();
    
    /* -- FM Synth Vars -- */
    float amplitude ;             
    float carrierFreq;            
    float modulatorIndex;         
    float modulatorFreq;          
    float cutoff;                 
    float resonance;              
    float lfo1DutyCylce;          
    float lfo1Depth;              
    float lfo2DutyCylce;          
    float lfo2Depth;              
    float lfo3DutyCylce;
    float lfo3Depth;              
};

class ShaderState: public Destination{
public:
    ShaderState(){
        ShaderState::init();
    }
    void init();
};

class BufferParams: public Destination{
public:
    
    AVGSState avgs;
    DspState dsp;
    REORDERState reorder;
};

class DestinationState
{
public:
    DestinationState(){
        DestinationState::init();
    }
    
    void init();
    string getName() { return "DestinationState"; }
    
    // Add DestinationState + DestinationState
    friend AVGSState operator+(const AVGSState &a1, const AVGSState &a2);
    friend DspState operator+(const DspState &d1, const DspState &d2);
    vector<Destination*> getDestinations();
    float* paramAtIndexTree(vector<int>* indexTree);

    vector<BufferParams> bufferParams;
    
    AVGSState avgs;
    DspState dsp;
    REORDERState reorder;
    FMState fm;
    ShaderState shader;
    
private:
};

// Need to write a combine method to sum the MIDI and Timline using the behaviour i want

AVGSState operator+(const AVGSState &a1, const AVGSState &a2)
{
    AVGSState newState = a1;

    newState.params[PLAYPOS] += ofMap(a2.params[PLAYPOS],-1.0,1.0,0.0,1.0);
    newState.params[PITCH] += ofMap(a2.params[PITCH], 0.0, 1.0, 0.0, (4.0 - a1.params[PITCH]));
    newState.params[GRAINSIZE] += ofMap(a2.params[GRAINSIZE], -1.0, 1.0, -0.2, 0.2);
    
    return newState;
}

DspState operator+(const DspState &d1, const DspState &d2)
{
    DspState newState = d1;
    
    newState.params[BITCRUSH_FREQ] += ofMap(d2.params[BITCRUSH_FREQ],-1.0,1.0,-0.5,0.5);
    newState.params[REVERB_SIZE] += ofMap(d2.params[REVERB_SIZE],-1.0,1.0,-0.5,0.5);
    
    return newState;
}