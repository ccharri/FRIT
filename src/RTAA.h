#ifndef _RTAA_H_
#define _RTAA_H_

#include <vector>
#include <queue>
#include <set>

#include "real_time_algorithm.h"
#include "Directions.h"

class RTAA : public RealTimeAlgorithm {
public:
	RTAA(graph_t& graph);
	~RTAA();

	void setStart(node_t& start) override;

	path_t search(graph_t& graph, h_func_t heuristic, const node_t& goal) override;
private:
	void AStar(h_func_t heuristic, const node_t& goal);
	path_t getResult(const node_t& goal);

	float** m_gValues;
	float** m_hValues;
	dir_t** m_directions;
	graph_t* m_graph;
	node_t m_current;
	node_t m_next;
	std::set<node_t> m_closed;

	friend class NodeComparison;

	class NodeComparison {
	public:
		NodeComparison(const RTAA* rta, const bool& reverse = true) :m_rta(rta), m_reverse(reverse) {}

		bool operator() (const node_t& lhs, const node_t& rhs) const {
			if (m_reverse)
				return (m_rta->m_gValues[lhs.first][lhs.second] + m_rta->m_hValues[lhs.first][lhs.second]) <
					(m_rta->m_gValues[rhs.first][rhs.second] + m_rta->m_hValues[rhs.first][rhs.second]);
			else
				return (m_rta->m_gValues[lhs.first][lhs.second] + m_rta->m_hValues[lhs.first][lhs.second]) >
				(m_rta->m_gValues[rhs.first][rhs.second] + m_rta->m_hValues[rhs.first][rhs.second]);
		}
	private:
		const RTAA* m_rta;
		const bool m_reverse;
	};
	
	std::priority_queue<node_t, std::vector<node_t>, NodeComparison > m_open;
};

#endif