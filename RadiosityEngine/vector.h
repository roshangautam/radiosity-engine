//
//  vector.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/7/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_vector_h
#define RadiosityEngine_vector_h

#include <math.h>

class Vector3D
{
private:
    float x, y, z;
public:
    //default constructor
    Vector3D(float X = 0, float Y = 0, float Z = 0)
    {
        x = X;
        y = Y;
        z = Z;
    }
    ~Vector3D(){};
    
    //calculate and return the magnitude of this vector
    float GetMagnitude()
    {
        return sqrtf(x * x + y * y + z * z);
    }
    
    //multiply this vector by a scalar
    Vector3D operator*(float num) const
    {
        return Vector3D(x * num, y * num, z * num);
    }
    
    //pass in a vector, pass in a scalar, return the product
    friend Vector3D operator*(float num, Vector3D const &vec)
    {
        return Vector3D(vec.x * num, vec.y * num, vec.z * num);
    }
    
    //add two vectors
    Vector3D operator+(const Vector3D &vec) const
    {
        return Vector3D(x + vec.x, y + vec.y, z + vec.z);
    }
    
    //subtract two vectors
    Vector3D operator-(const Vector3D &vec) const
    {
        return Vector3D(x - vec.x, y - vec.y, z - vec.z);
    }
    
    //normalize this vector
    void normalizeVector3D()
    {
        float magnitude = sqrtf(x * x + y * y + z * z);
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
    
    //calculate and return dot product
    float dotVector3D(const Vector3D &vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }
    
    //calculate and return cross product
    Vector3D crossVector3D(const Vector3D &vec) const
    {
        return Vector3D(y * vec.z - z * vec.y,
                        z * vec.x - x * vec.z,
                        x * vec.y - y * vec.x);
    }
};

#endif
