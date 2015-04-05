#ifndef _IDEAL_TREE_H_
#define _IDEAL_TREE_H_

#include "map.h"

class IdealTree {
public:
    IdealTree(const std::pair<int, int>& goalstate, const Map& graph);

  ~IdealTree();

    bool inTree(const std::pair<int, int>& v);

private:
  int m_color;


};

#endif
