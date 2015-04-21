
#include "ideal_tree.h"

#include <float.h>

#include "IterPrioQueue.h"
#include "Parameters.h"
#include "Heuristics.h"

using std::list;
using std::priority_queue;

IT_RTAA::IT_RTAA(Freespace_Map &graph, float (*heuristic)(node_t, node_t))
    : RTAA(graph, heuristic, false),
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
    if (octileHeuristic(node, goal) < m_hObstacle) return true;

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
    if(node == FAIL_NODE) return;
    getMap().observe(node.first, node.second);
  dir_t const *nDirs = getMap().getNeighborDirs();
  for (int i = 0; i < getMap().numNeighbors(); ++i) {
    node_t n = getMap().getNeighbor(node.first, node.second, nDirs[i]);
      if(n == FAIL_NODE) continue;
    getMap().observe(n.first, n.second);
  }
}

list<node_t> IT_RTAA::search(Map &graph, float (*heuristic)(node_t, node_t)) {
  while (getLoc() != getGoal()) {
    observeAround(getLoc());
    dir_t const *nDirs = graph.getNeighborDirs();
    for (int i = 0; i < graph.numNeighbors(); ++i) {
      node_t n = graph.getNeighbor(getLoc().first, getLoc().second, nDirs[i]);
          if(n == FAIL_NODE) continue;
      if (!graph.isConnected(getLoc().first, getLoc().second, n.first,
                             n.second)) {
        float h = octileHeuristic(n, getGoal());
        if (h < m_hObstacle) m_hObstacle = h;
        m_idealDirs[n.first][n.second] = 0;
        pruneAround(n);
      }
    }
    if (getParent(getLoc()) == FAIL_NODE) {
      ++m_color;
      if(getLoc() != getStart()) RTAA::setStart(getLoc(), false);
      list<node_t> result = RTAA::search(graph, heuristic);
      if (result.empty()) return result;
        if(!isPathFound()) continue;
        node_t first = *result.begin();
        for (auto it = ++result.begin(); it != result.end(); it++){
            node_t next = *it;
            dir_t const * nDirs = getMap().getNeighborDirs();
            for(int n = 0; n < getMap().numNeighbors(); ++n) {
                node_t nn = graph.getNeighbor(first.first, first.second, nDirs[n]);
                if(next == nn) m_idealDirs[first.first][first.second] = nDirs[n];
            }
            first = next;
        }
    }
      node_t next = getParent(getLoc());
      moveTo(next);
    
  }

  return getPath();
}

void IT_RTAA::refresh() {
  getMap().clearObservations();
  m_iPath.clear();
    m_iPath.resize(1);

  for (int x = 0; x < getMap().getWidth(); ++x) {
    for (int y = 0; y < getMap().getHeight(); ++y) {
      m_idealDirs[x][y] = 0;
    }
  }

  m_hObstacle = FLT_MAX;
  m_color = 0;

  node_t goal = getGoal();
  if (goal == FAIL_NODE) return;

  float **distances;

  IterPrioQueue<node_t, NodeComparison> q((NodeComparison(distances)));

  distances = new float *[getMap().getWidth()];
  for (int x = 0; x < getMap().getWidth(); ++x) {
    distances[x] = new float[getMap().getHeight()];
    for (int y = 0; y < getMap().getHeight(); ++y) {
      if (x == getGoal().first && y == getGoal().second)
        distances[x][y] = 0;
      else
        distances[x][y] = FLT_MAX;
    }
  }

  q.push(getGoal());

  while (!q.empty()) {
    node_t expand = q.front();
    q.pop();

    dir_t const *nDir = getMap().getNeighborDirs();
    for (int i = 0; i < getMap().numNeighbors(); ++i) {
      node_t n = getMap().getNeighbor(expand.first, expand.second, nDir[i]);
      if (n == FAIL_NODE) continue;
      bool shouldPush = m_idealDirs[n.first][n.second] == 0;
      float pos = distances[expand.first][expand.second] +
                  getMap().getCost(expand.first, expand.second, nDir[i]);
      if (pos < distances[n.first][n.second]) {
        distances[n.first][n.second] = pos;
        m_idealDirs[n.first][n.second] = getOppositeDir(nDir[i]);
      }
      if (shouldPush) q.push(n);
    }
  }

  for (int i = 0; i < getMap().getWidth(); ++i) {
    delete distances[i];
  }
  delete distances;
}

void IT_RTAA::pruneAround(const node_t &node) {
  dir_t const *nDirs = getMap().getNeighborDirs();
  for (int i = 0; i < getMap().numNeighbors(); ++i) {
    node_t n = getMap().getNeighbor(node.first, node.second, nDirs[i]);
      if(getParent(n) == node)
      m_idealDirs[n.first][n.second] = 0;
  }
}
