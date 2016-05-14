/*
 * VideoBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoBuffer.h"

namespace ofxPm{
VideoBuffer::VideoBuffer(VideoSource & source, int size) {
	setup(source,size);
}

VideoBuffer::VideoBuffer(){
    //bRecord = true;
	source = NULL;
	totalFrames=0;
	stopped = false;
	maxSize = 0;
	microsOneSec=0;
	realFps = 0;
	framesOneSec = 0;
    
    framePos = 0; // NEW VARIABLE
    lastVal = 0;
}


void VideoBuffer::setup(VideoSource & source, int size, bool isTracer, bool allocateOnSetup){
	this->source=&source;
    this->isTracer = isTracer;
	totalFrames=0;
	maxSize = size;
	if(allocateOnSetup){
		for(int i=0;i<size;i++){
			VideoFrame videoFrame = VideoFrame::newVideoFrame(source.getNextVideoFrame().getPixelsRef());
            if(!isTracer) pushNewVideoFrame(videoFrame);
            else pushNewVideoFrameTracer(videoFrame);
		}
	}
	resume();
	microsOneSec=-1;
}

VideoBuffer::~VideoBuffer() {

}

void VideoBuffer::setSize(int numFrames){
    maxSize = numFrames;
    for(int i=0;i<numFrames;i++){
        VideoFrame videoFrame = VideoFrame::newVideoFrame(source->getNextVideoFrame().getPixelsRef());
        newVideoFrame(videoFrame);
    }
    
}
    
  
void VideoBuffer::pushNewVideoFrameTracer(VideoFrame & frame){
    if(isStopped()){
        return;
    }
    int64_t time = frame.getTimestamp().epochMicroseconds();
    if(microsOneSec==-1) microsOneSec=time;
    framesOneSec++;
    int64_t diff = time-microsOneSec;
    if(diff>=1000000){
        realFps = double(framesOneSec*1000000.)/double(diff);
        framesOneSec = 0;
        microsOneSec = time-(diff-1000000);
    }
    totalFrames++;
    if(size()==0)initTime=frame.getTimestamp();
    //timeMutex.lock();
    
    frames.push_back(frame);
    
    while(size()>maxSize){
        frames.erase(frames.begin());
    }
}
  
    
//////////////////////////////////////////////////////////////////////////////
void VideoBuffer::pushNewVideoFrame(VideoFrame & frame){
    
    int64_t time = frame.getTimestamp().epochMicroseconds();
    if(microsOneSec==-1) microsOneSec=time;
    framesOneSec++;
    int64_t diff = time-microsOneSec;
    if(diff>=1000000){
        realFps = double(framesOneSec*1000000.)/double(diff);
        framesOneSec = 0;
        microsOneSec = time-(diff-1000000);
    }
    totalFrames++;
    if(size()==0)initTime=frame.getTimestamp();
    //timeMutex.lock();

    
    if (size() >= maxSize) {
        // THIS LINE IS GIVING ME CRASHES SOMETIMES ..... SERIOUS WTF : if i dont see this happen again its fixed
        frames[ofClamp(framePos, 0, size()-1)] = frame; // Here we use the framePos variable to specify where new frames
                                  // should be stored in the video buffer instead of using the vector push_back call.
    }
    else if (size() < maxSize) {
        frames.push_back(frame);
    }
    
    while(size() > maxSize){
        frames.erase(frames.begin()+framePos);
    }
}

     
// This function sets the position in the videoBuffer to write new frames to
// Is being driven by the normalized record position of the Maxi sample so the 2 are synchronised
    
void VideoBuffer::setFramePos(float posPerc) {
    
    float outVal = posPerc * (float)(size()-1);    // convert percentage to numFrames
    float tempVal = floor(outVal+0.5);             // used to round float to next int
    
    if(tempVal==lastVal){
        framePos = tempVal+1;         // If the percenatge to frames = name int as last time +1
        //cout << "frames Pos = " << framePos << " --- " << "lastVal = " << lastVal << " --- " << "tempVal + 1 = " << tempVal + 1;
    } else if(tempVal-lastVal==1){
        framePos = tempVal;           // else use the rounded value
        //cout << "frames Pos = " << framePos << " --- " << "lastVal = " << lastVal << " --- " << "tempVal = " << tempVal;
    } else {
        framePos = outVal;            // else percentage to frame converstion was correct
        //cout << "frames Pos = " << framePos << " --- " << "lastVal = " << lastVal << " --- " << "outVal = " << outVal;
    }
    
    // If the audio rec position conversion is greater than a single frame, or if the framerate changes
    // work out the dirreference and paste the previous frame accross to avoid all glitches :)
    
    int diff = ofWrap(framePos-lastVal, 0, size());
    if(diff>1 && lastVal <= framePos){
        for(int i = lastVal; i < lastVal+diff; i++){
           // cout << "FRAMEPOS = " << framePos << " -- " << "LAST VAL = " << lastVal << " -- " << "DIFF = " << diff << " -- " << "LAST VAL + DIFF = " << lastVal+diff << " -- " << "framePos - lastVal = " << framePos - lastVal << endl;

            frames[ofWrap(i,0,size())] = frames[lastVal];
        }
    }

    lastVal = framePos;   // Store the last truncated integer
}


void VideoBuffer::setFramePos(int pos) {
    framePos = pos;
}

void VideoBuffer::iterFramePos() {
    framePos++;
    if (framePos > maxSize) {
        framePos = 0;
    }
}


Timestamp VideoBuffer::getLastTimestamp(){
    if(size()>0)
        return frames.back().getTimestamp();
    else
        return Timestamp();
}

TimeDiff VideoBuffer::getTotalTime(){
    return getLastTimestamp()-getInitTime();
}

Timestamp VideoBuffer::getInitTime(){
    return initTime;
}

unsigned int VideoBuffer::size(){
    return frames.size();
}


unsigned int VideoBuffer::getMaxSize(){
	return maxSize;
}


float VideoBuffer::getFps(){
    if(source) return source->getFps();
    else return 0;
}

VideoFrame VideoBuffer::getVideoFrame(TimeDiff time){
    VideoFrame frame;
    if(size()>0){
        int frameback = CLAMP((int)((float)time/1000000.0*(float)getFps()),1,int(size()));
        int currentPos = CLAMP(size()-frameback,0,size()-1);
        frame = frames[currentPos];
    }

    return frame;

}

VideoFrame VideoBuffer::getVideoFrame(int position){
    //return buffer.find(times[times.size()-position])->second;
    if(size()){
        position = CLAMP(position,0,int(size())-1);

        /*
        // At the moment there is a weird jump in position when the buffers fills up for the 1st time
        // Once it has filled up then it behaves as expected.... FIX THIS ONE DAY
        if(size() < maxSize){
            position = CLAMP(position,0,int(size())-1);
            cout << "1st position = " << position << endl;
        } else {
            position = ofWrap(position+framePos, 0, maxSize);
            cout << "2nd position = " << position << endl;
        }
*/
        return frames[position];
    }else{
        return VideoFrame();
    }
         
}

