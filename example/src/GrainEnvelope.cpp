//
//  GrainEnvelope.cpp
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 13/08/14.
//
//

#include "GrainEnvelope.h"

int GrainEnvelope::getGrainFrame(int _grainNumber, int _maxGrains, int _grainLength){
    int grainPosition = ofGetFrameNum() % _grainLength + ofMap(_grainNumber,0,_maxGrains,1,_grainLength);
    grainPosition = ofWrap(grainPosition, 1, _grainLength);
    return grainPosition;
}

void GrainEnvelope::setEnvelope(string name, int _windowLength, int _windowPos){
    envName = name;
    windowLength = _windowLength;
    windowPos = _windowPos;
}

float GrainEnvelope::getEnvelope(){
    float val;
    if(envName == "Gaussian"){
        val = gaussianWinFunctor(windowLength, windowPos);
    }
    else if(envName == "Cosine"){
        val = cosineWinFunctor(windowLength, windowPos);
    }
    else if(envName == "Triangle"){
        val = triangleWinFunctor(windowLength, windowPos);
    }
    else if(envName == "Rect"){
        val = rectWinFunctor(windowLength, windowPos);
    }
    else if(envName == "Harris"){
        val = hannWinFunctor(windowLength, windowPos);
    }
    return val;
}

float GrainEnvelope::hannWinFunctor(int _windowLength, int _windowPos) {
    return 0.5 * (1.0 - cos((2.0 * PI * _windowPos) / (_windowLength - 1)));
}

//this window can produce clicks
float GrainEnvelope::hammingWinFunctor(int _windowLength, int _windowPos) {
    return 0.54 - (0.46 * cos((2.0 * PI * _windowPos) / (_windowLength - 1)));
}

float GrainEnvelope::cosineWinFunctor(int _windowLength, int _windowPos) {
    return sin((PI * _windowPos) / (_windowLength - 1));
}

float GrainEnvelope::rectWinFunctor(int _windowLength, int _windowPos) {
    return 1;
}

float GrainEnvelope::triangleWinFunctor(int _windowLength, int _windowPos) {
    return (2.0 / (_windowLength-1.0)) * (((_windowLength-1.0)/2.0) - fabs(_windowPos - ((_windowLength-1.0)/2.0)));
}

float GrainEnvelope::triangleNZWinFunctor(int _windowLength, int _windowPos) {
	//non zero end points
    return (2.0 / _windowLength) * ((_windowLength/2.0) - fabs(_windowPos - ((_windowLength-1.0)/2.0)));
}

float GrainEnvelope::blackmanHarrisWinFunctor(int _windowLength, int _windowPos) {
    return 0.35875 -
    (0.48829 * cos((2 * PI * _windowPos) / (_windowLength-1))) +
    (0.14128 * cos((4 * PI * _windowPos) / (_windowLength-1))) +
    (0.01168 * cos((6 * PI * _windowPos) / (_windowLength-1)));
}

float GrainEnvelope::blackmanNutallWinFunctor(int _windowLength, int _windowPos) {
    return 0.3635819 -
    (0.4891775 * cos((2 * PI * _windowPos) / (_windowLength-1))) +
    (0.1365995 * cos((4 * PI * _windowPos) / (_windowLength-1))) +
    (0.0106411 * cos((6 * PI * _windowPos) / (_windowLength-1)));
}

float GrainEnvelope::gaussianWinFunctor(int _windowLength, int _windowPos) {
    double gausDivisor;

    gausDivisor = (-2.0 * 0.5 * 0.5);
    double phase = ((windowPos / (double) windowLength) - 0.5) * 2.0;
    return exp((phase * phase) / gausDivisor);
};

