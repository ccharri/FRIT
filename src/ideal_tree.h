#ifndef _IDEAL_TREE_H_
#define _IDEAL_TREE_H_

#include "RTAA.h"

#include <list>

#include "node.h"
#include "freespace_map.h"

class IT_RTAA : public RTAA {
 public:
  IT_RTAA(Freespace_Map& graph, float (*heuristic)(node_t, node_t));

  ~IT_RTAA();
    
    virtual inline void refresh();

    virtual std::list<node_t> search(Map &graph, float (*heuristic)(node_t, node_t)) override;
    

    virtual void setEnd(node_t end) override {
        RTAA::setEnd(end);
        refresh();
    }
 protected:

  virtual bool isGoalNode(const node_t& node) override;
  inline virtual node_t getParent(const node_t& node);

    virtual void observeAround(const node_t& node);
    
    virtual inline Freespace_Map& getMap() {return *m_map;}
    
    virtual void pruneAround(const node_t& node);
    
 private:
    Freespace_Map* m_map;
    float m_hObstacle;
  int m_color;
  int** m_colors;
    dir_t** m_idealDirs;
};

#endif
