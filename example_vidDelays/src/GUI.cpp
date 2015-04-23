//
//  GUI.cpp
//  example_PlayModes_Maxim
//
//  Created by Joshua Batty on 15/08/14.
//
//

#include "GUI.h"

//--------------------------------------------------------------
void GUI::setup(AVGS *_avgs){
    avgs = _avgs;
    currentPreset = 1;
    
    backGroundColor = 0;
    
    img.loadImage("Images/Background3.jpg");
    
    //INIT XMLS
    xmlAVGS.load("GUI/PRESETS/AVGS/avgs1.xml");
    xmlWindows.load("GUI/PRESETS/WINDOW/window1.xml");
    xmlBlends.load("GUI/PRESETS/BLEND/blend1.xml");
    
    numAVGSWidgets = xmlAVGS.getNumTags("Widget");
    numWindowWidgets = xmlWindows.getNumTags("Widget");
    numBlendWidgets = xmlBlends.getNumTags("Widget");
    
    //GUI
    guiMaster = new ofxUICanvas(0,0,240,87);
    guiMaster->setFont("GUI/Fett.ttf");
    guiMaster->addLabel("AVGS");//->setColorFill(ofxUIColor::green);
    guiMaster->addSpacer();
    
    guiMaster->addLabelToggle("Record Input", true)->setColorFill(ofxUIColor::darkRed);
    guiMaster->addSlider("Volume", 0.0, 1.0, &avgs->grainPlayer.volume);
    guiMaster->autoSizeToFitWidgets();

    //AVGS
    guiAVGS = new ofxUICanvas(0,88,240,218);
    guiAVGS->setFont("GUI/Fett.ttf");
    guiAVGS->addSpacer();
    guiAVGS->addLabelToggle("Set Position", &avgs->grainPlayer.bSetPosition);
    ofxUIWidgetWithLabel *n = (ofxUIWidgetWithLabel *) guiAVGS->addWidgetDown(new ofxUIRotarySlider(15,0.0,0.3,0.0,"Position Random"));n->setLabelVisible(false);
    guiAVGS->addWidgetRight(new ofxUISlider("Position", 0.0, 1.0, 1.0, 210, 15));

    guiAVGS->addSpacer();
    guiAVGS->addWidgetDown(new ofxUISpacer(15,0));
    guiAVGS->addWidgetRight(new ofxUISlider("Speed", -4.0, 4.0, &avgs->grainPlayer.speed, 210, 15));

    ofxUIWidgetWithLabel *p = (ofxUIWidgetWithLabel *) guiAVGS->addWidgetDown(new ofxUIRotarySlider(15,0.0,1.0,0.0,"Pitch Random"));p->setLabelVisible(false);
    guiAVGS->addWidgetRight(new ofxUISlider("Pitch",0.0,4.0, avgs->grainPlayer.pitch, 210,15));

    ofxUIWidgetWithLabel *g = (ofxUIWidgetWithLabel *) guiAVGS->addWidgetDown(new ofxUIRotarySlider(15,0.0,0.45,0.0,"GrainSize Random"));g->setLabelVisible(false);
    guiAVGS->addWidgetRight(new ofxUISlider("GrainSize", 0.025, 0.45, avgs->grainPlayer.grainSize, 210, 15));

    guiAVGS->addWidgetDown(new ofxUISpacer(15,0));
    guiAVGS->addWidgetRight(new ofxUIIntSlider("Overlaps", 1, 5, avgs->grainPlayer.overlaps, 210, 15));
    guiAVGS->addSpacer();
    guiAVGS->autoSizeToFitWidgets();
    
    // Grain Windows
    guiWindows = new ofxUICanvas(0,307,240,72);
    guiWindows->setFont("GUI/Fett.ttf");
    guiWindows->addLabel("Grain Window");
    int dim = 40;
    envelopes.push_back("Gaussian"); envelopes.push_back("Cosine"); envelopes.push_back("Triangle"); envelopes.push_back("Rect"); envelopes.push_back("Harris");
    
    for(int i = 0; i < envelopes.size(); i++){
        if(i == 0){
            ofxUIWidget* w = guiWindows->addWidgetDown(new ofxUIImageToggle(dim, dim, false , "Grains/" + envelopes[i] + ".jpg", envelopes[i]));
            ofxUIToggle* toggle = (ofxUIToggle*) w;
            toggles.push_back(toggle);
            toggles.at(0)->setValue(true);
            
        } else {
            ofxUIWidget* w = guiWindows->addWidgetRight(new ofxUIImageToggle(dim, dim, false , "Grains/" + envelopes[i] + ".jpg", envelopes[i]));
            ofxUIToggle* toggle = (ofxUIToggle*) w;
            toggles.push_back(toggle);
        }
    }
    guiWindows->addSpacer();
    guiWindows->autoSizeToFitWidgets();
    
    //Blend Modes
    guiBlends = new ofxUICanvas(0,380,232,129);
    guiBlends->setFont("GUI/Fett.ttf");
    vector<string> blends; blends.push_back("ALPHA"); blends.push_back("ADD"); blends.push_back("SCREEN"); blends.push_back("MULTIPLY"); blends.push_back("SUBTRACT");
    ddl = new ofxUIDropDownList(235,"BlendModes", blends,OFX_UI_FONT_SMALL);
    ddl->open();
    
    ofxUIWidget *w;
    w = guiBlends->addWidgetDown(ddl);
    guiBlends->autoSizeToFitWidgets();
    
    //PRESETS
    guiPresets = new ofxUICanvas(0,520,240,100);
    guiPresets->setFont("GUI/Fett.ttf");
    guiPresets->addSpacer();
    guiPresets->addLabel("PRESETS");
    ofxUIWidget* wp =  guiPresets->addWidgetDown(new ofxUIToggleMatrix(44,44,5,5,"PRESETS", 50));
    mtx = (ofxUIToggleMatrix *) wp;
    mtx->setAllowMultiple(false);
    
    mtx->setToggle(0, 0, true);
    
    //guiPresets->loadSettings("GUI/guiSettings.xml");
    guiPresets->autoSizeToFitWidgets();
   
    ofAddListener(guiMaster->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(guiAVGS->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(guiWindows->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(guiBlends->newGUIEvent,this,&GUI::guiEvent);
    ofAddListener(guiPresets->newGUIEvent,this,&GUI::guiEvent);
    

}

//--------------------------------------------------------------
void GUI::draw(){
    ofSetColor(255);
    img.draw(0,0,ofGetWidth(),ofGetHeight());
    
    ofRect(245,5,ofGetWidth()-245,ofGetHeight()-270);
    
    ofSetColor(backGroundColor);
    ofRect(250,10,ofGetWidth()-255,ofGetHeight()-280);
    
    for(int y = 0; y < 5; y++){
        for(int x = 0; x < 5; x++){
            ofxUIRectangle* rect = (ofxUIRectangle *) mtx->getToggle(y, x)->getRect();
            int r =ofMap(sin((ofGetElapsedTimef()*0.012)*x),-1.0,1,0,255);//x*(1+sin(ofGetElapsedTimef()*0.5)*200);
            int g =ofMap(sin((ofGetElapsedTimef()*0.013)*x),-1.0,1,0,255);//x*(1+sin(ofGetElapsedTimef()*0.5)*200);
            int b =ofMap(sin(y*(ofGetElapsedTimef()*0.0135)),-1.0,1,0,255);//x*(1+sin(ofGetElapsedTimef()*0.5)*200);
            ofSetColor(r,g,b);
            ofNoFill();
            ofRectangle box = ofRectangle(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            ofRect(box);
            
        }
    }
}

//--------------------------------------------------------------
void GUI::saveCurrentPreset(){
    guiAVGS->saveSettings("GUI/PRESETS/AVGS/avgs" + ofToString(currentPreset) + ".xml");
    guiBlends->saveSettings("GUI/PRESETS/BLEND/blend" + ofToString(currentPreset) + ".xml");
    guiWindows->saveSettings("GUI/PRESETS/WINDOW/window" + ofToString(currentPreset) + ".xml");
    
}

//--------------------------------------------------------------
void GUI::savePreset(int _presetNumber){
    guiAVGS->saveSettings("GUI/PRESETS/AVGS/avgs" + ofToString(_presetNumber) + ".xml");
    guiBlends->saveSettings("GUI/PRESETS/BLEND/blend" + ofToString(_presetNumber) + ".xml");
    guiWindows->saveSettings("GUI/PRESETS/WINDOW/window" + ofToString(_presetNumber) + ".xml");
    
}

//--------------------------------------------------------------
void GUI::loadPreset(int _presetNumber){
    guiAVGS->loadSettings("GUI/PRESETS/AVGS/avgs" + ofToString(_presetNumber) + ".xml");
    guiBlends->loadSettings("GUI/PRESETS/BLEND/blend" + ofToString(_presetNumber) + ".xml");
    guiWindows->loadSettings("GUI/PRESETS/WINDOW/window" + ofToString(_presetNumber) + ".xml");
    
    xmlWindows.load("GUI/PRESETS/WINDOW/window" + ofToString(_presetNumber) + ".xml");
    xmlBlends.load("GUI/PRESETS/BLEND/blend" + ofToString(_presetNumber) + ".xml");

    loadBlendMode();
    loadWindows();
}

//--------------------------------------------------------------
void GUI::loadBlendMode(){
    string blendModeName;
    for(int i = 0; i < numBlendWidgets; i++){
        xmlBlends.pushTag("Widget", i);
        string name = xmlBlends.getValue("Name", "blend name");
        if(name != "BlendModes"){
        if(xmlBlends.getValue("Value", i) == 1){
            blendModeName = xmlBlends.getValue("Name", "blend name");
            cout << ofToString(i) + " - NAMMMEEE " << xmlBlends.getValue("Name", "blend name") << endl;
        }
        }
        xmlBlends.popTag();
    }
    ((ofxUIDropDownList *) guiBlends->getWidget("BlendModes"))->activateToggle(blendModeName);
    ddl->triggerSelf();
    
    cout << "blend Mode name " << blendModeName << endl;
}

//--------------------------------------------------------------
void GUI::loadWindows(){
    string windowName;
    for(int i = 0; i < numWindowWidgets; i++){
        xmlWindows.pushTag("Widget", i);
        //name =  xmlWindows.getValue("Name", "window name");
        if(xmlWindows.getValue("Value", i) == 1){
            windowName = xmlWindows.getValue("Name", "blend name");
            cout << ofToString(i) + " - WINDOW NAMMMEEE " << xmlWindows.getValue("Name", "window name") << endl;
        }
        
        xmlWindows.popTag();
    }
    
    for(int i = 0; i < envelopes.size(); i++){
        if(windowName == envelopes[i]){
            toggles[i]->setValue(true);
            avgs->playModes.setWindowShape(windowName);
            avgs->grainPlayer.setGrainEnvelope(i+1);
        } else {
            toggles[i]->setValue(false);
        }
    }
    
    cout << "window Mode name " << windowName << endl;
}



//--------------------------------------------------------------
void GUI::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
	int kind = e.widget->getKind();
	
    if(name == "Speed")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "Pitch")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        avgs->grainPlayer.pitch = slider->getScaledValue();
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "Pitch Random")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        avgs->randomPitch = slider->getScaledValue();
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "GrainSize")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        avgs->grainPlayer.grainSize = slider->getScaledValue();
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "GrainSize Random")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        avgs->randomGrainSize = slider->getScaledValue();
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "Overlaps")
	{
		ofxUISlider_<int> *slider = (ofxUISlider_<int> *) e.widget;
        slider->setColorFillHighlight(ofxUIColor :: red);
        avgs->grainPlayer.overlaps = slider->getScaledValue();
        cout << "val = " << slider->getValue() << endl;
	}
    else if(name == "Volume")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "Position")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        slider->setColorFillHighlight(ofxUIColor :: red);
		avgs->grainPlayer.playHead = slider->getScaledValue();
       // avgs->playModes.setDelay(slider->getScaledValue());
	}
    else if(name == "Set Position")
	{
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: yellow);
        toggle->setColorFill(ofxUIColor :: darkCyan);
	}
    else if(name == "Position Random")
	{
	  	ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        avgs->randomPosition = slider->getScaledValue();
        slider->setColorFillHighlight(ofxUIColor :: red);
	}
    else if(name == "Record Input"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: yellow);
        toggle->setColorFill(ofxUIColor :: darkRed);
        avgs->grainPlayer.bRecLiveInput = toggle->getValue();
        if(toggle->getValue()==true){
            avgs->playModes.bRecord = true;
        } else {
            avgs->playModes.bRecord = false;
        }
    }
    else if(name == "ALPHA"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: pink);
        avgs->playModes.setBlendMode(OF_BLENDMODE_ALPHA);
        backGroundColor = 0;
    }
    else if(name == "ADD"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: pink);
        avgs->playModes.setBlendMode(OF_BLENDMODE_ADD);
        backGroundColor = 0;
    }
    else if(name == "SCREEN"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: pink);
        avgs->playModes.setBlendMode(OF_BLENDMODE_SCREEN);
        backGroundColor = 0;
    }
    else if(name == "MULTIPLY"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: pink);
        avgs->playModes.setBlendMode(OF_BLENDMODE_MULTIPLY);
        backGroundColor = 255;
    }
    else if(name == "SUBTRACT"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setColorFillHighlight(ofxUIColor :: pink);
        avgs->playModes.setBlendMode(OF_BLENDMODE_SUBTRACT);
        backGroundColor = 255;
    }
    else {
        for(int i = 0; i < envelopes.size(); i++){
            if(name == envelopes[i]){
                toggles[i]->setValue(true);
                avgs->playModes.setWindowShape(name);
                avgs->grainPlayer.setGrainEnvelope(i+1);
            } else {
                toggles[i]->setValue(false);
            }
        }
    }
    
    // PRESET TOGGLE MATRIX
    int count = 0;
    for(int x = 0; x < 5; x++){
        for(int y=0; y < 5; y++){
            count++;
            if(name == "PRESETS(" + ofToString(x) + "," + ofToString(y) + ")")
            {
                loadPreset(count);
                currentPreset = count;
                cout << name << " | x = " << x << " | y = " << y << endl;
            }
        }
    }
}
