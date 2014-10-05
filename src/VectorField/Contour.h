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
        
        interpolationType = msa::kInterpolationCubic;
        spline2D.setInterpolation(interpolationType);
    }
    
    void interpolatePos()
    {
        spline2D.clear();
        
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
        ofPushStyle();
        ofSetColor(col, 100);
        line.draw();
        
//        ofSetColor(ofColor::pink, 50);
        for (auto wp : wps)
        {
            float windSpeed = 40;
            ofVec2f field(wp.wind->x, wp.wind->y);
            ofPushMatrix();
            ofTranslate(wp.pos);
            ofLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
            ofPopMatrix();
        }
        ofPopStyle();
    }
    
private:
    
    msa::Interpolator2D spline2D;
    msa::InterpolationType interpolationType;
    
    ofPolyline line;
    ofColor col;
    vector<WindAndPosition> wps;
    int numSample;
};

#endif
