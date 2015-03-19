#ifndef _PATH_NODE_H
#define _PATH_NODE_H

struct PathNode {
	int x;
	int y;
	char type;

	bool operator==(const PathNode& other) {
		return x == other.x && y == other.y && type == other.type;
	}

	bool operator!=(const PathNode& other) { return !(*this == other); }
};

#endif