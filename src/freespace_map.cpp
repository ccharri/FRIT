//
//  freespace_map.cpp
//  FRITRecreation
//
//  Created by Christopher Harris on 4/19/15.
//  Copyright (c) 2015 University of Michigan. All rights reserved.
//

#include "freespace_map.h"
using std::istream;
Freespace_Map::Freespace_Map(istream& stream) : Map(stream), m_observed(0) {
  generateOArray();
}
Freespace_Map::Freespace_Map(int width, int height)
    : Map(width, height), m_observed(0) {
  generateOArray();
}

Freespace_Map::~Freespace_Map() {
  if (m_observed) deleteOArray();
}

void Freespace_Map::observe(int x, int y) { m_observed[x][y] = true; }

bool Freespace_Map::isConnected(int x, int y, int nx, int ny) const {
  if (m_observed[x][y] && m_observed[nx][ny])
    return Map::isConnected(x, y, nx, ny);
  else {
    int xdif;
    int ydif;
    switch (getType()) {
      case OCTILE:
        xdif = x - nx;
        ydif = y - ny;
        if (abs(xdif) > 1) return false;
        if (abs(ydif) > 1) return false;
        if (x == nx && y == ny) return false;
        if ((abs(xdif) == 1) && (abs(ydif) == 1))  // If a corner
        {
          return true;
        }
        return (y == ny && abs(xdif) == 1) || (x == nx && abs(ydif) == 1);
      case QUARTILE:
        xdif = x - nx;
        ydif = y - ny;
        if (abs(xdif) > 1) return false;
        if (abs(ydif) > 1) return false;
        if (x == nx && y == ny) return false;
        return (y == ny && abs(xdif) == 1) || (x == nx && abs(ydif) == 1);
      default:
        return false;
    }
  }
}

void Freespace_Map::generateOArray() {
  m_observed = new bool* [getWidth()];
  for (int i = 0; i < getWidth(); ++i) {
    m_observed[i] = new bool[getHeight()];
  }
}

void Freespace_Map::deleteOArray() {
  for (int i = 0; i < getWidth(); ++i) {
    delete m_observed[i];
  }
  delete m_observed;
  m_observed = 0;
}

void Freespace_Map::clearObservations() {
  for (int x = 0; x < getWidth(); ++x) {
    for (int y = 0; y < getHeight(); ++y) {
      m_observed[x][y] = false;
    }
  }
}