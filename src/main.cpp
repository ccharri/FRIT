
// #include "types.h"

#include <fstream>
#include <iostream>
#include "map.h"

int main(int argc, char** argv)
{
	std::fstream file(argv[1]);
	Map test(file);
	test.printToStream(std::cout);
}