VideoFrame VideoBuffer::getVideoFrame(float pct){
    return getVideoFrame(getLastTimestamp()-(getInitTime()+getTotalTime()*pct));
}

VideoFrame VideoBuffer::getNextVideoFrame(){
    return getVideoFrame((int)size()-1);
//    return getVideoFrame((int)framePos);
}

long VideoBuffer::getTotalFrames(){
    return totalFrames;
}

float VideoBuffer::getRealFPS(){
    return realFps;
}


void VideoBuffer::draw(){
	
    float length = (float(size())/float(maxSize))*(ofGetWidth()-(PMDRAWSPACING));
    float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(maxSize);
	int drawBufferY = PMDRAWELEMENTSY+108;
    if(stopped) ofSetColor(255,0,0);
	else ofSetColor(255);
	
	ofDrawLine(0+PMDRAWSPACING,drawBufferY,length,drawBufferY);
	
	ofSetColor(255);
	
    char measureMessage[10];
    for(int i=0;i<(int)size()+1;i++){
		/*
       if(i%100==0){
            ofLine(oneLength*i,710,oneLength*i,700);
            sprintf(measureMessage,"%0.2f",(float)(frames[i]->getTimestamp()-initTime)/1000000.0);
            ofDrawBitmapString(measureMessage,oneLength*i,695);
        }
		 */
    	int fps = getFps();
		if(fps && i%(int)fps==0)
		{
			ofSetLineWidth(2.0);
			ofSetColor(255,128,0);
			if(i!=int(size())) ofDrawBitmapString(ofToString(int(size()-i-1)),oneLength*(i)+PMDRAWSPACING + oneLength/2,PMDRAWELEMENTSY+98);
			else 
			{
				ofSetColor(50);
				ofDrawBitmapString(ofToString(getTotalFrames()),oneLength*(i)+PMDRAWSPACING - 10,PMDRAWELEMENTSY+55); 
			}
			//if(i!=int(size())) ofDrawBitmapString(ofToString(getTotalFrames()-i),ofGetWidth()-PMDRAWSPACING-(oneLength*(i+1)) + oneLength/2,drawBufferY-15);
        }
		else 
		{
			ofSetLineWidth(1.0);
			ofSetColor(155,58,0);
		}
		ofDrawLine(oneLength*(i)+PMDRAWSPACING,drawBufferY,oneLength*(i)+PMDRAWSPACING,drawBufferY-10);
    }
}

