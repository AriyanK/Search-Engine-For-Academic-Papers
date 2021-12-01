#ifndef SEARCH_ENGINE_AVLTREE_H
#define SEARCH_ENGINE_AVLTREE_H
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

///TreeNode contains individual search terms
struct TreeNode{
    string word;            ///Search term contained within node
    vector<string> ids;         ///Vector containing paper ids
    vector<int> instances;
    TreeNode* left;     ///Pointer to left child node
    TreeNode* right;    ///Pointer to right child node
    int height;         ///Contains height value of node
};

///Tree used to index search terms
class AVLTree{
public:
    static TreeNode* newNode(string word);
    static TreeNode* newNodeID(const string& word, const string& id);   ///Creates new TreeNode given keyword and id
    static int height(TreeNode*& node);         ///Returns height of node
    static int balanceFactor(TreeNode*& node);      ///Determines balance factor at node
    static TreeNode* rotateLeft(TreeNode*& lower);  ///Performs left rotation at given node
    static TreeNode* rotateRight(TreeNode*& upper); ///Performs right rotation at given node
    TreeNode* insert(TreeNode* root, const string& word);
    TreeNode* insertID(TreeNode*& root, const string& word, const string& id);  ///Inserts node starting from the root

    TreeNode* search(TreeNode*& root, const string& word);      ///Returns node containing specified word

    void preOrderTraversal(TreeNode* root);     ///Prints a pre-order traversal of AVLTree
    void inOrderTraversal(TreeNode* root);      ///Prints an in-order traversal of AVLTree
    void postOrderTraversal(TreeNode* root);    ///Prints a post-order traversal of AVLTree
};



#endif //SEARCH_ENGINE_AVLTREE_H
