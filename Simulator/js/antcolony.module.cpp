#include <emscripten/bind.h>

#include "antColony/ant.h"
#include "antColony/ant.cpp"
#include "antColony/antcolony.cpp"
#include "antColony/exceptions.cpp"
#include "antColony/interfaces.cpp"
#include "antColony/landscape.cpp"
#include "antColony/path.cpp"
#include "antColony/pathiterator.cpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace emscripten;
using namespace rapidjson;
using namespace ant_colony;

class AntColonyJSON {
public:
  AntColonyJSON(const char* json) {

    Document document;
    document.Parse(json);
    assert(document.IsObject());

    assert(document.HasMember("size"));
    assert(document["size"].IsInt());

    int size = document["size"].GetInt();
    int colonyPosition = 0;
    int foodPosition = document["size"].GetInt() - 1;
    int antNumber = 40;

    const Value& graph = document["graph"];
    assert(graph.IsArray());

    for (SizeType i = 0; i < graph.Size(); i++) {
      printf("graph[%d] = %d\n", i, graph[i].GetInt());
    }

    const double *const* matrix;
    
    col = new AntColony(antNumber, matrix, size, colonyPosition, foodPosition, 40, 0.05);
    col->setPheromonWeight(2.5);
    col->setCostWeight(-1);
}

std::string nextStep(int n) {

    

    auto ph = col->getLandscape().getPheromone();
    
    for(unsigned int i = 0; i < ph.size(); ++i) {
      //printNum(i);
    }

    unsigned int i = 0;
    for(auto &row : ph) {
      // cout << setw(2) << i << ":  ";
      ++i;
      for(auto &el : row) {
        //printNum(el);
      }
      // cout << endl;
    }

    return "Hello World";

}

private:
  IAntColony * col;

};

EMSCRIPTEN_BINDINGS(antColony_module) {
  class_<AntColonyJSON>("AntColonyJSON")
    .constructor<const char*>()
    .function("nextStep", &AntColonyJSON::nextStep)
    ;
}