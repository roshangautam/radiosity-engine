//
//  intersection.h
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef LineIntersection_intersection_h
#define LineIntersection_intersection_h

#include <string>
#include "point.h"

using namespace std;

enum Side {LEFT, RIGHT, POSITIVE_TOP, NEGATIVE_TOP, NONE};

class Intersection {
    private:
        Point _point;
        Side _side;
    public:
        Intersection() {
            _side = NONE;
        }
    
        Intersection(Point point, Side side) {
            _point = point;
            _side = side;
        }
    
        Point getPoint() {
            return _point;
        }
    
        string getSide() {
            string side;
            switch (_side) {
                case 0:
                    side = "Left";
                    break;
                case 1:
                    side = "Right";
                    break;
                case 2:
                    side = "Positive Top";
                    break;
                case 3:
                    side = "Negative Top";
                    break;
                case 4:
                    side = "None";
                    break;
                default:
                    side = "Invalid";
                    break;
            }
            return side;
        }
    
        void setPoint(Point point) {
            _point = point;
        }
    
        void setSide(Side side) {
            _side = side;
        }
};

#endif
