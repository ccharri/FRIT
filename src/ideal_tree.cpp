
#include "ideal_tree.h"

#include <float.h>

#include "Parameters.h"

using std::list;

IT_RTAA::IT_RTAA(Freespace_Map &graph, float (*heuristic)(node_t, node_t))
    : RTAA(graph, heuristic),
      m_map(&graph),
      m_hObstacle(FLT_MAX),
      m_color(0),
      m_colors(0),
      m_idealDirs(0) {
  m_colors = new int *[graph.getWidth()];
  for (int i = 0; i < graph.getWidth(); ++i) {
    m_colors[i] = new int[graph.getHeight()];
  }

  switch (graph.getType()) {
    case QUARTILE:
      m_idealDirs = (char **)new QUARTILE_DIRECTION *[graph.getWidth()];
      for (int i = 0; i < graph.getWidth(); ++i) {
        ((QUARTILE_DIRECTION **)m_idealDirs)[i] =
            new QUARTILE_DIRECTION[graph.getHeight()];
      }
      break;
    case OCTILE:
      m_idealDirs = (char **)new OCTILE_DIRECTION *[graph.getWidth()];
      for (int i = 0; i < graph.getWidth(); ++i) {
        ((OCTILE_DIRECTION **)m_idealDirs)[i] =
            new OCTILE_DIRECTION[graph.getHeight()];
      }
      break;
    default:
      break;
  }

  refresh();
}

IT_RTAA::~IT_RTAA() {
  for (int i = 0; i < getMap().getWidth(); ++i) {
    delete m_colors[i];
  }
  delete m_colors;
  m_colors = 0;

  switch (getMap().getType()) {
    case QUARTILE:
      for (int i = 0; i < getMap().getWidth(); ++i) {
        delete ((QUARTILE_DIRECTION **)m_idealDirs)[i];
      }
      break;
    case OCTILE:
      for (int i = 0; i < getMap().getWidth(); ++i) {
        delete ((OCTILE_DIRECTION **)m_idealDirs)[i];
      }
      break;
    default:
      break;
  }
  delete m_idealDirs;
  m_idealDirs = 0;
}

bool IT_RTAA::isGoalNode(const node_t &node) {
  node_t next = node;
  node_t goal = getGoal();
  while (next != goal) {
    if (getHeuristicValue(node) < m_hObstacle) return true;

    m_colors[next.first][next.second] = m_color;
    node_t parent = getParent(next);
    if (parent == FAIL_NODE || m_colors[parent.first][parent.second] == m_color)
      return false;
    next = parent;
  }
  return true;
}

node_t IT_RTAA::getParent(const node_t &node) {
  if (node == FAIL_NODE) return FAIL_NODE;
  return getMap().getNeighbor(node.first, node.second,
                              m_idealDirs[node.first][node.second]);
}

void IT_RTAA::observeAround(const node_t &node) {
  dir_t const *nDirs = getMap().getNeighborDirs();
  for (int i = 0; i < getMap().numNeighbors(); ++i) {
    node_t n = getMap().getNeighbor(node.first, node.second, nDirs[i]);
    getMap().observe(n.first, n.second);
  }
}

list<node_t> IT_RTAA::search(Map &graph, float (*heuristic)(node_t, node_t)) {
  while (getLoc() != getGoal()) {
    bool i = m_idealDirs[getLoc().first][getLoc().second] != 0;
    observeAround(getLoc());
    dir_t const *nDirs = graph.getNeighborDirs();
    for (int i = 0; i < graph.numNeighbors(); ++i) {
      node_t n = graph.getNeighbor(getLoc().first, getLoc().second, nDirs[i]);
      if (!graph.isConnected(getLoc().first, getLoc().second, n.first,
                             n.second)) {
        float h = heuristic(getLoc(), getGoal());
        if (h < m_hObstacle) m_hObstacle = h;
        m_idealDirs[n.first][n.second] = 0;
        pruneAround(n);
      }
    }
    if (m_idealDirs[getLoc().first][getLoc().second] == 0) {
      ++m_color;
      if (i) RTAA::setStart(getLoc());
      list<node_t> result = RTAA::search(graph, heuristic);
      if (result.empty()) return result;
    } else {
      node_t next =
          graph.getNeighbor(getLoc().first, getLoc().second,
                            m_idealDirs[getLoc().first][getLoc().second]);
      moveTo(next);
    }
  }

  return getPath();
}


void IT_RTAA::refresh() {
  getMap().clearObservations();

  for (int x = 0; x < getMap().getWidth(); ++x) {
    for (int y = 0; y < getMap().getHeight(); ++y) {
      m_idealDirs[x][y] = 0;
    }
  }

  m_hObstacle = FLT_MAX;
  m_color = 0;

  if (getGoal() == FAIL_NODE) return;

  list<node_t> openList;
  openList.push_back(getGoal());
  m_idealDirs[getGoal().first][getGoal().second] = 0;
  while (!openList.empty()) {
    node_t expand = openList.front();
    openList.pop_front();

    dir_t const *nDir = getMap().getNeighborDirs();
    for (int i = 0; i < getMap().numNeighbors(); ++i) {
      node_t n = getMap().getNeighbor(expand.first, expand.second, nDir[i]);
      if (n == FAIL_NODE || n == getGoal() || m_idealDirs[n.first][n.second] != 0) continue;
      openList.push_back(n);
      m_idealDirs[n.first][n.second] = getOppositeDir(nDir[i]);
    }
  }
}

void IT_RTAA::pruneAround(const node_t &node) {
  dir_t const *nDirs = getMap().getNeighborDirs();
  for (int i = 0; i < getMap().numNeighbors(); ++i) {
    node_t n = getMap().getNeighbor(node.first, node.second, nDirs[i]);
    if (n == FAIL_NODE) continue;
    if (m_idealDirs[n.first][n.second] != 0 &&
        (m_idealDirs[n.first][n.second] == getOppositeDir(nDirs[i]))) {
      m_idealDirs[n.first][n.second] = 0;
      pruneAround(n);
    }
  }
}
