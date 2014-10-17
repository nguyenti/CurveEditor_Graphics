#ifndef __LAGRANGECURVE_H_INCLUDED__  
#define __LAGRANGECURVE_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "freeform.h"

class LagrangeCurve: public Freeform {
    // control points from Freeform class
    std::vector<float> knots;   // parameter (knot) values

    float lagrange(int i, float t) {
        float Li = 1.0f;
        for(int j = 0; j < controlPoints.size(); j++) {
            if (j != i) {
                Li *= (t - knots[j]) / (knots[i] - knots[j]); 
            }
        }
        return Li;
    }

    double lagrangeDerivative(int i, float t) {
        float num = 1.0f;
        float derivnum = 1.0f;
        float denom = 1.0f;
        for(int j = 0; j < controlPoints.size(); j++) {
            if (j != i) {
                derivnum = derivnum * (t - knots[j]) + num;
                num *= (t - knots[j]);
                denom *= (knots[i] - knots[j]);
            } 
        }
        return derivnum/denom;
    }


public:
    void addControlPoint(float2 cp) { 
        float ti = controlPoints.size();  // or something better
        controlPoints.push_back(cp);
        knots.push_back(ti);
        if (controlPoints.size() > 1) {
            for (float i = 0; i < controlPoints.size(); i++) {
                knots.at(i) = i / (controlPoints.size() - 1);
            }
        }
    }

    char getType() {
        return 'l';
    }

    float2 getPoint(float t) {
        float2 rr(0.0, 0.0);
        for(int i = 0; i < controlPoints.size(); i++) {
            rr += controlPoints[i] * lagrange(i,t);
        }
        return rr;
    }

    float2 getDerivative(float t){
        float2 rr(0.0, 0.0);
        for(int i = 0; i < controlPoints.size(); i++) {
            rr += controlPoints[i] * lagrangeDerivative(i,t);
        }
        return rr;
    }

    void deleteControlPoint(int i) {
        controlPoints.erase(controlPoints.begin() + i);
        if (controlPoints.size() > 1) {
            for (float i = 0; i < controlPoints.size(); i++) {
                knots.at(i) = i / (controlPoints.size() - 1);
            }
        } else {
            knots.at(0) = 0;
        }
    }

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
        return new LagrangeCurve(*this);
    }
};

#endif