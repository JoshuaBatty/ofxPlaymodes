//
//  GrainReorder.h
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 5/09/14.
//
//

#pragma once

#include "ofMain.h"
#include "Algebraic.h"

#include "DestinationState.h"

class GrainReorder{
    
public:
    GrainReorder();
    void setup();
    void update(DestinationState &state);
    void draw();
    void drawOscillators();
    
    void drawCarrier(int _x, int  _y, int _w, int _h);
    void drawModulator(int _x, int  _y, int _w, int _h);
    void drawAmp(int _x, int  _y, int _w, int _h);
    void drawGrainPattern(int _x, int  _y, int _w, int _h);
    
    void speedChanged();
    
    void setCarrierShape(int _shape);
    void setModulatorShape(int _shape);
    void setAmpShape(int _shape);
    
    float getCarrier();
    float getModulator();
    float getAmp();
    float getGrainPattern();
    
    vector<ofPolyline> trail;
    float x[4], prevX[4];
    vector<float> y;
	
    float preSpeed;
    float lastUpdate;

    bool showPos;
    int rightMargin;
    float radius,initTime,t;
    
    //Algebraic
    Algebraic carrierOsc;
    Algebraic modulatorOsc;
    Algebraic ampOsc;
    
    float carrierPhase;
    float modulatorPhase;
    float ampPhase;

    float speed;
    float carrierAmp;
    float carrierFreq;
    float modulatorAmp;
    float modulatorFreq;
    float ampAmp;
    float ampFreq;
    
    int carrierShape;
    int modulatorShape;
    int ampShape;
    
    float carrierSignal;
    float modulatorSignal;
    float ampSignal;
    float masterSignal;
};
