
#include "ideal_tree.h"

IT_RTAA::IT_RTAA(Map& graph, float(*heuristic)(node_t, node_t)) : RTAA(graph, heuristic) {

	m_colors = new int*[graph.getWidth()];
	for (int i = 0; i < graph.getWidth(); ++i)
	{
		m_colors[i] = new int[graph.getHeight()];
	}

}

IT_RTAA::~IT_RTAA() {
	for (int i = 0; i < getMap().getWidth(); ++i)
	{
		delete m_colors[i];
	}
	delete m_colors;
	m_colors = 0;
}

bool IT_RTAA::isGoalNode(const node_t& node)
{
	node_t next = node;
	while (next != getGoal()) {
		m_colors[next.first][next.second] = m_color;
		node_t parent = getParent(next);
		if (parent == FAIL_NODE || m_colors[parent.first][parent.second] == m_color) return false;
		next = parent;
	}
	return true;
}
