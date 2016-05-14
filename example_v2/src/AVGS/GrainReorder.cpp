//
//  GrainReorder.cpp
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 5/09/14.
//
//

#include "GrainReorder.h"

//--------------------------------------------------------------
GrainReorder::GrainReorder(){
    carrierFreq = 0.0;
    modulatorFreq = 0.0;
    ampFreq = 0.0;
    
    carrierAmp = 0.0;
    modulatorAmp = 0.0;
    ampAmp = 0.0;
    
    carrierShape = 0;
    modulatorShape = 0;
    ampShape = 0;
}

//--------------------------------------------------------------
void GrainReorder::setup(){
    ofSetColor(20);

    trail.resize(13);
    y.resize(13);

    for(int i = 0; i < 4; i ++){
        x[i] = 0;
        prevX[0] = 0;
    }
    
    y[0] = 60*2;
    radius = 3;
    
    showPos = false;
    speed = 100;
    preSpeed = speed;

    carrierOsc.setup(0.83f);
    carrierOsc.setAmplitude(1.0f);
    carrierOsc.setFrequency(0.3f);
    carrierOsc.setMinMax(0.0, 1.0);

    modulatorOsc.setup(0.83f);
    modulatorOsc.setAmplitude(1.0f);
    modulatorOsc.setFrequency(0.3f);
    modulatorOsc.setMinMax(0.0, 1.0);

    ampOsc.setup(0.83f);
    ampOsc.setAmplitude(1.0f);
    ampOsc.setFrequency(0.3f);
    ampOsc.setMinMax(0.0, 1.0);
}

//--------------------------------------------------------------
float GrainReorder::getCarrier(){
    switch (carrierShape) {
        case 0:
            carrierSignal = carrierOsc.getSine();
            break;
        case 1:
            carrierSignal = carrierOsc.getTriangle();
            break;
        case 2:
            carrierSignal = carrierOsc.getRamp();
            break;
        case 3:
            carrierSignal = carrierOsc.getSaw();
            break;
        case 4:
            carrierSignal = carrierOsc.getSquare();
            break;
        case 5:
            carrierSignal = carrierOsc.getRandom();
            break;
    }

    return carrierSignal;
}

float GrainReorder::getModulator(){
    switch (modulatorShape) {
        case 0:
            modulatorSignal = modulatorOsc.getSine();
            break;
        case 1:
            modulatorSignal = modulatorOsc.getTriangle();
            break;
        case 2:
            modulatorSignal = modulatorOsc.getRamp();
            break;
        case 3:
            modulatorSignal = modulatorOsc.getSaw();
            break;
        case 4:
            modulatorSignal = modulatorOsc.getSquare();
            break;
        case 5:
            modulatorSignal = modulatorOsc.getRandom();
            break;
    }
    return modulatorSignal;
}

float GrainReorder::getAmp(){
    switch (ampShape) {
        case 0:
            ampSignal = ampOsc.getSine();
            break;
        case 1:
            ampSignal = ampOsc.getTriangle();
            break;
        case 2:
            ampSignal = ampOsc.getRamp();
            break;
        case 3:
            ampSignal = ampOsc.getSaw();
            break;
        case 4:
            ampSignal = ampOsc.getSquare();
            break;
        case 5:
            ampSignal = ampOsc.getRandom();
            break;
    }
    return ampSignal;//ofMap(ampSignal, 0.0, 1.0, -1.0, 1.0);
}

float GrainReorder::getGrainPattern(){
//    return masterSignal = getCarrier()*(1+getModulator())*(1+getAmp());
//      return masterSignal = getCarrier()*(getModulator())*(getAmp());
//    cout << "carrier * modulkator = " << (getCarrier()+(getModulator()*0.1)) << "amp = " << (0.5+(getAmp()*0.5)) << endl;
   // return masterSignal = (getCarrier()+getModulator()) * getAmp();

//    return masterSignal = (getCarrier()+getModulator()) * (0.5+(getAmp()*0.5));
    return masterSignal = (getCarrier()+(getModulator()*0.2)) * (0.5+(getAmp()*0.5));
}


//--------------------------------------------------------------
void GrainReorder::setCarrierShape(int _shape){
    carrierShape = _shape;
}

void GrainReorder::setModulatorShape(int _shape){
    modulatorShape = _shape;
}

void GrainReorder::setAmpShape(int _shape){
    ampShape = _shape;
}


