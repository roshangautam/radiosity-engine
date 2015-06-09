//
//  3dintersection.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine__dintersection_h
#define RadiosityEngine__dintersection_h

#include <string>
#include <math.h>

#include "vector.h"

using namespace std;

enum Face {TOP_FACE, LEFT_FACE, RIGHT_FACE, FRONT_FACE, BACK_FACE, NA};

class ThreeDIntersection {
    
private:
    Vector _vector;
    Face _intersectingFace;
public:
    ThreeDIntersection() {
        _intersectingFace = NA;
    }
    
    ThreeDIntersection(Vector Vector, Face intersectingFace) {
        _vector = Vector;
        _intersectingFace = intersectingFace;
    }
    
    Vector getVector() {
        return _vector;
    }
    
    Face getIntersectingFace() {
        return _intersectingFace;
    }
    
    string getHumanReadableIntersectingFace() {
        switch (_intersectingFace) {
            case 0:
                return "TOP";
                break;
            case 1:
                return "LEFT";
                break;
            case 2:
                return "RIGHT";
                break;
            case 3:
                return "FRONT";
                break;
            case 4:
                return "BACK";
                break;
            case 5:
                return "ERROR: Invalid Coordinates Provided. Please try again";
                break;
            default:
                break;
        }
    }
    
    void setVector(Vector Vector) {
        _vector = Vector;
    }
    
    void setIntersectingFace(Face intersectingFace) {
        _intersectingFace = intersectingFace;
    }
    
    void intersect(Vector *vertex) {  // parametric form approach
        float t;
        if (vertex->getY() >= 0) {
            if ((vertex->getY() <= (-(vertex->getX())))  &&
                (-(vertex->getX())) > vertex->getZ() &&
                (-(vertex->getX())) > (-(vertex->getZ()))) { // LEFT_FACE
                _intersectingFace =  LEFT_FACE;
                _vector.setX(-1.0);
                t = _vector.getX() / vertex->getX();
                _vector.setY(t * vertex->getY());
                _vector.setZ(t * vertex->getZ());
            } else if(vertex->getY() <= vertex->getX() &&
                      vertex->getX() > vertex->getZ() &&
                      vertex->getX() > (-(vertex->getZ()))) { //RIGHT_FACE
                _intersectingFace = RIGHT_FACE;
                _vector.setX(1.0);
                t = _vector.getX() / vertex->getX();
                _vector.setY(t * vertex->getY());
                _vector.setZ(t * vertex->getZ());
            } else if (vertex->getY() <= vertex->getZ() &&
                       vertex->getZ() >= vertex->getX() &&
                       vertex->getZ() >= (-(vertex->getX()))) { //FRONT_FACE
                _intersectingFace = FRONT_FACE;
                _vector.setZ(1.0);
                t = _vector.getZ() / vertex->getZ();
                _vector.setX(t * vertex->getX());
                _vector.setY(t * vertex->getY());
            } else if(vertex->getY() <= (-(vertex->getZ())) &&
                      (-(vertex->getX())) <= (-(vertex->getZ())) &&
                      vertex->getX() <= (-(vertex->getZ()))) { //BACK_FACE
                _intersectingFace = BACK_FACE;
                _vector.setZ(-1.0);
                t = _vector.getZ() / vertex->getZ();
                _vector.setX(t * vertex->getX());
                _vector.setY(t * vertex->getY());
            } else if((vertex->getY() > (-(vertex->getZ())) &&
                       vertex->getY() > vertex->getZ()) &&
                      (vertex->getY() > vertex->getX() &&
                       vertex->getY() > (-(vertex->getX())))) { //TOP
                _intersectingFace = TOP_FACE;
                _vector.setY(1.0);
                t = _vector.getY() / vertex->getY();
                _vector.setX(t * vertex->getX());
                _vector.setZ(t * vertex->getZ());
            }
        }
    }
    
    void print() {
        cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
    }
    
    void printWithFace() {
        if (_intersectingFace == NA) {
            cout << "\n" << getHumanReadableIntersectingFace() << "\n\n";
        } else {
            cout << "\nCoordinates of intersection are:";
            cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
            cout << " on " << getHumanReadableIntersectingFace() << "\n\n";
        }
    }
    
    //    function SameSide(p1,p2, a,b)
    //    cp1 = CrossProduct(b-a, p1-a)
    //    cp2 = CrossProduct(b-a, p2-a)
    //    if DotProduct(cp1, cp2) >= 0 then return true
    //        else return false
    //
    //    function PointInTriangle(p, a,b,c)
    //    if SameSide(p,a, b,c) and SameSide(p,b, a,c)
    //        and SameSide(p,c, a,b) then return true
    //        else return false
};

#endif
