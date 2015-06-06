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
};

#endif
