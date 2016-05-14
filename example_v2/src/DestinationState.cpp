//
//  KortexState.cpp
//  Kortex_v3
//
//  Created by Joshua Batty on 12/11/2014.
//
//

#include "DestinationState.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
void AVGSState::init(){
    /* -- Linked State Vars -- */
    playPosition = 0.0;
    speed = 1.0;
    multiplier = 0.0;
    pitch = 1.0;
    grainSize = 0.25;
    overlaps = 3;
    randomPitch = 0.0;
    randomGrainSize = 0.0;
    randomPosition = 0.0;
    
    params.push_back(playPosition);
    params.push_back(speed);
    params.push_back(multiplier);
    params.push_back(pitch);
    params.push_back(grainSize);
    params.push_back(overlaps);
    params.push_back(randomPitch);
    params.push_back(randomGrainSize);
    params.push_back(randomPosition);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void DspState::init(){
    /* -- Linked State Vars -- */
    bitCrushFreq = 1.0;
    bitCrushMix = 1.0;
    
    flangeDelay = 0.2;
    flangeFreq = 0.1;
    flangeMod = 0.5;
    flangeMix = 1.0;
    
    filterType = 0;
    filterCutoff = 1.0;
    filterMix = 1.0;
    
    echoTime = 0.3;
    echoFeedback = 0.7;
    echoMix = 1.0;
    
    distortionAmount = 0.1;
    distortionMix = 1.0;
    
    reverbSize = 0.2;
    reverbMix = 1.0;
    
    /* -- Audio State Vars -- */
    bitCrushStep = 0.0;
    filterResonance = 0.1;
    reverbDamp = 0.2;
    reverbWidth = 0.99;
    flangeType = 5;
    
    params.push_back(bitCrushFreq);
    params.push_back(bitCrushStep);
    params.push_back(bitCrushMix);
    params.push_back(flangeDelay);
    params.push_back(flangeFreq);
    params.push_back(flangeMod);
    params.push_back(flangeType);
    params.push_back(flangeMix);
    params.push_back(filterCutoff);
    params.push_back(filterResonance);
    params.push_back(filterType);
    params.push_back(filterMix);
    params.push_back(echoTime);
    params.push_back(echoFeedback);
    params.push_back(echoMix);
    params.push_back(distortionAmount);
    params.push_back(distortionMix);
    params.push_back(reverbSize);
    params.push_back(reverbDamp);
    params.push_back(reverbWidth);
    params.push_back(reverbMix);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void REORDERState::init(){
    carrierAmp = 0.0;
    carrierFreq = 0.0;
    modulatorAmp = 0.0;
    modulatorFreq = 0.0;
    ampAmp = 0.0;
    ampFreq = 0.0;
    playPosition = 0.0;
        
    params.push_back(playPosition);
    params.push_back(carrierAmp);
    params.push_back(carrierFreq);
    params.push_back(modulatorAmp);
    params.push_back(modulatorFreq);
    params.push_back(ampAmp);
    params.push_back(ampFreq);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void FMState::init(){
    amplitude = 0.0;
    carrierFreq = 0.0;
    modulatorIndex = 0.0;
    modulatorFreq = 0.0;
    cutoff = 1.0;
    resonance = 0.1;
    lfo1DutyCylce = 0.0;
    lfo1Depth = 0.0;
    lfo2DutyCylce = 0.0;
    lfo2Depth = 0.0;
    lfo3DutyCylce = 0.0;
    lfo3Depth = 0.0;
    
    params.push_back(amplitude);
    params.push_back(carrierFreq);
    params.push_back(modulatorIndex);
    params.push_back(modulatorFreq);
    params.push_back(cutoff);
    params.push_back(resonance);
    params.push_back(lfo1DutyCylce);
    params.push_back(lfo1Depth);
    params.push_back(lfo2DutyCylce);
    params.push_back(lfo2Depth);
    params.push_back(lfo3DutyCylce);
    params.push_back(lfo3Depth);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ShaderState::init(){
    for(int i = 0; i < 16; i++){
        params.push_back(0.0);
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void DestinationState::init(){
    /* -- Preset State Triggers -- */
//    avgsPreset = 1;
//    reorderPreset = 1;
//    dspPreset = 1;
//    fmPreset = 1;
//    shmPreset = 1;

    for(int i = 0; i < NUM_BUFFERS; i++){
        BufferParams bufParam;
        bufferParams.push_back(bufParam);
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
vector<Destination*> DestinationState::getDestinations() {
    vector<Destination*> dests;
    dests.reserve(4);
    
    dests.push_back(&avgs);
    dests.push_back(&dsp);
    dests.push_back(&fm);
    dests.push_back(&shader);
    
    return dests;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
float* DestinationState::paramAtIndexTree(vector<int>* indexTree){
    if (indexTree->size() == 2) {
        int destIdx = indexTree->at(0);
        int paramIdx = indexTree->at(1);
        return &getDestinations()[destIdx]->params[paramIdx];
    } else {
        ofLog(OF_LOG_ERROR, "Only checking for indexTree with 2 levels of branching!");
    }
}
