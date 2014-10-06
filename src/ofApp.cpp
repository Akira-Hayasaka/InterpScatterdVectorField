#include "ofApp.h"


void ofApp::setup()
{
    ofBackground(100, 100, 100);
    ofSetFrameRate(60);
    
    is.setup();
    vf.setup(ofGetWidth(), ofGetHeight(), 30, 30,
             is.getWindAndPositions());
    
    toggleIS = false;
    toggleVF = true;
}

void ofApp::update()
{
    Globals::ELAPSEDTIME = ofGetElapsedTimef();
    is.update();
    vf.update();
    
    if (ofGetFrameNum() % 60 == 0)
        ofLog() << "fps: " << ofGetFrameRate();
}

void ofApp::draw()
{
    if (toggleVF)
        vf.draw();
    if (toggleIS)
        is.draw();
}

void ofApp::keyPressed(int key)
{
    if (key == 's')
    {
        toggleIS = !toggleIS;
    }
    if (key == 'v')
    {
        toggleVF = !toggleVF;
    }
}

void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}