//
//  ConnectionLine.cpp
//  Interp
//
//  Created by Akira on 2014/10/04.
//
//

#include "ConnectionLine.h"
#include "MSAInterpolator.h"

void ConnectionLine::interpolateWind()
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