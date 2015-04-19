#ifndef _RTAA_H_
#define _RTAA_H_

#include <list>
#include <queue>
#include <set>

#include "real_time_algorithm.h"
#include "IterPrioQueue.h"
#include "Directions.h"
#include "Parameters.h"
#include "node.h"
#include "map.h"

class RTAA : public RealTimeAlgorithm {
 public:
  RTAA(Map& graph, float (*heuristic)(node_t, node_t));
  virtual ~RTAA();

  virtual void setStart(node_t start) override;
  virtual void setEnd(node_t end) override;
    
    virtual std::list<node_t> getPath() const {return m_path;}
    virtual float getCost() const;
    
  virtual float getGoalValue(const node_t node) const {
    return m_gValues[node.first][node.second];
  }
    
    virtual float getHeuristicValue(const node_t node) const {
        return m_hValues[node.first][node.second];
    }

  virtual float getGuess(const node_t node) const {
    return m_gValues[node.first][node.second] +
           m_hValues[node.first][node.second];
  }

  virtual std::list<node_t> search(Map& graph,
                                   float (*heuristic)(node_t, node_t)) override;

 protected:
  virtual bool isGoalNode(const node_t& node);
  inline Map& getMap() { return *m_graph; }
  inline node_t getGoal() { return m_end; }
  virtual dir_t findBestNeighbor(Map& map, const node_t& node);
    virtual void moveTo(node_t& node) { m_current = node; m_path.push_back(node); }
  virtual node_t getLoc() { return m_current; }
  virtual node_t getNext() { return m_next; }

 private:
  void AStar(Map& graph);
  std::list<node_t> getResult(const node_t& goal);

    std::list<node_t> m_path;
    
  float** m_gValues;
  float** m_hValues;
  char** m_directions;
  Map* m_graph;
  node_t m_current;
  node_t m_next;
  node_t m_end;
  node_t m_start;
  std::set<node_t> m_closed;

  friend class NodeComparison;

  class NodeComparison {
   public:
    NodeComparison(const RTAA* rta, const bool reverse)
        : m_rta(rta), m_reverse(reverse) {}

    bool operator()(const node_t& lhs, const node_t& rhs) const {
      float flhs = m_rta->getGuess(lhs);
      float frhs = m_rta->getGuess(rhs);
      if (m_reverse) {
        if (flhs == frhs)
          return m_rta->getGoalValue(lhs) < m_rta->getGoalValue(rhs);
        return flhs < frhs;
      } else {
        if (flhs == frhs)
          return m_rta->getGoalValue(lhs) > m_rta->getGoalValue(rhs);
        return flhs > frhs;
      }
    }

   private:
    const RTAA* m_rta;
    const bool m_reverse;
  };

  float (*m_heuristic)(node_t, node_t);

  IterPrioQueue<node_t, NodeComparison> m_open;
};

#endif