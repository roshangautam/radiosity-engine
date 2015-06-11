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

void loop();
void findAndPrintPatch(Vector,Vector,Vector);
void printPatchOnSameSide(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
int findPointToBeProjected(ThreeDIntersection[]);
HemiCubeFace findProjectionFace(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
Vector findCommonVertex(HemiCubeFace[]);

int main(int argc, const char * argv[]) {
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions for 2D:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n";
    loop();
    return 0;
}

void loop() {
    Point point, secondPoint;
    Intersection intersection, secondIntersection;
    Vector vertex, vertex1, vertex2;
    ThreeDIntersection threeDIntersection;
    char c = '1';
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Enter Coordinates for a 2D Point and Hemicube intersection\n";
        cout << "(2)Enter Coordinates for a 2D line segment and Hemicube intersection\n";
        cout << "(3)Enter Coordinates for a Vertex (3D) and Hemicube intersection\n";
        cout << "(4)Enter Coordinates to calculate shadow (patch) of a triangle (3D) on Hemicube surface\n";
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
                    cout << "Enter Coordinates of the point!\n";
                    point.read();
                    if (point.getY() > 0 && (point.getX() > 1 || point.getX() < -1) ) {
                        intersection.intersect(&point);
                        cout << "\nCoordinates of intersection are:";
                        intersection.simplePrint();
                    } else {
                        cout << "\nInvalid Coordinates provided.\nx must be greater than 1 and less than -1.\ny must be greater than 1.\nPlease try again.\n";
                    }
                }
                    break;
                case '2':
                {
                    cout << "Enter Coordinates of first point!\n";
                    point.read();
                    cout << "Enter Coordinates of second point!\n";
                    secondPoint.read();
                    intersection.intersect(&point);
                    secondIntersection.intersect(&secondPoint);
                    if(intersection.getSide() == secondIntersection.getSide()) {
                        cout << "\nCoordinates of intersecting segment are:";
                        intersection.getPoint().print();
                        cout << ",";
                        secondIntersection.getPoint().print();
                        cout << " on " << intersection.getHumanReadableSide() << "\n";
                    } else {
                        cout << "\nCoordinates of intersecting segment are:";
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
                    if(vertex.read()) {
                        threeDIntersection.intersect(&vertex);
                        threeDIntersection.printWithFace();
                    } else {
                        cout << "\nERROR:Coordinates must be greater than 1 and less than -1. Try again with a different set of coordinates.\n";
                    }
                }
                    break;
                case '4':
                {

                    findAndPrintPatch(Vector(5,4,2),Vector(2,5,7),Vector(3,5,9));
//
//                    cout << "Enter Coordinates for first vertex:\n";
//                    if (vertex.read() && vertex1.read()) {
//                        if(vertex2.read()) {
//                            findAndPrintPatch(vertex, vertex1, vertex2);
//                            break;
//                        }
//                    }
//                    cout << "\nERROR:Coordinates must be greater than 0 and less than -1. Try again with a different set of coordinates.\n";
                }
                    break;
                case 'q':
                case 'Q':
                    cout << "\n--- End of Program ---\n";
                    exit(1);
                default:
                    cout << "\nInvalid option\n";
                    break;
            }
        }
    }
}

