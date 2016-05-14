//
//  PostProcessing.h
//  Kortex_v3
//
//  Created by Joshua Batty on 4/10/2014.
//
//

#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "DestinationState.h"

class PostProcess {
public:
//            VideoDelayPass(const ofVec2f& aspect, bool arb, float param = 0.1, float blendAmount = 1.0);
//    PostProcess(){}
  
    PostProcess(){}

	void setup();
    void init();
    void update(DestinationState &state);
	void beginScene();
    void endScene();
    void draw(int _x, int _y, int _w, int _h);
   
    ofxPostProcessing post;
    
//    AcidShiftPass::Ptr           acidShift;
//    MeltGlitchPass::Ptr          meltGlitch;
//    DnaForcePass::Ptr            dnaForce;
//    ColorZebraPass::Ptr          zebra;
//    ChromaticAbberationPass::Ptr chroma;
    KaleidoscopePass::Ptr        kaleido;
//    NoiseWarpPass::Ptr           warp;
//    EdgePass::Ptr                edge;
//    IntegerHexelPass::Ptr        hexel;
//    DistortionPass::Ptr          distortion;
    
    PixelatePass::Ptr            pixelate;
    FlangePass::Ptr              flange;
//    VideoDelayPass::Ptr          delay;
    FilterPass::Ptr              filter;
    KortexDistortionPass::Ptr    distortion;
    HashedBlurPass::Ptr          blur;
    
    // Could Distortion be some sort of polygonal tex Z offset?
    
    ///// GENERATIVE
//    vector<int> genTime;
//    void randomiseEffectPass();
//    
//    void randomiseAcidShift(int timeInFrames);
//    void randomiseColourZebra(int timeInFrames);
};