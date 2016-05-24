/*
 * VideoRate.h
 *
 *  Created on: 16/07/2012
 *      Author: arturo
 */

#ifndef VIDEORATE_H_
#define VIDEORATE_H_

#include "VideoFilter.h"

namespace ofxPm{
class VideoRate: public VideoFilter, public ofThread {
public:
	VideoRate();
	virtual ~VideoRate();

    void setup(VisualSource & _source, float fps=30);
    
    VideoFrame getNextVideoFrame();
	void setNewVideoFrame(VideoFrame & frame);
    float getFps();
    void setFps(float fps);
 //   void glThreadUpdate();

//protected:
//    void threadedFunction();
//    void glThreadUpdate(ofEventArgs & args);

private:
    VideoFrame back, front;
    VisualSource* source;
    double fps;
    ofMutex mutexFront;
    queue<VideoFrame> framesToSend;
    double remainder;
};
}
#endif /* VIDEORATE_H_ */
