#ifndef __FREEFORM_H_INCLUDED__  
#define __FREEFORM_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "curve.h"
#include <iterator>

class Freeform : public Curve {
protected:
    std::vector<float2> controlPoints;
public:
    virtual float2 getPoint(float t)=0;
    virtual void addControlPoint(float2 p){
        controlPoints.push_back(p);
    }
    virtual  void drawTracker(float t)=0;
    virtual char getType()=0;
    virtual Freeform * clone() const = 0;

    virtual void drawControlPoints(){
        // draw points at control points
        glBegin(GL_POINTS);
        for (int i = 0; i < controlPoints.size(); i++) {
            float2 p = controlPoints.at(i);
            glVertex2d(p.x, p.y);
        }
        glEnd();
    }

    float2 getControlPoint(int i) {
        return controlPoints.at(i);
    }

    virtual void deleteControlPoint(int i) {
        controlPoints.erase(controlPoints.begin() + i);
    }

    void moveControlPoint(int i, float2 f) {
        controlPoints.at(i) = f;
    }

    int getCPSize() {
        return controlPoints.size();
    }

    void printCP() {
        for (int i = 0; i < controlPoints.size(); i++) {
            controlPoints[i].print();
        }
    }

};

#endif