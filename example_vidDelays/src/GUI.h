//
//  GUI.h
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 15/08/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "AVGS.h"
#include "ofxXmlSettings.h"

class GUI{
    
public:
    void setup(AVGS *_avgs);
    void draw();
    
    void saveCurrentPreset();
    void savePreset(int _presetNumber);
    void loadPreset(int _presetNumber);
    void loadBlendMode();
    void loadWindows();
    
    //AVGS
    AVGS *avgs;
    
    //GUI
    ofxUICanvas *guiMaster;
    ofxUICanvas *guiAVGS;
    ofxUICanvas *guiWindows;
    ofxUICanvas *guiBlends;
    ofxUICanvas *guiPresets;
    ofxUIDropDownList *ddl;
    ofxUIToggleMatrix* mtx;
    ofxUIRadio *radio;
    void guiEvent(ofxUIEventArgs &e);
    vector<string> envelopes;
    vector<ofxUIToggle*> toggles;

    ofxXmlSettings xmlAVGS;
    ofxXmlSettings xmlWindows;
    ofxXmlSettings xmlBlends;

    int numAVGSWidgets;
    int numWindowWidgets;
    int numBlendWidgets;
    
    //Interface
    ofImage img;
    
private:
    int backGroundColor;
    int currentPreset;
};
