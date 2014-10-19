//
//  
//  Graphics Class B
//
//  Tiffany
//


#define _USE_MATH_DEFINES


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// Download glut from: http://www.opengl.org/resources/libraries/glut/
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>

/* custom files */

#include "float2.h"
#include "freeform.h"
#include "beziercurve.h"
#include "lagrangecurve.h"
#include "polyline.h"
#include "catmullrom.h"
#include "hermite.h"
// #include "catmullclark.h"

// g++ curveEditor.cpp -framework OpenGL -framework GLUT && ./a.out

std::vector<Freeform *> curves;
float trackPosition = 0;
bool polyline = false;
bool lagrng = false;
bool bezier = false;
bool rom = false;
bool hermite = false;
bool adding = false;
bool deleting = false;
int selected;
int cpMoving; // must be used in conjunction with dragging

BezierCurve bc;
LagrangeCurve lc;
bool dragging = false;
bool originalBool = false;
bool dragCurve = false;


void onKeyboardDown(unsigned char key, int x, int y) {
    if (key == 'p') { // polyline
        if (!polyline) {
            curves.push_back(new Polyline());
            selected = curves.size() - 1;
        }
        polyline = true;
    } else if (key == 'l') { // lagrange
        if (!lagrng) {
            curves.push_back(new LagrangeCurve());
            selected = curves.size() - 1;
        }
        lagrng = true;
    } else if (key == 'b') { // bezier
        if (!bezier) {
            curves.push_back(new BezierCurve());
            selected = curves.size() - 1;
        }
        bezier = true;
    } else if (key == 'r') { // bezier
        if (!rom) {
            curves.push_back(new CatmullRomCurve());
            selected = curves.size() - 1;
        }
        rom = true;
    } else if (key == 'u') { // bezier
        if (!hermite) {
            curves.push_back(new HermiteCurve());
            selected = curves.size() - 1;
        }
        hermite = true;
    } else if (key == 'a') { // add control points
        adding = true;
    } else if (key == 'd') { // delete control points
        deleting = true;
    } else if (key == ' ' && curves.size() > 0 && selected >= 0) { // cycle through curves
        selected = (selected + 1) % curves.size();
    }

    // glutPostRedisplay();
}

void onKeyboardUp(unsigned char key, int x, int y) {
    if (key == 'p') {
        polyline = false;
        if (curves.at(curves.size() - 1)->getCPSize() < 2) {
            curves.pop_back();
        }
    } else if (key == 'l') {
        lagrng = false;
        if (curves.at(curves.size() - 1)->getCPSize() < 2) {
            curves.pop_back();
        }
    } else if (key == 'b') {
        bezier = false;
        if (curves.at(curves.size() - 1)->getCPSize() < 2) {
            curves.pop_back();
        }
    } else if (key == 'r') {
        rom = false;
        if (curves.at(curves.size() - 1)->getCPSize() < 4) {
            curves.pop_back();
        }
    } else if (key == 'u') {
        hermite = false;
        if (curves.at(curves.size() - 1)->getCPSize() < 2) {
            curves.pop_back();
        }
    } else if (key == 'a') {
        adding = false;
    } else if (key == 'd') {
        deleting = false;
    }

    glutPostRedisplay(); // reload image
}


float mouseX;
float mouseY;

// check if mouse click is on a control point
float2 onControlPoint(float x, float y) {  // need to convert the controlpoint to window coords
    float xPix10 = 20.0/glutGet(GLUT_WINDOW_WIDTH);
    float yPix10 = 20.0/glutGet(GLUT_WINDOW_HEIGHT);
    float xPix5 = xPix10/2.0;
    float yPix5 = yPix10/2.0;
    float offsetX = xPix5;
    float offsetY = yPix5;

    for (int i = 0; i < curves.size(); i++) {
        Freeform * obj = curves.at(i);
        if (i == selected) {
            offsetX = xPix10;
            offsetY = yPix10;
        } else {
            offsetX = xPix5;
            offsetY = yPix5;
        }
        for (int j = 0; j < obj->getCPSize(); j++) {
            float2 f = obj->getControlPoint(j);
            if (fabsf(f.x - x) <= offsetX && fabsf(f.y - y) <= offsetY) {
                dragging = true;
                return float2(i, j);
            }
        }
    }
    // not on a control point
    return float2(-1, -1);
}

float2 onTangentPoint(float x, float y) {
    
}

// check if the click is on a curve
int onCurve(float x, float y) {
    float xPix = 4.0/glutGet(GLUT_WINDOW_WIDTH);
    float yPix = 4.0/glutGet(GLUT_WINDOW_HEIGHT);
    for (int i = 0; i < curves.size(); i++) {
        Freeform * obj = curves.at(i);
        if (obj->getType() == 'p') { // special case for when polyline
            for (int j = 0; j < obj->getCPSize() - 1; j++) {
                float2 cpA = obj->getControlPoint(j);
                float2 cpB = obj->getControlPoint(j + 1);
                bool condition;
                if (cpA.x != cpB.x) {
                    condition = ((cpA.x <= x && x <= cpB.x) 
                        || (cpB.x <= x && x <= cpA.x));
                } else {
                    condition = ((cpA.y <= y && y <= cpB.y) 
                        || (cpB.y <= y && y <= cpA.y));
                }
                float crossProduct = fabsf((cpB.x - cpA.x) * (y - cpA.y) 
                    - (x - cpA.x) * (cpB.y - cpA.y));
                if ((crossProduct <= xPix || crossProduct <= yPix)  && condition) {
                    dragCurve = true;
                    return i;
                }
            }
        } else {
            for (float t = 0; t < 1; t += 0.0001) {
                float2 p = obj->getPoint(t);
                if (fabsf(p.x - x) <= xPix && fabsf(p.y - y) <= yPix) {
                    dragCurve = true;
                    return i;
                }
            }
        }
    }
    return -1;
}

