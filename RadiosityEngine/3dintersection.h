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
#include "vertex.h"
using namespace std;

enum Face {TOP_FACE, LEFT_FACE, RIGHT_FACE, FRONT_FACE, BACK_FACE, NA};

class ThreeDIntersection {
    
private:
    Vertex _vertex;
    Face _intersectingFace;
public:
    ThreeDIntersection() {
        _intersectingFace = NA;
    }
    
    ThreeDIntersection(Vertex Vertex, Face intersectingFace) {
        _vertex = Vertex;
        _intersectingFace = intersectingFace;
    }
    
    Vertex getVertex() {
        return _vertex;
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
                return "Error:Invalid Coordinates Provided.";
                break;
            default:
                break;
        }
    }
    
    void setVertex(Vertex Vertex) {
        _vertex = Vertex;
    }
    
    void setIntersectingFace(Face intersectingFace) {
        _intersectingFace = intersectingFace;
    }
    
    void intersect(Vertex *vertex) {
        float t;
        if (vertex->getY() >= 0) {
            if ((vertex->getY() < vertex->getZ() || vertex->getY() < (-(vertex->getZ()))) &&
                (vertex->getX() < vertex->getZ() || vertex->getX() < (-(vertex->getZ())))) {
                if(vertex->getX() >= 0) {
                    _vertex.setX(1.0);
                    t = _vertex.getX() / vertex->getX();
                    _vertex.setY(t * vertex->getY());
                    _vertex.setZ(t * vertex->getZ());
                    _intersectingFace = RIGHT_FACE;
                } else if(vertex->getX() < 0) {
                    _vertex.setX(-1.0);
                    t = _vertex.getX() / vertex->getX();
                    _vertex.setY(t * vertex->getY());
                    _vertex.setZ(t * vertex->getZ());
                    _intersectingFace = LEFT_FACE;
                }
                if(vertex->getY() >= 0) {
                     _vertex.setY(1.0);
                    t = _vertex.getY() / vertex->getY();
                    _vertex.setX(t * vertex->getX());
                    _vertex.setZ(t * vertex->getZ());
                    _intersectingFace = FRONT_FACE;
                } else if(vertex->getY() < 0) {
                     _vertex.setY(-1.0);
                    t = _vertex.getY() / vertex->getY();
                    _vertex.setX(t * vertex->getX());
                    _vertex.setZ(t * vertex->getZ());
                    _intersectingFace = BACK_FACE;
                }
            } else {
                _vertex.setZ(1.0);
                t = _vertex.getZ() / vertex->getZ();
                _vertex.setX(t * vertex->getX());
                _vertex.setY(t * vertex->getY());
                _intersectingFace = TOP_FACE;
            }
        }
    }
    
    void simplePrint() {
        cout << "(" << _vertex.getX() << "," << _vertex.getY() << "," << _vertex.getZ() << ")";
        cout << " on " << getHumanReadableIntersectingFace() << "\n\n";
    }
    
    void getPlaneNormal(Face face) {
        Vertex one,two,three, normal;
        if (face == TOP_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(-1,-1,1);
            cross()
        } else if(face == LEFT_FACE) {
            one.setCoordinates(-1,1,1);
            two.setCoordinates(-1,1,0);
            three.setCoordinates(-1,-1,1);
        } else if(face == RIGHT_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(1,1,0);
        } else if(face == FRONT_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(-1,-1,1);
        } else if(face == BACK_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(-1,-1,1);
        }
    }
};

#endif
