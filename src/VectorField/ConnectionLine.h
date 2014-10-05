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
#include "MSAInterpolator.h"

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
        
        interpolationType = msa::kInterpolationCubic;
        spline2D.setInterpolation(interpolationType);
    }
    
    void resample()
    {
        line = line.getResampledByCount(numSample);
    }
    
    void initialWindInterpolation()
    {
        spline2D.clear();
        
        for (auto wp : wps)
        {
            ofVec2f wind(wp.wind->x, wp.wind->y);
            spline2D.push_back(wind);
        }
        
        float step = 1.0 / (line.getVertices().size()-1);
        int lineIdx = 0;
        for (float i = 0.0; i <= 1.0; i += step)
        {
            ofVec2f wind = spline2D.sampleAt(i);
            WindAndPosition wp;
            wp.wind = ofPtr<ofVec2f>(new ofVec2f);
            wp.wind->x = wind.x;
            wp.wind->y = wind.y;
            wp.pos = line.getVertices().at(lineIdx);
            wps.insert(wps.begin() + lineIdx + 1, wp);
            lineIdx++;
        }
    }
    
    void updateWindInterpolation()
    {
        spline2D.clear();
        
        spline2D.push_back(ofVec2f(wps.front().wind->x, wps.front().wind->y));
        spline2D.push_back(ofVec2f(wps.back().wind->x, wps.back().wind->y));
        
        float step = 1.0 / (line.getVertices().size()-1);
        int wpIdx = 0;
        for (float i = 0.0; i <= 1.0; i += step)
        {
            ofVec2f wind = spline2D.sampleAt(i);
            wps.at(wpIdx).wind->x = wind.x;
            wps.at(wpIdx).wind->y = wind.y;
            wpIdx++;
        }
    }
    
    void draw()
    {
        line.draw();
        
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
            ofVec2f field(wp.wind->x, wp.wind->y);
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
    
    msa::Interpolator2D spline2D;
    msa::InterpolationType interpolationType;
    
    int numSample;
    ofPoint begin;
    ofPoint end;
    ofPolyline line;
    bool bFromBegin;
    vector<WindAndPosition> wps;
};

#endif
