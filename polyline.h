#ifndef __POLYLINE_H_INCLUDED__  
#define __POLYLINE_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "freeform.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>



class Polyline: public Freeform {

public:
    float2 getPoint(float t){
        return float2(0.0, 0.0);
    }

    char getType() {
        return 'p';
    }

    void draw() {
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i < controlPoints.size(); i++) {
            glVertex2d(controlPoints[i].x, controlPoints[i].y);
        }
        glEnd();
    }

    void addControlPoint(float2 p){
        controlPoints.push_back(p);
    }

    void drawTracker(float t) {
    }

    Freeform * clone() const {
        return new Polyline(*this);
    }
};

#endif