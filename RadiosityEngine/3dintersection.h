//
//  ThreeDintersection.h
//  LineIntersection
//
//  Created by Roshan Gautam on 6/2/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef LineIntersection__ThreeDintersection_h
#define LineIntersection__ThreeDintersection_h

#include <string>
#include "3Dpoint.h"

using namespace std;

enum Plane {TOP, LEFT_PLANE, RIGHT_PLANE, FRONT, BACK, NA};

class ThreeDIntersection {
    
private:
    ThreeDPoint _ThreeDPoint;
    Plane _intersectingPlane;
public:
    ThreeDIntersection() {
        _intersectingPlane = NA;
    }
    
    ThreeDIntersection(ThreeDPoint ThreeDPoint, Plane intersectionPlane) {
        _ThreeDPoint = ThreeDPoint;
        _intersectingPlane = intersectionPlane;
    }
    
    ThreeDPoint getThreeDPoint() {
        return _ThreeDPoint;
    }
    
    Plane getIntersectingPlane() {
        return _intersectingPlane;
    }
    
    string getHumanReadableIntersectingPlane() {
        switch (_intersectingPlane) {
            case 0:
                return "Top";
                break;
            case 1:
                return "Left";
                break;
            case 2:
                return "Right";
                break;
            case 3:
                return "Front";
                break;
            case 4:
                return "Back";
                break;
            case 5:
                return "None";
                break;
            default:
                break;
        }
    }
    
    void setThreeDPoint(ThreeDPoint ThreeDPoint) {
        _ThreeDPoint = ThreeDPoint;
    }
    
    void setIntersectingPlane(Plane intersectingPlane) {
        _intersectingPlane = intersectingPlane;
    }
};
#endif
