#ifndef _IDEAL_TREE_NODE_H_
#define _IDEAL_TREE_NODE_H_

#include <vector>

#include "types.h"

typedef IdealTreeNode inode_t;

class IdealTreeNode {
public:
  IdealTreeNode(node_t* node);
  ~IdealTreeNode();

  void setParent(inode_t* node) {m_parent = node;}

  color_t getColor() {return m_color;}
  void setColor(color_t color) {m_color = color;}

private:
  node_t* m_node;
  node_t* m_parent;
  color_t m_color;
}

#endif
