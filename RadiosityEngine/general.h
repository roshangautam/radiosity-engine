//
//  hemicube.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/11/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_general_h
#define RadiosityEngine_general_h

#define PI              3.141592654
#define MIN_VALUE       1.0e-10         // Minimum value
#define MAX_VALUE       1.0e10          // Maximum value

int DEBUG_MODE = 1;  // 1 – PRINT DEBUG OUTPUT , 0 – DO NOT PRINT DEBUG OUTPUT

int POLY_COUNT = 10000;

static int HemiFaceNum = 5;

enum HemiCubeFace {
    TOP_FACE = 0,  // y = +1.0
    LEFT_FACE = 1, // x = -1.0
    RIGHT_FACE = 2, // x = +1.0
    FRONT_FACE = 3, // z = +1.0
    BACK_FACE = 4 // z = -1.0
};

inline double RadToDeg( double r )
{ return r * 180.0 / PI; }

inline double DegToRad( double d )
{ return d * PI / 180.0; }

inline double GetNormRand()
{ return (double) rand() / (double) RAND_MAX; }

#endif