// deletes the curve at index i
void deleteCurve(int i) {
    curves.erase(curves.begin() + i);
}

void onMouse(int button, int state, int x, int y) {

    int viewportRect[4];
    glGetIntegerv(GL_VIEWPORT, viewportRect);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x * 2.0 / viewportRect[2] - 1.0;
        mouseY = -y * 2.0 / viewportRect[3] + 1.0;
        if (polyline || lagrng || bezier || rom || hermite) {
            curves.at(curves.size() - 1)->addControlPoint(float2(mouseX, mouseY));
            if (hermite) {
                curves.at(curves.size() - 1)->printCP();
            }
        } else if (adding) {
            if (selected >= 0) {
                curves.at(selected)->addControlPoint(float2(mouseX, mouseY));
            }
        } else {
            float2 vals = onControlPoint(mouseX, mouseY);
            int curveIndex = onCurve(mouseX, mouseY);
            if (vals.x < 0) {
                // will be -1 if not on curve as well. otherwise will have index
                selected = curveIndex;
            } else {
                selected = vals.x;
                cpMoving = vals.y;
                if (deleting && selected >= 0) {
                    curves.at(selected)->deleteControlPoint(cpMoving);
                    if (cpMoving == curves.at(selected)->getCPSize()) {
                        cpMoving--;
                    }
                    if (curves.at(selected)->getCPSize() < 1) {
                        deleteCurve(selected);
                        selected = -1;
                    }
                }
            }
        }
    } else {
        dragCurve = false;
        dragging = false;
        originalBool = false;
    }
    glutPostRedisplay();
}


float2 original;
Freeform * originalCurve;

void onMotion(int x, int y) {
    // get the original location in on mouse and move object in onmotion
    // depending on how far the mouse is from the original location if dragging == true
    int viewportRect[4];
    glGetIntegerv(GL_VIEWPORT, viewportRect);
    float2 dragDistance = float2(x * 2.0 / viewportRect[2] - 1.0 - mouseX, -y * 2.0 / viewportRect[3] + 1.0 - mouseY);
    if (dragging && cpMoving >= 0 && !deleting) {
        if (!originalBool) {
            original = curves.at(selected)->getControlPoint(cpMoving);
            originalBool = true;
        }
        curves.at(selected)->moveControlPoint(cpMoving, original + dragDistance);
    } else if (dragCurve) {
        if (!originalBool) {
            originalCurve = curves.at(selected)->clone();
            originalBool = true;
        }
        for (int i = 0; i < curves.at(selected)->getCPSize(); i++) {
            curves.at(selected)->moveControlPoint(i, 
                originalCurve->getControlPoint(i) + dragDistance);
        }
    }
    glutPostRedisplay();
}

void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);           // clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen
    glColor3d(0.0, 1.0, 0.0);
    glLineWidth(3);

    for (int i = 0; i < curves.size(); i++) {
        char type = curves.at(i)->getType();
        if (selected == i) {
            glColor3d(0.0, 1.0, 1.0);
            glPointSize(20);
        } else if (type == 'b') {
            glColor3d(0.0, 1.0, 0.0);
            glPointSize(10);
        } else if (type == 'l') {
            glColor3d(1.0, 1.0, 0.0);
            glPointSize(10);
        } else if (type == 'p') {
            glColor3d(1.0, 0.0, 0.0);
            glPointSize(10);
        } else if (type == 'c') {
            glColor3d(1.0, 0.0, 1.0);
            glPointSize(10);
        } else if (type == 'u') {
            glColor3d(1.0, 0.5, 0.5);
            glPointSize(10);
        }

        curves.at(i)->drawControlPoints();
        curves.at(i)->draw();

        //curves.at(i)->drawTracker(trackPosition);
    }
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    
    glutInit(&argc, argv);// GLUT initialization
    glutInitWindowSize(640, 640);// Initial resolution of the MsWindows Window is 600x600 pixels
    glutInitWindowPosition(100, 100);            // Initial location of the MsWindows window
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);    // Image = 8 bit R,G,B + double buffer + depth buffer
    
    srand (time(NULL));

    glutCreateWindow("freeform");           // Window is born
    glutKeyboardUpFunc(onKeyboardUp);
    glutKeyboardFunc(onKeyboardDown);
    glutMouseFunc(onMouse);
    glutDisplayFunc(onDisplay);
    glutMotionFunc(onMotion);

    // curves.push_back(new CatmullClarkCurve());
    // for (int i = 0; i < 4; i++) {
    //     curves.at(0)->addControlPoint(float2().random());
    // }

    glutMainLoop();                             // Event loop
    
    return 0;
}

