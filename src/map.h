#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>

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
	void printToStream(std::ostream& stream);

	int getWidth() const {return m_width;}
	int getHeight() const {return m_height;}
	MapType getType() const { return m_type; }

	bool isPathable(char n) const;
    bool isConnected(int x, int y, int nx, int ny);

	char getNode(int x, int y) const;
	void setNode(int x, int y, char n);

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