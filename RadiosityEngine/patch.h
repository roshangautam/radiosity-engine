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

#define QUAD_FLAG   0x01    // Quadrilateral flag

class Patch3; //forward declaration

class ElemList              // Element list
{
private:
    class Element3 *pelem;  // Element pointer
    ElemList *pnext;        // Next element list pointer
    
public:
    ElemList( Element3 *pe, ElemList *pel )
    { pelem = pe; pnext = pel; }
    
    Element3 *getElemPtr() { return pelem; }
    ElemList *getNext() { return pnext; }
};

class Vertex3           // 3-D vertex
{
private:
    Point3 posn;        // Vertex co-ordinates
    Vector3 normal;     // Vertex normal
    ElemList *pelhd;    // Element list head pointer
    Vertex3 *pnext;     // Next vertex pointer
    
public:
    Vertex3( Point3 &coord )
    {
        posn = coord;
        normal = 0.0;
        pelhd = NULL;
        pnext = NULL;
    }
    
    ~Vertex3()
    {
        ElemList *pel = pelhd;
        ElemList *pelnext;
        
        // Delete element list
        while (pel != NULL)
        {
            pelnext = pel->getNext();
            delete pel;
            pel = pelnext;
        }
    }
    
    ElemList *getElemListPtr() { return pelhd; }
    Point3 &getPosn() { return posn; }
    Point3 *getPosnPtr() { return &posn; }
    Vector3 &getNormal() { return normal; }
    Vertex3 *getNext() { return pnext; }
    void calcNormal();
    void setElemListPtr( ElemList *ppl) { pelhd = ppl; }
    void setNext( Vertex3 *pn ) { pnext = pn; }
    void setPosn( Point3 &p ) { posn = p; }
};

class Element3                  // 3-D element
{
protected:
    unsigned char flags;                 // Flags bitmap
    float area;                 // Element area
    Patch3 *ppatch;             // Parent patch pointer
    Vector3 normal;             // Normal vector
    Vertex3 *pvertex[4];        // Vertex pointer array
    Element3 *pnext;            // Next element pointer
    
public:
    Element3( Vertex3 *pvtx[4], Patch3 *pp )
    {
        int index;        // Array index
        
        ppatch = pp;
        area = (float) 0.0;
        flags = (unsigned char) 0;
        pnext = NULL;
        
        for (index = 0; index < 4; index++)
            pvertex[index] = pvtx[index];
    }
    
    bool IsQuad() { return (flags & QUAD_FLAG); }
    double getArea() { return area; }
    int getNumVert()
    { return (flags & QUAD_FLAG) ? 4 : 3; }
    Element3 *getNext() { return pnext; }
    Patch3 *getParentPtr() { return ppatch; }
    Vector3 &getNormal() { return normal; }
    Vertex3 *getVertexPtr( int i ) { return pvertex[i]; }
    void calcArea()       // Calculate element area
    {
        Vector3 temp;     // Temporary 3-D vector
        
        Vector3 va(pvertex[0]->getPosn(), pvertex[1]->getPosn());
        Vector3 vb(pvertex[0]->getPosn(), pvertex[2]->getPosn());
        
        temp = cross(va, vb);
        area = (float) (temp.getMagnitude() / 2.0);
        
        if (IsQuad() == true)
        {
            Vector3 vc(pvertex[3]->getPosn(),
                       pvertex[0]->getPosn());
            
            temp = cross(vb, vc);
            area += (float) (temp.getMagnitude() / 2.0);
        }
    }
    void calcNormal()     // Calculate element normal
    {
        Vector3 va(pvertex[0]->getPosn(), pvertex[1]->getPosn());
        Vector3 vb(pvertex[0]->getPosn(), pvertex[2]->getPosn());
        
        normal = cross(va, vb);
        normal.norm();
    }
    void setNext( Element3 *pn ) { pnext = pn; }
    void setQuad() { flags |= QUAD_FLAG; }
};

class Patch3 : public Element3 {
    Point3  center;
    Element3 *pelhd;    // Element list head ptr
public:
    Patch3( Vertex3 *pvtx[4]) :
    Element3( pvtx, NULL )
    {
        pelhd = NULL;
    }

    ~Patch3()
    {
        Element3 *pe = pelhd;
        Element3 *penext;
        
        while (pe != NULL)        // Delete elements
        {
            penext = pe->getNext();
            delete pe;
            pe = penext;
        }
    }

    Element3 *getElementPtr() { return pelhd; }
    Patch3 *getNext() { return (Patch3 *) pnext; }
    Point3 &getCenter() { return center; }
    void setElementPtr( Element3 *pe ) { pelhd = pe; }
    
    void calcCenter()       // Calculate patch centroid
    {
        int i;            // Loop index
        int num_vert;     // Number of vertices
        Vector3 cv;       // Centroid vector
        
        num_vert = getNumVert();
        
        // Initialize centroid vector to origin
        cv = Vector3(0.0, 0.0, 0.0);
        
        // Determine patch centroid
        for (i = 0; i < num_vert; i++)
            cv = cv + Vector3(pvertex[i]->getPosn());
        
        cv /= (double) num_vert;
        
        // Convert centroid vector to 3-D point
        center.setX(cv.getX());
        center.setY(cv.getY());
        center.setZ(cv.getZ());
    }
    
};

void Vertex3::calcNormal()      // Calculate vertex normal
{
    ElemList *pelist = pelhd;     // Element list pointer
    
    // Sum element normals
    while (pelist != NULL)
    {
        normal += pelist->getElemPtr()->getNormal();
        pelist = pelist->getNext();
    }
    
    normal.norm();        // Normalize vector
}
#endif
