//
//  AVGSManager.cpp
//  Kortex_v3
//
//  Created by Joshua Batty on 3/11/2014.
//
//

#include "AVGSManager.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
AVGSManager::~AVGSManager(){
    for(auto buffer : avRingBuffer) delete buffer;
    for(auto avgs : player) delete avgs;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void AVGSManager::setup(string _lastSavedProjectName, bool _bNewProject){
    avSource.setup();
    //avRingBuffer.setup();
    
    avPathLoader.setup(&avSource, _lastSavedProjectName, _bNewProject);
    avSource.setMediaPaths(avPathLoader.videoPaths, avPathLoader.audioPaths);

    for(int i =0 ; i < NUM_BUFFERS; i++){
        AVRingBuffer* ringBuffer = new AVRingBuffer; /// I should be using unique ptrs....
        avRingBuffer.push_back(ringBuffer);
        avRingBuffer[i]->setup();
        
        WaveformDrawer wave;
        waveformDrawer.push_back(wave);
    }
    
    for(int i = 0 ; i < NUM_SCREENS; i++){
        AVGS* avgs = new AVGS;
        player.push_back(avgs);
        player[i]->setup(avRingBuffer[i]);
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void AVGSManager::update(vector<DestinationState> &state){
    avSource.update();
    
    for(int i =0 ; i < NUM_BUFFERS; i++){
        avRingBuffer[i]->update();
    }
    
    avRingBuffer[0]->setTextureRef(avSource.getTexture());
    
    for(int i = 0 ; i < NUM_SCREENS; i++){
        player[i]->update(state[i],*avRingBuffer[i]);
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void AVGSManager::draw(){

    if(bDrawWaveform){
        waveformDrawer[0].draw(532+(124*0), *avRingBuffer[0], player);
    }
    
//    for(int i =0 ; i < NUM_BUFFERS; i++){
//        waveformDrawer[i].draw(532+(124*i), avRingBuffer, player);
//    }
    
    for(int i = 0; i < NUM_SCREENS; i++){
        player[i]->drawOscillators(); //Grain reordering pattern
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void AVGSManager::audioRequested(float * buffer, int numFrames, int numChannels){
    avRingBuffer[0]->loopRecordAudio(*avSource.getNextAudioBuffer());
}

