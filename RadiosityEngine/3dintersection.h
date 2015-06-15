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

#include "hemicube.h"

using namespace std;

class ThreeDIntersection {
    
private:
    Vector3 _vector;
    HemiCubeFace _intersectingHemiCubeFace;
public:
    ThreeDIntersection() {
        _intersectingHemiCubeFace = TOP_FACE;
    }
    
    ThreeDIntersection(Vector3 Vector3, HemiCubeFace intersectingHemiCubeFace) {
        _vector = Vector3;
        _intersectingHemiCubeFace = intersectingHemiCubeFace;
    }
    
    Vector3 getVector() {
        return _vector;
    }
    
    HemiCubeFace getIntersectingFace() {
        return _intersectingHemiCubeFace;
    }
    
    string getHumanReadableIntersectingFace() {
        switch (_intersectingHemiCubeFace) {
            case TOP_FACE:
                return "TOP";
                break;
            case LEFT_FACE:
                return "LEFT";
                break;
            case RIGHT_FACE:
                return "RIGHT";
                break;
            case FRONT_FACE:
                return "FRONT";
                break;
            case BACK_FACE:
                return "BACK";
                break;
            default:
                return "ERROR: Invalid Coordinates Provided. Please try again";
                break;
        }
    }
    
    void setVector(Vector3 Vector3) {
        _vector = Vector3;
    }
    
    void setIntersectingFace(HemiCubeFace intersectingHemiCubeFace) {
        _intersectingHemiCubeFace = intersectingHemiCubeFace;
    }
    
    void intersect(Vector3 *vertex) {  // parametric form approach
        if(DEBUG_MODE) {
            cout << "\nCalculating intersection of ";
            vertex->print();
            cout << " with the surface of hemicube\n";
        }
        if (vertex->getY() > 0) {
            if (vertex->getY() < vertex->getZ() &&
                vertex->getX() < vertex->getZ() &&
                (-(vertex->getX())) < vertex->getZ()) {
                _intersectingHemiCubeFace = FRONT_FACE;
            } else if(vertex->getY() < (-(vertex->getZ())) &&
                      vertex->getX() < (-(vertex->getZ())) &&
                      (-(vertex->getX())) < (-(vertex->getZ()))) {
                _intersectingHemiCubeFace = BACK_FACE;
            } else if(vertex->getX() > vertex->getY() &&
                      vertex->getX() > vertex->getZ() &&
                      vertex->getX() > (-(vertex->getZ()))) {
                _intersectingHemiCubeFace = RIGHT_FACE;
            } else if((-(vertex->getX())) >  vertex->getY() &&
                      (-(vertex->getX())) > vertex->getZ() &&
                      (-(vertex->getX())) > (-(vertex->getZ()))) {
                _intersectingHemiCubeFace = LEFT_FACE;
            } else if(vertex->getY() > (-(vertex->getZ())) &&
                      vertex->getY() > vertex->getZ() &&
                      vertex->getY() > vertex->getX() &&
                      vertex->getY() > (-(vertex->getX()))) {
                _intersectingHemiCubeFace = TOP_FACE;
            }
            _vector = Vector3::findPointOnALine(Vector3(0,0,0), *vertex, _intersectingHemiCubeFace);
        }
    }
    
    void print() {
        cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
    }
    
    void printWithFace() {
        cout << "Coordinates of intersection are:";
        cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
        cout << " on " << getHumanReadableIntersectingFace() << "\n";
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
