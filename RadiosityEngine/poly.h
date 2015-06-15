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
static const int FF_ArrayRes = 100;

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

    Vector4 intersect( Vector4 &s, Vector4 &e )
    {
        double d, t;  // Temporary variables
        Vector4 r;    // Temporary vector
        
        // Calculate parameter
        r = (e - s);
        d = dot(normal, r);
        
        if (fabs(d) > MIN_VALUE)
            t = -dot(normal, s) / d;
        else
            t = 1.0;
        
        if (t < 0.0)  // Ensure lower limit
            t = 0.0;
        
        if (t > 1.0)  // Ensure upper limit
            t = 1.0;
        
        // Calculate intersection vertex co-ordinates
        r *= t;
        
        return (s + r);
    }
    void output( Vector4 &v, FormPoly &out )
    {
        if (pnext != NULL)    // More planes ?
            pnext->clip(v, out);
        else
            out.AddVertex(v);
    }
public:
    FormClipEdge() { first_flag = false; }
    
    void add( FormClipEdge *pc ) { pnext = pc; }
    void clip( Vector4 &current, FormPoly &out )
    {
        bool curr_inside;     // Current point inside flag
        Vector4 isect;        // Intersection vertex
        
        // Determine vertex visibility
        curr_inside = IsInside(current);
        
        if (first_flag == false)      // First vertex seen ?
        {
            first = current;
            first_inside = curr_inside;
            first_flag = true;
        }
        else
        {
            // Does edge intersect plane ?
            if (start_inside ^ curr_inside)
            {
                isect = intersect(start, current);
                output(isect, out);
            }
        }
        
        if (curr_inside == true)
            output(current, out);
        
        start = current;
        start_inside = curr_inside;
    }

    void close( FormPoly &out )
    {
        Vector4 isect;        // Intersection vertex
        
        if (first_flag == true)
        {
            // Does edge intersect plane ?
            if (start_inside ^ first_inside)
            {
                isect = intersect(start, first);
                output(isect, out);
            }
            
            if (pnext != NULL)  // More planes ?
                pnext->close(out);
            
            // Reset first vertex seen flag
            first_flag = false;
        }
    }
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
    
    int clip( Element3 *pelem, FormPoly &out, unsigned short
         poly_id )
    {
        int i;            // Loop index
        int num_vert;     // Number of vertices
        Vertex3 *pvert;   // 3-D world space vertex pointer
        Vector4 hv;       // 4-D homogeneous co-ord vertex
        
        out.Reset(poly_id);   // Reset output polygon
        
        num_vert = pelem->getNumVert();
        for (i = 0; i < num_vert; i++)
        {
            // get world space vertex position pointer
            pvert = pelem->getVertexPtr(i);
            
            // set homogeneous co-ordinates vertex
            hv.projTransform(pvert->getPosn(), vtm);
            
            pclip->clip(hv, out);       // Clip polygon edge
        }
        
        pclip->close(out);    // Close polygon
        
        return out.getNumVert();
    }
};

struct FormCellInfo     // Face cell information
{
    float depth;          // Polygon cell depth
    unsigned short id;              // Polygon identifier
};

struct FormVertexInfo   // Vertex information
{
    struct                // Face cell array offsets
    {
        int x;              // Width offset
        int y;              // Height offset
    }
    face;
    Point3 posn;          // Scaled position
};

struct FormScanInfo     // Scan line intersection info
{
    double x;             // X-axis co-ordinate
    double z;             // Pseudodepth
};

struct FormEdgeInfo     // Edge information
{
    bool first;               // First intersection flag
    FormScanInfo isect[2];    // Scan line intersection array
};

// Form factor polygon scan conversion (abstract class)
class FormScan
{
protected:
    bool status;                // Object status
    int ymin;                   // Minimum y-axis co-ord
    int ymax;                   // Maximum y-axis co-ord
    int num_vert;               // Number of vertices
    FormCellInfo **cell_buffer; // Cell info buffer ptr
    FormEdgeInfo *edge_list;    // Edge list pointer
    FormVertexInfo v_info[8];   // Vertex info table
    unsigned short poly_id;               // Polygon identifier
    
