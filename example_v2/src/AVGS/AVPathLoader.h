//
//  AVPathLoader.h
//  testCpp11
//
//  Created by Joshua Batty on 18/06/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "AVSource.h"

class AVPathLoader{
    
public:
    void setup(AVSource *_avSource, string _lastSavedProjectName, bool _bNewProject);
    void loadProject(string _projectName);
    void savePaths(string _projectName);
    void loadNewMedia(int _fileID);
    void dragEvent(ofDragInfo info);

    AVSource *avSource;
    
    //Save Load Paths Presets
    ofxXmlSettings XMLVideoPaths;
    ofxXmlSettings XMLAudioPaths;
    ofxXmlSettings XMLShaderPaths;
    
    //Video Thumbs
    void getVideoPathList();
    void getAudioPathList();
    vector<string> videoPaths;
    vector<string> audioPaths;
    vector<string> shaderPaths;
    
    vector<ofImage> videoThumbs;
    vector<ofImage> shaderThumbs;

    bool bGenerateThumbs;
    
    
private:
    bool init;
    int filesSize;
    int fileCounter;
    string projectName;

};
