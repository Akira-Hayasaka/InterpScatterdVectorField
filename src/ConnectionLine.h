//
//  ConnectionLine.h
//  Interp
//
//  Created by Akira on 2014/10/04.
//
//

#ifndef Interp_ConnectionLine_h
#define Interp_ConnectionLine_h

#include "ofMain.h"
#include "WindAndPosition.h"

class ConnectionLine
{
public:
    
    void setup(ofPoint begin, ofPoint end, vector<WindAndPosition> wps, int numSample)
    {
        this->begin = begin;
        this->end = end;
        this->wps = wps;
        this->numSample = numSample;
        line.addVertex(this->begin);
        line.addVertex(this->end);
    }
    
    void resample()
    {
        line = line.getResampledByCount(numSample);
    }
    
    void interpolateWind();
    
    void draw()
    {
//        line.draw();
        
        ofPushStyle();
        ofFill();
        
        ofSetColor(ofColor::blueSteel);
        for (int i = 0; i < line.getVertices().size(); i++)
        {
            ofCircle(line.getVertices().at(i), 3);
        }
        
        ofSetColor(ofColor::pink);
        for (auto wp : wps)
        {
            float windSpeed = 40;
            ofVec2f field = wp.wind;
            ofPushMatrix();
            ofTranslate(wp.pos);
            ofSetColor(ofColor::pink);
            ofLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
            ofPopMatrix();
        }
        
        ofPopStyle();
    }
    
    bool operator== (ConnectionLine cl)
    {
        if ((begin == cl.begin && end == cl.end) ||
            (begin == cl.end && end == cl.begin))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    ofPoint getBegin()
    {
        return begin;
    }
    
    ofPoint getEnd()
    {
        return end;
    }
    
    ofPolyline getLine()
    {
        return line;
    }
    
    vector<WindAndPosition> getWindPositions()
    {
        return wps;
    }
    
    void setBeginFrag(bool b)
    {
        bFromBegin = b;
    }
    
    bool isBegin()
    {
        if (bFromBegin)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
private:
    
    int numSample;
    ofPoint begin;
    ofPoint end;
    ofPolyline line;
    bool bFromBegin;
    vector<WindAndPosition> wps;
};

#endif
