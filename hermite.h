#ifndef __HERMITE_H_INCLUDED__  
#define __HERMITE_H_INCLUDED__ 

#define _USE_MATH_DEFINES

#include "freeform.h"

class HermiteCurve: public Freeform {
    // control points from Freeform class
    std::vector<float2> tangents;
    std::vector<float> knots;

    double square(double x) {
        return x * x;
    }

    double cube(double x) {
        return x * x * x;
    }

    float2 average(float2 a, float2 b) {
        return (a + b) / 2;
    }

    double h00(double t) {
        return 2 * (t * t * t) - 3 * (t * t) + 1;
    }

    double h10(double t) {
        return t * t * t - 2 * (t * t) + t;
    }

    double h01(double t) {
        return -2 * (t * t * t) + 3 * (t * t);
    }

    double h11(double t) {
        return (t * t * t) - (t * t);
    }

    // float2 a0 (int i) {
    //     return controlPoints.at(i);
    // }
    
    // float2 a1 (int i) {
    //     return tangents.at(i);
    // }
    // float2 a2 (int i) {
    //     return ((controlPoints.at(i+1) - a0(i)) * 3.0f / (square(knots.at(i + 1) - knots.at(i)))) 
    //         - ((tangents.at(i + 1) + a1(i) * 2.0f) / (knots.at(i+1) - knots.at(i)));
    // }

    // float2 a3 (int i) {
    //     return ((a0(i) - controlPoints.at(i + 1)) * 2.0f / (cube(knots.at(i + 1) - knots.at(i))))
    //         + ((tangents.at(i+1) + a1(i)) / (square(knots.at(i + 1) - knots.at(i))));
    // }
    // float2 hermite(double t) {
    //     return h00(t) * controlPoints.at(0) + h10(t) * tangent 
    //         + h01(t) * controlPoints.at(controlPoints.size())
    //         + h11(t) * tangent;
    // }
    float2 hermite(double t, float2 p1, float2 t1, float2 p2, float2 t2) {
        return p1 * h00(t) + t1 * h10(t) + p2 * h01(t) + t2 * h11(t);
    }

    // float2 hermite(float t, float2 p1, float2 p2, float2 p3, float2 p4) {
     
    //     float t2 = t*t;
    //     float t3 = t*t*t;
    //     float2 v; // Interpolated point
     
    //     /* Hermite spline Calculation */
     


    //     v.x = ((-t3 + 2*t2-t)*(p1.x) + (3*t3-5*t2+2)*(p2.x) 
    //         + (-3*t3+4*t2+t)* (p3.x) + (t3-t2)*(p4.x))/2;
    //     v.y = ((-t3 + 2*t2-t)*(p1.y) + (3*t3-5*t2+2)*(p2.y) 
    //         + (-3*t3+4*t2+t)* (p3.y) + (t3-t2)*(p4.y))/2;

    //     return v;
    // }

    // float2 hermite (int i, double t) {
    //     // float2 r;
    //     // for (int i = 0; i < controlPoints.size() - 1; i++) {
    //         return a3(i) * cube(t - knots.at(i)) + a2(i) * square(t - knots.at(i)) 
    //             + a1(i) * (t - knots.at(i)) + a0(i);
    //     // }
    //     // return r;
    // }

    // void printInfo() {
    //     printf("tangents\n");
    //     for (int i = 0; i < controlPoints.size(); i++) {
    //         tangents[i].print();
    //     }
    //     for (int i = 0; i < controlPoints.size(); i++) {
    //         printf("knot: %f\n", knots[i]);
    //     }
    // }
public:

    void draw() {
        float2 p;
        float2 v[4];
        float t;
        int i;
        int size = controlPoints.size();
        for (i = 0; i < size - 1; i++) {
            glBegin(GL_LINE_STRIP);
            for(t = 0; t < 1; t += 0.001) {
                p = hermite(t, controlPoints.at(i), tangents.at(i),
                    controlPoints.at(i+1), tangents.at(i+1));
                glVertex2f(p.x,p.y);
            }
            glEnd();
        }
        glFlush();
    }

    float2 getPoint(float t) { return float2(0.0, 0.0);
        // float2 r;
        // for (int i = 0; i < controlPoints.size() - 1; i++) {
        //     // return a3(i) * cube(t - knots.at(i)) + a2(i) * square(t - knots.at(i)) 
        //     //     + a1(i) * (t - knots.at(i)) + a0(i);
        //     return controlPoints[i] * hermite(i, t);
        // }
        // return r;
    }

    void addControlPoint(float2 p) {
        float ti = controlPoints.size();
        controlPoints.push_back(p);

        // tangents
        float2 t = float2(0.2f, 0.2f);
        float2 newP = p + t;
        if (fabsf(newP.x) < 1 && fabsf(newP.y) < 1)
            tangents.push_back(p + t);
        else if (fabsf(newP.x) > 1)
            if (fabsf(newP.y) > 1)
                tangents.push_back(p - t);
            else
                tangents.push_back(p + (t * float2(-1, 1)));
        else
            tangents.push_back((t * float2(1, -1)) + p);
        // knots
        knots.push_back(ti);
        if (controlPoints.size() > 1) {
            for (float i = 0; i < controlPoints.size(); i++) {
                knots.at(i) = i / (controlPoints.size() - 1);
            }
        }
    }

    void drawControlPoints(){
        // draw points at control points
        float2 p, q;

        glBegin(GL_POINTS);
        for (int i = 0; i < controlPoints.size(); i++) {
            p = controlPoints.at(i);
            glVertex2d(p.x, p.y);
        }
        glEnd();

        glColor3d(1.0, 0.5, 0.5);
        glPointSize(5);
        glBegin(GL_POINTS);
        for (int i = 0; i < controlPoints.size(); i++) {
            q = tangents.at(i);
            glVertex2d(q.x, q.y);
            p = controlPoints.at(i);

            glBegin(GL_LINE_STRIP);
                glVertex2d(p.x, p.y);
                glVertex2d(q.x, q.y);
            glEnd();
        }
        glEnd();

    }

    void drawTracker(float t) {}

    char getType() {
        return 'u';
    }

    float2 getDerivative(float t){
    }

    Freeform * clone() const {
        return new HermiteCurve(*this);
    }

};

#endif