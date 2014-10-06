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
#include "InterpSurface.h"
#include "WindAndPosition.h"

class TestParticle
{
public:
    
    void setup(float w, float h, ofPoint pos)
    {
        width = w;
        height = h;
        this->pos = pos;
        pollenMass = 0.2;
        timeSpeed = 0.2;
        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    
    void update(ofVec2f field)
    {
        float t = Globals::FRAMENUM * timeSpeed;
        float speed = (1 + ofNoise(t, field.x, field.y)) / pollenMass;
        
        pos.x += ofLerp(-speed, speed, field.x);
        pos.y += ofLerp(-speed, speed, field.y);
        
        if (pos.x < 0 || pos.x > width ||
            pos.y < 0 || pos.y > height)
        {
            pos.x = ofRandom(0, width);
            pos.y = ofRandom(0, height);
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(col, 250);
        ofCircle(pos, 4);
        ofPopStyle();
    }
    
    ofPoint pos;
    ofColor col;
    float pollenMass;
    float timeSpeed;
    float width;
    float height;
};


class GridCell : public ofRectangle
{
public:
    
    GridCell()
    {
        col = ofColor::pink;//ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    
    void draw()
    {
        ofVec2f wind = getWind();
        ofPushStyle();
        ofSetColor(ofFloatColor(wind.x, wind.y, 0));
        ofRect(*this);
        ofPopStyle();
    }
    
    void debugDraw()
    {
        ofVec2f wind = getWind();
        
        ofPushStyle();
        
        ofPushStyle();
        ofNoFill();
        ofRect(*this);
        
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
    
    void setup(float w, float h, int stepx, int stepy)
    {
        is.setup();
        
        setupGrid(w, h, stepx, stepy);
        feedWindToGrid(is.getWindAndPositions());
        
        int nTps = 1000;
        for (int i = 0; i < nTps; i++)
        {
            ofPoint pos = ofVec2f(ofRandom(0, ofGetWidth()),
                                  ofRandom(0, ofGetHeight()));
            TestParticle tp;
            tp.setup(w, h, pos);
            tps.push_back(tp);
        }
        
        fieldFbo.allocate(w, h, GL_RGB);
        fieldFbo.begin();
        ofClear(0);
        fieldFbo.end();
        
        rgbaFboFloat.allocate(w, h, GL_RGBA32F_ARB);
        rgbaFboFloat.begin();
        ofClear(0);
        rgbaFboFloat.end();
    }
    
    void update()
    {
        is.update();
        
        fieldFbo.begin();
        ofClear(0);
        for (auto c : grid)
        {
            c.draw();
        }
        fieldFbo.end();
        fieldFbo.readToPixels(fieldPx);
    }
    
    void drawVectorField()
    {
        fieldFbo.draw(0, 0);

        for (auto c : grid)
        {
            c.debugDraw();
        }

        debugUpdate();
        rgbaFboFloat.begin();
        ofPushStyle();
        ofFill();
        ofSetColor(ofColor::white, 1);
        ofRect(0, 0, rgbaFboFloat.getWidth(), rgbaFboFloat.getHeight());
        for (auto tp : tps)
        {
            tp.draw();
        }
        ofPopStyle();        
        rgbaFboFloat.end();

        rgbaFboFloat.draw(0, 0);
    }
    
    void drawInterpSurface()
    {
        is.draw();
    }
    
    ofVec2f getField(ofPoint pos)
    {
        if (pos.x < ofGetWidth() && pos.y < ofGetHeight())
        {
            ofFloatColor fCol = fieldPx.getColor(pos.x, pos.y);
            return ofVec2f(fCol.r, fCol.g);
        }
        else
        {
            return ofVec2f::zero();
        }
    }
    
private:
    
    void debugUpdate()
    {
        for (auto &tp : tps)
        {
            ofVec2f field = getField(tp.pos);
            tp.update(field);
        }
    }
    
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
        
        for (int i = 0; i < grid.size(); i++)
        {
            if (i != 0 && i != grid.size() - 1)
            {
                if (grid.at(i).winds.empty())
                {
                    grid.at(i).winds.insert(grid.at(i).winds.end(),
                                            grid.at(i - 1).winds.begin(),
                                            grid.at(i - 1).winds.end());
                    grid.at(i).winds.insert(grid.at(i).winds.end(),
                                            grid.at(i + 1).winds.begin(),
                                            grid.at(i + 1).winds.end());
                }
            }
        }
    }
    
    InterpSurface is;
    
    vector<GridCell> grid;
    vector<TestParticle> tps;
    ofFbo fieldFbo;
    ofFloatPixels fieldPx;
    
    ofFbo rgbaFboFloat;
};

#endif
