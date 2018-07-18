
#include "ofxImageSequencePlayer.h"

ofxImageSequencePlayer::ofxImageSequencePlayer() {
	ofLogVerbose("ofxImageSequence") << "Ctor";

    bPlaying = false;
    bReversed = false;
    bPingPong = false;
    bComplete = false;
    bPingPongForwardComplete = false;
	bLooping = true;
	mFPS = -1;
	mLastUpdateTime = -1;
    mFrameIncrement = 1;
	partialFrame = 0;
}

ofxImageSequencePlayer::~ofxImageSequencePlayer() { 
	ofLogVerbose("ofxImageSequence") << "Dtor";
}

void ofxImageSequencePlayer::importSequence(string pathToDir) { 
	ofxImageSequence::importSequence(pathToDir); 
	newSequenceSetup();  
}

void ofxImageSequencePlayer::newSequenceSetup() {
    mLastUpdateTime = ofGetElapsedTimef();
}

// playing
void ofxImageSequencePlayer::play() {
    bPlaying = true;
    bComplete = false;
    bPingPongForwardComplete = false;
}

void ofxImageSequencePlayer::playInReverse() {
    reverse();
    play();
}

void ofxImageSequencePlayer::pause() {
    bPlaying = false;
}

void ofxImageSequencePlayer::stop() {
    bPlaying = false;
    bPingPongForwardComplete = false;
    bComplete = false;

    if(bReversed) {
        setCurrentFrameIndex(getTotalFrames()-1);
    }
    else {
        setCurrentFrameIndex(0);
    }
}

void ofxImageSequencePlayer::reverse() {
    bPingPong = false;
    bReversed = true;
}

void ofxImageSequencePlayer::setShouldPingPong(bool shouldPingPong) {
    bPingPong = shouldPingPong;

    if(bPingPong) {
        bComplete = false;
    }
}

void ofxImageSequencePlayer::setShouldLoop(bool shouldLoop) {
    bLooping = shouldLoop;

    if(bLooping) {
        bComplete = false;
    }
}


// update and draw
void ofxImageSequencePlayer::update() {
    float currentTime = ofGetElapsedTimef();

    if(mFPS && (currentTime-mLastUpdateTime) < 1.0f/mFPS) return;
    
    if(bPlaying){
        float newFrameIndex;
        float currentFrameIndex = getCurrentFrameIndex();
        int totalFrames = getTotalFrames();

        if(bPingPong) {
			if (partialFrame > 0.0f) {
				newFrameIndex = currentFrameIndex + (bPingPongForwardComplete ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);
			}
			else {
				newFrameIndex = currentFrameIndex + ceil(bPingPongForwardComplete ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);

			}
			partialFrame = (bPingPongForwardComplete ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);
			partialFrame = fmod(partialFrame, 1.0);
			partialFrame = fabs(partialFrame);
		}
        else {
            if (partialFrame > 0.0f) {
				newFrameIndex = currentFrameIndex + (bReversed ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);
			}
			else {
				newFrameIndex = currentFrameIndex + ceil((bReversed ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame));
			}
            partialFrame = (bReversed ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);
            partialFrame = fmod(partialFrame, 1.0f);
//			cout << "partialFrame " << partialFrame << endl;
//			cout << "currentFrameIndex " << currentFrameIndex << endl;
//          cout << "newFrameIndex " << newFrameIndex << endl;
        }
        
        // we're playing of forwards if the new frame > total frames.
        if(newFrameIndex > totalFrames -1) {
            if(bPingPong) {
                bPingPongForwardComplete = !bPingPongForwardComplete;
                newFrameIndex = totalFrames -1;
            }
            else {
                if(bLooping) {
                    newFrameIndex = newFrameIndex - totalFrames;
                }
                else {
                    bComplete = true;
                    dispatchCompleteNotification();
                }
            }
        }
        // we're playing backwards, if the new frame < 0.
        else if(newFrameIndex <= 0) {
            if(bPingPong) {
                bPingPongForwardComplete = !bPingPongForwardComplete;
                newFrameIndex = 0;
            }
            else {
                if(bLooping) {
                    newFrameIndex = totalFrames - abs(newFrameIndex);
                }
                else {
                    bComplete = true;
                    dispatchCompleteNotification();
                }
            }
        }
        
        if (bComplete == false) { 
            if (int(newFrameIndex) != int(currentFrameIndex)) {
                setCurrentFrameIndex(int(newFrameIndex));
                partialFrame = (bReversed ? -mFrameIncrement + partialFrame : mFrameIncrement + partialFrame);
                partialFrame = fmod(partialFrame, 1.0f);
            }
        }
    }
	else if (bPlaying == false) {
		// force update texture if not allocated
		if (isReady()) {
			if (getTextureReference().isAllocated() == false) {
				setCurrentFrameIndex(getCurrentFrameIndex());
			}
		}
	}

    mLastUpdateTime = ofGetElapsedTimef();
}

void ofxImageSequencePlayer::dispatchCompleteNotification() {
    static ofEventArgs args;
    ofNotifyEvent(sequenceCompleted, args, this);
}


void ofxImageSequencePlayer::draw(int x, int y) {
	if (isReady()) {
//		getTextureForFrame(getCurrentFrameIndex()).draw(x, y);
		getTexture().draw(x, y);
	}
}