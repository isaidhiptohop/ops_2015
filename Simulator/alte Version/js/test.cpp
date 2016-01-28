#include <emscripten/bind.h>

using namespace emscripten;
using namespace std;

vector<string> *vectorFromIntPointer(uintptr_t vec) {
  return reinterpret_cast<vector<string> *>(vec);
}

EMSCRIPTEN_BINDINGS(Wrappers) {
  register_vector<string>("VectorString").constructor(&vectorFromIntPointer, allow_raw_pointers());
};

vector<string> myVector;
myVector.push_back("First item");
myVector.push_back("Second item");

EM_ASM_ARGS({
  var myVector = new Module.VectorString($0);
  // Prints "Second item"
  console.log(myVector.get(1));
}, &myVector);