//
//  3dintersection.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine__dintersection_h
#define RadiosityEngine__dintersection_h

#include <string>
#include <math.h>

#include "vector.h"

using namespace std;

enum Face {TOP_FACE, LEFT_FACE, RIGHT_FACE, FRONT_FACE, BACK_FACE, NA};

class ThreeDIntersection {
    
private:
    Vector _vector;
    Face _intersectingFace;
public:
    ThreeDIntersection() {
        _intersectingFace = NA;
    }
    
    ThreeDIntersection(Vector Vector, Face intersectingFace) {
        _vector = Vector;
        _intersectingFace = intersectingFace;
    }
    
    Vector getVector() {
        return _vector;
    }
    
    Face getIntersectingFace() {
        return _intersectingFace;
    }
    
    string getHumanReadableIntersectingFace() {
        switch (_intersectingFace) {
            case 0:
                return "TOP";
                break;
            case 1:
                return "LEFT";
                break;
            case 2:
                return "RIGHT";
                break;
            case 3:
                return "FRONT";
                break;
            case 4:
                return "BACK";
                break;
            case 5:
                return "Error:Invalid Coordinates Provided.";
                break;
            default:
                break;
        }
    }
    
    void setVector(Vector Vector) {
        _vector = Vector;
    }
    
    void setIntersectingFace(Face intersectingFace) {
        _intersectingFace = intersectingFace;
    }
    
    void intersect(Vector *vertex) {
        float t;
        if (vertex->getY() >= 0) {
            if((vertex->getY() <= (-(vertex->getZ())) ||
                vertex->getY() <= vertex->getZ()) &&
               (vertex->getY() <= vertex->getX() ||
                vertex->getY() <= (-(vertex->getX())))) { // Inequalities to check if the point lies on one of the sides
                   if (vertex->getX() < 0) { // If x is negative its the left side
                       _intersectingFace =  LEFT_FACE;
                       _vector.setX(-1.0);
                       t = _vector.getX() / vertex->getX();
                       _vector.setY(t * vertex->getY());
                       _vector.setZ(t * vertex->getZ());
                   } else if(vertex->getX() >= 0) { //If x is positive its the right side
                       _intersectingFace = RIGHT_FACE;
                       _vector.setX(1.0);
                       t = _vector.getX() / vertex->getX();
                       _vector.setY(t * vertex->getY());
                       _vector.setZ(t * vertex->getZ());
                   } else if (vertex->getZ() >= 0) { //If z is positive its the front side
                       _intersectingFace = FRONT_FACE;
                       _vector.setZ(1.0);
                       t = _vector.getZ() / vertex->getZ();
                       _vector.setX(t * vertex->getX());
                       _vector.setY(t * vertex->getY());
                   } else if(vertex->getZ() < 0) { //if z is negative its the back side
                       _intersectingFace = BACK_FACE;
                       _vector.setZ(-1.0);
                       t = _vector.getZ() / vertex->getZ();
                       _vector.setX(t * vertex->getX());
                       _vector.setY(t * vertex->getY());
                   }
               } else { // else its on top
                   _intersectingFace = TOP_FACE;
                   _vector.setY(1.0);
                   t = _vector.getY() / vertex->getY();
                   _vector.setX(t * vertex->getX());
                   _vector.setZ(t * vertex->getZ());
               }
        }
    }
    
    void intersection(Vector &vertex) {
        float t;
        Plane plane;
        Line line;
        if (vertex.getY() >= 0) {
            if((vertex.getY() >= (-(vertex.getZ())) &&
                vertex.getY() >= vertex.getZ()) &&
               (vertex.getX() >= (-(vertex.getZ())) &&
                vertex.getX() >= vertex.getZ())) {
                _intersectingFace = TOP_FACE;
            } else {
                if(vertex.getX() >= 1) {
                    _intersectingFace = RIGHT_FACE;
                } else if(vertex.getX() <= -1) {
                    _intersectingFace = LEFT_FACE;
                }
                if(vertex.getY() >= 1) {
                    _intersectingFace = FRONT_FACE;
                } else if(vertex.getY() <= -1) {
                    _intersectingFace = BACK_FACE;
                }
            }
            plane = getPlane(_intersectingFace);
            line = getLine(vertex);
            Vector diff = plane.getVertex() - line.getVertex();
            t = diff.dot(plane.getNormal()) / line.getNormal().dot(plane.getNormal());
            Vector intersection = line.getVertex() + Vector(t*line.getNormal().getX(), t*line.getNormal().getY(), t*line.getNormal().getZ());
            _vector.setX(intersection.getX());
            _vector.setY(intersection.getY());
            _vector.setZ(intersection.getZ());
        }

    }
    
    void simplePrint() {
        cout << "(" << _vector.getX() << "," << _vector.getY() << "," << _vector.getZ() << ")";
        cout << " on " << getHumanReadableIntersectingFace() << "\n\n";
    }
    
    Plane getPlane(Face face) {
        Vector one,two,three;
        if (face == TOP_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(-1,-1,1);
        } else if(face == LEFT_FACE) {
            one.setCoordinates(-1,1,1);
            two.setCoordinates(-1,1,0);
            three.setCoordinates(-1,-1,1);
        } else if(face == RIGHT_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(1,-1,1);
            three.setCoordinates(1,1,0);
        } else if(face == FRONT_FACE) {
            one.setCoordinates(1,1,1);
            two.setCoordinates(-1,1,1);
            three.setCoordinates(-1,1,0);
        } else if(face == BACK_FACE) {
            one.setCoordinates(1,-1,1);
            two.setCoordinates(-1,-1,1);
            three.setCoordinates(-1,-1,0);
        }
        Vector four = two - one;
        Vector five = three - one;
        Vector six = four.cross(five);
        Vector normal = six.unit();
        return Plane(one, normal);
    }
    
    Line getLine(Vector vector) {
        Vector one;
        Vector two = vector - one;
        Vector direction = two.unit();
        return Line(vector, direction);
    }
    
//    function SameSide(p1,p2, a,b)
//    cp1 = CrossProduct(b-a, p1-a)
//    cp2 = CrossProduct(b-a, p2-a)
//    if DotProduct(cp1, cp2) >= 0 then return true
//        else return false
//            
//    function PointInTriangle(p, a,b,c)
//    if SameSide(p,a, b,c) and SameSide(p,b, a,c)
//        and SameSide(p,c, a,b) then return true
//        else return false
    
};

#endif
