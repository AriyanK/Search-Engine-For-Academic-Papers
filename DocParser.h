#ifndef SEARCH_ENGINE_DOCPARSER_H
#define SEARCH_ENGINE_DOCPARSER_H
#include "AVLTree.h"

///Ranks documents
class DocParser{
public:
    void rankResults(vector<string> papers);    ///Ranks results given list of viable papers
};


#endif //SEARCH_ENGINE_DOCPARSER_H
