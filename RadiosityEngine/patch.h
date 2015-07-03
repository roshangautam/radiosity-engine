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

class Patch;

class HemiCell {
    float tmin;
    Patch *owner;
    int patchIndex;
public:
    HemiCell() {
        tmin = 0.0;
        owner = NULL;
    }
    
    void setTMin(float t) {
        tmin = t;
    }
    
    void setOwner(Patch *patch) {
        owner = patch;
    }
    
    void setPatchIndex(int index) {
        patchIndex = index;
    }
    
    float getTMin() {
        return tmin;
    }
    
    Patch *getOwner() {
        return owner;
    }
    
    int getPatchIndex() {
        return patchIndex;
    }
};

class Patch {
    Vector vertices[3];
    Vector vectors[3];
    Vector center;
    float area;
    HemiCell cellData[192];

public:
    
    Patch() {
        area = -1.0;
    }
    
    void setVertices(Vector vertex1, Vector vertex2, Vector vertex3) {
        vertices[0] = vertex1;
        vertices[1] = vertex2;
        vertices[2] = vertex3;
    }
    
    Vector* getVertices() {
        return vertices;
    }

    Vector* getVectors() {
        return vectors;
    }
    
    Vector getCenter() {
        return center;
    }
    
    float getArea() {
        return area;
    }

    void setCellData(int cellIndex, float tmin, Patch *owner, int patchIndex) {
        cellData[cellIndex].setTMin(tmin);
        cellData[cellIndex].setOwner(owner);
        cellData[cellIndex].setPatchIndex(patchIndex);
    }
    
    HemiCell getCellData(int index) {
        return cellData[index];
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
    
    void calcArea() {
        Vector temp;     // Temporary 3-D vector
        
        Vector va = vertices[1] - vertices[0];
        Vector vb = vertices[2] - vertices[0];
        
        temp = va.cross(vb);
        area = (float) (temp.getMagnitude() / 2.0);
        
    }
    
    void calcVectors() {
        vectors[0] = (vertices[1] - vertices[0]);
        vectors[0].normalizeVector();
        vectors[1] = (vertices[1] - vertices[0]).cross((vertices[2] - vertices[1]));
        vectors[2] = vectors[0].cross(vectors[1]);
    }
    
    void printPatch() {
        for (int i =0 ; i < 3; i++) {
            vertices[i].print();
            cout << " ––– ";
        }
    }
};
#endif
