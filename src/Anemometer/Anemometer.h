//
//  Anemometer.h
//  VectorField
//
//  Created by Akira on 2014/10/03.
//
//

#ifndef VectorField_Anemometer_h
#define VectorField_Anemometer_h

#include "ofMain.h"

class Anemometer
{
public:
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(ofColor::red, 100);
        ofCircle(pos, 6);
        ofPopStyle();
    }
    
    ofPoint pos;
    ofVec2f wind;
    
};

#endif
