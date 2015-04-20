#ifndef _IDEAL_TREE_H_
#define _IDEAL_TREE_H_

#include "RTAA.h"

#include <list>

#include "node.h"
#include "freespace_map.h"
#include "Heuristics.h"

class IT_RTAA : public RTAA {
 public:
  IT_RTAA(Freespace_Map& graph, float (*heuristic)(node_t, node_t));

  ~IT_RTAA();
    
    virtual void refresh();
    virtual std::list<node_t> getPath() const override {return m_iPath;}

    virtual std::list<node_t> search(Map &graph, float (*heuristic)(node_t, node_t)) override;
    virtual void setEnd(node_t end) override {
        RTAA::setEnd(end);
        refresh();
        m_iPath.push_back(getStart());
    }
 protected:

  virtual bool isGoalNode(const node_t& node) override;
  inline virtual node_t getParent(const node_t& node);

    virtual void observeAround(const node_t& node);
    
    virtual inline Freespace_Map& getMap() {return *m_map;}
    
    virtual void pruneAround(const node_t& node);
    virtual void moveTo(node_t& node) override { RTAA::moveTo(node); m_iPath.push_back(node); }
    
 private:
    
    class NodeComparison {
    public:
        NodeComparison(float** &distances) : distances(distances) {}
        
        bool operator()(const node_t& lhs, const node_t& rhs) const {
            return distances[lhs.first][lhs.second] > distances[rhs.first][rhs.second];
        }
        
    private:
        float** &distances;
    };
    
    
    Freespace_Map* m_map;
    std::list<node_t> m_iPath;
    float m_hObstacle;
  int m_color;
  int** m_colors;
    dir_t** m_idealDirs;
};

#endif
