/*
 * FileGrabber.h
 *
 *  Created on: 01/11/2009
 *      Author: arturo castro
 */

#ifndef FILEGRABBER_H_
#define FILEGRABBER_H_

#include "VisualSource.h"
#include "ofMain.h"


namespace ofxPm{
    class FileGrabber: public VisualSource, public ofVideoPlayer {
public:
	FileGrabber();
	virtual ~FileGrabber();

	VideoFrame getNextVideoFrame();
    float getFps();

    void update();

};
}
#endif /* FILEGRABBER_H_ */
