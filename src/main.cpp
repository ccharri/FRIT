
// #include "types.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <list>
#include <string>

#include "map.h"
#include "RTAA.h"
#include "ScenarioLoader.h"


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
    char type = test.getNodeType(67,318);
    ScenarioLoader loader((string(argv[1]) + ".scen").c_str());
    int numOptimal = 0;
    bool allOptimal = true;
    for(int i = 0 ; i < loader.GetNumExperiments(); ++i) {
        Experiment e = loader.GetNthExperiment(58);
        rta.setStart(node_t(e.GetStartX(), e.GetStartY()));
        rta.setEnd(node_t(e.GetGoalX(), e.GetGoalY()));
        std::list<node_t> result = rta.search(test, euclideanHeuristic);
        float goal =rta.getGoalValue(node_t(e.GetGoalX(), e.GetGoalY()));
        bool optimal = (fabs(goal-e.GetDistance()) < .1);
        if(optimal) ++ numOptimal;
        allOptimal = allOptimal && optimal;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Test:" << i << "\t\tGoal value = " << goal;
        std::cout << "\t\tOptimal=" << e.GetDistance() << "\t\tOptimal? " << optimal << std::endl;
    }
    std::cout << "Num tests:" << loader.GetNumExperiments() << std::endl;
    std::cout << "Num optimal:" << numOptimal << "\t\tAll optimal="<< allOptimal<< std::endl;
//    rta.setStart(node_t(328,401));
//    rta.setEnd(node_t(323,216));
//    std::list<node_t> result = rta.search(test, euclideanHeuristic);
//    std::cout << result.size() << std::endl;
//    std::cout << rta.getGoalValue(node_t(323, 216)) << std::endl;
}
