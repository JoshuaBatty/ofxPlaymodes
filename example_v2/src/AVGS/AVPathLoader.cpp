//
//  AVPathLoader.cpp
//  testCpp11
//
//  Created by Joshua Batty on 18/06/14.
//
//

#include "AVPathLoader.h"



//--------------------------------------------------------------
void AVPathLoader::setup(AVSource *_avSource, string _lastSavedProjectName, bool _bNewProject){
    
    avSource = _avSource;
    
    projectName = _lastSavedProjectName;
    bGenerateThumbs = false;
    init = true;
    filesSize = 0;
    fileCounter = 0;

    videoThumbs.reserve(32);
    shaderThumbs.reserve(32);

    if(_bNewProject == true){
        XMLVideoPaths.load("Projects/" + projectName + "/Media/videoPaths.xml");
        XMLAudioPaths.load("Projects/" + projectName + "/Media/audioPaths.xml");
        XMLShaderPaths.load("Projects/" + projectName + "/Media/shaderPaths.xml");
        loadProject(projectName);
    }
    

}

//--------------------------------------------------------------
void AVPathLoader::savePaths(string _projectName){
    
    while (XMLVideoPaths.getNumTags("VIDEO_PATH")) {
        XMLVideoPaths.removeTag("VIDEO_PATH", 0);
    }
    while (XMLAudioPaths.getNumTags("AUDIO_PATH")) {
        XMLAudioPaths.removeTag("AUDIO_PATH", 0);
    }
    while (XMLShaderPaths.getNumTags("SHADER_PATH")) {
        XMLShaderPaths.removeTag("SHADER_PATH", 0);
    }
    
    for (int i=0; i < videoPaths.size(); i++) {
        XMLVideoPaths.addValue("VIDEO_PATH", videoPaths.at(i));
    //    cout << "VIDEO Paths @ " + ofToString(i) + "= " << videoPaths.at(i) << endl;
    }
    for (int i=0; i < audioPaths.size(); i++) {
        XMLAudioPaths.addValue("AUDIO_PATH", audioPaths.at(i));
     //   cout << "AUDIO Paths @ " + ofToString(i) + "= " << audioPaths.at(i) << endl;
    }
    for (int i=0; i < shaderPaths.size(); i++) {
        XMLShaderPaths.addValue("SHADER_PATH", shaderPaths.at(i));
     //   cout << "SHADER Paths @ " + ofToString(i) + "= " << shaderPaths.at(i) << endl;
    }
    
    XMLVideoPaths.saveFile("Projects/" + _projectName + "/Media/videoPaths.xml");
    XMLAudioPaths.saveFile("Projects/" + _projectName + "/Media/audioPaths.xml");
    XMLShaderPaths.saveFile("Projects/" + _projectName + "/Media/shaderPaths.xml");
}

//--------------------------------------------------------------
void AVPathLoader::loadProject(string _projectName){
    projectName = _projectName;

    videoPaths.clear();
    audioPaths.clear();
    shaderPaths.clear();
    
    videoThumbs.clear();
    shaderThumbs.clear();

    if(init != true){
        XMLVideoPaths.load("Projects/" + projectName + "/Media/videoPaths.xml"); //initXMLpreset
        XMLAudioPaths.load("Projects/" + projectName + "/Media/audioPaths.xml"); //initXMLpreset
        XMLShaderPaths.load("Projects/" + projectName + "/Media/shaderPaths.xml"); //initXMLpreset
    }
    
    //SHADER
    for(int i = 0; i < XMLShaderPaths.getNumTags("SHADER_PATH"); i++){
        shaderPaths.push_back(XMLShaderPaths.getValue("SHADER_PATH", "DEFAULT", i));
        //cout << "SHADER PATHS = " << shaderPaths.at(i) << endl;
        
        ofImage image;
        image.load("Thumbnails/" + shaderPaths.at(i) + ".png");
        shaderThumbs.push_back(image);
    }
    //VIDEO
    for(int i = 0; i < XMLVideoPaths.getNumTags("VIDEO_PATH"); i++){
        videoPaths.push_back(XMLVideoPaths.getValue("VIDEO_PATH", "DEFAULT", i));
        //cout << "VIDEO PATHS = " << videoPaths.at(i) << endl;
        
        vector<string> fileName = ofSplitString(videoPaths.back(), "/"); //Find on the last name in the path
        fileName = ofSplitString(fileName.at(fileName.size()-1), "."); //Remove the .mov extension from
        ofImage image;
        image.load("Thumbnails/" + fileName.at(fileName.size()-2) + ".png");
        videoThumbs.push_back(image);
        filesSize++;
    }
    //AUDIO
    for(int i = 0; i < XMLAudioPaths.getNumTags("AUDIO_PATH"); i++){
        audioPaths.push_back(XMLAudioPaths.getValue("AUDIO_PATH", "DEFAULT", i));
        //cout << "AUDIO PATHS = " << audioPaths.at(i) << endl;
    }
    
    bGenerateThumbs = true;
    init = false;
}

//--------------------------------------------------------------
void AVPathLoader::loadNewMedia(int _fileID){
    avSource->loadVideo(_fileID);
    avSource->loadAudio(_fileID);
}

//--------------------------------------------------------------
void AVPathLoader::getVideoPathList(){
    for(int i = 0; i < videoPaths.size(); i++){
        vector<string> result = ofSplitString(videoPaths.at(i), "/");
        cout << "files = " << result.at(result.size()-1) << endl;
    }
}

//--------------------------------------------------------------
void AVPathLoader::getAudioPathList(){
    for(int i = 0; i < audioPaths.size(); i++){
        vector<string> result = ofSplitString(audioPaths.at(i), "/");
        cout << "files = " << result.at(result.size()-1) << endl;
    }
}

//--------------------------------------------------------------
void AVPathLoader::dragEvent(ofDragInfo info){
    
    if(info.files.size() > 0){
        filesSize = info.files.size();
        
        for(int i = 0; i < info.files.size(); i++){
            cout << "file = " << info.files.at(i) << endl;
            videoPaths.push_back(info.files.at(i));
            vector<string> fileName = ofSplitString(videoPaths.back(), "/"); //Find on the last name in the path
            fileName = ofSplitString(fileName.at(fileName.size()-1), "."); //Remove the .mov extension from the fileName

            //Create the audio paths by replacing .mov from path with .wav
            vector<string> audioFileName = ofSplitString(videoPaths.back(), ".");
            audioPaths.push_back(audioFileName[0]+".wav");
            
            avSource->addNewMediaPath(videoPaths.back(), audioPaths.back());

            ofVideoPlayer vid;
            vid.load(videoPaths.back());
            vid.play();
            
            if (vid.isLoaded()) {
                cout << "LOADED!" << endl;
            }
            else {
                cout << "FUCK!" << endl;
            }
            //Basically until the video is loaded then all these calls aren't going to work
            if (vid.isLoaded()) {
                
                vid.setPosition(0.5);
                vid.update();
                
                ofImage image;
                
                image.setFromPixels(vid.getPixelsRef());
                image.crop(0,0,vid.getWidth(),vid.getHeight());
                image.resize(100, 100);
                
                vid.stop();
                vid.close();
                
                image.save("Thumbnails/" + fileName.at(fileName.size()-2) + ".png");
                
                image.load("Thumbnails/" + fileName.at(fileName.size()-2) + ".png");
                videoThumbs.push_back(image);
            }
            
        }
    }
    
    bGenerateThumbs = true;
}
