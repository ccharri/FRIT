#ifndef _REAL_TIME_ALGORITHM_H_
#define _REAL_TIME_ALGORITHM_H_

#include <list>

#include "map.h"
#include "node.h"

class RealTimeAlgorithm {
 public:
  virtual ~RealTimeAlgorithm(){};

  virtual void setStart(node_t start, bool refreshHeuristics) = 0;
  virtual void setEnd(node_t end) = 0;

  virtual std::list<node_t> search(Map& graph,
                                   float (*heuristic)(node_t, node_t)) = 0;
};

#endif
