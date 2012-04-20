#pragma once

#include "ofMain.h"
#include "ofxPlaymodes.h"
#include "ofxOsc.h"
#include "pmUtils.h"
#include "ofxSimpleGuiToo.h"



class testApp : public ofBaseApp{

	public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxPm::VideoGrabber			vGrabber;
	ofxPm::AudioGrabber			aGrabber;
	ofxPm::VideoBuffer			vBuffer;
	ofxPm::AudioBufferSamples	aBuffer;
	ofxPm::avLooperRenderer		avRenderer;

	// audio
	int							aBufferSize;
	int							aSampleRate;
	int							aNumChannels;
	ofSoundStream				soundStream;

	void audioReceived(float * input, int bufferSize, int nChannels);
	void audioRequested (float * output, int bufferSize, int nChannels);

	bool					freeze;
	bool					audioSetupFinished;

	//osc eloi
	ofxOscReceiver			receiver;
	ofxOscSender			sender;
	void					updateOsc();

	// needed for gui ?�
	float					guiDelay;
	float					guiIn;
	float					guiOut;
	float					guiLength;
	
	bool					guiFreeze;
	bool					guiPlay;
			
};
