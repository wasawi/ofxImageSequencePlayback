#pragma once

// OF
#include "ofMain.h"

// addons
#include "ofxImageSequence.h"

class ofxImageSequencePlayer : public ofxImageSequence 
{
  public:
    void update();
    void draw(int x=0, int y=0);
    
	ofxImageSequencePlayer();
	~ofxImageSequencePlayer();
    
    void play();
    void playInReverse();
    void pause();
    void stop(); 
    void reverse();
    
    bool isComplete() { return bComplete; }
    bool isReversed() { return bReversed; }
    bool isPingPong() { return bPingPong; }
    bool isPlaying()  { return bPlaying; }

	void importSequence(string pathToDir);
    void setFPS(const float fps) { mFPS = fps; }
    void setShouldLoop(bool shouldLoop);
    void setShouldPingPong(bool shouldPingPong);
    float getFrameIncrement() { return mFrameIncrement; }
    void setFrameIncrement(float frames) { mFrameIncrement = frames; }
    
    ofEvent<ofEventArgs> sequenceCompleted;

private:
    void newSequenceSetup();
    void dispatchCompleteNotification();

    bool bPlaying;
    bool bReversed;
    bool bPingPong;
    bool bComplete;
    bool bLooping;
    bool bPingPongForwardComplete;
    
    float mFrameIncrement;
    float partialFrame;
    float mFPS;
    float mLastUpdateTime;
};


// definitions
typedef shared_ptr<ofxImageSequencePlayer> SequencePtr;
typedef vector<SequencePtr> SequenceCollection;
typedef shared_ptr<SequenceCollection> SequenceCollectionPtr;
