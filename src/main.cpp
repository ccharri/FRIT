
// #include "types.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <list>
#include <vector>
#include <string>
#include <utility>

#include "map.h"
#include "freespace_map.h"
#include "RTAA.h"
#include "ideal_tree.h"
#include "ScenarioLoader.h"

float euclideanHeuristic(node_t a, node_t b) {
  int xdif = a.first - b.first;
  int ydif = a.second - b.second;
  return sqrtf(xdif * xdif + ydif * ydif);
}

int main(int argc, char** argv) {
    string dataPath = argv[1];
  std::fstream testsFile((dataPath + "tests.txt").c_str());
  std::pair<string, string> headers;
  std::vector<std::pair<string, string>> files;
  testsFile >> headers.first >> headers.second;
    string line;
  while (testsFile >> line) {
    files.push_back(std::pair<string, string>(
        dataPath + headers.first + line + ".map", dataPath + headers.second + line + ".map.scen"));
  }

  for (int i = 0; i < files.size(); ++i) {
    std::fstream file(files[i].first);
    Map test(file);
    // test.printToStream(std::cout);
    RTAA rta(test, euclideanHeuristic);
//    node_t tnode = node_t(67, 318);
    //    char type[3][3] = {{test.getNodeType(tnode.first-1, tnode.second-1),
    //        test.getNodeType(tnode.first, tnode.second-1),
    //    test.getNodeType(tnode.first +1, tnode.second-1)},
    //        {test.getNodeType(tnode.first-1,
    //        tnode.second),test.getNodeType(tnode.first, tnode.second),
    //        test.getNodeType(tnode.first+1,tnode.second)},
    //        {test.getNodeType(tnode.first-1, tnode.second+1),
    //        test.getNodeType(tnode.first, tnode.second+1),
    //        test.getNodeType(tnode.first+1, tnode.second+1)}};
    ScenarioLoader loader(files[i].second.c_str());
    int numOptimal = 0;
    bool allOptimal = true;
    for (int i = 0; i < loader.GetNumExperiments(); ++i) {
      Experiment e = loader.GetNthExperiment(i);
      rta.setStart(node_t(e.GetStartX(), e.GetStartY()));
      rta.setEnd(node_t(e.GetGoalX(), e.GetGoalY()));
      rta.search(test, euclideanHeuristic);
        std::list<node_t> result = rta.getPath();
        float goal = rta.getCost();
      bool optimal = (fabs(goal - e.GetDistance()) < .1);
      if (optimal) ++numOptimal;
      allOptimal = allOptimal && optimal;
      std::cout << std::fixed << std::setprecision(2);
      std::cout << "Test:" << i << "\t\tGoal value = " << goal;
      std::cout << "\t\tOptimal=" << e.GetDistance() << "\t\tOptimal? "
                << optimal << std::endl;
    }
    std::cout << "Num tests:" << loader.GetNumExperiments() << std::endl;
    std::cout << "Num optimal:" << numOptimal
              << "\t\tAll optimal=" << allOptimal << std::endl;
  }
  //    rta.setStart(node_t(328,401));
  //    rta.setEnd(node_t(323,216));
  //    std::list<node_t> result = rta.search(test, euclideanHeuristic);
  //    std::cout << result.size() << std::endl;
  //    std::cout << rta.getGoalValue(node_t(323, 216)) << std::endl;
}
