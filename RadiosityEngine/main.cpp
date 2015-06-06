//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>

#include "point.h"
#include "intersection.h"

using namespace std;

Intersection findIntersection(Point *);

int main(int argc, const char * argv[]) {
    Point point1;
    Intersection intersection;
    char c;
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions for 2D:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n\n";
    while(1) {
        cout << "Options:\n";
        cout << "(1)Enter Coordinates for a 2D Point\n";
        cout << "(Q)Quit\n";
        cin >> c;
        if (cin.get() != '\n') // Look at the next character
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input stream
            cout << "Invalid Option\n\n";
        } else {
            switch (c) {
                case '1':
                {
                    cout << "Enter Coordinates for the point!\n";
                    point1.readInput();
                    if (point1.getY() > 0 && (point1.getX() > 1 || point1.getX() < -1) ) {
                        intersection.intersect(&point1);
                        cout << "Coordinates of intersecting segment are:";
                        cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ")";
                        cout << " on " << intersection.getHumanReadableSide() << "\n\n";
                    } else {
                        cout << "Invalid Coordinates provided.\nx must be greater than 1 and less than -1.\ny must be greater than 1.\nPlease try again.\n\n";
                    }
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