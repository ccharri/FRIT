#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>

#include "node.h"
#include "Directions.h"
#include "Parameters.h"

typedef enum { UNKNOWN, QUARTILE, OCTILE } MapType;

class Map {
 public:
  Map(std::istream& stream);
  Map(int width, int height);
  virtual ~Map();

  void readFromStream(std::istream& stream);
  void printToStream(std::ostream& stream) const;

  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }
  inline MapType getType() const { return m_type; }
  int numNeighbors() const;
  virtual node_t getNeighbor(int x, int y, char dir) const;
    virtual char const * getNeighborDirs() const;
  virtual float getCost(int x, int y, char dir) const;

  virtual bool isPathable(char n) const;
  virtual bool isConnected(int x, int y, int nx, int ny) const;

  char getNodeType(int x, int y) const;
  void setNodeType(int x, int y, char n);

  node_t getNode(int x, int y) const;
    
  virtual void observe(int x, int y) {};

 private:
  void generateArray();
  void deleteArray();
  bool isValidNode(char n) const;

  char** m_map;
  int m_width;
  int m_height;
  MapType m_type;
};

#endif