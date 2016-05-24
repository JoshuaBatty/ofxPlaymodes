/*
 * BasicVideoRenderer.h
 *
 *  Created on: 11/02/2012
 *      Author: arturo
 */

#ifndef BASICVIDEORENDERER_H_
#define BASICVIDEORENDERER_H_

#include "VideoSink.h"
#include "VisualSource.h"
#include "VideoHeader.h"

namespace ofxPm
{
	
class BasicVideoRenderer: public VideoSink {
public:
	BasicVideoRenderer();
	BasicVideoRenderer(VisualSource & source);
	virtual ~BasicVideoRenderer();

	void setup(VisualSource & source);

	void draw();
	void draw(int x,int y,int w,int h);

private:
	VisualSource * source;
};
}
#endif /* BASICVIDEORENDERER_H_ */
