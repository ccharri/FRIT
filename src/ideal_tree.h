#ifndef _IDEAL_TREE_H_
#define _IDEAL_TREE_H_

#include "types.h"

class IdealTree {
public:
  IdealTree(const vertex_t& goalstate, const graph_t& graph);

  ~IdealTree();

  bool inTree(const vertex_t& v);

private:
  color_t m_color;


};

#endif
