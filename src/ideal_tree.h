#ifndef _IDEAL_TREE_H_
#define _IDEAL_TREE_H_

#include "RTAA.h"

#include "node.h"
#include "map.h"

class IT_RTAA : public RTAA {
 public:
  IT_RTAA(Map& graph, float (*heuristic)(node_t, node_t));

  ~IT_RTAA();

 protected:
  virtual bool isGoalNode(const node_t& node) override;
  inline virtual node_t getParent(const node_t& node);

 private:
  int m_color = 0;
  int** m_colors;
};

#endif
