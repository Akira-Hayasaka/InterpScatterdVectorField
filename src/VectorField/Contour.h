//
//  Contour.h
//  Interp
//
//  Created by Akira on 2014/10/04.
//
//

#ifndef Interp_Contour_h
#define Interp_Contour_h

#include "ofMain.h"
#include "WindAndPosition.h"
#include "MSAInterpolator.h"

class Contour
{
public:
    
    void setup(ofPolyline line, vector<WindAndPosition> wps, int numSample)
    {
        this->line = line;
        this->wps = wps;
        this->numSample = numSample;
        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    
    void interpolatePos()
    {
        msa::Interpolator2D spline2D;
        spline2D.clear();
        msa::InterpolationType interpolationType = msa::kInterpolationCubic;
        spline2D.setInterpolation(interpolationType);
        
        vector<ofPoint> pts = line.getVertices();
        line.clear();
        for (auto p : pts)
        {
            spline2D.push_back(p);
        }
        spline2D.push_back(pts.front());
        
        float step = 1.0 / numSample;
        for (float i = 0.0; i <= 1.0; i += step)
        {
            ofPoint p = spline2D.sampleAt(i);
            line.addVertex(p);
        }
    }
    
    void interpolateWind()
    {
        msa::Interpolator2D spline2D;
        spline2D.clear();
        msa::InterpolationType interpolationType = msa::kInterpolationCubic;
        spline2D.setInterpolation(interpolationType);
        
        vector<WindAndPosition> wpHolder = wps;
        wps.clear();
        for (auto wp : wpHolder)
        {
            spline2D.push_back(wp.wind);
        }
        
        float step = 1.0 / (line.getVertices().size()-1);
        int lineIdx = 0;
        for (float i = 0.0; i <= 1.0; i += step)
        {
            ofVec2f wind = spline2D.sampleAt(i);
            WindAndPosition wp;
            wp.wind = wind;
            wp.pos = line.getVertices().at(lineIdx);
            wps.push_back(wp);
            lineIdx++;
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(col, 100);
        line.draw();
        
//        ofSetColor(ofColor::pink, 50);
        for (auto wp : wps)
        {
            float windSpeed = 40;
            ofVec2f field = wp.wind;
            ofPushMatrix();
            ofTranslate(wp.pos);
            ofLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
            ofPopMatrix();
        }
        ofPopStyle();
    }
    
private:
    
    ofPolyline line;
    ofColor col;
    vector<WindAndPosition> wps;
    int numSample;
};

#endif
