#ifndef _RTAA_H_
#define _RTAA_H_

#include <vector>
#include <queue>
#include <set>

#include "real_time_algorithm.h"
#include "IterPrioQueue.h"
#include "Directions.h"
#include "node.h"
#include "map.h"

class RTAA : public RealTimeAlgorithm {
public:
    RTAA(Map& graph, float (*heuristic)(node_t, node_t));
	~RTAA();

    void setStart(node_t& start) override;
    void setEnd(node_t& end) override;

    std::vector<node_t> search(Map& graph, float (*heuristic)(node_t, node_t), const node_t& goal) override;
protected:
    bool isGoalNode(const node_t& node);
private:
    void AStar(const node_t& goal);
    std::vector<node_t> getResult(const node_t& goal);

	float** m_gValues;
	float** m_hValues;
	char** m_directions;
	Map* m_graph;
    node_t m_current;
    node_t m_next;
    node_t m_end;
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

    float (*m_heuristic)(node_t, node_t);
	
    IterPrioQueue<node_t, NodeComparison > m_open;
};

#endif