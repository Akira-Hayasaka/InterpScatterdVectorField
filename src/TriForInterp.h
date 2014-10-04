//
//  TriForInterp.h
//  Interp
//
//  Created by Akira on 2014/10/04.
//
//

#ifndef Interp_TriForInterp_h
#define Interp_TriForInterp_h

#include "ofMain.h"

class TriForInterp
{
public:
    
    TriForInterp()
    {
        col = ofColor::yellow;
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(col);
        ofTriangle(pts.at(0), pts.at(1), pts.at(2));
        ofPopStyle();
    }
    
    bool isIside(ofPoint p)
    {
        ofPoint p1 = pts.at(0);
        ofPoint p2 = pts.at(1);
        ofPoint p3 = pts.at(2);
        float alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
        float beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
        float gamma = 1.0f - alpha - beta;
        
        if (alpha > 0.0 && beta > 0.0 && gamma > 0.0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    ofPoint getNearest(ofPoint pt)
    {
        int nearestIdx = 0;
        float nearestDist = 0.0;
        for (int i = 0; i < pts.size(); i++)
        {
            float dist = pts.at(i).distance(pt);
            if (i == 0 || dist < nearestDist)
            {
                nearestDist = dist;
                nearestIdx = i;
            }
        }
        return pts.at(nearestIdx);
    }
    
    ofPoint getFarest(ofPoint pt)
    {
        int farestIdx = 0;
        float farestDist = 0.0;
        for (int i = 0; i < pts.size(); i++)
        {
            float dist = pts.at(i).distance(pt);
            if (i == 0 || dist > farestDist)
            {
                farestDist = dist;
                farestIdx = i;
            }
        }
        return pts.at(farestIdx);
    }
    
    bool operator== (TriForInterp t)
    {
        if (pts.at(0) == t.pts.at(0) &&
            pts.at(1) == t.pts.at(1) &&
            pts.at(2) == t.pts.at(2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    vector<ofPoint> pts;
    ofColor col;
    
private:
    
    float sign(ofPoint p1, ofPoint p2, ofPoint p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }
};

#endif
