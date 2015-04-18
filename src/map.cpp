#include "map.h"

#include <assert.h>
#include <string>
#include <math.h>
#include <float.h>

#include "Directions.h"
#include "Parameters.h"

using std::string;
using std::istream;
using std::ostream;
using std::endl;

Map::Map(istream& stream) { readFromStream(stream); }

Map::Map(int width, int height)
    : m_map(0), m_width(width), m_height(height), m_type(UNKNOWN) {
  generateArray();
}

Map::~Map() {
  if (m_map) deleteArray();
}

void Map::readFromStream(istream& stream) {
  if (m_map) deleteArray();
  string label, strvalue;
  int ivalue;

  // Read map type
  stream >> label;
  assert(!label.compare("type"));

  stream >> strvalue;

  if (!strvalue.compare("octile"))
    m_type = OCTILE;
  else
    m_type = UNKNOWN;
  assert(m_type != UNKNOWN);

  // Read height
  stream >> label;
  assert(!label.compare("height"));

  stream >> ivalue;
  assert(ivalue >= 0);

  m_height = ivalue;

  // Read width
  stream >> label;
  assert(!label.compare("width"));

  stream >> ivalue;
  assert(ivalue >= 0);

  m_width = ivalue;

  stream >> label;
  assert(!label.compare("map"));

  generateArray();

  char n;
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      stream >> n;
      assert(isValidNode(n));
      setNodeType(x, y, n);
    }
  }
}

void Map::printToStream(ostream& stream) const {
  stream << "type ";
  switch (m_type) {
    case OCTILE:
      stream << "octile" << endl;
      break;
    case QUARTILE:
      stream << "quartile" << endl;
      break;
    default:
      stream << "UNKNOWN" << endl;
      break;
  }

  stream << "height " << m_height << endl;
  stream << "width " << m_width << endl;
  stream << "map" << endl;
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      stream << getNodeType(x, y);
    }
    stream << endl;
  }
}

int Map::numNeighbors() const {
  switch (m_type) {
    case QUARTILE:
      return NUM_VALID_QUARTILE_DIRECTIONS;
    case OCTILE:
      return NUM_VALID_OCTILE_DIRECTIONS;
    default:
      break;
  }
  return 0;
}

node_t Map::getNeighbor(int x, int y, char dir) const {
  switch (dir) {
    case VALUE(OCTILE_DIRECTION, NORTH):
      return getNode(x, y - 1);
    case VALUE(OCTILE_DIRECTION, SOUTH):
      return getNode(x, y + 1);
    case VALUE(OCTILE_DIRECTION, EAST):
      return getNode(x + 1, y);
    case VALUE(OCTILE_DIRECTION, WEST):
      return getNode(x - 1, y);
    case VALUE(OCTILE_DIRECTION, NORTH_WEST):
      return getNode(x - 1, y - 1);
    case VALUE(OCTILE_DIRECTION, NORTH_EAST):
      return getNode(x + 1, y - 1);
    case VALUE(OCTILE_DIRECTION, SOUTH_EAST):
      return getNode(x + 1, y + 1);
    case VALUE(OCTILE_DIRECTION, SOUTH_WEST):
      return getNode(x - 1, y + 1);
    default:
      return FAIL_NODE;
  }
}

float Map::getCost(int x, int y, char dir) const {
  switch (dir) {
    case VALUE(OCTILE_DIRECTION, NORTH):
      return BorderingCost;
    case VALUE(OCTILE_DIRECTION, SOUTH):
      return BorderingCost;
    case VALUE(OCTILE_DIRECTION, EAST):
      return BorderingCost;
    case VALUE(OCTILE_DIRECTION, WEST):
      return BorderingCost;
    case VALUE(OCTILE_DIRECTION, NORTH_WEST):
      return CornerCost;
    case VALUE(OCTILE_DIRECTION, NORTH_EAST):
      return CornerCost;
    case VALUE(OCTILE_DIRECTION, SOUTH_EAST):
      return CornerCost;
    case VALUE(OCTILE_DIRECTION, SOUTH_WEST):
      return CornerCost;
    default:
      return FLT_MAX;
  }
}

node_t Map::getNode(int x, int y) const {
  if (x < 0 || y < 0 || x >= m_width || y >= m_height) return FAIL_NODE;
  return node_t(x, y);
}

bool Map::isPathable(char n) const {
  switch (n) {
    case '.':
    case 'G':
    case 'S':
      return true;
    default:
      return false;
  }
}

bool Map::isConnected(int x, int y, int nx, int ny) const {
  int xdif;
  int ydif;
  switch (m_type) {
    case OCTILE:
      xdif = x - nx;
      ydif = y - ny;
      if (abs(xdif) > 1) return false;
      if (abs(ydif) > 1) return false;
      if (x == nx && y == ny) return false;
      if (!(isPathable(getNodeType(x, y)) && isPathable(getNodeType(nx, ny))))
        return false;
      if ((abs(xdif) == 1) && (abs(ydif) == 1))  // If a corner
      {
        // Only allow connections to corners if both neighboring middle nodes
        // are.
        // Otherwise, the diagonal is cut off and so not diagonally connected.
        return isPathable(getNodeType(nx, y)) && isPathable(getNodeType(x, ny));
        // return true;
      } else
        return (y == ny && abs(xdif) == 1) || (x == nx && abs(ydif) == 1);
    case QUARTILE:
      xdif = x - nx;
      ydif = y - ny;
      if (abs(xdif) > 1) return false;
      if (abs(ydif) > 1) return false;
      if (x == nx && y == ny) return false;
      if (!(isPathable(getNodeType(x, y)) && isPathable(getNodeType(nx, ny))))
        return false;
      return (y == ny && abs(xdif) == 1) || (x == nx && abs(ydif) == 1);
    default:
      return false;
  }
}

bool Map::isValidNode(char n) const {
  switch (n) {
    case '.':
    case 'G':
    case '@':
    case 'O':
    case 'T':
    case 'S':
    case 'W':
      return true;
    default:
      return false;
  }
}

char Map::getNodeType(int x, int y) const {
  if (m_map) return m_map[x][y];
  return 0;
}

void Map::setNodeType(int x, int y, char n) {
  if (m_map) m_map[x][y] = n;
}

void Map::generateArray() {
  m_map = new char* [m_width];
  for (int i = 0; i < m_width; ++i) {
    m_map[i] = new char[m_height];
  }
}

void Map::deleteArray() {
  for (int i = 0; i < m_width; ++i) {
    delete m_map[i];
  }
  delete m_map;
  m_map = 0;
}
