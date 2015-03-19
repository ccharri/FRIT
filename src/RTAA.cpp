#include "RTAA.h"
#include "Parameters.h"

using std::vector;

RTAA::RTAA(graph_t& graph) : m_gValues(0), m_hValues(0), m_current(0), m_next(0), m_graph(&graph) {
	m_gValues = new int*[graph.getWidth()];
	for (int i = 0; i < graph.getWidth(); ++i)
	{
		m_gValues[i] = new int[graph.getHeight()];
	}

	m_hValues = new int*[graph.getWidth()];
	for (int i = 0; i < graph.getWidth(); ++i)
	{
		m_hValues[i] = new int[graph.getHeight()];
	}
	switch (graph.getType()){
	case QUARTILE:
		m_directions = (void**)new QUARTILE_DIRECTION*[graph.getWidth()];
		for (int i = 0; i < graph.getWidth(); ++i)
		{
			((QUARTILE_DIRECTION**)m_directions)[i] = new QUARTILE_DIRECTION[graph.getHeight()];
		}
		break;
	case OCTILE:
		m_directions = (void**)new OCTILE_DIRECTION*[graph.getWidth()];
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

void RTAA::astar() {

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

	while ((*m_current) != goal) {
		astar();
		//m_next == 0 is failure
		if (!m_next) return path;
		for (auto it = m_closed.begin(); it != m_closed.end(); it++) {
			m_hValues[it->x][it->y] = m_gValues[m_current->x][m_current->y] + m_hValues[m_current->x][m_current->y] - m_gValues[it->x][it->y];
		}
		int movements = MoveMax;
		while ((*m_current) != (*m_next) && movements > 0) {
			int cost = 

			movements--;
		}
	}

	return path;
}