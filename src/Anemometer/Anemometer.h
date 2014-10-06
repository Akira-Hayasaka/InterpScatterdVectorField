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
#include "Constants.h"

class Anemometer
{
public:
    
    Anemometer()
    {
        incx = 0.004;
        incy = 0.003;
        deltax = ofRandom(0.0, 0.8);
        deltay = ofRandom(0.0, 0.8);
    }
    
    void setup(ofPoint pos)
    {
        this->pos = pos;
        wind = ofPtr<ofVec2f>(new ofVec2f);
        wind->x = ofRandomuf();
        wind->y = ofRandomuf();
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(ofColor::red, 200);
        ofCircle(pos, 6);
        ofPopStyle();
        
        incx = 0.0001;//ofRandom(0.01, 0.02);
        incy = 0.0001;//ofRandom(0.01, 0.02);
    }
    
    void debugUpdateWind()
    {
        wind->x = ofNoise(deltax);
        wind->y = ofNoise(deltay);
        deltax += incx;
        deltay += incy;
    }
    
    ofPoint pos;
    ofPtr<ofVec2f> wind;
  
    // debug
    float incx;
    float incy;
    float deltax;
    float deltay;
};

#endif
