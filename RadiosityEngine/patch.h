//
//  patch.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/18/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_patch_h
#define RadiosityEngine_patch_h

#include "vector.h"

class Patch {
    Vector vertices[3];
    Vector center;
public:
    void setVertices(Vector vertex1, Vector vertex2, Vector vertex3) {
        vertices[0] = vertex1;
        vertices[1] = vertex2;
        vertices[2] = vertex3;
    }
    
    Vector* getVertices() {
        return vertices;
    }
    
    Vector getCenter() {
        return center;
    }
    
    void calcCenter()       // Calculate patch centroid
    {
        int i;            // Loop index
        int num_vert;     // Number of vertices
        Vector cv;       // Centroid vector
        
        num_vert = 3;
        
        
        
        // Determine patch centroid
        for (i = 0; i < num_vert; i++)
            cv = cv + Vector(vertices[i].getX(),vertices[i].getY(),vertices[i].getZ());
        
        cv /= (double) num_vert;
        
        // Convert centroid vector to 3-D point
        center.setX(cv.getX());
        center.setY(cv.getY());
        center.setZ(cv.getZ());
    }
};
#endif
