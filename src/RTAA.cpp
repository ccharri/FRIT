#include "RTAA.h"

#include <assert.h>
#include <algorithm>
#include <float.h>

#include "Parameters.h"

using std::list;
using std::pair;

RTAA::RTAA(Map &graph, float (*heuristic)(node_t, node_t))
    : m_heuristic(heuristic),
      m_gValues(0),
      m_hValues(0),
      m_current(FAIL_NODE),
      m_end(FAIL_NODE),
      m_start(FAIL_NODE),
      m_graph(&graph),
      m_open(
          IterPrioQueue<node_t, NodeComparison>(NodeComparison(this, false))) {
  assert(graph.getType() != UNKNOWN);

  m_gValues = new float *[graph.getWidth()];
  for (int i = 0; i < graph.getWidth(); ++i) {
    m_gValues[i] = new float[graph.getHeight()];
  }

  m_hValues = new float *[graph.getWidth()];
  for (int i = 0; i < graph.getWidth(); ++i) {
    m_hValues[i] = new float[graph.getHeight()];
  }
  switch (graph.getType()) {
    case QUARTILE:
      m_directions = (char **)new QUARTILE_DIRECTION *[graph.getWidth()];
      for (int i = 0; i < graph.getWidth(); ++i) {
        ((QUARTILE_DIRECTION **)m_directions)[i] =
            new QUARTILE_DIRECTION[graph.getHeight()];
      }
      break;
    case OCTILE:
      m_directions = (char **)new OCTILE_DIRECTION *[graph.getWidth()];
      for (int i = 0; i < graph.getWidth(); ++i) {
        ((OCTILE_DIRECTION **)m_directions)[i] =
            new OCTILE_DIRECTION[graph.getHeight()];
      }
      break;
    default:
      break;
  }
}

RTAA::~RTAA() {
  for (int i = 0; i < m_graph->getWidth(); ++i) {
    delete m_gValues[i];
  }
  delete m_gValues;
  m_gValues = 0;

  for (int i = 0; i < m_graph->getWidth(); ++i) {
    delete m_hValues[i];
  }
  delete m_hValues;
  m_hValues = 0;

  switch (m_graph->getType()) {
    case QUARTILE:
      for (int i = 0; i < m_graph->getWidth(); ++i) {
        delete ((QUARTILE_DIRECTION **)m_directions)[i];
      }
      break;
    case OCTILE:
      for (int i = 0; i < m_graph->getWidth(); ++i) {
        delete ((OCTILE_DIRECTION **)m_directions)[i];
      }
      break;
    default:
      break;
  }
  delete m_directions;
  m_directions = 0;
}

void RTAA::setStart(node_t start, bool refreshHeuristics) {
      assert(m_graph->isPathable(m_graph->getNodeType(start.first,
      start.second)));

  // Set current location
  m_start = m_current = start;

  m_path.clear();
  m_path.push_back(start);

  // Clear current state.
  while (!m_open.empty()) m_open.pop();
  m_closed.clear();

  // Initialize variables
  for (int x = 0; x < m_graph->getWidth(); ++x) {
    for (int y = 0; y < m_graph->getHeight(); ++y) {
      m_gValues[x][y] = FLT_MAX;
      if(refreshHeuristics)m_hValues[x][y] = m_heuristic(node_t(x, y), m_end);
      m_directions[x][y] = 0;
    }
  }
  m_gValues[m_start.first][m_start.second] = 0;

  m_open.push(start);
}

void RTAA::setEnd(node_t end) {
      assert(m_graph->isPathable(m_graph->getNodeType(end.first,
      end.second)));
  m_end = end;

  for (int x = 0; x < m_graph->getWidth(); ++x) {
    for (int y = 0; y < m_graph->getHeight(); ++y) {
      m_gValues[x][y] = FLT_MAX;
      m_hValues[x][y] = m_heuristic(node_t(x, y), m_end);
      m_directions[x][y] = 0;
    }
  }

  m_gValues[m_start.first][m_start.second] = 0;
  m_path.clear();
  m_path.push_back(m_start);
}

bool RTAA::isGoalNode(const node_t &node) { return node == m_end; }

