#ifndef SEARCH_ENGINE_QUERYPROCESSOR_H
#define SEARCH_ENGINE_QUERYPROCESSOR_H
#include "AVLTree.h"
#include "porter2_stemmer.h"
#include "include/rapidjson/document.h"
#include <string>
#include <iostream>
#include "IndexHandler.h"
#include <sstream>
using namespace std;
///Handles queries inputted by user
class QueryProcessor{
public:
    IndexHandler index;                    ///Index handler containing the AVLTree
    void runProcessor(char** argv);         ///Reads input from user and calls appropriate search function
    void printData(vector<string> results); ///Prints relevant statistics of the engine
    void rankTop15();
};


#endif //SEARCH_ENGINE_QUERYPROCESSOR_H
