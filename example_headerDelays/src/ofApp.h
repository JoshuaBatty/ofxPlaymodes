#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "VisualRingBuffer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);
		
        VisualRingBuffer ringBuffer;
        ofVideoGrabber grabber;
    
        ofFbo fbo;

		ofxPanel gui;
		ofxIntSlider fps;
		ofxIntSlider delay;
    
private:
        int w, h;
        bool bTrailsOrChaser;
};
