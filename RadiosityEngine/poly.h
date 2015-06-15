//
//  poly.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/14/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_poly_h
#define RadiosityEngine_poly_h

#include "patch.h"
#include "vector.h"

// Maximum number of output vertices
static const int MaxVert = 10;

// View normalization parameters
static const double FPD = -0.99;
static const double BPD = MAX_VALUE;
static const double EYE = -1.0;
static const double SN = (EYE - BPD) * (EYE - FPD) / (EYE *
                                                      EYE * (BPD - FPD));
static const double RN = FPD * (EYE - BPD) / (EYE * (FPD -
                                                     BPD));

class FormPoly          // Form factor polygon
{
private:
    Point3 posn[MaxVert];       // Output vertex array
    int num_vert;               // Number of vertices
    unsigned short ident;                 // Polygon identifier
    
    void AddVertex( Vector4 &v )
    { v.perspective(&(posn[num_vert++])); }
    
    void Reset( unsigned short identifier )
    {
        num_vert = 0;
        ident = identifier;
    }
    
    friend class FormClipEdge;
    friend class FormClip;
    
public:
    FormPoly()
    {
        num_vert = 0;
        ident = 0;
    }
    
    int getNumVert() { return num_vert; }
    Point3 &getVertex( int i )
    { return posn[i]; }
    unsigned short getPolyId() { return ident; }
};

class FormClipEdge      // Edge-plane clipper
{
private:
    FormClipEdge *pnext;        // Next clipper pointer
    Vector4 normal;             // Plane normal
    Vector4 first;              // First vertex
    Vector4 start;              // Start vertex
    bool first_inside;          // First vertex inside flag
    bool start_inside;          // Start vertex inside flag
    bool first_flag;            // First vertex seen flag
    
    bool IsInside( Vector4 &v )
    { return (dot(normal, v) >= 0.0); }
    Vector4 Intersect( Vector4 &, Vector4 & );
    void Output( Vector4 &, FormPoly & );
    
public:
    FormClipEdge() { first_flag = false; }
    
    void Add( FormClipEdge *pc ) { pnext = pc; }
    void Clip( Vector4 &, FormPoly & );
    void Close( FormPoly & );
    void setNormal( Vector4 &n ) { normal = n; }
};

class FormClip  // Form factor polygon clipper
{
protected:
    int num_vert;               // # of polygon vertices
    Vector3 u, v, n;            // View system co-ordinates
    double vtm[4][4];           // Transformation matrix
    FormClipEdge clipper[5];    // Clipper array
    FormClipEdge *pclip;        // Clipper list head pointer
    Point3 center;              // Polygon center
    
    Vector3 RandomVector()      // Generate random vector
    {
        Vector3 temp;     // Temporary vector
        
        temp.setX(getNormRand() * 2.0 - 1.0);
        temp.setY(getNormRand() * 2.0 - 1.0);
        temp.setZ(getNormRand() * 2.0 - 1.0);
        
        return temp;
    }
    
public:
    bool BackFaceCull( Patch3 *ppatch )
    {
        Vector3 view;     // View vector
        
        // Calculate view vector
        view = Vector3(ppatch->getVertexPtr(0)->getPosn(),
                       center);
        
        // Indicate whether patch is backface
        return (dot(ppatch->getNormal(), view) < MIN_VALUE) ?
        true : false;
    }
    
    int Clip( Element3 *, FormPoly &, unsigned short );
};

#endif
