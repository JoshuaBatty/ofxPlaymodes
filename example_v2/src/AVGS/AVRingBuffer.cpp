//
//  AVRingBuffer.cpp
//  Kortex
//
//  Created by Joshua Batty on 9/03/2016.
//
//

#include "AVRingBuffer.h"
#include "PlayModes.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
AVRingBuffer::AVRingBuffer(){
    fps = 60;
    recordPosition = 0;
    bRecord = true;
    bRecordSwitch = true;
    bRecLiveInput = true;
}

//--------------------------------------------------------------
void AVRingBuffer::setup(){
    tGrabber.initGrabber(640, 480);
    vRate.setup(tGrabber,fps);
    vBuffer.setup(vRate,400,false,true);
    samp.load(ofToDataPath("400Frames(60fps).wav"));
    
   // setLengthInFrames(100);
}

// 1. tGrabber writes a VideoFrame into vBuffer
// 2.


//--------------------------------------------------------------
void AVRingBuffer::setFramePos(float _framePos){
    vBuffer.setFramePos(_framePos);
}

//--------------------------------------------------------------
void AVRingBuffer::recordingStatus(){
    if(bRecord) {
        if(bRecordSwitch==true){
            vBuffer.resume();
        }
        bRecordSwitch = false;
    } else if(bRecord==false){
        if(bRecordSwitch==false){
            vBuffer.stop();
        }
        bRecordSwitch = true;
    }
}

//--------------------------------------------------------------
void AVRingBuffer::setTextureRef(ofTexture &tex){
    tGrabber.begin();
        ofClear(0,0,0,0);
        tex.draw(0,0,640,480);
    tGrabber.end();
}

//--------------------------------------------------------------
void AVRingBuffer::update(){
    setFramePos(getRecordPostion()); //Here we use the audio record postion to
                                     //set the % of the video buffer to write to
    recordingStatus();
    tGrabber.update();
    vRate.setFps(fps);
    
    if(ofGetKeyPressed()){
      //  setLengthInFrames(ofRandom(30,240));
    }
}

//--------------------------------------------------------------
void AVRingBuffer::setLengthInSamples(float numSamples){
    samp.setRingBufferLength(numSamples);
    vBuffer.clear();
    vBuffer.setSize(getLengthInFrames());
}
//--------------------------------------------------------------
void AVRingBuffer::setLengthInFrames(int numFrames){
    samp.setRingBufferLength((numFrames / fps) * (float)SAMPLE_RATE * NUM_CHANNELS);
    vBuffer.clear();
    vBuffer.setSize(numFrames);
}
//--------------------------------------------------------------
int AVRingBuffer::getLengthInSamples(){
    return samp.ringBufferLength;
}
//--------------------------------------------------------------
int AVRingBuffer::getLengthInFrames(){
    return floor(((float)(samp.ringBufferLength / NUM_CHANNELS) / (float)SAMPLE_RATE) * fps);
}

//--------------------------------------------------------------
float AVRingBuffer::getRecordPostion(){
    return recordPosition;
}

//--------------------------------------------------------------
void AVRingBuffer::loopRecordAudio(vector<double> input){
    recordPosition = samp.recordPosition / (double)getLengthInSamples();
    // This records the next sample that we were given into the `samp` ringbuffer.
    float recordAmp = 0.0;
    for(int i = 0; i < input.size()/NUM_CHANNELS; i++){
        samp.loopRecord(input[i*NUM_CHANNELS], bRecLiveInput, recordAmp);
    }
}