void RTAA::AStar(Map &graph) {
  int expands = LookAhead;
  while (expands-- > 0) {
    // If nothing left to expand, set error flag and return.
    if (m_open.empty()) {
      m_current = FAIL_NODE;
      return;
    }

    if(getNext() == FAIL_NODE) return;
    if (isGoalNode(getNext())) return;

    node_t top = m_open.top();
    m_open.pop();

    m_closed.insert(top);
    const char *neighborDirs = graph.getNeighborDirs();
    for (int i = 0; i < graph.numNeighbors(); ++i) {
      node_t neighbor =
          graph.getNeighbor(top.first, top.second, neighborDirs[i]);
      if (neighbor == FAIL_NODE) continue;
      graph.observe(neighbor.first, neighbor.second);
      if (!graph.isConnected(top.first, top.second, neighbor.first,
                             neighbor.second))
        continue;
      if (m_closed.find(neighbor) != m_closed.end()) continue;
      float tentativeScore =
          m_gValues[top.first][top.second] +
          graph.getCost(top.first, top.second, neighborDirs[i]);
      bool inOpenSet;
      if (!(inOpenSet =
                find(m_open.begin(), m_open.end(), neighbor) != m_open.end()) ||
          tentativeScore < m_gValues[neighbor.first][neighbor.second]) {
        m_directions[neighbor.first][neighbor.second] =
            getOppositeDir(neighborDirs[i]);
        m_gValues[neighbor.first][neighbor.second] = tentativeScore;
        if (!inOpenSet) {
          m_open.push(neighbor);
        } else {
          m_open.refresh();
        }
      }
    }
//    if (m_next == FAIL_NODE) m_next = m_open.empty() ? FAIL_NODE : m_open.top();
  }
}

std::list<node_t> RTAA::getResult(const node_t &goal) {
  list<node_t> path;
  node_t next = goal;
  path.push_back(next);
  while (next != m_current) {
    next = m_graph->getNeighbor(next.first, next.second,
                                m_directions[next.first][next.second]);
    if (next == FAIL_NODE) return path;
    path.push_front(next);
  }
  return path;
}

dir_t RTAA::findBestNeighbor(Map &graph, const node_t &node) {
  dir_t bestDir = 0;
  float lowestCost = FLT_MAX;
    const char const *neighborDirs = graph.getNeighborDirs();
  for (int i = 0; i < graph.numNeighbors(); ++i) {
    node_t neighbor =
        graph.getNeighbor(node.first, node.second, neighborDirs[i]);
    if (neighbor == FAIL_NODE) continue;
    dir_t nDir = m_directions[neighbor.first][neighbor.second];
    if (nDir != getOppositeDir(neighborDirs[i])) continue;
    float fValue = getGuess(neighbor);
    if (fValue < lowestCost) {
      lowestCost = fValue;
      bestDir = neighborDirs[i];
    }
  }
  return bestDir;
}

list<node_t> RTAA::search(Map &graph, float (*heuristic)(node_t, node_t)) {
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
{10}	a := the action in A(scurr) on the cost-minimal trajectory from
scurr to s¯;
{11}	scurr := succ(scurr, a);
{12}	movements := movements − 1;
{13}	for any desired number of times (including zero) do
{14}		increase any desired c[s, a] where s ∈ S and a ∈ A(s);
{15}	if any increased c[s, a] is on the cost-minimal trajectory from
scurr to s¯ then
{16}		break;
{17} return SUCCESS;
*/

  while (!isGoalNode(m_current)) {
    AStar(graph);
    if (getNext() == FAIL_NODE || getLoc() == FAIL_NODE) return list<node_t>();
    for (auto it = m_closed.begin(); it != m_closed.end(); it++) {
      m_hValues[it->first][it->second] =
          m_gValues[getNext().first][getNext().second] +
          m_hValues[getNext().first][getNext().second] -
          m_gValues[it->first][it->second];
    }
    m_open.refresh();
    int movements = MoveMax;
    list<node_t> path = getResult(getNext());
    while (m_current != getNext() && movements > 0) {
      if (m_current != path.front()) {
        moveTo(path.front());
        --movements;
      }
      path.pop_front();
    }
  }

  return getPath();
}

float RTAA::getCost() const {
  float total = 0;
  auto path = getPath();
  auto it = path.begin();
  if (it == path.end()) return total;
  node_t first = *it;
  for (int i = 0; i < path.size(); ++i, it++) {
    node_t next = *it;
    if ((first.first == next.first && first.second != next.second) ||
        (first.first != next.first && first.second == next.second)) {
      total += BorderingCost;
    } else if (first.first == next.first && first.second == next.second) {
      total += 0;
    } else {
      total += CornerCost;
    }
    first = next;
  }
  return total;
}