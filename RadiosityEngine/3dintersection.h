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

class ThreeDIntersection {
    
private:
    Vector _vector;
    HemiCubeFace _intersectingHemiCubeFace;
public:
    ThreeDIntersection() {
        _intersectingHemiCubeFace = TOP_FACE;
    }
    
    ThreeDIntersection(Vector Vector, HemiCubeFace intersectingHemiCubeFace) {
        _vector = Vector;
        _intersectingHemiCubeFace = intersectingHemiCubeFace;
    }
    
    Vector getVector() {
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
    
    void setVector(Vector Vector) {
        _vector = Vector;
    }
    
    void setIntersectingFace(HemiCubeFace intersectingHemiCubeFace) {
        _intersectingHemiCubeFace = intersectingHemiCubeFace;
    }
    
    void intersect(Vector *vertex) {  // parametric form approach
//        float t;
        if(DEBUG_MODE) {
        cout << "\nCalculating intersection of ";
        vertex->print();
        cout << " with the surface of hemicube\n";
        }
        if (vertex->getY() >= 0) {
            if ((vertex->getY() <= (-(vertex->getX())))  &&
                (-(vertex->getX())) > vertex->getZ() &&
                (-(vertex->getX())) > (-(vertex->getZ()))) { // LEFT_FACE
                if(DEBUG_MODE) {
                cout << "Intersects LEFT HemiCubeFace as Y:" << vertex->getY() << " < -(-X):" << (-(vertex->getX())) << " AND -(-X):" << (-(vertex->getX())) << " > Z:" << vertex->getZ() << " AND -(-X):" << (-(vertex->getX())) << " > -Z:" << (-(vertex->getZ())) << "\n";
                }
                _intersectingHemiCubeFace =  LEFT_FACE;
            } else if(vertex->getY() <= vertex->getX() &&
                      vertex->getX() > vertex->getZ() &&
                      vertex->getX() > (-(vertex->getZ()))) { //RIGHT_FACE
                if(DEBUG_MODE) {
                cout << "Intersects RIGHT HemiCubeFace as Y:" << vertex->getY() << " <= X:" << ((vertex->getX())) << " AND -(-X):" << (-(vertex->getX())) << " > Z:" << vertex->getZ() << " AND X:" << ((vertex->getX())) << " > -Z:" << (-(vertex->getZ())) << "\n";
                }
                _intersectingHemiCubeFace = RIGHT_FACE;
            } else if (vertex->getY() <= vertex->getZ() &&
                       vertex->getZ() >= vertex->getX() &&
                       vertex->getZ() >= (-(vertex->getX()))) { //FRONT_FACE
                if (DEBUG_MODE) {
                cout << "Intersects FRONT HemiCubeFace as Y:" << vertex->getY() << " <= Z:" << ((vertex->getZ())) << " AND Z:" << ((vertex->getZ())) << " >= X:" << vertex->getX() << " AND Z:" << ((vertex->getZ())) << " >= -(-X:" << (-(vertex->getX())) << "\n";
                }
                _intersectingHemiCubeFace = FRONT_FACE;
            } else if(vertex->getY() <= (-(vertex->getZ())) &&
                      (-(vertex->getX())) <= (-(vertex->getZ())) &&
                      vertex->getX() <= (-(vertex->getZ()))) { //BACK_FACE
                if (DEBUG_MODE) {
                cout << "Intersects BACK HemiCubeFace as Y:" << vertex->getY() << " <= -(-X):" << (-(vertex->getX())) << " AND -(-X):" << (-(vertex->getX())) << " <= -(-Z):" << (-(vertex->getZ())) << " AND X:" << ((vertex->getX())) << " <= -(-Z):" << (-(vertex->getZ())) << "\n";
                }

                _intersectingHemiCubeFace = BACK_FACE;
            } else if((vertex->getY() > (-(vertex->getZ())) &&
                       vertex->getY() > vertex->getZ()) &&
                      (vertex->getY() > vertex->getX() &&
                       vertex->getY() > (-(vertex->getX())))) { //TOP
                          
                if (DEBUG_MODE) {
                  cout << "Intersects TOP HemiCubeFace as Y:" << vertex->getY() << " > -(-Z):" << (-(vertex->getZ())) << " AND Y:" << ((vertex->getY())) << " <= Z:" << ((vertex->getZ())) << " AND Y:" << ((vertex->getY())) << " > X:" << ((vertex->getX())) << " AND Y:" << ((vertex->getY())) << " > -(-X):" << (-(vertex->getX())) <<"\n";
                }
                _intersectingHemiCubeFace = TOP_FACE;
            }
        }
        _vector = Vector::findPointOnALine(*vertex, Vector(0,0,0), _intersectingHemiCubeFace);
        if (DEBUG_MODE) {
            cout << "The point of intersection is:";
            print();
            cout << "\n";
        }
    }
    
    void print() {
        cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
    }
    
    void printWithFace() {
        cout << "\nCoordinates of intersection are:";
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
