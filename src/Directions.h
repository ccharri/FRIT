#ifndef _DIRECTIONS_H_
#define _DIRECTIONS_H_

#include "map.h"

typedef enum : char {
	NORTH = 'N',
	EAST = 'E',
	SOUTH = 'S',
	WEST = 'W',
	NORTH_EAST = 'A',
	NORTH_WEST = 'B',
	SOUTH_EAST = 'C',
	SOUTH_WEST = 'D',
	NONE = 0
} OCTILE_DIRECTION;

typedef enum : char {
	NORTH= 'N',
	EAST = 'E',
	SOUTH = 'S',
	WEST = 'W',
	NONE = 0
} QUARTILE_DIRECTION;

#endif