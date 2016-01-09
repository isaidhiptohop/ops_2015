#include <emscripten/bind.h>
#include <emscripten.h>

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
  AntColonyJSON(int size, std::string transfer, int sourceID, int targetID) {

    EM_ASM_({
      console.log('size: ' + $0);
    }, size);

    this->size = size;

    char *json = new char[transfer.length() + 1];
    strcpy(json, transfer.c_str());

    this->document.Parse(json);

    const Value& a = this->document;
    assert(a.IsArray());

    // Create Matrix
    double arr [size][size];
    for(size_t i = 0; i < size; i++) {
      for(size_t j = 0; j < size; j++) {
        arr[i][j] = 0;
      } 
    }

    // Fill values in matrix
    for (SizeType i = 0; i < a.Size(); i++) {

      // For each object
      assert(a[i].IsObject());
      const Value &o = a[i];
      assert(o["source"].IsNumber());
      assert(o["target"].IsNumber());
      arr[o["source"].GetInt()][o["target"].GetInt()] = 1;
      arr[o["target"].GetInt()][o["source"].GetInt()] = 1;

      EM_ASM_({
      console.log('Insert 1 at: ' + $0 + ' , ' + $1);
    }, o["source"].GetInt(), o["target"].GetInt());
    }

    EM_ASM_({
      console.log('matrix test 1: ' + $0);
    }, arr[0][1]);

            
    double * arr1[size];
    for(int i = 0; i < size; i++)
      arr1[i] = arr[i];
    double ** matrix = arr1;

    /**
    for(size_t i = 0; i < size; i++) {
      for(size_t j = 0; j < size; j++) {          
          EM_ASM_({
            console.log('insert value: ' + $0);
          }, arr[i][j]);
      }
    }
    */
  
    col = new AntColony(40, matrix, size, sourceID, targetID, 40, 0.05);
    col->setPheromonWeight(2.5);
    col->setCostWeight(-1);

    std::ostringstream strs = this->printLandscape(col->getLandscape());
    std::string str = strs.str();

    EM_ASM_({
      console.log('Matrix: ' + $0);
    }, str.c_str());

    delete [] json;
}

std::string nextStep(int steps) {

    col->nextStep(steps);

    ILandscape & landscape = col->getLandscape();
    auto ph = landscape.getPheromone();

    EM_ASM_({
      console.log('pheromone: ' + $0);
    }, ph.size());

    for(size_t i = 0; i < this->size; i++) {
      for(size_t j = 0; j < this->size; j++) { 
        EM_ASM_({
            console.log('value: ' + $0 + " at " + $1 + "," + $2);
          }, ph[i][j], i, j);

      }
    }

    // Value Werte eintragen in a (Dokument Value Array)

    const Value& a = this->document;
    assert(a.IsArray());

    Document::AllocatorType& allocator = this->document.GetAllocator();


    for (SizeType i = 0; i < a.Size(); i++) {

      const Value& v = this->document[i];
      double vout = ph[v["source"].GetInt()][v["target"].GetInt()];

      if(vout < 0) {
        this->document[i]["value"].SetString("10");
      } else if (vout == 0) {
        this->document[i]["value"].SetString("20");
      } else if (vout < 1) {
        this->document[i]["value"].SetString("30");
      } else if (vout == 1) {
        this->document[i]["value"].SetString("40");
      } else if (vout <= 10) {
        this->document[i]["value"].SetString("50");
      } else if (vout <= 50) {
        this->document[i]["value"].SetString("60");
      } else if (vout <= 100) {
        this->document[i]["value"].SetString("70");
      } else if (vout <= 200) {
        this->document[i]["value"].SetString("80");
      } else {
        this->document[i]["value"].SetString("100");
      }

    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    this->output = buffer.GetString();

    return this->output;
}

private:
  IAntColony * col;
  Document document;
  std::string output;
  int size;

  bool isType(emscripten::val value, const std::string& type) {
      return (value.typeof().as<std::string>() == type);
  }

  std::ostringstream printLandscape(ILandscape & landscape) {

    std::ostringstream strs;

    auto ph = landscape.getPheromone();
    strs << "     ";
    for(unsigned int i = 0; i < ph.size(); ++i) {
      strs << i;
    }
    strs << std::endl << std::endl;
    unsigned int i = 0;
    for(auto &row : ph) {
      strs << i << ":  ";
      ++i;
      for(auto &el : row) {
        strs << el;
      }
      strs << std::endl;
    }

    return strs;
  }
};

EMSCRIPTEN_BINDINGS(antColony_module) {
  class_<AntColonyJSON>("AntColonyJSON")
    .constructor<int, std::string, int, int>()
    .function("nextStep", &AntColonyJSON::nextStep);
}