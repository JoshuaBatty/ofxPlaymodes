//
//  AVSource.h
//  Kortex
//
//  Created by Joshua Batty on 11/03/2016.
//
//

#pragma once

#include "ofSoundUnit.h"
#include "ofMain.h"
#include "Constants.h"
#include "ofxHapPlayer.h"
#include "ofxMaxim.h"

static bool SAMP_FILE_A = true;
static bool SAMP_FILE_B = false;

class AVSource : public ofSoundSource{
    
public:
    ~AVSource();
    AVSource();
    
    void setMediaPaths(vector<string> videoPaths, vector<string> audioPaths);
    void addNewMediaPath(string videoPath, string audioPath);
    
    void setup();
    void update();
    
    ofTexture& getTexture();
    void setShaderTexture(ofFbo fbo);
    void setCameraIndex(int camIdx);
    void triggerRandomPosition();
    
    void loadAudio(int _audioIndex);
    void loadVideo(int _videoIndex);
    
    void switchSampFile();
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested (float * output, int numFrames, int nChannels);
    vector<double> *getNextAudioBuffer();
    string getName() { return "AVSource"; }
    
    ofFbo shaderFbo;
    
    int currentCameraID;
    int currentIdlePlaying;
    
    #ifdef USE_LIVE_VIDEO
     vector<ofVideoGrabber> vGrabber;
    #else
     vector<ofPtr<ofxHapPlayer> > idlePlayer;
     vector<string> videoPaths;
    #endif
    
    // WE'll use this to switch when loading.
    maxiSample sampFileA;
    maxiSample sampFileB;
    
    // Pointer to either sampFileA of sampFileB! Do not delete.
    maxiSample* currentSampFile;
    
    bool whichSampFile;
    bool hasSwitchedSampFile;
    
    // Indicate to the audio thread that we've loaded a new sampFile.
    bool hasLoadedNewSampFile;
    
    vector<string> audioPaths;
    bool bUseShader; // TAKE OUT RESONAtE
    
private:
    int	sampleRate;
    
    bool bLoadVideoFlag; // Used to make sure video is only on the main thread.
    bool bLoadAudioFlag; // Used to make sure video is only on the main thread.
    int newVideoIndex;
    string newAudioPath;
    
    double startAudioFrame;
    vector<double> audioSamples;
    
};