#ifndef SEARCH_ENGINE_INDEXHANDLER_H
#define SEARCH_ENGINE_INDEXHANDLER_H
#include <string>
#include "AVLTree.h"
#include "include/rapidjson/rapidjson.h"
#include "HashTable.h"
using namespace std;
/// Index Handler that contains AVLTree of search terms
class IndexHandler{
public:
    AVLTree tree;           ///AVLTree that holds search terms
    TreeNode* root = nullptr;       ///Tree Node Pointer that holds root of AVLTree
    HashTable<string, vector<string>> authors;      ///Hash table to index authors
    vector<string> stopWords;       ///Vector containing all stop words
    string path;        ///String containing folder directory

    int totalArticles = 0;      ///Total number of articles
    long totalWords = 0;        ///Total number of words
    int totalAuthors = 0;       ///Total number of Authors

    void makeTree(char** argv);         ///Creates the index using directory specified as command-line argument
    vector<string> searchQuery(string &query);      ///Searches the index for a query, returns vector of viable paper IDs
    vector<string> searchAndOr(string &query);      ///Searches index for queries involving AND/OR operators

};



#endif //SEARCH_ENGINE_INDEXHANDLER_H