//--------------------------------------------------------------
void GrainReorder::update(DestinationState &state){
    carrierAmp = state.reorder.params[GR_CARRIER_AMP];
    carrierFreq = state.reorder.params[GR_CARRIER_FREQ];
    modulatorAmp = state.reorder.params[GR_MODULATOR_AMP];
    modulatorFreq = state.reorder.params[GR_MODULATOR_FREQ];
    ampAmp = state.reorder.params[GR_AMP_AMP];
    ampFreq = state.reorder.params[GR_AMP_FREQ];
    
    float t = ofGetElapsedTimef();
    float time_difference = t - lastUpdate;
    
    carrierPhase += time_difference * (carrierFreq*10.0);
    modulatorPhase += time_difference * (modulatorFreq*10.0);
    ampPhase += time_difference * (ampFreq*1.0);

    lastUpdate = t;
    
    carrierOsc.setAmplitude(carrierAmp);
    //carrierOsc.setFrequency(t * (carrierFreq*10.0));
    carrierOsc.setFrequency(carrierPhase);
    
    modulatorOsc.setAmplitude(modulatorAmp);
    //modulatorOsc.setFrequency(t * (modulatorFreq*10.0));
    modulatorOsc.setFrequency(modulatorPhase);

    ampOsc.setAmplitude(ampAmp);
    //ampOsc.setFrequency(t * (ampFreq*1.0));
    ampOsc.setFrequency(ampPhase);
    
	//if the speed has changed we need to clear the lines.
    // we can't test for float equality or non-equality
    // (speed.getValue() != preSpeed)
    // so we look at the change and if the change is greater then a certain
    // amount we clear.
  
	if( fabs(speed - preSpeed) > 0.00001f ){
		speedChanged();
	}
	preSpeed = speed;
    
    t = ofGetElapsedTimef() * 0.75;
    
	if(trail[9].size()==0){
		initTime = t;
	}
	t = t - initTime;
    
    for(int i = 0; i < 4; i ++){
        x[i] = 5+int(t*speed)%(299);
    }
}


//--------------------------------------------------------------
void GrainReorder::drawCarrier(int _x, int  _y, int _w, int _h){

    y[9] = _y+(getCarrier()*0.5) *_h;
    
    if((_x+x[0])<prevX[0]){
        trail[9].clear();
    }else{
        trail[9].addVertex(ofPoint(_x+x[0],y[9]));
    }

    ofEnableSmoothing();
    ofDrawCircle(_x+x[0],y[9],radius);
    trail[9].draw();
    
    ofDisableSmoothing();
    //ofLine(_x,_y+(_h/2)-10,_w,_y+(_h/2)-10);
    
    prevX[0] = _x+x[0];

}

//--------------------------------------------------------------
void GrainReorder::drawModulator(int _x, int  _y, int _w, int _h){
    
    y[10] = _y+(getModulator()*0.5)*_h;
    
    if((_x+x[1])<prevX[1]){
        trail[10].clear();
    }else{
        trail[10].addVertex(ofPoint(_x+x[1],y[10]));
    }
    
    ofEnableSmoothing();
    ofDrawCircle(_x+x[1],y[10],radius);
    trail[10].draw();
    
    ofDisableSmoothing();
  //  ofLine(_x,_y+(_h/2)-10,_w,_y+(_h/2)-10);
    
    prevX[1] = _x+x[1];

}

//--------------------------------------------------------------
void GrainReorder::drawAmp(int _x, int  _y, int _w, int _h){
    
    y[11] = _y+(getAmp()*0.5)*_h;
    
    if((_x+x[2])<prevX[2]){
        trail[11].clear();
    }else{
        trail[11].addVertex(ofPoint(_x+x[2],y[11]));
    }
    
    ofEnableSmoothing();
    ofDrawCircle(_x+x[2],y[11],radius);
    trail[11].draw();
    
    ofDisableSmoothing();
   // ofLine(_x,_y+(_h/2)-10,_w,_y+(_h/2)-10);
    
    prevX[2] = _x+x[2];
}


//--------------------------------------------------------------
void GrainReorder::drawGrainPattern(int _x, int  _y, int _w, int _h){
    
    y[12] = _y+getGrainPattern()*_h;

    if((_x+x[3])<prevX[3]){
        trail[12].clear();
    }else{
        trail[12].addVertex(ofPoint(_x+x[3],y[12]));
    }
    
    ofEnableSmoothing();
    ofDrawCircle(_x+x[3],y[12],radius);
    trail[12].draw();
    
    ofDisableSmoothing();
 //   ofSetColor(255,0,0,100);
 //   ofLine(_x,_y+(_h/2)-35,_w,_y+(_h/2)-35);
    
    prevX[3] = _x+x[3];

}

//--------------------------------------------------------------
void GrainReorder::drawOscillators(){
    ofSetColor(0,255,0);

    //Draw under oscialltors
    drawCarrier(8,226,308,20);
    drawModulator(8,381,308,20);
    drawAmp(325,226,308,20);
}

//--------------------------------------------------------------
void GrainReorder::draw(){
    ofSetColor(255,0,5);
    drawGrainPattern(322,360,230,30);
    
    ofSetColor(255,255,255);
}

//--------------------------------------------------------------
void GrainReorder::speedChanged(){
	for(unsigned int i=0;i<trail.size();i++){
		trail[i].clear();
	}
}
