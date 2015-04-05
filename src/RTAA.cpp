#include "RTAA.h"


#include <assert.h>
#include <algorithm>

#include "Parameters.h"

using std::vector;
using std::pair;

RTAA::RTAA(Map& graph, float (*heuristic)(node_t, node_t)) : m_heuristic(heuristic),
			m_gValues(0), m_hValues(0), m_current(FAIL_NODE), m_next(FAIL_NODE), m_end(FAIL_NODE), m_graph(&graph)
			, m_open(IterPrioQueue<node_t, NodeComparison>(this)) {
	assert(graph.getType() != UNKNOWN);

	m_gValues = new float*[graph.getWidth()];
	for (int i = 0; i < graph.getWidth(); ++i)
	{
		m_gValues[i] = new float[graph.getHeight()];
	}

	m_hValues = new float*[graph.getWidth()];
	for (int i = 0; i < graph.getWidth(); ++i)
	{
		m_hValues[i] = new float[graph.getHeight()];
	}
	switch (graph.getType()){
	case QUARTILE:
		m_directions = (char**)new QUARTILE_DIRECTION*[graph.getWidth()];
		for (int i = 0; i < graph.getWidth(); ++i)
		{
			((QUARTILE_DIRECTION**)m_directions)[i] = new QUARTILE_DIRECTION[graph.getHeight()];
		}
		break;
	case OCTILE:
		m_directions = (char**)new OCTILE_DIRECTION*[graph.getWidth()];
		for (int i = 0; i < graph.getWidth(); ++i)
		{
			((OCTILE_DIRECTION**)m_directions)[i] = new OCTILE_DIRECTION[graph.getHeight()];
		}
		break;
        default: break;
	}
}

RTAA::~RTAA() {
	for (int i = 0; i < m_graph->getWidth(); ++i)
	{
		delete m_gValues[i];
	}
	delete m_gValues;
	m_gValues = 0;

	for (int i = 0; i < m_graph->getWidth(); ++i)
	{
		delete m_hValues[i];
	}
	delete m_hValues;
	m_hValues = 0;

	switch (m_graph->getType()) {
	case QUARTILE:
		for (int i = 0; i < m_graph->getWidth(); ++i)
		{
			delete ((QUARTILE_DIRECTION**)m_directions)[i];
		}
		break;
	case OCTILE:
		for (int i = 0; i < m_graph->getWidth(); ++i)
		{
			delete ((OCTILE_DIRECTION**)m_directions)[i];
		}
		break;
        default: break;
	}
	delete m_directions;
	m_directions = 0;
}

void RTAA::setStart(node_t& start) {
	//Set current location
	m_current = start;

	//Clear current state.
	while (!m_open.empty()) m_open.pop();
	m_closed.clear();
	m_next = FAIL_NODE;

	//Initialize variables
	m_open.push(start);

	for (int x = 0; x < m_graph->getWidth(); ++x) {
		for (int y = 0; y < m_graph->getHeight(); ++y) {
			m_gValues[x][y] = 0;
			m_hValues[x][y] = 0;
		}
	}

}

void RTAA::setEnd(node_t& end) {
	m_end = end;

	for (int x = 0; x < m_graph->getWidth(); ++x) {
		for (int y = 0; y < m_graph->getHeight(); ++y) {
			m_gValues[x][y] = 0;
			m_hValues[x][y] = m_heuristic(node_t(x,y), m_end);
		}
	}
}

void RTAA::AStar(const node_t& goal) {
	int expands = LookAhead;
	while (expands-- > 0) {
		//If nothing left to expand, set error flag and return.
		if (m_open.empty()) {
			m_next = FAIL_NODE;
			return;
		}

		node_t top = m_open.top();
		m_open.pop();

		if (top == goal) {
			m_next = top;
			return;
		}

		m_closed.insert(top);
		char* neighborDirs = (m_graph->getType() == OCTILE) ? VALID_OCTILE_DIRECTIONS : VALID_QUARTILE_DIRECTIONS;
		for (int i = 0; i < m_graph->numNeighbors(); ++i) {
			node_t neighbor = m_graph->getNeighbor(top.first, top.second, neighborDirs[i]);
			if (neighbor == FAIL_NODE) continue;
			if (m_closed.find(neighbor) != m_closed.end()) continue;
			float tentativeScore = m_gValues[top.first][top.second] + m_hValues[top.first][top.second];
			if (find(m_open.begin(), m_open.end(), neighbor) == m_open.end()  || tentativeScore < m_gValues[top.first][top.second])
			{
				m_directions[neighbor.first][neighbor.second] = getOppositeDir(neighborDirs[i]);
				m_gValues[neighbor.first][neighbor.second] = tentativeScore;
                if(find(m_open.begin(), m_open.end(), neighbor) != m_open.end())
                {
                    m_open.push(neighbor);
                }
			}

		}
	}
}

vector<node_t> RTAA::search(Map& graph, float (*heuristic)(node_t, node_t), const node_t& goal) {
	vector<node_t> path;

	/*
procedure realtime adaptive astar():
{01} while (scurr 6∈ GOAL) do
{02} lookahead := any desired integer greater than zero;
{03} astar();
{04} if s¯ = FAILURE then
{05}	return FAILURE;
{06} for all s ∈ CLOSED do
{07}	 h[s] := g[s¯] + h[s¯] − g[s];
{08} movements := any desired integer greater than zero;
{09} while (scurr 6= s¯ AND movements > 0) do
{10}	a := the action in A(scurr) on the cost-minimal trajectory from scurr to s¯;
{11}	scurr := succ(scurr, a);
{12}	movements := movements − 1;
{13}	for any desired number of times (including zero) do
{14}		increase any desired c[s, a] where s ∈ S and a ∈ A(s);
{15}	if any increased c[s, a] is on the cost-minimal trajectory from scurr to s¯ then
{16}		break;
{17} return SUCCESS;
*/

	while (m_current != goal) {
		AStar(goal);
		//m_next == 0 is failure
		if (m_next != FAIL_NODE) return path;
		for (auto it = m_closed.begin(); it != m_closed.end(); it++) {
			m_hValues[it->first][it->second] = m_gValues[m_current.first][m_current.second] + m_hValues[m_current.first][m_current.second] - m_gValues[it->first][it->second];
		}
		int movements = MoveMax;
		while (m_current != m_next && movements > 0) {
            int cost = 0;

			movements--;
		}
	}

	return path;
}