#include "AVLTree.h"
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

TreeNode* AVLTree::rotateLeft(TreeNode*& lower) {                                   //Left Rotate
    TreeNode* upper = lower->right;
    TreeNode* subTree = upper->left;

    upper->left = lower;
    lower->right = subTree;

    lower->height = 1 + max(height(lower->left), height(lower->right));
    upper->height = 1 + max(height(upper->left), height(upper->right));
    return upper;
}

TreeNode* AVLTree::rotateRight(TreeNode*& upper) {                                  //Right Rotate
    TreeNode* lower = upper->left;
    TreeNode* subTree = lower->right;

    lower->right = upper;
    upper->left = subTree;

    lower->height = 1 + max(height(lower->left), height(lower->right));
    upper->height = 1 + max(height(upper->left), height(upper->right));
    return lower;
}

TreeNode* AVLTree::insert(TreeNode* root, const string& word) {                   //Insert function for string
    if(root == nullptr){
        return newNode(word);
    }

    if(word < root->word) {
        root->left = insert(root->left, word);
    }
    else if(word > root->word){
        root->right = insert(root->right, word);
    }
    else{
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));
    int balanceFactor = AVLTree::balanceFactor(root);

    if(balanceFactor > 1 && word < root->left->word){
        return rotateRight(root);
    }
    if(balanceFactor > 1 && word > root->left->word){
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if(balanceFactor < -1 && word > root->right->word){
        return rotateLeft(root);
    }
    if(balanceFactor < -1 && word < root->right->word){
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

TreeNode* AVLTree::insertID(TreeNode*& root, const string& word, const string& id) {     //Insert function for query and ID
    if(root == nullptr){                    //Adds node if tree is empty
        return newNodeID(word, id);
    }

    if(word == root->word){                                         //Checks if word has been found
        if(!count(root->ids.begin(), root->ids.end(), id)) {         //Adds ID if it isn't in the node
            root->ids.push_back(id);

            if(root->instances.empty())
                root->instances.push_back(0);
            else
                root->instances.at(root->instances.size() - 1)++;
        }

        return root;
    }
    else if(word < root->word) {                                    //Insert into left subtree
        root->left = insertID(root->left, word, id);
    }
    else if(word > root->word){                                     //Insert into right subtree
        root->right = insertID(root->right, word, id);
    }

    root->height = 1 + max(height(root->left), height(root->right));    //Set height
    int balanceFactor = AVLTree::balanceFactor(root);


    if(root->left && balanceFactor > 1 && word < root->left->word){
        return rotateRight(root);
    }
    if(root->left && root->left->right && balanceFactor > 1 && word > root->left->word){
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if(root->right && balanceFactor < -1 && word > root->right->word){
        return rotateLeft(root);
    }
    if(root->right && root->right->left && balanceFactor < -1 && word < root->right->word){
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void AVLTree::preOrderTraversal(TreeNode* root) {           //Printing pre-order traversal of tree
    if(root == nullptr)
        return;
    cout << root->word << ", ";
    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}

void AVLTree::inOrderTraversal(TreeNode* root) {            //Printing in-order traversal of tree
    if(root == nullptr)
        return;
    inOrderTraversal(root->left);
    cout << root->word << ", ";
    inOrderTraversal(root->right);
}

void AVLTree::postOrderTraversal(TreeNode* root) {          //Printing post-order traversal of tree
    if(root == nullptr)
        return;
    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    cout << root->word << ", ";
}

TreeNode* AVLTree::newNode(string word) {                   //Makes new node containing string
    auto* node = new TreeNode();
    node->word = std::move(word);
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

TreeNode* AVLTree::newNodeID(const string& word, const string& id) {        //Makes new node containing query and ID
    auto* node = new TreeNode();
    node->word = word;
    node->ids.push_back(id);
    node->instances.push_back(0);
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

int AVLTree::height(TreeNode*& node) {                      //Returns height of given node
    if(node == nullptr){
        return 0;
    }
    return node->height;
}

int AVLTree::balanceFactor(TreeNode*& node) {               //Finds balance factor of given node
    if(node == nullptr){
        return 0;
    }
    return height(node->left) - height(node->right);    //returns difference between height of right and left subtree
}

TreeNode* AVLTree::search(TreeNode*& root, const string& word) {            //Finds query in tree
    if(root->word == word)                      //Returns node if it contains the query
        return root;
    else if(word < root->word)                  //Moves to left subtree
        return search(root->left, word);
    else if(word > root->word)                  //Moves to right subtree
        return search(root->right, word);
}





