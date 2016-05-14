//
//  AVGSManager.h
//  Kortex_v3
//
//  Created by Joshua Batty on 3/11/2014.
//
//


#pragma once

#include "ofSoundUnit.h"
#include "ofMain.h"
#include "Constants.h"

#include "AVSource.h"
#include "AVRingBuffer.h"

#include "AVPathLoader.h"
#include "AVGS.h"
#include "WaveformDrawer.h"


class AVGSManager : public ofSoundSource{
    
public:
    ~AVGSManager();
    void setup(string _lastSavedProjectName, bool _bNewProject);
    void update(vector<DestinationState> &state);
    void draw();

    //AUDIO
    void audioRequested(float * buffer, int numFrames, int numChannels);
    string getName() { return "AVGSManager"; }
        
    AVSource     avSource;
    //AVRingBuffer avRingBuffer;
    AVPathLoader avPathLoader;
   
    vector<AVRingBuffer*> avRingBuffer;
    vector<AVGS*> player;
    
    vector<WaveformDrawer> waveformDrawer;
    bool bDrawWaveform;
    
private:
};
