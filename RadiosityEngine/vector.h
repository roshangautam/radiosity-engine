//
//  vector.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//


#ifndef Radiosit_yEngine_vector_h
#define Radiosit_yEngine_vector_h

#include "general.h"

class Vector3;  // Forward reference

class Space3    // 3-D co-ordinates
{
protected:
    float x;    // X-axis co-ordinate
    float y;    // Y-axis co-ordinate
    float z;    // Z-axis co-ordinate
    
public:
    Space3() { };
    Space3( double xval, double yval, double zval )
    {
        x = (float) xval;
        y = (float) yval;
        z = (float) zval;
    }
    
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }
    
    void setX( double xval ) { x = (float) xval; }
    void setY( double yval ) { y = (float) yval; }
    void setZ( double zval ) { z = (float) zval; }
    
    void read() {
        cout << "Enter x:";
        cin >> x;
        cout << "Enter y:";
        cin >> y;
        cout << "Enter z:";
        cin >> z;
    }
    
    void print() {
        cout << "(" << x << "," << y << "," << z << ")";
    }
};

class Point3 : public Space3    // 3-D point
{
public:
    Point3() : Space3() { };
    
    Point3( double xval, double yval, double zval ) :
    Space3 ( xval, yval, zval )
    { };
    
    // Add vector v to point p
    friend Point3 operator+( Point3 p, Vector3 v );
    
    // Add point p to vector v
    friend Point3 operator+( Vector3 v, Point3 p );
       
    
    friend class Vector3;
};

class Vector3 : public Space3    // 3D vector
{

public:
    Vector3() : Space3() { };
    
    Vector3( double xval, double yval, double zval ) :
    Space3 ( xval, yval, zval )
    { };
    
    Vector3( Point3 &p ) : Space3()
    { x = p.x; y = p.y; z = p.z; }
    
    Vector3( Point3 &start, Point3 &end ) : Space3()
    {
        x = end.x - start.x;
        y = end.y - start.y;
        z = end.z - start.z;
    }
    
    //calculate and return the magnitude of this vector
    double getMagnitude()
    {
        return sqrtf(x * x + y * y + z * z);
    }
    
    // Assign scalar
    Vector3 &operator=( double s )
    {
        x = (float) s;
        y = (float) s;
        z = (float) s;
        
        return *this;
    }
    
    // Add/assign vector v
    Vector3 &operator+=( Vector3 &v )
    { x += v.x; y += v.y; z += v.z; return *this; }
    
    // Subtract/assign vector v
    Vector3 &operator-=( Vector3 &v )
    { x -= v.x; y -= v.y; z -= v.z; return *this; }
    
    // Multiply/assign by scalar s
    Vector3 &operator*=( double s )
    {
        x *= (float) s;
        y *= (float) s;
        z *= (float) s;
        
        return *this;
    }
    
    // Divide/assign by scalar s
    Vector3 &operator/=( double s )
    {
        x /= (float) s;
        y /= (float) s;
        z /= (float) s;
        
        return *this;
    }
    // Negation
    Vector3 operator-()
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = -x;
        temp.y = -y;
        temp.z = -z;
        
        return temp;
    }
    
    // Normalize
    Vector3 &norm()
    {
        double len = getMagnitude();
        
        if (len < MIN_VALUE)
            len = 1.0;
        
        x /= (float) len;
        y /= (float) len;
        z /= (float) len;
        
        return *this;
    }
    
    
    // Add vector v2 to vector v1
    friend Vector3 operator+( Vector3 v1, Vector3 v2 )
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = v1.x + v2.x;
        temp.y = v1.y + v2.y;
        temp.z = v1.z + v2.z;
        
        return temp;
    }
    
    // Subtract vector v2 from vector v1
    friend Vector3 operator-( Vector3 v1, Vector3 v2 )
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = v1.x - v2.x;
        temp.y = v1.y - v2.y;
        temp.z = v1.z - v2.z;
        
        return temp;
    }
    
    // Multiply vector v by scalar s
    friend Vector3 operator*( Vector3 v, double s )
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = v.x * (float) s;
        temp.y = v.y * (float) s;
        temp.z = v.z * (float) s;
        
        return temp;
    }
    
    // Multiply scalar s by vector v
    friend Vector3 operator*( double s, Vector3 v )
    { return v * s; }
    
    // Divide vector v by scalar s
    friend Vector3 operator/( Vector3 v, double s )
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = v.x / (float) s;
        temp.y = v.y / (float) s;
        temp.z = v.z / (float) s;
        
        return temp;
    }
    
    // Divide scalar s by vector v
    friend Vector3 operator/( double s, Vector3 v )
    { return v / s; }
    
    
    // Return dot product of vectors v1 and v2
    double dot( Vector3 &v)
    { return (x * v.x + y * v.y + z * v.z); }
    
    // Return cross product of vectors v1 and v2
    Vector3 cross( Vector3 &v)
    {
        Vector3 temp;     // Temporary 3-D vector
        
        temp.x = y * v.z - z * v.y;
        temp.y = z * v.x - x * v.z;
        temp.z = x * v.y - y * v.x;
        
        return temp;
    }
    
    static Vector3 findPointOnALine(Vector3 point1, Vector3 point2, int face) { //parametric equation
        float t; //parameter
        Vector3 projectedPoint;
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
        cout << " ––– ";
        projectedPoint.print();
        cout << "\n";
        return projectedPoint;
    }
};

// Add vector v to point p
Point3 operator+( Point3 p, Vector3 v )
{
    Point3 temp;  // Temporary 3-D point
    
    temp.x = p.x + (float) v.getX();
    temp.y = p.y + (float) v.getY();
    temp.z = p.z + (float) v.getZ();
    
    return temp;
}

// Add point p to vector v
Point3 operator+( Vector3 v, Point3 p )
{ return p + v; }
#endif
