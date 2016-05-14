//
//  WaveformDrawer.cpp
//  Kortex
//
//  Created by Joshua Batty on 11/03/2016.
//
//

#include "WaveformDrawer.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
WaveformDrawer::WaveformDrawer(){
    //Waveform Drawing
    prevXpos = prevYpos = 0;
    curXpos = curYpos = 0;
    bDrawWaveform = true;
    
    mesh.setMode(OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void WaveformDrawer::draw(int yPos, AVRingBuffer &avRingBuffer, vector<AVGS*> &avgs){    
    mesh.clear();
    
    if(bDrawWaveform==true){
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
            ofSetColor(255);
            ofFill();
            
            const float waveformWidth  = ((1920)/3) - 14;
            const float waveformHeight = 124;
            
            float top = yPos - waveformHeight ;
            float left = 8;
        
            // draw the audio waveform, just one buffer at a time
            for(int i= 0; i < avRingBuffer.getLengthInSamples(); i+=BUFFER_SIZE){
                curXpos = ofMap(i,0,avRingBuffer.getLengthInSamples(),left,waveformWidth);
                curYpos = ofMap(avgs[0]->grainPlayer.samp->temp[i],-32768,32768,top,waveformHeight+top);
                mesh.addVertex(ofVec2f(curXpos,curYpos));
                mesh.addVertex(ofVec2f(prevXpos, prevYpos));
                if(i < avRingBuffer.getLengthInSamples() - BUFFER_SIZE){
                    prevXpos = curXpos;
                    prevYpos = curYpos;
                } else {
                    prevXpos = left;
                    prevYpos = waveformHeight+top;
                }
            }
        
            mesh.draw();
        
            // draw a playhead over the waveform
            ofSetColor(ofColor::white);
            for(int i =0; i < avgs.size(); i++){
                ofDrawLine(left + avgs[i]->grainPlayer.getPlayPosition() * waveformWidth, top, left + avgs[i]->grainPlayer.getPlayPosition() * waveformWidth, top + waveformHeight);
                ofDrawBitmapString("PlayHead" + ofToString(1+i), left + avgs[i]->grainPlayer.getPlayPosition() * waveformWidth-69, top+30+(i*15));
            }
            
            // Draw Current Recording Position
            ofSetColor(ofColor::red);
            ofDrawLine(left + avRingBuffer.getRecordPostion() * waveformWidth, top, left + avRingBuffer.getRecordPostion() * waveformWidth, top + waveformHeight);
            ofDrawBitmapString("RecPos", left + avRingBuffer.getRecordPostion() * waveformWidth-52, top+15);
            
            // draw a frame around the whole thing
            ofSetColor(ofColor::white);
            ofNoFill();
            ofDrawRectangle(left, top, waveformWidth, waveformHeight);
            ofFill();
        
        ofDisableAlphaBlending();
    }
}
