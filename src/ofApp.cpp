#include "ofApp.h"


void ofApp::setup()
{
    ofBackground(100, 100, 100);
    is.setup();
}

void ofApp::update()
{
    Globals::ELAPSEDTIME = ofGetElapsedTimef();
    is.update();
    
    if (ofGetFrameNum() % 60 == 0)
        ofLog() << "fps: " << ofGetFrameRate();
}

void ofApp::draw()
{
    is.draw();
}

void ofApp::mouseReleased(int x, int y, int button)
{
//    for (auto &t : tris)
//    {
//        if (t.isIside(ofPoint(x, y)))
//        {
//            spline2D.clear();
//            interpolationType = msa::kInterpolationCubic;
//            spline2D.setInterpolation(interpolationType);
////            spline2D.push_back(t.getNearest(ofPoint(x, y)));
////            spline2D.push_back(ofPoint(x, y));
////            spline2D.push_back(t.getFarest(ofPoint(x, y)));
//            spline2D.push_back(t.pts.at(0));
//            spline2D.push_back(t.pts.at(1));
//            spline2D.push_back(t.pts.at(2));
//            spline2D.push_back(ofPoint(x, y));
//            spline2D.push_back(t.pts.at(2));
//            spline2D.push_back(t.pts.at(1));
//            spline2D.push_back(t.pts.at(0));
//            break;
//        }
//    }
}

void ofApp::mousePressed(int x, int y, int button)
{

}

void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}