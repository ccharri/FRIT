#include "RTAA.h"


#include <assert.h>
#include "Parameters.h"

using std::vector;

RTAA::RTAA(graph_t& graph) : 
			m_gValues(0), m_hValues(0), m_current(FAIL_NODE), m_next(FAIL_NODE), m_graph(&graph)
			, m_open(std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, NodeComparison>(this)) {
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
		m_directions = (dir_t**)new QUARTILE_DIRECTION*[graph.getWidth()];
		for (int i = 0; i < graph.getWidth(); ++i)
		{
			((QUARTILE_DIRECTION**)m_directions)[i] = new QUARTILE_DIRECTION[graph.getHeight()];
		}
		break;
	case OCTILE:
		m_directions = (dir_t**)new OCTILE_DIRECTION*[graph.getWidth()];
		for (int i = 0; i < graph.getWidth(); ++i)
		{
			((OCTILE_DIRECTION**)m_directions)[i] = new OCTILE_DIRECTION[graph.getHeight()];
		}
		break;
	default:
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
	default:
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

void RTAA::AStar(h_func_t heuristic, const node_t& goal) {
	int Expands = LookAhead;
	while (Expands-- > 0) {
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
		dir_t* neighborDirs = (m_graph->getType == OCTILE) ? VALID_OCTILE_DIRECTIONS : VALID_QUARTILE_DIRECTIONS;
		for (int i = 0; i < m_graph->numNeighbors(); ++i) {
			node_t neighbor = m_graph->getNeighbor(top.first, top.second, neighborDirs[i]);
			if (neighbor == FAIL_NODE) continue;
			if (m_closed.find(neighbor) != m_closed.end()) continue;
			float hVal = heuristic(top, goal);
			float tentativeScore = m_gValues[top.first][top.second] + hVal;
			if (/*neighbor not in openset  || */tentativeScore < m_gValues[top.first][top.second])
			{
				m_directions[neighbor.first][neighbor.second] = getOppositeDir(neighborDirs[i]);
				m_gValues[neighbor.first][neighbor.second] = tentativeScore;
				m_hValues[neighbor.first][neighbor.second] = hVal;
				/*
					if(neighbor not in openset)
						m_open.push(neighbor);
				*/
			}

		}
	}
}

path_t RTAA::search(graph_t& graph, h_func_t heuristic, const node_t& goal) {
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
		AStar(heuristic, goal);
		//m_next == 0 is failure
		if (m_next != FAIL_NODE) return path;
		for (auto it = m_closed.begin(); it != m_closed.end(); it++) {
			m_hValues[it->first][it->second] = m_gValues[m_current.first][m_current.second] + m_hValues[m_current.first][m_current.second] - m_gValues[it->first][it->second];
		}
		int movements = MoveMax;
		while (m_current != m_next && movements > 0) {
			int cost = 

			movements--;
		}
	}

	return path;
}