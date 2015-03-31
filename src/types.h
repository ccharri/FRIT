#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>

#include "map.h"
#include "real_time_algorithm.h"
#include "node.h"

typedef char color_t;   //Type of 'color' used for ideal_tree.
typedef Map graph_t;   //Type of 'graph'.
typedef std::vector<node_t> path_t;    //Type of 'path'.
typedef int (*h_func_t)(node_t start, node_t end);  //Type of heuristic function.
typedef RealTimeAlgorithm r_func_t; //Type of realtime algorithm.

#endif
