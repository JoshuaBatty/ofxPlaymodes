#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "VisualRingBuffer.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);
		
        VisualRingBuffer ringBuffer;
        ofVideoGrabber grabber;

		ofxPanel gui;
		ofxIntSlider fps;
		ofxIntSlider delay;

};
