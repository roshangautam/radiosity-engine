//
//  intersection.h
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef LineIntersection_intersection_h
#define LineIntersection_intersection_h

#include <assert.h>
#include "point.h"

enum Side {LEFT, RIGHT, TOP};
class Intersection {
    private:
        Point _point;
        Side _side;
    public:
        Intersection() {
            _side = TOP;
        }
    
        Intersection(Point point, Side side) {
            _point = point;
            _side = side;
        }
    
        Point getPoint() {
            return _point;
        }
    
        Side getSide() {
            return _side;
        }
    
        void setPoint(Point point) {
            _point = point;
        }
    
        void setSide(Side side) {
            _side = side;
        }
};

#endif
