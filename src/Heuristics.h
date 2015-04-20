//
//  Heuristics.h
//  FRITRecreation
//
//  Created by Christopher Harris on 4/19/15.
//  Copyright (c) 2015 University of Michigan. All rights reserved.
//

#ifndef FRITRecreation_Heuristics_h
#define FRITRecreation_Heuristics_h

#include <math.h>
#include "Parameters.h"

static float euclideanHeuristic(node_t a, node_t b) {
    int xdif = a.first - b.first;
    int ydif = a.second - b.second;
    return sqrtf(xdif * xdif + ydif * ydif);
}

static float octileHeuristic(node_t a, node_t b) {
    int fxdif = std::abs(a.first - b.first);
    int fydif = std::abs(a.second - b.second);
    return fmaxf(fxdif, fydif) + (CornerCost - BorderingCost)*fminf(fxdif, fydif);
}

#endif
