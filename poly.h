////
////  poly.h
////  RadiosityEngine
////
////  Created by Roshan Gautam on 6/14/15.
////  Copyright (c) 2015 Roshan Gautam. All rights reserved.
////
//
//#ifndef RadiosityEngine_poly_h
//#define RadiosityEngine_poly_h
//
#include "patch.h"
//
//// Maximum number of output vertices
//static const int MaxVert = 10;
//
//class FormPoly          // Form factor polygon
//{
//private:
//    Point3 posn[MaxVert];       // Output vertex array
//    int num_vert;               // Number of vertices
//    short unsigned ident;       // Polygon identifier
//    
//    void Reset( short unsigned identifier )
//    {
//        num_vert = 0;
//        ident = identifier;
//    }
//    
//    friend class FormClip;
//    
//public:
//    FormPoly()
//    {
//        num_vert = 0;
//        ident = 0;
//    }
//    
//    int GetNumVert() { return num_vert; }
//    Point3 &GetVertex( int i )
//    { return posn[i]; }
//    short unsigned GetPolyId() { return ident; }
//};
//
//class FormClip  // Form factor polygon clipper
//{
//protected:
//    int num_vert;               // # of polygon vertices
//    Vector3 u, v, n;            // View system co-ordinates
//    double vtm[4][4];           // Transformation matrix
//    Point3 center;              // Polygon center
//
//    Vector3 RandomVector()      // Generate random vector
//    {
//        Vector3 temp;     // Temporary vector
//        
//        temp.setX(GetNormRand() * 2.0 - 1.0);
//        temp.setY(GetNormRand() * 2.0 - 1.0);
//        temp.setZ(GetNormRand() * 2.0 - 1.0);
//        
//        return temp;
//    }
//    
//public:
//    bool BackFaceCull(Patch *ppatch )
//    {
//        Vector3 view;     // View vector
//        
//        // Calculate view vector
//        view = Vector3(*ppatch->getVertex(0), center);
//
//        // Indicate whether patch is backface
//        return (ppatch->getNormal().dot(view) < MIN_VALUE) ? true : false;
//    }
//    
//    int clip(FormPoly &, short unsigned );
//};
//
//// Clip element
//int FormClip::clip(FormPoly &out, short unsigned poly_id)
//{
//    int i;            // Loop index
//    int num_vert;     // Number of vertices
//    Vertex3 *pvert;   // 3-D world space vertex pointer
//    Vector4 hv;       // 4-D homogeneous co-ord vertex
//    
//    out.Reset(poly_id);   // Reset output polygon
//    
//    num_vert = pelem->GetNumVert();
//    for (i = 0; i < num_vert; i++)
//    {
//        // Get world space vertex position pointer
//        pvert = pelem->GetVertexPtr(i);
//        
//        // Set homogeneous co-ordinates vertex
//        hv.ProjTransform(pvert->GetPosn(), vtm);
//        
//        pclip->Clip(hv, out);       // Clip polygon edge
//    }
//    
//    pclip->Close(out);    // Close polygon
//    
//    return out.GetNumVert();
//}
//#endif
