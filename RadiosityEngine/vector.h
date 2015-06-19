//
//  vector.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//


#ifndef Radiosit_yEngine_vector_h
#define Radiosit_yEngine_vector_h

#include <iomanip>

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

    bool read() {
        cout << "Enter x:";
        cin >> _x;
        cout << "Enter y:";
        cin >> _y;
        cout << "Enter z:";
        cin >> _z;
        if (_x == 0 &&
            _y == 0 &&
            _z == 0) {
            return false;
        }
        return true;
    }
    
    void print() {
        cout << "(" << setprecision(2) << _x << "," << setprecision(2) << _y << "," << setprecision(2) << _z << ")";
    }
    
    //calculate and return the magnitude of this vector
    float getMagnitude()
    {
        return sqrtf(_x * _x + _y * _y + _z * _z);
    }
    
    //multiply this vector by a scalar
    Vector operator*(float num) const
    {
        return Vector(_x * num, _y * num, _z * num);
    }
    
    //pass in a vector, pass in a scalar, return the product
    friend Vector operator*(float num, Vector const &vec)
    {
        return Vector(vec._x * num, vec._y * num, vec._z * num);
    }
    
    //add two vectors
    Vector operator+(const Vector &vec) const
    {
        return Vector(_x + vec._x, _y + vec._y, _z + vec._z);
    }
    
    //subtract two vectors
    Vector operator-(const Vector &vec) const
    {
        return Vector(_x - vec._x, _y - vec._y, _z - vec._z);
    }
    
    // Divide/assign by scalar s
    Vector &operator/=( double s )
    {
        _x /= (float) s;
        _y /= (float) s;
        _z /= (float) s;
        
        return *this;
    }
    
    //normalize this vector
    void normalizeVector()
    {
        float magnitude = getMagnitude();
        _x /= magnitude;
        _y /= magnitude;
        _z /= magnitude;
    }
    
    //calculate and return dot product
    float dot(Vector vec) {
        return _x * vec.getX() + _y * vec.getY() + _z * vec.getZ();
    }
    
    //calculate and return cross product
    Vector cross(Vector vec) {
        return Vector(_y * vec.getZ() - _z * vec.getY(),
                      _z * vec.getX() - _x * vec.getZ(),
                      _x * vec.getY() - _y * vec.getX());
    }
    
    Vector unit() {
        return Vector(_x / sqrt(dot(Vector(_x,_y,_z))),
                      _y / sqrt(dot(Vector(_x,_y,_z))),
                      _z / sqrt(dot(Vector(_x,_y,_z))));
    }
};

#endif
