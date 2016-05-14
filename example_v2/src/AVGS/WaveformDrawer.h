//
//  WaveformDrawer.h
//  Kortex
//
//  Created by Joshua Batty on 11/03/2016.
//
//

#pragma once
#include "ofMain.h"
#include "AVSource.h"
#include "AVRingBuffer.h"
#include "AVGS.h"

class WaveformDrawer{
    
public:
   
    WaveformDrawer();
    void draw(int yPos, AVRingBuffer &avRingBuffer, vector<AVGS*> &grainPlayers);
    
    bool bDrawWaveform;
    
private:
    //Waveform Drawing
    int curXpos, curYpos;
    int prevXpos, prevYpos;
    
    ofMesh mesh;
};