#include <emscripten/bind.h>
#include <emscripten.h>
#include <sstream>

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

    char *json = new char[transfer.length() + 1];
    strcpy(json, transfer.c_str());

    Document document;
    document.Parse(json);

    const Value& a = document;
    assert(a.IsArray());

    // Create Matrix
    double arr [size][size];
    for(size_t i = 0; i < size; i++) {
      for(size_t j = 0; j < size; j++) {
        arr[i][j] = 1;
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
    }

    EM_ASM_({
      console.log('matrix test 1: ' + $0);
    }, arr[0][1]);

            
    double * arr1[size];
    for(int i = 0; i < size;++i)
      arr1[i] = arr[i];
    double ** matrix = arr1;

    
    for(size_t i = 0; i < size; i++) {
      for(size_t j = 0; j < size; j++) {          
          EM_ASM_({
            console.log('insert value: ' + $0);
          }, arr[i][j]);
      }
    }

/**
    double arr9 [15][15] = { {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
              {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
              {1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
              {0,1,1,0,0,0,0,0,1,0,1,0,0,0,0},
              {0,0,1,0,0,1,1,0,0,0,0,0,0,0,0},
              {0,0,0,0,1,0,1,1,0,0,0,0,0,0,0},
              {0,0,0,0,1,1,0,0,1,0,0,0,0,0,0},
              {0,0,0,0,0,1,0,0,1,1,0,0,0,0,0},
              {0,0,0,1,0,0,1,1,0,1,1,1,0,0,0},
              {0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
              {0,0,0,1,0,0,0,0,1,0,0,1,0,1,0},
              {0,0,0,0,0,0,0,0,1,1,1,0,1,1,1},
              {0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
              {0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
              {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0} };
  
  double * arr19[15];
  for(int i = 0; i < 15;++i)
    arr19[i] = arr9[i];

  double ** matrix9 = arr19;
*/    
    col = new AntColony(40, matrix, size, sourceID, targetID, 40, 0.05);
    col->setPheromonWeight(2.5);
    col->setCostWeight(-1);
    col->nextStep(10);

    ILandscape & landscape = col->getLandscape();
    auto ph = landscape.getPheromone();

    EM_ASM_({
      console.log('pheromone: ' + $0);
    }, ph.size());

    for(size_t i = 0; i < size; i++) {
      for(size_t j = 0; j < size; j++) { 
        EM_ASM_({
            console.log('value: ' + $0);
          }, ph[i][j]);

      }
    }

    // Value Werte eintragen in a (Dokument Value Array)

    Document::AllocatorType& allocator = document.GetAllocator();
    assert(a.IsArray());

    for (SizeType i = 0; i < a.Size(); i++) {

      std::ostringstream strs;
      double vout = ph[document[i]["source"].GetInt()][document[i]["target"].GetInt()];
      
      document[i]["value"].SetString(vout);

      //const Value& e = a[i]["value"];
      //assert(e.IsObject());

      //e.SetString("0.001"); // call SetString() on the reference
      
      // e.AddMember("test","test",allocator);
      //Value v;
      //v.SetObject();

      //e.AddMember(Value("value",ph[v["source"].GetInt()][v["target"].GetInt()],allocator));
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    this->output = buffer.GetString();


    //ph[i][j]    

    /**
    Document outputDoc;
    outputDoc.SetArray();
    Document::AllocatorType& allocator = outputDoc.GetAllocator();

    unsigned int j = 0;
    for(size_t i = 0; i < ph.size(); i++) {
      for(size_t j = 0; j < ph.size(); j++) {          
          Value objValue;
          objValue.SetObject();
          objValue.AddMember("source",i,allocator);
          objValue.AddMember("target",j,allocator);

          EM_ASM_({
            console.log('value: ' + $0);
          }, ph[i][j]);

          objValue.AddMember("value",ph[i][j],allocator);
          outputDoc.PushBack(objValue,allocator);
          ++j;
      }    
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    outputDoc.Accept(writer);

    std::string output2 = buffer.GetString();

    */

    delete [] json;
}

std::string nextStep(int steps) {
    
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

    return this->output;
}

private:
  IAntColony * col;
  std::string output;

  bool isType(emscripten::val value, const std::string& type) {
      return (value.typeof().as<std::string>() == type);
  }
};

EMSCRIPTEN_BINDINGS(antColony_module) {
  class_<AntColonyJSON>("AntColonyJSON")
    .constructor<int, std::string, int, int>()
    .function("nextStep", &AntColonyJSON::nextStep);
}