void findAndPrintPatch(Vector point1, Vector point2, Vector point3) {
    ThreeDIntersection patchPoint1, patchPoint2, patchPoint3;
    Vector givenPoints[3] = {point1, point2, point3 };
    Vector projectedPoint;
    Vector pointOnEdges[4];
    HemiCubeFace projectionFace;
    
    patchPoint1.intersect(&point1);
    patchPoint2.intersect(&point2);
    patchPoint3.intersect(&point3);
    
    ThreeDIntersection patchPoints[3] = {patchPoint1, patchPoint2, patchPoint3};
    if (DEBUG_MODE) {
        cout << "Checking if the points are in same or different planes by comparing where they intersect on hemicube\n";
    }
    if (patchPoint1.getIntersectingFace() == patchPoint2.getIntersectingFace() &&
        patchPoint2.getIntersectingFace() == patchPoint3.getIntersectingFace() &&
        patchPoint3.getIntersectingFace() == patchPoint1.getIntersectingFace()) { // all in the same side of cube
        if (DEBUG_MODE) {
            cout << "Points are on the same plane because they all intersect on the same surface of hemicube\n";
        }
        printPatchOnSameSide(patchPoint1, patchPoint2, patchPoint3);
    } else {
        if (DEBUG_MODE) {
            cout << "Points are on different planes\n";
        }
        if (patchPoint1.getIntersectingFace() != patchPoint2.getIntersectingFace() &&
            patchPoint2.getIntersectingFace() != patchPoint3.getIntersectingFace() &&
            patchPoint3.getIntersectingFace() != patchPoint1.getIntersectingFace()) {
            if (DEBUG_MODE) {
                cout << "Points are on three different planes\n";
            }
            // patch is in three different sides of the cube // Need 4 new points: we need to calculate three new points and find another point based on what faces the points fall

            projectedPoint = Vector::findPointOnALine(Vector(0,0,0), givenPoints[0], patchPoints[2].getIntersectingFace());
            pointOnEdges[0] = Vector::findPointOnALine(patchPoints[2].getVector(), projectedPoint, patchPoints[0].getIntersectingFace());

            projectedPoint = Vector::findPointOnALine(Vector(0,0,0), givenPoints[1], patchPoints[0].getIntersectingFace());
            pointOnEdges[1] = Vector::findPointOnALine(patchPoints[0].getVector(), projectedPoint, patchPoints[1].getIntersectingFace());
            
            projectedPoint = Vector::findPointOnALine(Vector(0,0,0), givenPoints[2], patchPoints[1].getIntersectingFace());
            pointOnEdges[2] = Vector::findPointOnALine(patchPoints[1].getVector(), projectedPoint, patchPoints[2].getIntersectingFace());

            
            HemiCubeFace faces[3] = {patchPoints[0].getIntersectingFace(),patchPoints[1].getIntersectingFace(), patchPoints[2].getIntersectingFace()};
            
            pointOnEdges[3] = findCommonVertex(faces);
            
            patchPoints[0].print();
            cout << " ––– " ;
            pointOnEdges[1].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[0].print();
            cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << " \n" ;
            
            patchPoints[1].print();
            cout << " ––– ";
            pointOnEdges[1].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[2].print();
            cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << " \n" ;
            
            patchPoints[2].print();
            cout << " ––– ";
            pointOnEdges[2].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[0].print();
            cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << " \n " ;
        } else {
            if (DEBUG_MODE) {
                cout << "Points are on two different planes\n";
                cout << "Find the point which is singled out";
            }
            // patch is in two sides of the cube // Need 2 new points
           
            int index = findPointToBeProjected(patchPoints);
            if(index >= 0) {
                projectionFace = findProjectionFace(patchPoint1, patchPoint2, patchPoint3);
                projectedPoint = Vector::findPointOnALine(Vector(0,0,0), givenPoints[index], projectionFace);
                if (index == 0) {
                    pointOnEdges[1] = Vector::findPointOnALine(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = Vector::findPointOnALine(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
                    cout << "\nCoordinates of the patch are:\n";
                    patchPoints[0].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[1].print();
                    cout << " ––– ";
                    patchPoints[1].print();
                    cout << " ––– ";
                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << "\n";
                } else if(index == 1) {
                    pointOnEdges[0] = Vector::findPointOnALine(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = Vector::findPointOnALine(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
                    patchPoints[1].print();
                    cout << " ––– ";
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    patchPoints[0].print();
                    cout << " ––– ";
                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << "\n";
                } else if(index == 2) {
                    pointOnEdges[0] = Vector::findPointOnALine(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[1] = Vector::findPointOnALine(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());

                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    patchPoints[0].print();
                    cout << " ––– ";
                    patchPoints[1].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << "\n";
                }
                
            } else {
                cout << "ERROR:Invalid projection point\n";
            }
        }
    }
    
    
}

void printPatchOnSameSide(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    cout << "\nCoordinates of the patch are:\n";
    patchPoint1.print();
    cout << ",";
    patchPoint2.print();
    cout << ",";
    patchPoint3.print();
    cout << " on " << patchPoint1.getHumanReadableIntersectingFace() << "\n";
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

HemiCubeFace findProjectionFace(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    bool found = false;
    ThreeDIntersection uniquePoint;
    ThreeDIntersection patchPoints[3];
    patchPoints[0] = patchPoint1;
    patchPoints[1] = patchPoint2;
    patchPoints[2] = patchPoint3;
    HemiCubeFace intersectingFace;
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
                intersectingFace = patchPoints[i-1].getIntersectingFace();
            }
            intersectingFace = patchPoints[i+1].getIntersectingFace();
            break;
        }
    }
    return intersectingFace;
}

Vector findCommonVertex(HemiCubeFace faces[]) {
    if ((faces[0] == TOP_FACE && faces[1] == FRONT_FACE && faces[2] == LEFT_FACE) ||
        (faces[0] == TOP_FACE && faces[1] == LEFT_FACE && faces[2] == FRONT_FACE) ||
        (faces[0] == FRONT_FACE && faces[1] == TOP_FACE && faces[2] == LEFT_FACE) ||
        (faces[0] == FRONT_FACE && faces[1] == LEFT_FACE && faces[2] == TOP_FACE) ||
        (faces[0] == LEFT_FACE && faces[1] == TOP_FACE && faces[2] == FRONT_FACE) ||
        (faces[0] == LEFT_FACE && faces[1] == FRONT_FACE && faces[2] == TOP_FACE)) {
        return Vector(-1,1,1);
    } else if((faces[0] == TOP_FACE && faces[1] == FRONT_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == RIGHT_FACE && faces[2] == FRONT_FACE) ||
              (faces[0] == FRONT_FACE && faces[1] == TOP_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == FRONT_FACE && faces[1] == RIGHT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == TOP_FACE && faces[2] == FRONT_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == FRONT_FACE && faces[2] == TOP_FACE)) {
        return Vector(1,1,1);
    } else if((faces[0] == TOP_FACE && faces[1] == BACK_FACE && faces[2] == LEFT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == LEFT_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == TOP_FACE && faces[2] == LEFT_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == LEFT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == LEFT_FACE && faces[1] == TOP_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == LEFT_FACE && faces[1] == BACK_FACE && faces[2] == TOP_FACE)) {
        return Vector(-1,1,-1);
    } else if((faces[0] == TOP_FACE && faces[1] == BACK_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == RIGHT_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == TOP_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == RIGHT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == TOP_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == BACK_FACE && faces[2] == TOP_FACE)) {
        return Vector(1,1,-1);
    }
    return Vector(0,0,0);
}