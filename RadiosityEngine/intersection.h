//
//  intersection.h
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef LineIntersection_intersection_h
#define LineIntersection_intersection_h

#include <string>
#include "point.h"

using namespace std;

enum Side {LEFT, RIGHT, TOP, NONE};

class Intersection {
private:
    Point _point;
    Side _side;
public:
    Intersection() {
        _side = NONE;
    }

    Intersection(Point point, Side side) {
        _point = point;
        _side = side;
    }

    Point getPoint() {
        return _point;
    }
    
    Side getSide() {
        return _side;
    }
    
    string getHumanReadableSide() {
        string side;
        switch (_side) {
            case 0:
                side = "Left";
                break;
            case 1:
                side = "Right";
                break;
            case 2:
                side = "Top";
                break;
            case 3:
                side = "Error:Invalid Coordinate provided";
                break;
            default:
                side = "Invalid";
                break;
        }
        return side;
    }

    void setPoint(Point point) {
        _point = point;
    }

    void setSide(Side side) {
        _side = side;
    }
    
    
    void intersect(Point *pointTwo) {
        float t;
        if(pointTwo->getY() >= 0) {
            if(pointTwo->getY() > (-(pointTwo->getX())) &&
               pointTwo->getY() > pointTwo->getX()) {  // intersection on top
                _point.setY(1.0);
                t = _point.getY() / pointTwo->getY();
                _point.setX(t * pointTwo->getX());
                _side = TOP;
            } else if(pointTwo->getY() <= (-(pointTwo->getX()))) { //intersection on left
                _point.setX(-1.0);
                t = _point.getX() / pointTwo->getX();
                _point.setY(t * pointTwo->getY());
                _side = LEFT;
            } else if(pointTwo->getY() <= pointTwo->getX()) { //intersection on right
                _point.setX(1.0);
                t = _point.getX() / pointTwo->getX();
                _point.setY(t * pointTwo->getY());
                _side = RIGHT;
            }
        } else {
            _side = NONE;
        }
    }
    
    void simplePrint() {
        cout << "(" << _point.getX() << "," << _point.getY() << ")";
        cout << " on " << getHumanReadableSide() << "\n\n";
    }
    
    void print() {
        if(_side == 0) {  //left
            _point.print();
            cout << ",(-1,1)";
        } else if(_side == 1) { //right
            _point.print();
            cout << ",(1,1)";
        } else if(_side == 2 && _point.getX() < 0) { // on the negative side of top
            cout << "(-1,1),";
            _point.print();
        } else if(_side == 2 && _point.getX() >= 0) { // on the positive side of top
            _point.print();
            cout << ",(1,1)";
        }
        cout << " on " << getHumanReadableSide();
    }
};

#endif
