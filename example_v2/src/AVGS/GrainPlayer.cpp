//
//  GrainPlayer.cpp
//  testCpp11
//
//  Created by Joshua Batty on 4/06/14.
//
//

#include "GrainPlayer.h"

//--------------------------------------------------------------
GrainPlayer::~GrainPlayer(){
    delete ps1;
    delete ps2;
    delete ps3;
    delete ps4;
    delete ps5;
}

//--------------------------------------------------------------
void GrainPlayer::assignAudioBuffer(AVRingBuffer *_avRingBuffer){
    samp = &_avRingBuffer->samp;
    LENGTH_IN_SAMPLES = _avRingBuffer->getLengthInSamples();
}

//--------------------------------------------------------------
void GrainPlayer::setup(AVRingBuffer *_avRingBuffer){
    assignAudioBuffer(_avRingBuffer);

    //Recording
    bUpdatePlayheadEvent = false;
    
    //Speed Sherrif
    bSpeedSherrif = false;
    speedFrameCount = 0;
    
    //Granular
    speed = 0.985; // Put the play position just a tad behind the record position to prevent clicking
    grainSize = 0.25;
    pitch = 1.0;
    overlaps = 3;
    
    //Master
    playHead = 0.0;
    grainPosition = 0.0;
    grainPitch = 1.0;
    grainLength = 0.25;
    bSetPosition = false;

    //Modifier
    playPosModifier = 0.0;
    
    grainEnv = 1;

    volume = 0.95;
    
    //Maxi
    ps1 = new maxiTimePitchStretch<gaussianWinFunctor, maxiSample>(samp);
    ps2 = new maxiTimePitchStretch<triangleWinFunctor, maxiSample>(samp);
    ps3 = new maxiTimePitchStretch<rectWinFunctor, maxiSample>(samp);
    ps4 = new maxiTimePitchStretch<pulseExpWinFunctor, maxiSample>(samp);
    ps5 = new maxiTimePitchStretch<revPulseExpWinFunctor, maxiSample>(samp);
    
    ofxMaxiSettings::setup(SAMPLE_RATE, 2, BUFFER_SIZE);
}

//--------------------------------------------------------------
void GrainPlayer::setPosition(float _pos){
    switch (grainEnv) {
        case 1:
            ps1->setPosition(_pos);
            break;
        case 2:
            ps2->setPosition(_pos);
            break;
        case 3:
            ps3->setPosition(_pos);
            break;
        case 4:
            ps4->setPosition(_pos);
            break;
        case 5:
            ps5->setPosition(_pos);
            break;
    }
}

//--------------------------------------------------------------
float GrainPlayer::getPlayhead(){
    return ofWrap(playHead + grainPattern + playPosModifier, 0.0, 1.0);
}

//--------------------------------------------------------------
void GrainPlayer::setGrainReordering(float _pos){
    grainPattern = _pos;
}

//--------------------------------------------------------------
void GrainPlayer::setPlayheadPosition(float _pos){
    playHead = _pos;
}

//--------------------------------------------------------------
void GrainPlayer::setPositionRandom(float _val){
    grainPosition = ofWrap(getPlayhead() + (ofRandom(0.0,_val)),0.0,1.0);
}

//--------------------------------------------------------------
void GrainPlayer::setGrainSizeRandom(float _val){
    grainLength = grainSize * (ofRandom(0.0,_val)+1);
    ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE);
}

//--------------------------------------------------------------
void GrainPlayer::setPitchRandom(float _val){
    grainPitch = pitch * (ofRandom(0.0, ofMap(_val, 0.0, 1.0, 0.0, 4.0))+1);
}

//--------------------------------------------------------------
void GrainPlayer::setGrainEnvelope(int _grainEnv){
    grainEnv = _grainEnv;
}

//--------------------------------------------------------------
void GrainPlayer::updatePlayHead(){
    if(!bSetPosition) {
        if(bUpdatePlayheadEvent==true){
            setPosition(ofMap(samp->recordPosition,0,LENGTH_IN_SAMPLES,0.0,1.0));
            
            if(speed >= 0.95 || speed <= 1.05) { bSpeedSherrif = true; speedFrameCount = 0; }
            bUpdatePlayheadEvent = false;
        }
    } else {
        bUpdatePlayheadEvent = true;
    }
    
//    if(ofGetFrameNum()==100){
//        cout << "cent" << endl;
//        speed = 1.0;
//    }
}

//--------------------------------------------------------------
void GrainPlayer::resetPlayheadToRecordPosition(){
    setPosition(ofMap(samp->recordPosition,0,LENGTH_IN_SAMPLES,0.0,1.0));
    
    if(speed >= 0.95 || speed <= 1.05) { bSpeedSherrif = true; speedFrameCount = 0; }
}

//--------------------------------------------------------------
void GrainPlayer::update(DestinationState &state){
    playHead = ofWrap(state.avgs.params[PLAYPOS], 0.0, 1.0);
    speed = ofClamp(state.avgs.params[SPEED], -100.0, 100.0);
    multiplier = ofClamp(state.avgs.params[MULTIPLIER], -100.0, 100.0);
    pitch = ofClamp(state.avgs.params[PITCH], 0.0, 4.0);
    grainSize = ofClamp(state.avgs.params[GRAINSIZE], 0.025, MAX_GRAIN_SIZE);
    overlaps = ofClamp(state.avgs.params[OVERLAPS], 1, NUM_PLAYERS);
    
    float multiplier = 1 + (state.avgs.params[MULTIPLIER] * 100.0);
    speed *= multiplier;
    
    if(bSpeedSherrif == true){
        speed = 0.40;
        if(speedFrameCount > 2){
            speed = 1.0;
            bSpeedSherrif = false;
        }
    }
    speedFrameCount++;
}

//--------------------------------------------------------------
void GrainPlayer::audioRequested (float * output, int numFrames, int nChannels)
{
    for (int i = 0; i < numFrames; i++)
    {
        //Play the Granular Synth play method
        if(bSetPosition == true){
            setPosition(grainPosition);
        }
        
        switch (grainEnv) {
            case 1:
                wave = ps1->play(grainPitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);
                break;
            case 2:
                wave = ps2->play(grainPitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);
                break;
            case 3:
                wave = ps3->play(grainPitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);
                break;
            case 4:
                wave = ps4->play(grainPitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);
                break;
            case 5:
                wave = ps5->play(grainPitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);
                break;
        }
        
        
        //wave = ps2->play(pitch, speed, ofClamp(grainLength, 0.025, MAX_GRAIN_SIZE), (int)overlaps);

        
        mymix.stereo(wave, outputs, 0.5);
        output[i*nChannels    ] = outputs[0] * volume;
        output[i*nChannels + 1] = outputs[1] * volume;
    }
}



//--------------------------------------------------------------
float GrainPlayer::getPlayPosition(){
    float pos;
    switch (grainEnv) {
        case 1:
            pos = ps1->getNormalisedPosition();
            break;
        case 2:
            pos = ps2->getNormalisedPosition();
            break;
        case 3:
            pos = ps3->getNormalisedPosition();
            break;
        case 4:
            pos = ps4->getNormalisedPosition();
            break;
        case 5:
            pos = ps5->getNormalisedPosition();
            break;
    }
    return pos;
}


