//
//  AVRingBuffer.h
//  Kortex
//
//  Created by Joshua Batty on 9/03/2016.
//
//

// This class is essentially an AudioVisual Ring Buffer that has content fed into it


#pragma once

#include "ofMain.h"
#include "VideoBuffer.h"
#include "VideoRate.h"
#include "TextureGrabber.h"
#include "ofxMaxim.h"

class AVRingBuffer{
    
public:
    AVRingBuffer();

    void setup();
    void update();
    void setTextureRef(ofTexture &tex);
    
    void setLengthInSamples(float numSamples);
    void setLengthInFrames(int numFrames);
    
    int getLengthInSamples();
    int getLengthInFrames();
    
    float getRecordPostion();

    void setFramePos(float _framePos);
    void recordingStatus();
    void loopRecordAudio(vector<double> input);
    string getName() { return "AVRingBuffer"; }
    
    ofxPm::VideoBuffer vBuffer;
    ofxPm::TextureGrabber tGrabber;
    ofxPm::VideoRate vRate;
    
    maxiSample samp;
    
    bool bRecord;
    bool bRecLiveInput;
    
private:
    bool bRecordSwitch;
    float recordPosition;
    int fps;
};
