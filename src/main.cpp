
// #include "types.h"

#include <fstream>
#include <iostream>
#include <math.h>
#include <list>

#include "map.h"
#include "RTAA.h"


float euclideanHeuristic(node_t a, node_t b) {
    int xdif = a.first-b.first;
    int ydif = a.second-b.second;
    return sqrtf(xdif*xdif + ydif*ydif);
}

int main(int argc, char** argv) {
  std::fstream file(argv[1]);
  Map test(file);
  //test.printToStream(std::cout);
  RTAA rta(test, euclideanHeuristic);
    rta.setStart(node_t(328,401));
    rta.setEnd(node_t(323,216));
    std::list<node_t> result = rta.search(test, euclideanHeuristic);
    std::cout << result.size() << std::endl;
    std::cout << rta.getGoalValue(node_t(323, 216)) << std::endl;
}
