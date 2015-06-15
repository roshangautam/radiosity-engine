//
//  patch.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/14/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_patch_h
#define RadiosityEngine_patch_h

#include "vector.h"

class Patch {
    Point3  _center;
    Point3 _vertices[3];
    Vector3 _normal;
    float _area;
public:
    Patch(Point3 pvtx[3])
    {
        int index;
        for (index = 0; index < 3; index++)
            _vertices[index] = pvtx[index];
    }
    
    Vector3 getNormal() {
        return _normal;
    }
    
    Point3 getCenter() {
        return _center;
    }
    
    float getArea() {
        return _area;
    }
    
    Point3 *getVertex( int i ) { return &_vertices[i]; }
    
    void calcCenter()       // Calculate patch centroid
    {
        int i;            // Loop index
        int num_vert = 3;     // Number of vertices
        Vector3 cv;       // Centroid vector
        
        
        // Initialize centroid vector to origin
        cv = Vector3(0.0, 0.0, 0.0);
        
        // Determine patch centroid
        for (i = 0; i < num_vert; i++)
            cv = cv + Vector3(_vertices[i]);
        
        cv /= (double) num_vert;
        
    }
    
    void calcArea()       // Calculate element area
    {
        Vector3 temp;     // Temporary 3-D vector
        
        Vector3 va(_vertices[0], _vertices[1]);
        Vector3 vb(_vertices[0], _vertices[2]);
        
        temp = cross(va, vb);
        _area = (float) (temp.getMagnitude() / 2.0);
    }
};
#endif
