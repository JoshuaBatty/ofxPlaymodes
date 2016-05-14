//
//  PostProcessing.cpp
//  Kortex_v3
//
//  Created by Joshua Batty on 4/10/2014.
//
//

#include "PostProcess.h"

void PostProcess::setup(){
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    
    // Setup post-processing chain
    post.init(1024, 1024);
//    post.init(1920, 1080);
    
    pixelate = post.createPass<PixelatePass>();
    flange = post.createPass<FlangePass>();
    delay = post.createPass<VideoDelayPass>();
    filter = post.createPass<FilterPass>();
    distortion = post.createPass<KortexDistortionPass>();
    blur = post.createPass<HashedBlurPass>();
 //   meltGlitch = post.createPass<MeltGlitchPass>();

//    chroma = post.createPass<ChromaticAbberationPass>();
//    zebra = post.createPass<ColorZebraPass>();
//    dnaForce = post.createPass<DnaForcePass>();
    kaleido = post.createPass<KaleidoscopePass>();
//    warp = post.createPass<NoiseWarpPass>();
//    edge = post.createPass<EdgePass>();
//    hexel = post.createPass<IntegerHexelPass>();
//    acidShift = post.createPass<AcidShiftPass>();
    
    pixelate->setEnabled(false);
    flange->setEnabled(false);
    delay->setEnabled(false);
    filter->setEnabled(false);
    filter->init();
    distortion->setEnabled(false);
    distortion->setDistortionAmount(0.0);
    blur->setEnabled(false);
    
//    meltGlitch->setEnabled(false);
//    meltGlitch->setDistortionAmount(0.0);

//    chroma->setEnabled(false);
//    zebra->setEnabled(false);
//    dnaForce->setEnabled(false);
    kaleido->setEnabled(false);
//    warp->setEnabled(false);
//    edge->setEnabled(false);
//    hexel->setEnabled(false);
//    acidShift->setEnabled(true);
    

    
    pixelate->setID(0);
    flange->setID(1);
    delay->setID(2);
    filter->setID(3);
    distortion->setID(4);
    blur->setID(5);

    //   meltGlitch->setID(4);

//    genTime.push_back(1);
//    genTime.push_back(1);
    
//    delay->setup();
}

void PostProcess::update(DestinationState &state){
    //delay->update();
    
    // Pixelate
    pixelate->setAmount(ofClamp(state.dsp.params[BITCRUSH_FREQ], 0.0,1.0));
    pixelate->setBlendAmount(state.dsp.params[BITCRUSH_MIX]);
    
    // Flanger
    flange->setModAmount(state.dsp.params[FLANGE_DELAY]);
    flange->setModSpeed(state.dsp.params[FLANGE_FREQ]);
    flange->setSpreadAmount(state.dsp.params[FLANGE_MOD]);
    flange->setBlendAmount(state.dsp.params[FLANGE_MIX]);
    
    // Filter
    filter->setCutoffAmount(state.dsp.params[FILTER_CUTOFF]);
    filter->setFilterType(state.dsp.params[FILTER_TYPE]);
    filter->setBlendAmount(state.dsp.params[FILTER_MIX]);
    
    // Distortion
    distortion->setDistortionAmount(state.dsp.params[DISTORTION_AMOUNT]);
    distortion->setBlendAmount(state.dsp.params[DISTORTION_MIX]);
    
    //Blur
    blur->setBlurAmount(ofClamp(state.dsp.params[REVERB_SIZE], 0.0,1.0));
    blur->setBlendAmount(state.dsp.params[REVERB_MIX]);
    
//    int oneSecond = 60;
//    
//    if(ofGetFrameNum() % 360 == 0){
//        for(int i = 0; i < genTime.size(); i++){
//            genTime[i] = (int)ofRandom(1,oneSecond);
//        }
//    }
    
//#ifdef GENERATIVE
//    randomiseEffectPass();
//    randomiseAcidShift(oneSecond*genTime[0]);
//    randomiseColourZebra(oneSecond*genTime[1]);
//#endif
}

void PostProcess::beginScene(){
    glPushAttrib(GL_ENABLE_BIT); // copy enable part of gl state
    post.begin(); // begin scene to post process
}

void PostProcess::endScene(){
    post.end(); // end scene and draw
    
    glPopAttrib(); // set gl state back to original
}

void PostProcess::draw(int _x, int _y, int _w, int _h){
    ofSetColor(255);
    post.draw(_x,_y,_w*3,_h);

//    delay->draw();
   // delay->vRenderer.draw(_x,_y,_w*3,_h);
}

/*
///// GENERATIVE
void PostProcess::randomiseEffectPass(){
    int oneSecond = 60;
    int oneMinute = oneSecond * 60;
    static int genTime;
    
    if(ofGetFrameNum() % oneMinute == 0){
        genTime = oneSecond + ofRandom(oneMinute);
    }
    // ZEBRA
    if(ofGetFrameNum() % genTime == 0){
        zebra->setEnabled((int)ofRandom(2));
    }
    // ACID SHIFT
    if(ofGetFrameNum() % genTime == 0){
        acidShift->setEnabled((int)ofRandom(2));
    }
}

void PostProcess::randomiseAcidShift(int timeInFrames){
    int genTime = timeInFrames;
    if(ofGetFrameNum() % genTime == 0){
        acidShift->setAmpAmount(ofRandom(40));
        acidShift->setSpeedAmount(ofRandom(100));
        float probability = ofRandomuf();
        if(probability > 0.33){
            acidShift->setBlendAmount(ofRandomuf()*0.2);
        } else {
            acidShift->setBlendAmount(0.5+(cos(ofGetElapsedTimef()*ofRandom(0.0,4.))*0.5));
        }
    }
}
void PostProcess::randomiseColourZebra(int timeInFrames){
    int genTime = timeInFrames;
    
    if(ofGetFrameNum() % genTime == 0){
        zebra->setSpeed(ofRandom(20.0));
        zebra->setPoly(ofRandom(1,10));//1.0+(cos(ofGetFrameNum()*0.03)*10.5));
        zebra->setOffset(0.5+(sin(ofGetFrameNum()*0.03)*0.5));
        float probability = ofRandomuf();
        if(probability > 0.33){
            zebra->setBlendAmount(ofRandomuf()*0.2);
        } else {
            zebra->setBlendAmount(0.5+(sin(ofGetElapsedTimef()*ofRandom(0.0,4.))*0.5));
        }
    }
}
*/