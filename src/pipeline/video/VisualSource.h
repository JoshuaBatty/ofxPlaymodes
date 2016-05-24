/*
 * VisualSource.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#pragma once 

#include "VideoFrame.h"

namespace ofxPm{
class VisualSource{
public:
	VisualSource();
	virtual ~VisualSource();

	virtual VideoFrame getNextVideoFrame()=0;
    virtual float getFps()=0;
    ofEvent<VideoFrame> newFrameEvent;
};
}
