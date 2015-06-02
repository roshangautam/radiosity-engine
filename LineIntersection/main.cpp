//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include "point.h"
#include "intersection.h"

using namespace std;

void scanPoint(Point *);
Intersection findIntersection(Point *);
int main(int argc, const char * argv[]) {
    Point origin, point1, point2;
    char c;
    while(1) {
        cout << "Line Intersection Program - Solid Modeling CS6413 !!! \n";
        cout << "Assumptions:\n";
        cout << "1. Light Source Origin is always (0,0)\n";
        cout << "2. Height of half square = 1 i.e y = 1 \n";
        cout << "3. Left End of half square from origin = -1 i.e x = -1 \n";
        cout << "4. Right End of half square from origin = 1 i.e x = 1 \n";
        
        cout << "Menu\n";
        cout << "Enter Coordinates(E)\n";
        cout << "Quit (Q)\n";
        cin >> c;
        switch (c) {
            case 'E':
            case 'e':
                {
                    cout << "Enter Coordinates for first point!\n";
                    scanPoint(&point1);
                    Intersection intersection = findIntersection(&point1);
                    cout << "The given point intersects half square at: (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") on " << (intersection.getSide() == 0 ? "Left" : (intersection.getSide() == 1 ? "Right" : "Top")) << "\n";
                    cout << "Enter Coordinates for second point!\n";
                    scanPoint(&point2);
                    intersection = findIntersection(&point2);
                    cout << "The given point intersects half square at: (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") on " << (intersection.getSide() == 0 ? "Left" : (intersection.getSide() == 1 ? "Right" : "Top")) << "\n";
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
    cout << "Given Point is:(" << subject->getX() << "," << subject->getY() << ")\n";
    if(subject->getY() > 0) {
        if(subject->getX() < 0) { // if x is negative
            if (subject->getY() < subject->getAbsoluteX()) { // left side intersection
                intersection.setSide(LEFT);
            } else if(subject->getY() > subject->getAbsoluteX()) { //top intersection in negative abscissa
                intersection.setSide(TOP);
            }
        } else { // if x is positive
            if (subject->getY() < subject->getX()) { // right side intersection
                intersection.setSide(RIGHT);
            } else if(subject->getY() > subject->getX()) { //top intersection in positive abscissa
                intersection.setSide(TOP);
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