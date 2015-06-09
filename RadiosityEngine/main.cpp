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

int main(int argc, const char * argv[]) {
    Point point, secondPoint;
    Intersection intersection, secondIntersection;
    Vector vertex, vertex1, vertex2;
    ThreeDIntersection threeDIntersection, patch_point1, patch_point2, patch_point3;
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
                    threeDIntersection.simplePrint();
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
                    patch_point1.intersect(&vertex);
                    patch_point2.intersect(&vertex1);
                    patch_point3.intersect(&vertex2);
                    patch_point1.simplePrint();
                    patch_point2.simplePrint();
                    patch_point3.simplePrint();
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