#include "IndexHandler.h"
#include "QueryProcessor.h"

using namespace rapidjson;
using namespace std;

int main(int argc, char** argv) {
    QueryProcessor queries;
    queries.runProcessor(argv);     //Running query processor
}
