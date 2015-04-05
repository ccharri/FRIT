#ifndef _REAL_TIME_ALGORITHM_H_
#define _REAL_TIME_ALGORITHM_H_

#include <vector>

#include "map.h"
#include "node.h"

class RealTimeAlgorithm {
public:
	virtual void setStart(node_t& start) = 0;
	virtual void setEnd(node_t& end) = 0;

    virtual std::vector<node_t> search(Map& graph,
                            float (*heuristic)(node_t, node_t),
                            const node_t& goal) = 0;
};

#endif
