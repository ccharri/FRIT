#ifndef _RTAA_H_
#define _RTAA_H_

#include <vector>

#include "real_time_algorithm.h"
#include "Directions.h"

class RTAA : public RealTimeAlgorithm {
public:
	RTAA(graph_t& graph);
	~RTAA();

	void setStart(node_t& start) override { m_current = &start; }

	path_t search(graph_t& graph, h_func_t heuristic, const node_t& goal) override;
private:
	void astar();
	int** m_gValues;
	int** m_hValues;
	void** m_directions;
	graph_t* m_graph;
	node_t* m_current;
	node_t* m_next;
	std::vector<node_t> m_closed;
	std::vector<node_t> m_open;
};

#endif