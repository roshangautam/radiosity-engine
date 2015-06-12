//
//  vector.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//


#ifndef Radiosit_yEngine_vector_h
#define Radiosit_yEngine_vector_h

#include "hemicube.h"

class Vector {
    double _x, _y, _z;
public:
    Vector() {
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
    }
    
    Vector(double abscissa, double ordinate, double applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    void setCoordinates(double abscissa, double ordinate, double applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    double getX() {
        return _x;
    }
    
    double getY() {
        return _y;
    }
    
    double getZ() {
        return _z;
    }
    
    void setX(double abscissa) {
        _x = abscissa;
    }
    
    void setY(double ordinate) {
        _y = ordinate;
    }
    
    void setZ(double applicate) {
        _z = applicate;
    }

    bool read() {
        cout << "Enter x:";
        cin >> _x;
        cout << "Enter y:";
        cin >> _y;
        cout << "Enter z:";
        cin >> _z;
        if ((_x < 0 && _x > -1) ||
            (_y < 0 && _y > -1) ||
            (_z < 0 && _z > -1) ) {
            return false;
        }
        return true;
    }
    
    void print() {
        cout << "(" << _x << "," << _y << "," << _z << ")";
    }
    
    static Vector findPointOnALine(Vector point1, Vector point2, HemiCubeFace face) { //parametric equation
        float t; //parameter
        Vector projectedPoint;
        if (face == TOP_FACE) {
            projectedPoint.setY(1.0);
            t = (projectedPoint.getY() - point1.getY()) / (point2.getY() - point1.getY());
            projectedPoint.setX(((1 - t) * point1.getX()) + (t * point2.getX()));
            projectedPoint.setZ(((1 - t) * point1.getZ()) + (t * point2.getZ()));
        } else if(face == RIGHT_FACE) {
            projectedPoint.setX(1.0);
            t = (projectedPoint.getX() - point1.getX()) / (point2.getX() - point1.getX());
            projectedPoint.setY(((1 - t) * point1.getY()) + (t * point2.getY()));
            projectedPoint.setZ(((1 - t) * point1.getZ()) + (t * point2.getZ()));
        } else if(face == LEFT_FACE) {
            projectedPoint.setX(-1.0);
            t = (projectedPoint.getX() - point1.getX()) / (point2.getX() - point1.getX());
            projectedPoint.setY(((1 - t) * point1.getY()) + (t * point2.getY()));
            projectedPoint.setZ(((1 - t) * point1.getZ()) + (t * point2.getZ()));
        } else if(face == FRONT_FACE) {
            projectedPoint.setZ(1.0);
            t = (projectedPoint.getZ() - point1.getZ()) / (point2.getZ() - point1.getZ());
            projectedPoint.setX(((1 - t) * point1.getX()) + (t * point2.getX()));
            projectedPoint.setY(((1 - t) * point1.getY()) + (t * point2.getY()));
        } else if(face == BACK_FACE) {
            projectedPoint.setZ(-1.0);
            t = (projectedPoint.getZ() - point1.getZ()) / (point2.getZ() - point1.getZ());
            projectedPoint.setX(((1 - t) * point1.getX()) + (t * point2.getX()));
            projectedPoint.setY(((1 - t) * point1.getY()) + (t * point2.getY()));
        }
        cout << "t:" << t << " –––– ";
        point1.print();
        cout << " ––– ";
        point2.print();
        cout << "\n";
        return projectedPoint;
    }
    
    //calculate and return the magnitude of this vector
    double getMagnitude()
    {
        return sqrtf(_x * _x + _y * _y + _z * _z);
    }
    
    //multiply this vector by a scalar
    Vector operator*(double num) const
    {
        return Vector(_x * num, _y * num, _z * num);
    }
    
    //pass in a vector, pass in a scalar, return the product
    friend Vector operator*(double num, Vector const &vec)
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
    
    //normalize this vector
    void normalizeVector()
    {
        double magnitude = getMagnitude();
        _x /= magnitude;
        _y /= magnitude;
        _z /= magnitude;
    }
    
    //calculate and return dot product
    double dot(Vector vec) {
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