void VideoBuffer::draw(int _x, int _y, int _w, int _h){
    
        float length = _w;
        float oneLength=(float)(ofGetWidth()-_x)/(float)(maxSize);
        int drawBufferY = _y;
        if(stopped) ofSetColor(255,0,0);
        else ofSetColor(255);
    
        ofDrawLine(_x,drawBufferY,length,drawBufferY);
        
        ofSetColor(255);
        
        char measureMessage[10];
        for(int i=0;i<(int)size()+1;i++){
            /*
             if(i%100==0){
             ofLine(oneLength*i,710,oneLength*i,700);
             sprintf(measureMessage,"%0.2f",(float)(frames[i]->getTimestamp()-initTime)/1000000.0);
             ofDrawBitmapString(measureMessage,oneLength*i,695);
             }
             */
            int fps = getFps();
            if(fps && i%(int)fps==0)
            {
                ofSetLineWidth(2.0);
                ofSetColor(255,128,0);
                if(i!=int(size())) ofDrawBitmapString(ofToString(int(size()-i-1)),oneLength*(i)+_x + oneLength/2,PMDRAWELEMENTSY+104);
                else
                {
                    ofSetColor(50);
                    ofDrawBitmapString(ofToString(getTotalFrames()),oneLength*(i)+_x - 10,PMDRAWELEMENTSY+55);
                }
                //if(i!=int(size())) ofDrawBitmapString(ofToString(getTotalFrames()-i),ofGetWidth()-PMDRAWSPACING-(oneLength*(i+1)) + oneLength/2,drawBufferY-15);
            }
            else 
            {
                ofSetLineWidth(1.0);
                ofSetColor(155,58,0);
            }
            ofDrawLine(oneLength*(i)+_x,drawBufferY,oneLength*(i)+_x,drawBufferY-10);
        }
    }


void VideoBuffer::stop(){
    stopped = true;
}

void VideoBuffer::resume(){
    stopped = false;
}

bool VideoBuffer::isStopped(){
	return stopped;
}

void VideoBuffer::clear(){
    while(!frames.empty()){
        frames.erase(frames.begin());
    }
}
}
