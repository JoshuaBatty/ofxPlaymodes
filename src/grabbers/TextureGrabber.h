//
//  TextureGrabber.h
//  example-player
//
//  Created by Joshua Batty on 22/07/2014.
//
//

#ifndef TEXTUREGRABBER_H_
#define TEXTUREGRABBER_H_

#include "VideoSource.h"
#include "ofMain.h"

namespace ofxPm{
    class TextureGrabber: public VideoSource, public ofFbo {
    public:
        TextureGrabber();
        virtual ~TextureGrabber();
        
        bool initGrabber(int w, int h);
        
        VideoFrame getNextVideoFrame();
        float getFps();
        
        void update();
        
    private:
        VideoFrame frame;
    };
}
#endif /* FILEGRABBER_H_ */
