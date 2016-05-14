#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    bool bLoadLastProject = false;
    avgsManager.setup("kortex", bLoadLastProject);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    avgs.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    
    ofSetColor(255,255);
    ofFill();
    avgs.draw();

}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    avgs.audioReceived(input,bufferSize,nChannels);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    avgs.audioRequested(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            //SAVE PRESET
        case ' ':
            gui.saveCurrentPreset();
            break;
        case '1':
            gui.savePreset(1);
            break;
        case '2':
            gui.savePreset(2);
            break;
        case '3':
            gui.savePreset(3);
            break;
        case '4':
            gui.savePreset(4);
            break;
        case '5':
            gui.savePreset(5);
            break;
        case '6':
            gui.savePreset(6);
            break;
        case '7':
            gui.savePreset(7);
            break;
        case '8':
            gui.savePreset(8);
            break;
        case '9':
            gui.savePreset(9);
            break;
            
            //LOAD PRESET
        case 'q':
            gui.loadPreset(1);
            break;
        case 'w':
            gui.loadPreset(2);
            break;
        case 'e':
            gui.loadPreset(3);
            break;
        case 'r':
            gui.loadPreset(4);
            break;
        case 't':
            gui.loadPreset(5);
            break;
        case 'y':
            gui.loadPreset(6);
            break;
        case 'u':
            gui.loadPreset(7);
            break;
        case 'i':
            gui.loadPreset(8);
            break;
        case 'o':
            gui.loadPreset(9);
            break;
            
            default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
