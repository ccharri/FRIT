#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>

#include "types.h"

typedef enum
{
	UNKNOWN,
	QUARTILE,
	OCTILE
} MapType;

class Map {
public:
	Map(std::istream& stream);
	Map(int width, int height);
	~Map();


	void readFromStream(std::istream& stream);
	void printToStream(std::ostream& stream) const;

	int getWidth() const {return m_width;}
	int getHeight() const {return m_height;}
	MapType getType() const { return m_type; }
	int numNeighbors() const;
	node_t getNeighbor(int x, int y, dir_t dir) const;
	float getCost(int x, int y, dir_t dir) const;

	bool isPathable(char n) const;
    bool isConnected(int x, int y, int nx, int ny) const;

	char getNodeType(int x, int y) const;
	void setNodeType(int x, int y, char n);

	node_t getNode(int x, int y) const;

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