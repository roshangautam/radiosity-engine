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
};

#endif
