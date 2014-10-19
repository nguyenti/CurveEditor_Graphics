#ifndef __CATMULLROM_H_INCLUDED__  
#define __CATMULLROM_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "freeform.h"

class CatmullRomCurve: public Freeform {
    // control points from Freeform class

    float2 catmull(float t, float2 p1, float2 p2, float2 p3, float2 p4) {
     
        float t2 = t*t;
        float t3 = t*t*t;
        float2 v; // Interpolated point
     
        /* Catmull Rom spline Calculation */
     
        v.x = ((-t3 + 2*t2-t)*(p1.x) + (3*t3-5*t2+2)*(p2.x) 
            + (-3*t3+4*t2+t)* (p3.x) + (t3-t2)*(p4.x))/2;
        v.y = ((-t3 + 2*t2-t)*(p1.y) + (3*t3-5*t2+2)*(p2.y) 
            + (-3*t3+4*t2+t)* (p3.y) + (t3-t2)*(p4.y))/2;

        // (r{i}-r{i-1})/(t{i}-t{i-1})
        return v;
    }

public:
    void draw() {
        float2 p;
        float2 v[4];
        float t;
        int i;
        int size = controlPoints.size();
        if (size >= 4) {
            for (i = 0; i <= size - 4; i++) {
                for (int j = 0; j < 4; j++)
                    v[j] = controlPoints.at(i + j);
                glBegin(GL_LINE_STRIP);
                for(t = 0; t < 1; t += 0.0001) {
                    p = catmull(t, v[0], v[1], v[2], v[3]);
                    glVertex2f(p.x,p.y);
                }
                glEnd();
            }
        } else {

        }
        glFlush();
    }

    float2 getPoint(float t) { float2(0.0, 0.0); }

    char getType() {
        return 'c';
    }

    void drawTracker(float t) {}

    Freeform * clone() const {
        return new CatmullRomCurve(*this);
    }

};

#endif