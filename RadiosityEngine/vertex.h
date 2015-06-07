//
//  vertex.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/6/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_vertex_h
#define RadiosityEngine_vertex_h

class Vertex {
    float _x, _y, _z;
public:
    Vertex() {
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
    }
    
    Vertex(float abscissa, float ordinate, float applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    void setCoordinates(float abscissa, float ordinate, float applicate) {
        _x = abscissa;
        _y = ordinate;
        _z = applicate;
    }
    
    float getX() {
        return _x;
    }
    
    float getY() {
        return _y;
    }
    
    float getZ() {
        return _z;
    }
    
    void setX(float abscissa) {
        _x = abscissa;
    }
    
    void setY(float ordinate) {
        _y = ordinate;
    }
    
    void setZ(float applicate) {
        _z = applicate;
    }
    
    //calculate and return dot product
    float dot(Vertex vec) {
        return _x * vec.getX() + _y * vec.getY() + _z * vec.getZ();
    }
    
    //calculate and return cross product
    Vertex cross(Vertex vec) {
        return Vertex(_y * vec.getZ() - _z * vec.getY(),
                      _z * vec.getX() - _x * vec.getZ(),
                      _x * vec.getY() - _y * vec.getX());
    }
    
    Vertex unit(Vertex vec) {
        return Vertex(vec.getX() / sqrt(dot(Vertex(_x,_y,_z))),
                      vec.getY() / sqrt(dot(Vertex(_x,_y,_z))),
                      vec.getz() / sqrt(dot(Vertex(_x,_y,_z))));
    }

    void read() {
        cout << "Enter x:";
        cin >> _x;
        cout << "Enter y:";
        cin >> _y;
        cout << "Enter z:";
        cin >> _z;
    }
    
    void print() {
        cout << "(" << _x << "," << _y << "," << _z << ")";
    }
};

#endif
