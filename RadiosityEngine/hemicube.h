//
//  hemicube.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/11/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#ifndef RadiosityEngine_hemicube_h
#define RadiosityEngine_hemicube_h

int DEBUG_MODE = 1;

enum HemiCubeFace {
    TOP_FACE = 0,  // y = +1.0
    LEFT_FACE = 1, // x = -1.0
    RIGHT_FACE = 2, // x = +1.0
    FRONT_FACE = 3, // z = +1.0
    BACK_FACE = 4 // z = -1.0
};

#endif
