//
//  GrainPlayer.cpp
//  testCpp11
//
//  Created by Joshua Batty on 4/06/14.
//
//

#include "GrainPlayer.h"

//--------------------------------------------------------------
void GrainPlayer::setSampleRate( int rate )
{
	sampleRate = rate;
    
    //Drawing
    prevXpos = prevYpos = 0;
    curXpos = curYpos = 0;
    
    //Granular
    speed = 0.98; // Put the play position just a tad behind the record position to prevent clicking
    grainSize = 0.25;
    pitch = 1.0;
    playHead = 0.01;
    overlaps = 3;
    
    //Master
    grainPosition = 0.0;
    grainPitch = 1.0;
    grainLength = 0.25;
    
    grainEnv = 1;
    

}

//--------------------------------------------------------------
void GrainPlayer::setup(){
    
    //Recording
    recordPosition = 0;
    bSetPosition = false;
    bRecLiveInput = true;
    bUpdatePlayheadEvent = false;
    recMix = 0.0;
    
    volume = 0.50;
    sampleRate 	= 44100;
    bufferSize = 512;
    
    //Maxi
    samp.load(ofToDataPath("400Frames(60fps).wav"));
    sampFile.load(ofToDataPath("Video/Josh_AVGS_demo.wav"));
    ps1 = new maxiPitchStretch<gaussianWinFunctor>(&samp);
    ps2 = new maxiPitchStretch<cosineWinFunctor>(&samp);
    ps3 = new maxiPitchStretch<triangleWinFunctor>(&samp);
    ps4 = new maxiPitchStretch<rectWinFunctor>(&samp);
    ps5 = new maxiPitchStretch<hannWinFunctor>(&samp);
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
}

//--------------------------------------------------------------
void GrainPlayer::updatePlayHead(){
    if(!bSetPosition) {
        if(bUpdatePlayheadEvent==true){
            setPosition(ofMap(samp.recordPosition,0,LENGTH,0.0,1.0));
        }
        bUpdatePlayheadEvent = false;
    } else {
        bUpdatePlayheadEvent = true;
    }
    
    if(ofGetFrameNum()==100){
        speed = 1.0;
    }
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
void GrainPlayer::setGrainEnvelope(int _grainEnv){
    grainEnv = _grainEnv;
}

//--------------------------------------------------------------
void GrainPlayer::draw(){
    drawWaveform();
}

//--------------------------------------------------------------
void GrainPlayer::setPositionRandom(float _val){
    grainPosition = ofWrap(playHead + (ofRandom(0.0,_val)),0.0,1.0);
}

//--------------------------------------------------------------
void GrainPlayer::setGrainSizeRandom(float _val){    
    grainLength = grainSize * (ofRandom(0.0,_val)+1), 0.025, 0.45;
    ofClamp(grainLength, 0.025, 0.45);
}

//--------------------------------------------------------------
void GrainPlayer::setPitchRandom(float _val){
    grainPitch = pitch * (ofRandom(0.0, ofMap(_val, 0.0, 1.0, 0.0, 4.0))+1);
}

//--------------------------------------------------------------
void GrainPlayer::audioReceived(float * input, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++)
    {
        samp.loopRecord(input[i*nChannels], bRecLiveInput, recMix);
        recordPosition = samp.recordPosition / (double)(LENGTH);
    }
}

//--------------------------------------------------------------
void GrainPlayer::audioRequested (float * output, int numFrames, int nChannels)
{
 
    for (int i = 0; i < bufferSize; i++)
    {
        float waveFile = sampFile.play();
       // samp.loopRecord(waveFile, bRecLiveInput, recMix);

        //Play the Granular Synth play method
        if(bSetPosition == true){
            setPosition(grainPosition);
        }

        
        switch (grainEnv) {
            case 1:
                wave = ps1->play(&windowAmp, grainPitch, speed, grainLength, (int)overlaps);
                break;
            case 2:
                wave = ps2->play(&windowAmp, grainPitch, speed, grainLength, (int)overlaps);
                break;
            case 3:
                wave = ps3->play(&windowAmp, grainPitch, speed, grainLength, (int)overlaps);
                break;
            case 4:
                wave = ps4->play(&windowAmp, grainPitch, speed, grainLength, (int)overlaps);
                break;
            case 5:
                wave = ps5->play(&windowAmp, grainPitch, speed, grainLength, (int)overlaps);
                break;
        }
        
        mymix.stereo(wave, outputs, 0.5);
        output[i*nChannels    ] = outputs[0] * volume;
        output[i*nChannels + 1] = outputs[1] * volume;
    }
}

//--------------------------------------------------------------
float GrainPlayer::getRecordPostion(){
    return recordPosition;
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

//--------------------------------------------------------------
void GrainPlayer::drawWaveform(){
    ofSetColor(255);
    ofFill();
    
    const float waveformWidth  = ofGetWidth() - 40;
    const float waveformHeight = 220;
    
    float top = ofGetHeight() - waveformHeight - 20;
    float left = 20;
    
    // draw the audio waveform
    for(int i= 0; i < LENGTH; i+=bufferSize){
        curXpos = ofMap(i,0,LENGTH,left,waveformWidth);
        curYpos = ofMap(samp.temp[i],-32768,32768,top,waveformHeight+top);
        ofEllipse(curXpos, curYpos, 2, 2);
        ofLine(curXpos, curYpos, prevXpos, prevYpos);
        if(i < LENGTH-bufferSize){
            prevXpos = curXpos;
            prevYpos = curYpos;
        } else {
            prevXpos = left;
            prevYpos = waveformHeight+top;
        }
    }
  
    // draw a playhead over the waveform
    ofSetColor(ofColor::white);
    ofLine(left + getPlayPosition() * waveformWidth, top, left + getPlayPosition() * waveformWidth, top + waveformHeight);
    ofDrawBitmapString("PlayHead", left + getPlayPosition() * waveformWidth-69, top+30);
  
    // Draw Current Recording Position
    ofSetColor(ofColor::red);
    ofLine(left + getRecordPostion() * waveformWidth, top, left + getRecordPostion() * waveformWidth, top + waveformHeight);
    ofDrawBitmapString("RecPos", left + getRecordPostion() * waveformWidth-52, top+15);

    // draw a frame around the whole thing
    ofSetColor(ofColor::white);
    ofNoFill();
    ofRect(left, top, waveformWidth, waveformHeight);

    ofFill(); // For some reason I need to call ofFill() again to remove the memeory glitch? wtf
}

