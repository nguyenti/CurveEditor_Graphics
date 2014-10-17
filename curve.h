#ifndef __CURVE_H_INCLUDED__  
#define __CURVE_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "float2.h"

// curve class used for generalization
class Curve {
public:
    virtual float2 getPoint(float t)=0;
    
    virtual void draw() {
    	glBegin(GL_LINE_STRIP);
        for (float t = 0; t < 1; t += 0.0001) {
            float2 p = getPoint(t);
            glVertex2d(p.x, p.y);
        }
        glEnd();
    }
};

#endif