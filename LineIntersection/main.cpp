//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include "point.h"
#include <string>
#include "intersection.h"

using namespace std;

void scanPoint(Point *);
Intersection findIntersection(Point *);

int main(int argc, const char * argv[]) {
    Point origin, point1, point2;
    Intersection intersection, intersection1;
    char c;
    cout << "Line Intersection Program - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n\n";
    cout << "Options:\n";
    while(1) {
        cout << "(E)Enter Coordinates\n";
        cout << "(Q)Quit\n";
        cin >> c;
        switch (c) {
            case 'E':
            case 'e':
                {
                    cout << "Enter Coordinates for first point!\n";
                    scanPoint(&point1);
                    cout << "Enter Coordinates for second point!\n";
                    scanPoint(&point2);
                    intersection = findIntersection(&point1);
                    intersection1 = findIntersection(&point2);
                    if((intersection.getSide() == "Negative Top" && intersection1.getSide() == "Positive Top") || (intersection.getSide() == "Positive Top" && intersection1.getSide() == "Negative Top")) {
                        cout << "Coordinates of intersecting segment are:(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") on " << intersection.getSide() << " and " << intersection1.getSide() << "\n";
                    } else {
                        cout << "Coordinates of intersecting segment are:";
                        if(intersection.getSide() == "Left") {
                            cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (-1,1)";
                        } else if(intersection.getSide() == "Right") {
                            cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                        } else if(intersection.getSide() == "Negative Top") {
                            cout << "(-1,1) , (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ")";
                        } else if(intersection.getSide() == "Positive Top") {
                            cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (-1,1)";
                        }
                        cout << " on " << intersection.getSide() << " and ";
                        if(intersection1.getSide() == "Left") {
                            cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (-1,1)";
                        } else if(intersection1.getSide() == "Right") {
                            cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (1,1)";
                        } else if(intersection1.getSide() == "Negative Top") {
                            cout << "(-1,1) , (" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ")";
                        } else if(intersection1.getSide() == "Positive Top") {
                            cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (-1,1)";
                        }
                        cout << " on " << intersection1.getSide() << "\n";
                    }
                }
                break;
            case 'q':
            case 'Q':
                cout << "Thank You. --- End of Program ---\n";
                exit(1);
            default:
                cout << "Invalid option\n";
                break;
        }
    }
    return 0;
}

void scanPoint(Point *point) {
    float x = 0, y = 0;
    cout << "Enter x:";
    cin >> x;
    cout << "Enter y:";
    cin >> y;
    point->setX(x);
    point->setY(y);
}

Intersection findIntersection(Point *subject) {
    Intersection intersection;
    if(subject->getY() > 0) {
        if(subject->getX() < 0) { // if x is negative
            if (subject->getY() < subject->getAbsoluteX()) { // left side intersection
                intersection.setSide(LEFT);
            } else if(subject->getY() > subject->getAbsoluteX()) { //top intersection in negative abscissa
                intersection.setSide(NEGATIVE_TOP);
            }
        } else { // if x is positive
            if (subject->getY() < subject->getX()) { // right side intersection
                intersection.setSide(RIGHT);
            } else if(subject->getY() > subject->getX()) { //top intersection in positive abscissa
                intersection.setSide(POSITIVE_TOP);
            }
        }
        float y = 1.0;
        float x = y / (subject->getY()/subject->getX());
        Point point;
        point.setX(x);
        point.setY(y);
        intersection.setPoint(point);
    }
    return intersection;
}