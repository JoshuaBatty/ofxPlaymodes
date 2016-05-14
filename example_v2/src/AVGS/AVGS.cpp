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

    width = 1920;
    
    //Post Process
    bloom_amount = 0.0;
    
    //Grain
    randomPitch = 0.0;
    randomGrainSize = 0.0;
    bDrawOscillators = true;
}

//--------------------------------------------------------------
void AVGS::setup(AVRingBuffer *avRingBuffer){
    
    playModes.setup(&avRingBuffer->vBuffer);
    grainPlayer.setup(avRingBuffer);
    grainReorder.setup();

    //Post Processing
    ofDisableArbTex();
    
    post.setup();
    
    playModesFbo.allocate(512, 512, GL_RGBA);
    bloomFbo.allocate(512, 512, GL_RGBA);
    bloomShader.load("Shaders/FX/bloom");
    
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
   // cout << "NEW GRAIN" << endl;
    
    grainPlayer.setPositionRandom(randomPosition);
    grainPlayer.setGrainSizeRandom(randomGrainSize);
    grainPlayer.setPitchRandom(randomPitch);
}

//--------------------------------------------------------------
void AVGS::update(DestinationState &state, AVRingBuffer &avRingBuffer){
    randomPosition = state.avgs.params[RANDOMPOSITION];
    randomGrainSize = state.avgs.params[RANDOMGRAINSIZE];
    randomPitch = state.avgs.params[RANDOMPITCH];

    grainReorder.update(state);
    grainPlayer.update(state);

    grainPlayer.updatePlayHead();
    grainPlayer.setGrainReordering(grainReorder.getGrainPattern());
    
    playModes.setNumPlayers(state.avgs.params[OVERLAPS]);
    playModes.setVideoGrainAmount(state.avgs.params[GRAINSIZE]);
    
    if(avRingBuffer.bRecLiveInput==false){
        if(!grainPlayer.bSetPosition==true){
            playModes.setDelay(grainPlayer.getPlayPosition());
        } else {
            playModes.setDelay(grainPlayer.grainPosition);
        }
    }
    else if(avRingBuffer.bRecLiveInput==true){
        playModes.setDelay(grainPlayer.getPlayPosition());
    }
    
    float multiplier = 1 + (state.avgs.params[MULTIPLIER] * 100.0);
    playModes.setSpeed(state.avgs.params[SPEED] * multiplier);
    playModes.update();
    
    post.update(state);
    
    post.beginScene();
    drawPostProcessing(0,0,1920,1200);
    ofDisableAlphaBlending();
    post.endScene();
}

//--------------------------------------------------------------
void AVGS::draw(int _x, int _y, int _w, int _h){

    post.draw(_x,_y,_w,_h);
//    bloomFbo.draw(_x,_y,_w,_h);
}

//--------------------------------------------------------------
void AVGS::drawOscillators(){
    if(bDrawOscillators == true){
        grainReorder.drawOscillators();
        grainReorder.draw();
    }
}

//--------------------------------------------------------------
void AVGS::drawPostProcessing(int _x, int _y, int _w, int _h){
    
    ///////////////////////////
    playModesFbo.begin();
    ofClear(0,0,0,0);
    
    if(playModes.blendMode == 4 || playModes.blendMode == 3){
        ofSetColor(255,255);
        ofDrawRectangle(0,0,512,512);
    }
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(playModes.blendMode);
    
    ofSetColor(255, 255, 255, 255);
    playModes.draw(0, 0, 512, 512);
    
//    avStreamBuffer->shaderFbo.draw(0,0,512,512);

    playModesFbo.end();
    
    
    ////////////////////////
    playModesFbo.getTexture().bind();
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
    ofDrawRectangle( 0,0,512,512 ) ;
    bloomShader.end();
    bloomFbo.end();
    playModesFbo.getTexture().unbind();
    
    ////////////////////////////
    ofSetColor(255, 255, 255,255);
    bloomFbo.draw(_x, _y, _w, _h);
}




