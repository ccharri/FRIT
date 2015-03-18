#include "map.h"

#include <assert.h>
#include <string>

using std::string; using std::istream; using std::ostream; using std::endl;

Map::Map(istream& stream)
{
	readFromStream(stream);
}

Map::Map(int width, int height) : m_map(0), m_width(width), m_height(height), m_type(UNKNOWN)  {
	generateArray();
}

Map::~Map() {
	if(m_map) deleteArray();
}

void Map::readFromStream(istream& stream)
{
	if(m_map) deleteArray();
	string label, strvalue;
	int ivalue;
	
	//Read map type
	stream >> label;
	assert(!label.compare("type"));
	
	stream >> strvalue;
	
	if(!strvalue.compare("octile")) m_type = OCTILE;
	else m_type = UNKNOWN;
	assert(m_type != UNKNOWN);

	//Read height
	stream >> label;
	assert(!label.compare("height"));

	stream >> ivalue;
	assert(ivalue >=0);

	m_height = ivalue;

	//Read width
	stream >> label;
	assert(!label.compare("width"));

	stream >> ivalue;
	assert(ivalue >=0);

	m_width = ivalue;

	stream >> label;
	assert(!label.compare("map"));

	generateArray();

	char n;
	for(int y = 0; y < m_height; ++y)
	{
		for(int x = 0; x < m_width; ++x)
		{
			stream >> n;
			assert(isValidNode(n));
			setNode(x, y, n);
		}
	}
}

void Map::printToStream(ostream& stream) {
	stream << "type ";
	switch(m_type)
	{
		case OCTILE: stream << "octile" << endl; break;
		case QUARTILE: stream << "quartile" << endl; break;
		default: stream << "UNKNOWN" << endl; break;
	}

	stream << "height " << m_height << endl;
	stream << "width " << m_width << endl;
	stream << "map" << endl;
	for(int y = 0; y < m_height; ++y) {
		for(int x = 0; x < m_width; ++x) {
			stream << getNode(x, y);
		}
		stream << endl;
	}

}

bool Map::isPathable(char n) const {
	switch(n) {
		case '.':
		case 'G':
		case 'S':
			return true;
		default: return false;
	}
}

bool Map::isConnected(int x, int y, int nx, int ny) {
    switch (m_type) {
        case OCTILE:
            int xdif = x-nx;
            int ydif = y-ny;
            if(abs(xdif) > 1) return false;
            if(abs(ydif) > 1) return false;
            if(x == nx && y == ny) return false;
            if(!(isPathable(getNode(x, y)) && isPathable(getNode(nx, ny))) return false;
            if((abs(xdif) == 1) && (abs(ydif) == 1)) //If a corner
            {
                
                return true;
            }
            else return (y==ny && abs(xdif) == 1) || (x==nx && abs(ydif) == 1);
        case QUARTILE:
            int xdif = x-nx;
            int ydif = y-ny;
            if(abs(xdif) > 1) return false;
            if(abs(ydif) > 1) return false;
            if(x == nx && y == ny) return false;
            if(!(isPathable(getNode(x, y)) && isPathable(getNode(nx, ny))) return false;
            return (y==ny && abs(xdif) == 1) || (x==nx && abs(ydif) == 1);
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
		default: return false;
	}
}

char Map::getNode(int x, int y) const
{
	if(m_map) return m_map[x][y];
	return 0;
}

void Map::setNode(int x, int y, char n) {
	if(m_map) m_map[x][y] = n;
}

void Map::generateArray() {
	m_map = new char*[m_width];
	for(int i = 0; i < m_width; ++i)
	{
		m_map[i] = new char[m_height];
	}
}

void Map::deleteArray() {
	for(int i = 0; i < m_width; ++i)
	{
		delete m_map[i];
	}
	delete m_map;
	m_map = 0;
}
