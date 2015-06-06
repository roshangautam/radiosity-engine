//
//  point.h
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef LineIntersection_point_h
#define LineIntersection_point_h

#include <math.h>

using namespace std;

class Point {
    private:
        float _x, _y;
        
    public:
        Point() {
            _x = 0.0;
            _y = 0.0;
        }
        
        Point(float abscissa, float ordinate) {
            _x = abscissa;
            _y = ordinate;
        }
        
        float getX() {
            return _x;
        }
        
        float getAbsoluteX() {
            return fabs(_x);
        }
        
        float getY() {
            return _y;
        }
        
        void setX(float abscissa) {
            _x = abscissa;
        }
        
        void setY(float ordinate) {
            _y = ordinate;
        }
    
        void readInput() {
            cout << "Enter x:";
            cin >> _x;
            cout << "Enter y:";
            cin >> _y;
        }
};

#endif
