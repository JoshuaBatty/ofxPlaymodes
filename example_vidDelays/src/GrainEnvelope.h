//
//  GrainEnvelope.h
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 13/08/14.
//
//

//window functions from http://en.wikipedia.org/wiki/Window_function#High-_and_moderate-resolution_windows
//Copied from Maximilian

#pragma once

#include "ofMain.h"

class GrainEnvelope{
    
public:
    
    void setEnvelope(string name, int _windowLength, int _windowPos);
    int getGrainFrame(int _grainNumber, int _maxGrains, int _grainLength);
    float getEnvelope();
    
    float hannWinFunctor(int _windowLength, int _windowPos);
    float hammingWinFunctor(int _windowLength, int _windowPos);
    float cosineWinFunctor(int _windowLength, int _windowPos);
    float rectWinFunctor(int _windowLength, int _windowPos);
    float triangleWinFunctor(int _windowLength, int _windowPos);
    float triangleNZWinFunctor(int _windowLength, int _windowPos);
    float blackmanHarrisWinFunctor(int _windowLength, int _windowPos);
    float blackmanNutallWinFunctor(int _windowLength, int _windowPos);
    float gaussianWinFunctor(int _windowLength, int _windowPos);

private:
    string envName;
    int windowLength;
    int windowPos;
};
