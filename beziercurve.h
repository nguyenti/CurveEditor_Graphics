#ifndef __BEZIERCURVE_H_INCLUDED__  
#define __BEZIERCURVE_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "freeform.h"


class BezierCurve: public Freeform
{
    // double t = t in time
    static double bernstein(int i, int n, double t) {
        if(n == 1) {
            if(i == 0) return 1-t;
            if(i == 1) return t;
            return 0;
        }
        if(i < 0 || i > n) return 0;
        return (1-t) *bernstein(i, n-1, t) + t * bernstein(i-1, n-1, t);
    }

    static double bernsteinDerivative(int i, int n, double t) {
        if(n == 1) {
            if(i == 0) return 1-t;
            if(i == 1) return t;
            return 0;
        }
        if(i < 0 || i > n) return 0;
        return ((-1) * bernstein(i, n-1, t) + t * bernsteinDerivative(i-1, n-1, t)) 
            + ((1-t) * bernsteinDerivative(i, n-1, t) + bernstein(i-1, n-1, t));
    }

public:
    float2 getPoint(float t) {
        float2 r(0.0, 0.0);
        // for every control point
        for (int i = 0; i < controlPoints.size(); i++) {
        // compute weight using the Bernstein formula
        // add control point to r, weighted
            r = r + (controlPoints.at(i) * bernstein(i, controlPoints.size() - 1, t));
        }
        return r;
    }

    char getType() {
        return 'b';
    }

    float2 getDerivative(float t){
        float2 r(0.0, 0.0);
        // for every control point
        for (int i = 0; i < controlPoints.size(); i++) {
        // compute weight using the Bernstein formula
        // add control point to r, weighted
            r = r + (controlPoints.at(i) * bernsteinDerivative(i, controlPoints.size() - 1, t));
        }
        return r;
    }

    // draw the tracker on the point at time t
    void drawTracker(float t) {
        float2 p = getPoint(t);
        glPointSize(10);
        float2 v = getDerivative(t);

        // draw the point
        glBegin(GL_POINTS);
            glVertex2d(p.x, p.y);
        glEnd();

        // draw derivative
        glBegin(GL_LINE_STRIP);
            glVertex2d(p.x,p.y);
            glVertex2d(v.x,v.y);
        glEnd();
    }

    Freeform * clone() const {
        return new BezierCurve(*this);
    }

};

#endif