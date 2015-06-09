//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>

#include "intersection.h"
#include "3dintersection.h"

using namespace std;

void findPatch(Vector,Vector,Vector);
void printPatchOnSameSide(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
int findPointToBeProjected(ThreeDIntersection[]);
Face findProjectionFace(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
Vector findProjection(Vector, Face);

int main(int argc, const char * argv[]) {
    Point point, secondPoint;
    Intersection intersection, secondIntersection;
    Vector vertex, vertex1, vertex2;
    ThreeDIntersection threeDIntersection;
    char c = '1';
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions for 2D:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n";
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Enter Coordinates for a 2D Point\n";
        cout << "(2)Enter Coordinates for a 2D line segment\n";
        cout << "(3)Enter Coordinates for a Vertex (3D)\n";
        cout << "(4)Enter Coordinates for a Triangular Patch (3D)\n";
        cout << "(Q)Quit\n";
        cout << "[Select]:";
        cin >> c;
        if(cin.get() != '\n') { // Look at the next character
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input stream
            cout << "Invalid Option\n";
        } else {
            switch (c) {
                case '1':
                {
                    cout << "Enter Coordinates for the point!\n";
                    point.read();
                    if (point.getY() > 0 && (point.getX() > 1 || point.getX() < -1) ) {
                        intersection.intersect(&point);
                        cout << "Coordinates of intersection are:";
                        intersection.simplePrint();
                    } else {
                        cout << "Invalid Coordinates provided.\nx must be greater than 1 and less than -1.\ny must be greater than 1.\nPlease try again.\n";
                    }
                }
                    break;
                case '2':
                {
                    cout << "Enter Coordinates for first point!\n";
                    point.read();
                    cout << "Enter Coordinates for second point!\n";
                    secondPoint.read();
                    intersection.intersect(&point);
                    secondIntersection.intersect(&secondPoint);
                    if(intersection.getSide() == secondIntersection.getSide()) {
                        cout << "Coordinates of intersecting segment are:";
                        intersection.getPoint().print();
                        cout << ",";
                        secondIntersection.getPoint().print();
                        cout << " on " << intersection.getHumanReadableSide() << "\n";
                    } else {
                        cout << "Coordinates of intersecting segment are:";
                        intersection.print();
                        if(intersection.getSide() == 0) {
                            cout << " AND (-1,1),(1,1) on Top AND ";
                        } else if(intersection.getSide() == 1) {
                            cout << " AND (1,1),(-1,1) on Top AND ";
                        } else {
                            cout << " AND ";
                        }
                        secondIntersection.print();
                    }
                }
                    break;
                case '3':
                {
                    cout << "Enter Coordinates of the Vertex\n";
                    vertex.read();
                    threeDIntersection.intersect(&vertex);
                    threeDIntersection.printWithFace();
                }
                    break;
                case '4':
                {
                    cout << "Enter Coordinates of the Triangular Patch\n";
                    cout << "Enter Coordinates for first vertex:\n";
                    vertex.read();
                    cout << "Enter Coordinates for second vertex:\n";
                    vertex1.read();
                    cout << "Enter Coordinates for third vertex\n";
                    vertex2.read();
                    findPatch(vertex, vertex1, vertex2);
                }
                    break;
                case 'q':
                case 'Q':
                    cout << "--- End of Program ---\n";
                    exit(1);
                default:
                    cout << "Invalid option\n\n";
                    break;
            }
        }
    }
    return 0;
}

void findPatch(Vector point1, Vector point2, Vector point3) {
    ThreeDIntersection patchPoint1, patchPoint2, patchPoint3;
    patchPoint1.intersect(&point1);
    patchPoint2.intersect(&point2);
    patchPoint3.intersect(&point3);
    
    Vector givenPoints[3] = { point1, point2, point3 };
    ThreeDIntersection patchPoints[3] = { patchPoint1, patchPoint2, patchPoint3};
    
    if (patchPoint1.getIntersectingFace() == patchPoint2.getIntersectingFace() &&
        patchPoint2.getIntersectingFace() == patchPoint3.getIntersectingFace() &&
        patchPoint3.getIntersectingFace() == patchPoint1.getIntersectingFace()) { // all in the same side of cube
        printPatchOnSameSide(patchPoint1, patchPoint2, patchPoint3);
    } else {
        if (patchPoint1.getIntersectingFace() != patchPoint2.getIntersectingFace() &&
            patchPoint2.getIntersectingFace() != patchPoint3.getIntersectingFace() &&
            patchPoint3.getIntersectingFace() != patchPoint1.getIntersectingFace()) { // patch is in three different sides of the cube // 4 new points: we need to calculate three new points and find another point based on what faces the points fall
        } else { // patch is in two sides of the cube // 2 new points
            int index = findPointToBeProjected(patchPoints);
            if(index >= 0) {
                Face projectionFace = findProjectionFace(patchPoint1, patchPoint2, patchPoint3);
                givenPoints[index].print();
                cout << projectionFace;
                Vector projectedPoint = findProjection(givenPoints[index], projectionFace);
                
                //parametric equation changes for the following.
//                Vector pointOnEdge1 = findProjection(projectedPoint, patchPoints[index].getIntersectingFace());
//                Vector pointOnEdge2 = findProjection(projectedPoint, patchPoints[index].getIntersectingFace());
                if (index == 0) {
                    // find points for index = 1 and index = 2
                } else if(index == 1) {
                    // find points for index = 0 and index = 2
                } else if(index == 2) {
                    // find points for index = 0 and index = 1
                }
            } else {
                cout << "ERROR:Invalid projection point\n\n";
            }
        }
    }
    
    
}

void printPatchOnSameSide(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    cout << "Coordinates of the patch are:\n";
    patchPoint1.print();
    cout << ",";
    patchPoint2.print();
    cout << ",";
    patchPoint3.print();
    cout << " on " << patchPoint1.getHumanReadableIntersectingFace() << "\n\n";
}

int findPointToBeProjected(ThreeDIntersection patchPoints[]) {
    bool found = false;
    for (int i = 0; i <= 2; i++) {
        found = true;
        for (int j = i+1; j<= (2-i); j++) {
            if (patchPoints[i].getIntersectingFace() == patchPoints[j].getIntersectingFace()) {
                found = false;
                break;
            }
        }
        if (found) {
            return  i;
            break;
        }
    }
    return -1;
}

Face findProjectionFace(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    
    bool found = false;
    ThreeDIntersection uniquePoint;
    ThreeDIntersection patchPoints[3];
    patchPoints[0] = patchPoint1;
    patchPoints[1] = patchPoint2;
    patchPoints[2] = patchPoint3;
    
    for (int i = 0; i <= 2; i++) {
        found = true;
        for (int j = i+1; j<= (2-i); j++) {
            if (patchPoints[i].getIntersectingFace() == patchPoints[j].getIntersectingFace()) {
                found = false;
                break;
            }
        }
        if (found) {
            if (i == 2) {
                return patchPoints[i-1].getIntersectingFace();
            }
            return  patchPoints[i+1].getIntersectingFace();
            break;
        }
    }
    return NA;
}

Vector findProjection(Vector pointToBeProjected, Face face) {
    float t;
    Vector projectedPoint;
    if (face == TOP_FACE) {
        projectedPoint.setY(1.0);
        t = projectedPoint.getY() / pointToBeProjected.getY();
        projectedPoint.setX(t * pointToBeProjected.getX());
        projectedPoint.setZ(t * pointToBeProjected.getZ());
    } else if(face == RIGHT_FACE) {
        projectedPoint.setX(1.0);
        t = projectedPoint.getX() / pointToBeProjected.getX();
        projectedPoint.setY(t * pointToBeProjected.getY());
        projectedPoint.setZ(t * pointToBeProjected.getZ());
    } else if(face == LEFT_FACE) {
        projectedPoint.setX(-1.0);
        t = projectedPoint.getX() / pointToBeProjected.getX();
        projectedPoint.setY(t * pointToBeProjected.getY());
        projectedPoint.setZ(t * pointToBeProjected.getZ());
    } else if(face == FRONT_FACE) {
        projectedPoint.setZ(1.0);
        t = projectedPoint.getZ() / pointToBeProjected.getZ();
        projectedPoint.setY(t * pointToBeProjected.getY());
        projectedPoint.setX(t * pointToBeProjected.getX());
    } else if(face == BACK_FACE) {
        projectedPoint.setZ(-1.0);
        t = projectedPoint.getZ() / pointToBeProjected.getZ();
        projectedPoint.setY(t * pointToBeProjected.getY());
        projectedPoint.setX(t * pointToBeProjected.getX());
    }
    return projectedPoint;
}