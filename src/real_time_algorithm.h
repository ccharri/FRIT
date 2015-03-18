#ifndef _REAL_TIME_ALGORITHM_H_
#define _REAL_TIME_ALGORITHM_H_

#include "types.h"

class RealTimeAlgorithm {
public:
  virtual path_t search(graph_t& graph, h_func_t heuristic, const vertex_t& goal) = 0;
};

#endif
