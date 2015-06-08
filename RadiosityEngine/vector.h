//
//  vector.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//


#ifndef Radiosit_yEngine_vector_h
#define Radiosit_yEngine_vector_h

class Vector {
    float _x, _y, _z;
public:
    Vector() {
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
    }
    
    Vector(float abscissa, float ordinate, float applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    void setCoordinates(float abscissa, float ordinate, float applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    float getX() {
        return _x;
    }
    
    float getY() {
        return _y;
    }
    
    float getZ() {
        return _z;
    }
    
    void setX(float abscissa) {
        _x = abscissa;
    }
    
    void setY(float ordinate) {
        _y = ordinate;
    }
    
    void setZ(float applicate) {
        _z = applicate;
    }

    void read() {
        cout << "Enter x:";
        cin >> _x;
        cout << "Enter y:";
        cin >> _y;
        cout << "Enter z:";
        cin >> _z;
    }
    
    void print() {
        cout << "(" << _x << "," << _y << "," << _z << ")";
    }
};

#endif
