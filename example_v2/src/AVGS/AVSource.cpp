//
//  AVSource.cpp
//  Kortex
//
//  Created by Joshua Batty on 11/03/2016.
//
//

#include "AVSource.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
AVSource::~AVSource(){
    sampFileA.clear();
    sampFileB.clear();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
AVSource::AVSource(){
    
    bLoadVideoFlag = false;
    bLoadAudioFlag = false;
    hasSwitchedSampFile = false;
    newVideoIndex = 113; //"/Users/josh/Desktop/Media/PhD_Performance/blank.mov";
    newAudioPath = "400Frames(60fps).wav";
    
    bUseShader = false;
    
    startAudioFrame = 0.0;
    
    audioSamples.reserve(BUFFER_SIZE*NUM_CHANNELS);
    for(int i = 0; i < BUFFER_SIZE*NUM_CHANNELS; i++){
        audioSamples.push_back(0.0);
    }
}

//--------------------------------------------------------------
void AVSource::setMediaPaths(vector<string> videoPaths, vector<string> audioPaths){
#ifdef USE_LIVE_VIDEO
#else
    this->videoPaths = videoPaths;
    this->audioPaths = audioPaths;

    for(int i = 0; i < videoPaths.size(); i++){
        ofPtr<ofxHapPlayer> v (new ofxHapPlayer());
        v->load(videoPaths[i]);
        v->setVolume(0.0);
        idlePlayer.push_back(v);
    }
    currentIdlePlaying = 113; // 113 = "/Users/josh/Desktop/Media/PhD_Performance/blank.mov"
    idlePlayer[currentIdlePlaying]->play();
#endif
}

//--------------------------------------------------------------
void AVSource::addNewMediaPath(string videoPath, string audioPath){
    videoPaths.push_back(videoPath);
    audioPaths.push_back(audioPath);
    ofPtr<ofxHapPlayer> v (new ofxHapPlayer());
    v->load(videoPaths.back());
    v->setVolume(0.0);
    idlePlayer.push_back(v);
}

//--------------------------------------------------------------
void AVSource::setup(){
    
#ifdef USE_LIVE_VIDEO
    for(int i = 0; i < NUM_CAMERAS; i++){
        ofVideoGrabber cam;
        vGrabber.push_back(cam);
        vGrabber[i].setDeviceID(1+i);
        vGrabber[i].setDesiredFrameRate(60);
        vGrabber[i].initGrabber(640,480);
    }

    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vGrabber[0].listDevices();

    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }

    currentCameraID = 0;
#else
#endif

    sampFileA.load(ofToDataPath("400Frames(60fps).wav"));
    sampFileB.load(ofToDataPath("400Frames(60fps).wav"));

    currentSampFile = &sampFileA;
    whichSampFile = SAMP_FILE_A;
}

//--------------------------------------------------------------
void AVSource::loadAudio(int _audioIndex){
    bLoadAudioFlag = true;
    newAudioPath = audioPaths[_audioIndex];
}

//--------------------------------------------------------------
void AVSource::loadVideo(int _videoIndex){
#ifdef USE_LIVE_VIDEO
#else
    bLoadVideoFlag = true;
    newVideoIndex = _videoIndex;
#endif
}

//--------------------------------------------------------------
void AVSource::setCameraIndex(int camIdx){
    currentCameraID = camIdx;
}

//--------------------------------------------------------------
void AVSource::setShaderTexture(ofFbo fbo){
    // JOSH!! WHY ARE YOU ALLOCATING THE SHADER FBO EACH TIME THIS FUNCTION IS CALLED?!?!
    shaderFbo.allocate(640,480,GL_RGBA);
    shaderFbo.begin();
    fbo.draw(0,0,640,480);
    shaderFbo.end();
}

