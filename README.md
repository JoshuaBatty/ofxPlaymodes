ofxPlaymodes
============

ofxPlaymodes is an addon to make easy to work with video and audio buffers and headers that can point at any position in the buffer to be rendered. This allows to use different effects like composition of multiple delays.

**************************************************
I am forking Arturo's version of playModes in order to start hacking Maximilian into the addon to provide granular audio support. 


Addons needed are:
- ofxMaxim -> https://github.com/JoshuaBatty/Maximilian
- ofxDspChain -> https://github.com/jonbro/ofxDspChain
- ofxUI -> https://github.com/rezaali/ofxUI
- ofxXmlSettings
- ofxOsc

KNOWN ISSUE
============
The main issue is that the audio buffer is Maxim and the Video Buffer in Playmodes are working differently. Specifically in how new audiovisual content overwrites content in the ring buffers. 

New incoming audio over writes according to the current position of the recordPosition varibale in Maxim. (This is visualised in the example)

New Video Frames however are put on the very end of the buffer and the whole thing is pushed back in time to make room for the new frames. This happens in the VideoBuffer::newVideoFrame(VideoFrame &frame) method. 

The audio and visual buffers are working fundamentaly different in where new content is written into the ring buffer.  

As a result I have added my own verion of the VideoBuffer::newVideoFrame(VideoFrame &frame) method which changes the two recording buffers to behave the same but im getting quite a few artefacts sometimes. eg, frames not being properly owerwritten, also the video freezes and doesn't update on screen when recording. 


