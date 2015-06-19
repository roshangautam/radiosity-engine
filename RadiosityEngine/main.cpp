//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>

#include "intersection.h"
#include "3dintersection.h"
#include "patch.h"

using namespace std;

int POLY_COUNT = 10000;

void loop();
void findAndPrintPatch(Vector[]);
void printPatchOnSameSide(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
int findPointToBeProjected(ThreeDIntersection[]);
Face findProjectionFace(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
Vector findPointBetweenTwoPoints(Vector, Vector, Face);
Vector findCommonVertex(Face[]);
void generateHemicubeCellCenters(int, Patch);
void generatePolygons(double, double, double, string);
Vector calculateNormal(Vector[]);
bool onSameSide(Vector, Vector, Vector, Vector);
bool pointInTriangle(Vector, Vector, Vector, Vector);
void findClosestObject(int, float[], Vector[], Vector[]);

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
    double length = 0.0, width = 0.0, height = 0.0;
//    double boxLength = 0.0, boxWidth = 0.0, boxHeight = 0.0;
    char c = '1';
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Enter Coordinates for a 2D Point and Hemicube intersection\n";
        cout << "(2)Enter Coordinates for a 2D line segment and Hemicube intersection\n";
        cout << "(3)Enter Coordinates for a Vertex (3D) and Hemicube intersection\n";
        cout << "(4)Enter Coordinates to calculate shadow (patch) of a triangle (3D) on Hemicube surface\n";
        cout << "(5)Generate Hemicube cell centers\n";
        cout << "(6)Generate environment patch vertices\n";
        cout << "(7)Calculate all t value from hemicube cells to a given set of triangle vertices\n";
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
                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
                    }
                }
                    break;
                case '4':
                {
                    cout << "Enter Coordinates for first vertex:\n";
                    if(!vertex.read()) {
                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
                        break;
                    }
                    cout << "Enter Coordinates for second vertex:\n";
                    if(!vertex1.read()) {
                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
                        break;
                    }
                    cout << "Enter Coordinates for third vertex\n";
                    if(!vertex2.read()) {
                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
                        break;
                    }
                    Vector givenPonts[3] = {vertex, vertex1, vertex2};
                    findAndPrintPatch(givenPonts);
                }
                    break;
                case '5':
                {
                    int n;
                    cout << "Enter the resolution for hemicube:";
                    cin >> n;
                    Patch patch;
                    generateHemicubeCellCenters(n, patch);
                }
                    break;
                case '6':
                {
                    cout << "Enter the height of the room:";
                    cin >> height;
                    cout << "Enter the width of the room:";
                    cin >> width;
                    cout << "Enter the length of the room:";
                    cin >> length;
//                    cout << "Enter the height of the box:";
//                    cin >> boxHeight;
//                    cout << "Enter the width of the box:";
//                    cin >> boxWidth;
//                    cout << "Enter the length of the box:";
//                    cin >> boxLength;
                    generatePolygons(height, width, length, "room.obj");
//                    generatePolygons(boxHeight, boxWidth, boxLength, "box.obj");
                }
                    break;
                case '7':
                {
//                    cout << "Enter Coordinates for first vertex:\n";
//                    if(!vertex.read()) {
//                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
//                        break;
//                    }
//                    cout << "Enter Coordinates for second vertex:\n";
//                    if(!vertex1.read()) {
//                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
//                        break;
//                    }
//                    cout << "Enter Coordinates for third vertex\n";
//                    if(!vertex2.read()) {
//                        cout << "\nERROR:Coordinates can't be 0. Try again with a different set of coordinates.\n";
//                        break;
//                    }
                    
//                    Vector givenPoints[3] = {vertex, vertex1, vertex2};
                    int n;
                    cout << "Enter the resolution for hemicube:";
                    cin >> n;
                    ifstream objectFile("room.obj");
                    if (objectFile.is_open()) {
                        std::istream_iterator<double> start(objectFile), end;
                        std::vector<double> lines(start, end);

                        Vector *vertices = new Vector[lines.size()];
                        Patch *patches = new Patch[lines.size()/9];
                        for (int i = 0, j = 0 ; j < lines.size()/3 ; i+=3,j++) {
                            vertices[j].setCoordinates(lines.at(i), lines.at(i+1), lines.at(i+2));
                        }
                        int polyCount;
                        for (int i = 0, j = 0; j < lines.size()/9; i+=3, j++) {
                            patches[j].setVertices(vertices[i], vertices[i+1], vertices[i+2]);
                            polyCount++;
                        }
                        generateHemicubeCellCenters(n, patches[0]);
                    }
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

void findAndPrintPatch(Vector givenPoints[]) {
    ThreeDIntersection patchPoint1, patchPoint2, patchPoint3;
//    Vector givenPoints[3] = {point1, point2, point3 };
    Vector projectedPoint;
    Vector pointOnEdges[4];
    Face projectionFace;
    
    patchPoint1.intersect(&givenPoints[0]);
    patchPoint2.intersect(&givenPoints[1]);
    patchPoint3.intersect(&givenPoints[2]);
    
    
    ThreeDIntersection patchPoints[3] = {patchPoint1, patchPoint2, patchPoint3};
    
    
    if (patchPoint1.getIntersectingFace() == patchPoint2.getIntersectingFace() &&
        patchPoint2.getIntersectingFace() == patchPoint3.getIntersectingFace() &&
        patchPoint3.getIntersectingFace() == patchPoint1.getIntersectingFace()) { // all in the same side of cube
        printPatchOnSameSide(patchPoint1, patchPoint2, patchPoint3);
    } else {
        cout << "\nCoordinates of the patch are:\n";
        if (patchPoint1.getIntersectingFace() != patchPoint2.getIntersectingFace() &&
            patchPoint2.getIntersectingFace() != patchPoint3.getIntersectingFace() &&
            patchPoint3.getIntersectingFace() != patchPoint1.getIntersectingFace()) {
  
            // patch is in three different sides of the cube // Need 4 new points: we need to calculate three new points and find another point based on what faces the points fall
            
            projectedPoint = findPointBetweenTwoPoints(Vector(0,0,0), givenPoints[0], patchPoints[2].getIntersectingFace());
            pointOnEdges[0] = findPointBetweenTwoPoints(projectedPoint, patchPoints[2].getVector(), patchPoints[0].getIntersectingFace());

            projectedPoint = findPointBetweenTwoPoints(Vector(0,0,0), givenPoints[1], patchPoints[0].getIntersectingFace());
            pointOnEdges[1] = findPointBetweenTwoPoints(projectedPoint, patchPoints[0].getVector(), patchPoints[1].getIntersectingFace());
            
            projectedPoint = findPointBetweenTwoPoints(Vector(0,0,0), givenPoints[2], patchPoints[1].getIntersectingFace());
            pointOnEdges[2] = findPointBetweenTwoPoints(projectedPoint, patchPoints[1].getVector(), patchPoints[2].getIntersectingFace());

            
            Face faces[3] = {patchPoints[0].getIntersectingFace(),patchPoints[1].getIntersectingFace(), patchPoints[2].getIntersectingFace()};
            
            pointOnEdges[3] = findCommonVertex(faces);
            
            patchPoints[0].print();
            cout << " ––– ";
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
            // patch is in two sides of the cube // Need 2 new points
            int index = findPointToBeProjected(patchPoints);
            if(index >= 0) {
                projectionFace = findProjectionFace(patchPoint1, patchPoint2, patchPoint3);
                projectedPoint = findPointBetweenTwoPoints(Vector(0,0,0), givenPoints[index], projectionFace);
                if (index == 0) {
                    pointOnEdges[1] = findPointBetweenTwoPoints(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = findPointBetweenTwoPoints(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
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
                    pointOnEdges[0] = findPointBetweenTwoPoints(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = findPointBetweenTwoPoints(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
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
                    pointOnEdges[0] = findPointBetweenTwoPoints(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[1] = findPointBetweenTwoPoints(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());

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
//    return patchPoints;
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

Vector findPointBetweenTwoPoints(Vector point1, Vector point2, Face face) { //parametric equation approach
    float t; //parameter
    Vector projectedPoint;
    if (face == TOP_FACE) {
        projectedPoint.setY(1.0);
        t = projectedPoint.getY() - point1.getY()/ point2.getY() - point1.getY();
        projectedPoint.setX((1 - t) * point2.getX() + t * point1.getX());
        projectedPoint.setZ((1 - t) * point2.getZ() + t * point1.getZ());
    } else if(face == RIGHT_FACE) {
        projectedPoint.setX(1.0);
        t = projectedPoint.getX() - point1.getX() / point2.getX() - point1.getX();
        projectedPoint.setY((1 - t) * point2.getY() + t * point1.getY());
        projectedPoint.setZ((1 - t) * point2.getZ() + t * point1.getZ());
    } else if(face == LEFT_FACE) {
        projectedPoint.setX(-1.0);
        t = projectedPoint.getX() - point1.getX() / point2.getX() - point1.getX();
        projectedPoint.setY((1 - t) * point2.getY() + t * point1.getY());
        projectedPoint.setZ((1 - t) * point2.getZ() + t * point1.getZ());
    } else if(face == FRONT_FACE) {
        projectedPoint.setZ(1.0);
        t = projectedPoint.getZ() - point1.getZ() / point2.getZ() - point1.getZ();
        projectedPoint.setY((1 - t) * point2.getY() + t * point1.getY());
        projectedPoint.setX((1-t) * point2.getX() + t * point1.getX());
    } else if(face == BACK_FACE) {
        projectedPoint.setZ(-1.0);
        t = projectedPoint.getZ() - point1.getZ() / point2.getZ() - point1.getZ();
        projectedPoint.setY((1 - t) * point2.getY() + t * point1.getY());
        projectedPoint.setX((1-t) * point2.getX() + t * point1.getX());
    }
    return projectedPoint;
}

Vector findCommonVertex(Face faces[]) {
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

struct hemiDelta {
    float t;
    Vector center;
};

void generateHemicubeCellCenters(int n, Patch patch) {
    
    patch.calcCenter();
    patch.getCenter().print();
    Vector *givenPoints = patch.getVertices();
    Vector normal = calculateNormal(patch.getVertices());
    float d = normal.dot(givenPoints[0]);
    cout << "D : " << d << "\n";
    float dot = 0.0;
    Vector** top_buffer = new Vector*[n];
    Vector** front_buffer = new Vector*[n];
    Vector** back_buffer = new Vector*[n];
    Vector** left_buffer = new Vector*[n];
    Vector** right_buffer = new Vector*[n];
    
    for(int i = 0; i < n; ++i) {
        top_buffer[i] = new Vector[n];
        front_buffer[i] = new Vector[n];
        back_buffer[i] = new Vector[n];
        left_buffer[i] = new Vector[n];
        right_buffer[i] = new Vector[n];
    }
    
    int length = n * (n/2) * 6;
    float t[length];
    Vector *centers = new Vector[length];
    int counter = 0;
    
    float delta;
    delta = 2 / (float)n;
    float x, y, z;
    for ( int i = 0 ; i < 5 ; i++) { // for five faces of hemicube
        switch (i) {
            case TOP_FACE:
            {
                x = -1 + patch.getCenter().getX();
                y = 1 + patch.getCenter().getY();
                z = -1 + patch.getCenter().getZ();
                cout << "TOP FACE\n";
                for (int j = 0; j < n; j++) {
                    for (int k=0; k < n; k++) {
                        top_buffer[j][k] = *new Vector(((x+delta) + x) / 2, y, ((z+delta) + z)/2);
                        cout << "Cell[" << j << "][" << k << "]:\tCenter : " ;;
                        top_buffer[j][k].print();
                        dot = normal.dot(top_buffer[j][k]);
                        cout << setw(20) << "\tDOT Product : " << setprecision(2) << dot;
                        if (dot != 0) {
                            float preT = d / dot;
                            if (preT > 0) {
                                t[counter] = preT;
                                centers[counter] = top_buffer[j][k];
                                cout << setw(10) << "\tT : " << setprecision(2) << t[counter] ;
                                counter++;
                            } else {
                                cout << setw(10) << "\tT : " << setprecision(2) << preT ;
                            }
                        } else {
                            cout << setw(10) << "\tT : doesn't intersect";
                        }
                        cout << "\n";
                        x += delta;

                    }
                    x = -1;
                    z += delta;
                }
            }
                break;
            case FRONT_FACE:
            {
                x = -1 + patch.getCenter().getX();
                y = 1 + patch.getCenter().getY();
                z = 1 + patch.getCenter().getZ();
                cout << "\nFRONT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        front_buffer[j][k] = *new Vector(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        cout << "Cell[" << j << "][" << k << "]:\tCenter : " ;;
                        front_buffer[j][k].print();
                        dot = normal.dot(front_buffer[j][k]);
                        cout << setw(20) << "\tDOT Product : " << setprecision(2) << dot;
                        if (dot != 0) {
                            float preT = d / dot;
                            if (preT > 0) {
                                t[counter] = preT;
                                centers[counter] = front_buffer[j][k];
                                cout << setw(10) << "\tT : " << setprecision(2) << t[counter] ;
                                counter++;
                            } else {
                                cout << setw(10) << "\tT : " << setprecision(2) << preT ;
                            }
                        } else {
                            cout << setw(10) << "\tT : doesn't intersect";
                        }
                        cout << "\n";
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
            }
                break;
            case BACK_FACE:
                x = -1 + patch.getCenter().getX();
                y = 1 + patch.getCenter().getY();
                z = -1 + patch.getCenter().getZ();
                cout << "\nBACK FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        back_buffer[j][k] = *new Vector(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        cout << "Cell[" << j << "][" << k << "]:\tCenter : " ;;
                        back_buffer[j][k].print();
                        dot = normal.dot(back_buffer[j][k]);
                        cout << setw(20) << "\tDOT Product : " << setprecision(2) << dot;
                        if (dot != 0) {
                            float preT = d / dot;
                            if (preT > 0) {
                                t[counter] = preT;
                                centers[counter] = back_buffer[j][k];
                                cout << setw(10) << "\tT : " << setprecision(2) << t[counter] ;
                                counter++;
                            } else {
                                cout << setw(10) << "\tT : " << setprecision(2) << preT ;
                            }
                        } else {
                            cout << setw(10) << "\tT : doesn't intersect";
                        }
                        cout << "\n";
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
                break;
            case LEFT_FACE:
                x = -1 + patch.getCenter().getX();
                y = 1 + patch.getCenter().getY();
                z = 1 + patch.getCenter().getZ();
                cout << "\nLEFT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        left_buffer[j][k] = *new Vector(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        cout << "Cell[" << j << "][" << k << "]:\tCenter : " ;;
                        left_buffer[j][k].print();
                        dot = normal.dot(left_buffer[j][k]);
                        cout << setw(20) << "\tDOT Product : " << setprecision(2) << dot;
                        if (dot != 0) {
                            float preT = d / dot;
                            if (preT > 0) {
                                t[counter] = preT;
                                centers[counter] = left_buffer[j][k];
                                cout << setw(10) << "\tT : " << setprecision(2) << t[counter] ;
                                counter++;
                            } else {
                                cout << setw(10) << "\tT : " << setprecision(2) << preT ;
                            }
                        } else {
                            cout << setw(10) << "\tT : doesn't intersect";
                        }
                        cout << "\n";
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                break;
            case RIGHT_FACE:
                x = 1 + patch.getCenter().getX();
                y = 1 + patch.getCenter().getY();
                z = 1 + patch.getCenter().getZ();
                cout << "\nRIGHT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        right_buffer[j][k] = *new Vector(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        cout << "Cell[" << j << "][" << k << "]:\tCenter : " ;
                        right_buffer[j][k].print();
                        dot = normal.dot(right_buffer[j][k]);
                        cout << setw(20) << "\tDOT Product : " << setprecision(2) << dot;
                        if (dot != 0) {
                            float preT = d / dot;
                            if (preT > 0) {
                                t[counter] = preT;
                                centers[counter] = right_buffer[j][k];
                                cout << setw(10) << "\tT : " << setprecision(2) << t[counter] ;
                                counter++;
                            } else {
                                cout << setw(10) << "\tT : " << setprecision(2) << preT ;
                            }
                        } else {
                            cout << setw(10) << "\tT : doesn't intersect";
                        }
                        cout << "\n";
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                break;
            default:
                break;
        }
    }
    findClosestObject(counter, t, centers, givenPoints);
}

void generatePolygons(double width, double height, double length, string filename) {
    // WE ASSUME THE BOTTOM LEFT CORNER OF THE ROOM IS (0,0,0)
    ofstream objectFile;
    objectFile.open (filename);
    double factor = (4 * (( width * height) + (length * height) + (width * length))) / POLY_COUNT ;
    double DELTA = floor(sqrt(factor) * 100) / 100;
    if (objectFile) {
        cout << "File Generated. Now generating polygons\n";
        // FRONT AND BACK
        int i = 1; //polycount
        for (float z = 0; z <= length; z += length) {
            for (float y = 0; y < height; y += DELTA) {
                for (float x =0; x < width; x+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    i++;
                }
            }
        }
        // LEFT AND RIGHT
        for (float x = 0; x <= width; x += width) {
            for (float y = 0; y < height; y += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                    objectFile << x << " " << y + DELTA << " " << z + DELTA << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                }
            }
        }
        // TOP AND BOTTTOM
        for (float y = 0; y <= height; y += height) {
            for (float x = 0; x < width; x += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA  << " " << y << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                }
            }
        }
        cout << "Polygon count:" << i << " for " << filename << "\n";
        objectFile.close();
        cout << "Done Generating Polygons\n";
    } else {
        cout << "Error Generating object file. Try again\n";
    }
    
}

Vector calculateNormal(Vector givenPoints[]) {
    Vector a = givenPoints[1] - givenPoints[0];
    Vector b = givenPoints[2] - givenPoints[1];
    cout << "Vector (v2 - v1) : "; a.print(); cout << "\n";
    cout << "Vector (v3 - v2) : "; b.print(); cout << "\n";
    Vector normal = a.cross(b);
    cout << "Normal of the plane ( a X b) : "; normal.print(); cout << "\n";
    return normal;
}

void findClosestObject(int length, float t[], Vector centers[], Vector givenPoints[]) {
    float tMin = t[0];
    int minIndex = 0;
    
    for ( int i = 1;  i < length;  i++ ) {
        if ( t[i] > 0 &&
            t[i] < tMin ) {
            tMin = t[i] ;
            minIndex = i;
        }
    }
    cout << tMin << ":" << minIndex;
    Vector testPoint;
    testPoint.setX(tMin * centers[minIndex].getX());
    testPoint.setY(tMin * centers[minIndex].getY());
    testPoint.setZ(tMin * centers[minIndex].getZ());

    cout << pointInTriangle(testPoint, givenPoints[0], givenPoints[1], givenPoints[2]);
}

bool onSameSide(Vector p, Vector a, Vector b, Vector c) {
    Vector cb = c - b;
    Vector pb = p - b;
    Vector pa = p - a;
    Vector cp1 = cb.cross(pb);
    Vector cp2 = cb.cross(pa);
    if (cp1.dot(cp2) >= 0)
        return true;
    else
        return false;
}

bool pointInTriangle(Vector p, Vector a, Vector b, Vector c) {
    if (onSameSide(p, a, b, c) &&
        onSameSide(p, b, a, c) &&
        onSameSide(p, c, a, b))
        return true;
    else
        return false;
}