    virtual void drawEdgeList() = 0;
    void getVertexInfo( FormPoly &poly )
    {
        int i;                // Loop index
        FormVertexInfo *pv;   // Vertex info element pointer
        Point3 posn;          // Normalized vertex position
        
        // Initialize polygon y-axis limits
        ymax = 0;
        ymin = FF_ArrayRes - 1;
        
        // get number of vertices
        num_vert = poly.getNumVert();
        
        for (i = 0; i < num_vert; i++)
        {
            pv = &(v_info[i]);  // get vertex info element pointer
            
            // get vertex normalized view space co-ordinates
            posn = poly.getVertex(i);
            
            // Scale view space u-v co-ordinates
            pv->posn.setX(posn.getX() * FF_ArrayRes);
            pv->posn.setY(posn.getY() * FF_ArrayRes);
            pv->posn.setZ(posn.getZ());
            
            // Convert to cell array x-y co-ordinates
            pv->face.x = (int) pv->posn.getX();
            pv->face.y = (int) pv->posn.getY();
            
            // Update polygon y-axis limits
            if (pv->face.y < ymin)
                ymin = pv->face.y;
            if (pv->face.y > ymax)
                ymax = pv->face.y;
        }
    }
    void scanEdges()      // Scan convert edges
    {
        int i, j;             // Loop indices
        double dx;            // X-axis delta
        double dz;            // Pseudodepth delta
        double ix;            // Intersection X-axis co-ordinate
        double iz;            // Intersection pseudodepth
        double y_dist;        // Y-axis distance
        FormEdgeInfo *pedge;  // Edge info pointer
        FormScanInfo *pscan;  // Scan line info pointer
        FormVertexInfo *psv;  // Start vertex info pointer
        FormVertexInfo *pev;  // End vertex info pointer
        FormVertexInfo *psw;  // Swap vertex info pointer
        
        // Initialize edge list
        for (i = ymin; i < ymax; i++)
            edge_list[i].first = false;
        
        for (i = 0; i < num_vert; i++)
        {
            // Get edge vertex pointers
            psv = &(v_info[i]);
            pev = &(v_info[(i + 1) % num_vert]);
            
            if (psv->face.y == pev->face.y)
            {
                continue;         // Ignore horizontal edges
            }
            
            if (psv->face.y > pev->face.y)
            {
                // Swap edge vertex pointers
                psw = psv; psv = pev; pev = psw;
            }
            
            // Get start vertex info
            ix = psv->posn.getX();
            iz = psv->posn.getZ();
            
            // Determine inverse slopes
            y_dist = (double) (pev->face.y - psv->face.y);
            
            dx = (pev->posn.getX() - ix) / y_dist;
            dz = (pev->posn.getZ() - iz) / y_dist;
            
            // Scan convert edge
            pedge = &(edge_list[psv->face.y]);
            for (j = psv->face.y; j < pev->face.y; j++)
            {
                // Determine intersection info element
                if (pedge->first == false)
                {
                    pscan = &(pedge->isect[0]);
                    pedge->first = true;
                }
                else
                    pscan = &(pedge->isect[1]);
                
                // Insert edge intersection info
                pscan->x = ix;
                pscan->z = iz;
                
                // Update edge intersection info
                ix += dx;
                iz += dz;
                
                pedge++;  // Point to next edge list element
            }
        }
    }
    
public:
    virtual ~FormScan() { };
    
    bool getStatus() { return status; }
    void scan( FormPoly &poly )
    {
        poly_id = poly.getPolyId();   // get polygon identifier
        getVertexInfo(poly);          // get vertex information
        scanEdges();                  // Scan convert edges
        drawEdgeList();               // Draw edge list
    }
};

#endif
