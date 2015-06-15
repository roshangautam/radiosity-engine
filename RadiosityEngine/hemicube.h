//
//  hemicube.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/14/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_hemicube_h
#define RadiosityEngine_hemicube_h

#include "poly.h"

static const int no_cells = 8; // number of cells on long side of hemicube
static const int FF_ArrayRes = no_cells * no_cells/2 * 6;

static const float FF_Infinity = (float) MAX_VALUE;
static const short unsigned FF_None = 0;

struct FormCellInfo     // Face cell information
{
    float depth;          // Polygon cell depth
    short unsigned id;              // Polygon identifier
};

class HemiCube          // Hemicube
{

    Point3 center;
    Vector3 u, v, n;            // View system co-ordinates
    FormCellInfo **cell_buffer; // Cell info buffer ptr
    
public:

    void calcFormFactors(Patch *ppatch)
    {
        int i;                // Loop index
        bool hidden;          // Patch visibility flag
        bool self;            // Self patch flag
        unsigned short j;               // Loop index
        unsigned short elem_id;         // Element identifier
        Point3 points[3] = { Point3(0, 0, 0), Point3(0.78, 0, 0), Point3(0, 0.78, 0) };

        Patch patch(points);
        Patch *pp = &patch;
//        Element3 *pelem;      // Element pointer
//        Instance *pinst;      // Instance pointer
//        Patch *ppatch;       // Patch pointer
//        Surface3 *psurf;      // Surface pointer
        
        
        // Set the hemicube view transformation matrix
        setView(pp);
        
        // Project environment onto each hemicube face
        for (i = 0; i < HemiFaceNum; i++)
        {
            // Update view transformation matrix
            updateView(i);
            
            initBuffer();  // Reinitialize depth buffer

            // Walk the instance list
//            while (pinst != NULL)
//            {
//                // Walk the surface list
//                psurf = pinst->GetSurfPtr();
//                while (psurf != NULL)
//                {
//                    // Walk the patch list
//                    ppatch = psurf->GetPatchPtr();
//                    while (ppatch != NULL)
//                    {
//                        // Check for self patch
                        self = (ppatch == pp) ? true : false;
//
//                        // Determine patch visibility
                        hidden = backFaceCull(ppatch);
            for (int row = 0; row < FF_ArrayRes; row++)
                for (int col = 0; col < FF_ArrayRes; col++)
                {
                    if (hidden == false && self == false) {
                        if(clip())
                    }
                    cell_buffer[row][col].depth = FF_Infinity;
                    cell_buffer[row][col].id = FF_None;
                }
//
//                        // Walk the element list
//                        pelem = ppatch->GetElementPtr();
//                        while (pelem != NULL)
//                        {
//                            if (hidden == FALSE && self == FALSE)
//                            {
//                                // Clip element to face view volume
//                                if (clip.Clip(pelem, out, elem_id) > 0)
//                                {
//                                    scan.Scan(out);     // Scan convert polygon
//                                }
//                            }
//                            pelem = pelem->GetNext();
//                            elem_id++;
//                        }
//                        ppatch = ppatch->GetNext();
//                    }
//                    psurf = psurf->GetNext();
//                }
//                pinst = pinst->GetNext();
//            }
//            
//            // Sum delta form factors
//            scan.SumDeltas(ff_array, i);
        }
    }

    Vector3 RandomVector()      // Generate random vector
    {
        Vector3 temp;     // Temporary vector
        
        temp.setX(GetNormRand() * 2.0 - 1.0);
        temp.setY(GetNormRand() * 2.0 - 1.0);
        temp.setZ(GetNormRand() * 2.0 - 1.0);
        
        return temp;
    }
    
    void setView(Patch *ppatch )
    {
        Vector3 rv;   // Random vector
        
        // Get eye position (hemicube center)
        center = ppatch->getCenter();
        // Select random vector for hemicube orientation
        rv = RandomVector();
        
        n = ppatch->getNormal();      // Get patch normal
        
        do   // Get valid u-axis vector
        {
            u = n.cross(rv);
        }
        while (u.getMagnitude() < MIN_VALUE);
        
        u.norm();             // Normalize u-axis
        v = u.cross(n);      // Determine v-axis
    }
    
    void updateView(int face_id )
    {
        Vector3 nu, nv, nn;   // View space co-ordinates
        
        switch (face_id )     // Exchange co-ordinates
        {
            case TOP_FACE:
                nu = u; nv = v; nn = n;
                break;
            case FRONT_FACE:
                nu = -u; nv = n; nn = v;
                break;
            case RIGHT_FACE:
                nu = v; nv = n; nn = u;
                break;
            case BACK_FACE:
                nu = u; nv = n; nn = -v;
                break;
            case LEFT_FACE:
                nu = -v; nv = n; nn = -u;
                break;
            default:
                break;
        }
    }
    
    void initBuffer()
    {
        int row, col;     // Loop indices
        
        for (row = 0; row < FF_ArrayRes; row++)
            for (col = 0; col < FF_ArrayRes; col++)
            {
                cell_buffer[row][col].depth = FF_Infinity;
                cell_buffer[row][col].id = FF_None;
            }
    }
    
    bool backFaceCull(Patch *ppatch )
    {
        Vector3 view;     // View vector

        // Calculate view vector
        view = Vector3(*ppatch->getVertex(0), center);

        // Indicate whether patch is backface
        return (ppatch->getNormal().dot(view) < MIN_VALUE) ? true : false;
    }
};

typedef HemiCube FormFactor;    // Class alias

#endif
