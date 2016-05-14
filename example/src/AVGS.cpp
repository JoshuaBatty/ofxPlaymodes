//
//  AVGS.cpp
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 17/07/14.
//
//

#include "AVGS.h"

//--------------------------------------------------------------
AVGS::AVGS(){
    //AUDIO
    sampleRate 	= 44100;
    bufferSize = 512;
    
    //Post Process
    bloom_amount = 0.0;
    
    //Grain
    randomPitch = 0.0;
    randomGrainSize = 0.0;
}

//--------------------------------------------------------------
void AVGS::setup(){

    playModes.setup();
    
    grainPlayer.setup();
    mixer.addInputFrom(&grainPlayer);
    
    //SoundStream
    soundStream.listDevices();
	soundStream.setup(this, 2, 2, sampleRate, bufferSize, 4);
	soundStream.setOutput(this);

    //Post Processing
    ofDisableArbTex();

    playModesFbo.allocate(512, 512, GL_RGBA);
    bloomFbo.allocate(512, 512, GL_RGBA);
    bloomShader.load("Shaders/bloom");
    
    playModesFbo.begin();
        ofClear(0,0,0,0);
    playModesFbo.end();
    
    bloomFbo.begin();
        ofClear(0,0,0,0);
    bloomFbo.end();
    
    //Events
    ofAddListener(playModes.newGrain, this, &AVGS::bNewGrain);
    
}

//--------------------------------------------------------------
void AVGS::bNewGrain(bool &b){
    cout << "NEW GRAIN" << endl;
    
    grainPlayer.setPositionRandom(randomPosition);
    grainPlayer.setGrainSizeRandom(randomGrainSize);
    grainPlayer.setPitchRandom(randomPitch);
}

//--------------------------------------------------------------
void AVGS::update(){
    
    grainPlayer.updatePlayHead();
    
//    playModes.setFramePos((float)grainPlayer.getRecordPostion()); //Here we use the audio record postion to
    //set the % of the video buffer to write to
    
    playModes.setNumPlayers(grainPlayer.overlaps);
    playModes.setVideoGrainAmount(grainPlayer.grainLength);
    
    if(grainPlayer.bRecLiveInput==false){
        if(!grainPlayer.bSetPosition==true){
            playModes.setDelay(grainPlayer.getPlayPosition());
        } else {
            playModes.setDelay(grainPlayer.grainPosition);
        }
    }
    else if(grainPlayer.bRecLiveInput==true){
        playModes.setDelay(grainPlayer.getPlayPosition());
    }
    
    playModes.setSpeed(grainPlayer.speed);
    playModes.update();

}

//--------------------------------------------------------------
void AVGS::drawPostProcessing(){

    ///////////////////////////
    playModesFbo.begin();
        ofClear(0,0,0,0);
    
        if(playModes.blendMode == 4 || playModes.blendMode == 3){
            ofSetColor(255,255);
            ofRect(0,0,512,512);
        }
        
        ofEnableAlphaBlending();
        ofEnableBlendMode(playModes.blendMode);
        
        ofSetColor(255, 255, 255, 255);
        playModes.draw(0, 0, 512, 512);
    playModesFbo.end();
  
    
    ////////////////////////
    playModesFbo.getTextureReference().bind();
    bloomFbo.begin();
    ofClear(0, 0, 0,0);
     bloomShader.begin();
     bloomShader.setUniform1f("iGlobalTime", ofGetElapsedTimef() );
     bloomShader.setUniform3f("iResolution", 512, 512, 0 );
    if(grainPlayer.pitch > 1.0){
        bloomShader.setUniform1f("intensity", ofMap(grainPlayer.grainPitch,01.0, 4.0, 0.0, 0.3));
    } else {
        bloomShader.setUniform1f("intensity", ofMap(grainPlayer.grainPitch,0.0, 1.0, -0.5, 0.0));
    }
    
     ofSetColor( 255 , 255 , 255 , 255) ;
     ofFill();
     ofRect( 0,0,512,512 ) ;
     bloomShader.end();
    bloomFbo.end();
    playModesFbo.getTextureReference().unbind();

    ////////////////////////////
    ofSetColor(255, 255, 255,255);
    bloomFbo.draw(250,10,ofGetWidth()-6,ofGetHeight()-280);
}

//--------------------------------------------------------------
void AVGS::draw(){
    
    drawPostProcessing();
    ofDisableAlphaBlending();

    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofSetColor(255);
    grainPlayer.draw();
    
    playModes.drawData();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void AVGS::audioReceived(float * input, int bufferSize, int nChannels)
{
    grainPlayer.audioReceived(input,bufferSize,nChannels);
}

//--------------------------------------------------------------
void AVGS::audioRequested (float * output, int numFrames, int nChannels)
{
    mixer.audioRequested(output, bufferSize, nChannels);
}