//--------------------------------------------------------------
ofTexture& AVSource::getTexture(){
#ifdef USE_LIVE_VIDEO
    return vGrabber[currentCameraID].getTexture();
#else
    return *idlePlayer[currentIdlePlaying]->getTexture();
#endif
    if(bUseShader == true){
        return shaderFbo.getTexture();
    }
}

//--------------------------------------------------------------
void AVSource::triggerRandomPosition(){
    startAudioFrame = ofRandomuf() * (double)currentSampFile->length;
}

//--------------------------------------------------------------
void AVSource::update(){
    
     if(ofGetKeyPressed()){
       //  triggerRandomPosition();
     }
     
     // This makes sure that the audio sample has been loaded before
     // we set a new random position for the audio and video
     if(hasSwitchedSampFile == true){
         startAudioFrame = ofRandomuf() * (double)sampFileA.length;
         hasSwitchedSampFile = false;
     }
     
     // Synchronises Video to the current Audio play head!!! :D
     idlePlayer[currentIdlePlaying]->setPosition(startAudioFrame / currentSampFile->length);
     
     
     if(bLoadVideoFlag == true && hasLoadedNewSampFile == false){
     #ifdef USE_LIVE_VIDEO
     #else
         idlePlayer[currentIdlePlaying]->stop();
         currentIdlePlaying = newVideoIndex;
         idlePlayer[currentIdlePlaying]->play();
     #endif
     
         if (whichSampFile == SAMP_FILE_A) {
             sampFileB.load(newAudioPath);
             hasLoadedNewSampFile = true;
         } else {
             sampFileA.load(newAudioPath);
             hasLoadedNewSampFile = true;
         }
     }
     bLoadVideoFlag = false;
     bLoadAudioFlag = false;
     
     #ifdef USE_LIVE_VIDEO
     for(int i = 0; i < NUM_CAMERAS; i++){
         vGrabber[i].update();
     }
     #else
     if(idlePlayer[currentIdlePlaying]->isLoaded()){
         idlePlayer[currentIdlePlaying]->update();
     }
     #endif
}

//--------------------------------------------------------------
void AVSource::switchSampFile()
{
    if (whichSampFile == SAMP_FILE_A) {
        currentSampFile = &sampFileB;
        whichSampFile = SAMP_FILE_B;
    } else {
        currentSampFile = &sampFileA;
        whichSampFile = SAMP_FILE_A;
    }
    hasSwitchedSampFile = true;
}

//--------------------------------------------------------------
vector<double> *AVSource::getNextAudioBuffer(){
    return &audioSamples;
}

//--------------------------------------------------------------
void AVSource::audioReceived(float * input, int bufferSize, int nChannels)
{

    for (int i = 0; i < bufferSize; i++)
    {
        for(int j = 0; j < NUM_CHANNELS; j++){
#ifdef USE_LIVE_VIDEO
            int idx = i*nChannels+j;
            audioSamples[idx] = input[idx];
#endif
        }
    }
    
}

//--------------------------------------------------------------
void AVSource::audioRequested (float * output, int numFrames, int nChannels)
{
    float volume = 0.9;

    if (hasLoadedNewSampFile) {
        switchSampFile();
        hasLoadedNewSampFile = false;
    }

    float oneBillion = 1000000000.;
    // Seems to always be some value between 0.0 and 1.0.
    double rate = ((oneBillion*nChannels)/(maxiSettings::sampleRate))*(1.0/currentSampFile->length);
    // Returns the next sample for currentSampFile playing back at the given rate having started at startAudioFrame.
    // Note that `startAudioFrame` will be magically stepped forward as it is taken by reference. Fancy that.
    double start = 0;
    double end = (double)currentSampFile->length;
    
    for (int i = 0; i < numFrames; i++)
    {
        double wave = currentSampFile->play(rate, start, end, startAudioFrame);
        
        for(int j = 0; j < NUM_CHANNELS; j++){
#ifdef USE_LIVE_VIDEO
#else
            int idx = i*nChannels+j;
            audioSamples[idx] = wave;
#endif
        }
    }
}
