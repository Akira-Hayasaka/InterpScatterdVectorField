//
//  VectorField.h
//  Interp
//
//  Created by Akira on 2014/10/05.
//
//

#ifndef Interp_VectorField_h
#define Interp_VectorField_h

#include "ofMain.h"
#include "WindAndPosition.h"

class GridCell : public ofRectangle
{
public:
    
    GridCell()
    {
        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    
    void draw()
    {
        ofPushStyle();
        ofNoFill();
        ofRect(*this);
        
        ofVec2f wind = getWind();
        ofSetColor(ofColor::pink);
        float windSpeed = 40;
        ofVec2f field(wind.x, wind.y);
        ofPushMatrix();
        ofTranslate(getCenter());
        ofSetColor(col);
        ofLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofVec2f getWind()
    {
        ofVec2f wind;
        for (auto w : winds)
        {
            ofVec2f v(w->x, w->y);
            wind += v;
        }
        wind /= winds.size();
        return wind;
    }
    
    vector<ofPtr<ofVec2f> > winds;
    
private:
    
    ofColor col;
};

class VectorField
{
public:
    
    void setup(float w, float h, int stepx, int stepy,
               vector<WindAndPosition> wps)
    {
        setupGrid(w, h, stepx, stepy);
        feedWindToGrid(wps);
    }
    
    void draw()
    {
        for (auto c : grid)
        {
            c.draw();
        }
    }
    
    ofVec2f getField(ofPoint pos)
    {
        
    }
    
private:
    
    void setupGrid(float w, float h, int stepx, int stepy)
    {
        float cellw = w / stepx;
        float cellh = h / stepy;
        float accumx = 0.0;
        float accumy = 0.0;
        for (int x = 0; x < stepx; x++)
        {
            for (int y = 0; y < stepy; y++)
            {
                GridCell cell;
                cell.set(accumx, accumy, cellw, cellh);
                grid.push_back(cell);
                accumy += cellh;
            }
            accumx += cellw;
            accumy = 0.0;
        }
    }
    
    void feedWindToGrid(vector<WindAndPosition> wps)
    {
        for (int i = 0; i < wps.size(); i++)
        {
            for (auto &c : grid)
            {
                if (c.inside(wps.at(i).pos))
                {
                    c.winds.push_back(wps.at(i).wind);
                }
            }
        }
    }
    
    vector<GridCell> grid;
};

#endif
