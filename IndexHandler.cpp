#include "IndexHandler.h"
#include "include/rapidjson/document.h"
#include "porter2_stemmer.h"
#include <sstream>
#include "AVLTree.h"
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <vector>
using namespace std;
using namespace rapidjson;

void IndexHandler::makeTree(char** argv) {
    DIR *directory = opendir(argv[1]);          //Opening folder directory
    dirent *d;

    string wholeFile, buffer, id, word;         //Declaring strings for file input
    vector<string> authorNames;
    path = argv[1];
    stringstream ss;                            //Stringstream will hold current line
    Document doc;
    ifstream input;

    input.open("stopWords.csv");                        //Populates stopWords vector with all stop words
    while(getline(input, buffer, ',')){
        stopWords.push_back(buffer);
    }
    buffer = "";
    input.close();

    d = readdir(directory);
    d = readdir(directory);

    while ((d = readdir(directory))) {                          //Loops until end of directory
        input.open(path + "/" + d->d_name);                 //Opening file
        while (getline(input, buffer))                  //Loops until end of line
            wholeFile += buffer;
        input.close();
        doc.Parse(wholeFile.c_str());                           //Parsing .json
        wholeFile = "";                                         //Resetting wholeFile after doc has been parsed
        id = doc["paper_id"].GetString();                       //Holds current paper's ID
        authorNames.clear();

        for(int x = 0; x < doc["metadata"]["authors"].Size(); x++){
            authorNames.push_back(doc["metadata"]["authors"][x]["last"].GetString());
            totalAuthors++;
        }

        authors.insert(id, authorNames);


        for(int x = 0; x < doc["body_text"].Size(); x++) {              //Loops until end of file's body text
            ss.clear();                                                 //Clears stringstream for new line
            ss.str(doc["body_text"][x]["text"].GetString());         //Moves stringstream to next line
            while (ss >> word) {
                //Porter2Stemmer::trim(word);
                Porter2Stemmer::stem(word);
                if(count(stopWords.begin(), stopWords.end(), word))
                    continue;

                root = tree.insertID(root, word, id);               //Inserting node
                totalWords++;
            }

        }
        totalArticles++;
    }
    input.close();
}

vector<string> IndexHandler::searchQuery(string& query) {       //Returns vector of viable papers given a simple keyword query
    stringstream ss(query);                         //Declaring variables for file input
    string keyWord, buffer, notWord, author;
    ss >> keyWord;

    if(ss >> buffer){                           //Reading until end of query
        if(buffer == "NOT"){                    //Stems and stores word that should be avoided
            ss >> notWord;
            Porter2Stemmer::stem(notWord);
            if(ss >> buffer)                    //If the next word is "AUTHOR" operator, stores name in author string
                if(buffer == "AUTHOR")
                    ss >> author;
        }
        if(buffer == "AUTHOR"){             //Stores author's name in string
            ss >> author;
        }
    }

    vector<string> viable, notWords;        //Vectors to hold ids that will be compared
    Porter2Stemmer::stem(keyWord);
    TreeNode* node = nullptr;

    cout << "Query: " << query << endl;
    node = tree.search(root, keyWord);      //Finding keyword in the index

    for(int x = 0; x < node->ids.size(); x++){      //Copying paper id's from node into viable
        viable.push_back(node->ids.at(x));
    }
    if(!notWord.empty()) {                          //Populates notWords vector if NOT operator is used
        node = tree.search(root, notWord);
        for(int x = 0; x < node->ids.size(); x++){
            notWords.push_back(node->ids.at(x));
        }
    }
    vector<string>::iterator endRange;              //Modifies viable list to only contain values not found in stopWords
    endRange = set_difference(viable.begin(), viable.end(), notWords.begin(), notWords.end(), viable.begin());
    viable.erase(endRange, viable.end());

    if(!author.empty()) {               //Checks for AUTHOR operator
        vector<string> final;                               //Vector final holds remaining ids after all operators
        for(int x = 0; x < viable.size(); x++){
            for(int y = 0; y < authors.getValue(viable.at(x)).size(); y++){
                if(author == authors.getValue(viable.at(x)).at(y))          //Parses authors at each key and adds current paper id to final vector if author is found
                    final.push_back(viable.at(x));
            }
        }
        return final;       //Returns final vector if AUTHOR operator was used
    }
    return viable;          //Returns viable vector if either NOT or no operator was used

}

vector<string> IndexHandler::searchAndOr(string &query) {       //Returns a vector of viable papers given a query with AND/OR operators
    stringstream ss(query);                         //Declaring variables for file input
    string buffer, notWord, author, boolOperator;
    ss >> boolOperator;
    TreeNode* node = nullptr;
    vector<string> viable, results, notWords;

    vector<string> keyWords;
    while(ss >> buffer){                //Reading until end of query
        if(buffer == "NOT"){
            ss >> notWord;                      //Reading in word to avoid
            Porter2Stemmer::stem(notWord);
            continue;
        }
        if(buffer == "AUTHOR"){         //Reading in author's name
            ss >> author;
            break;
        }
        Porter2Stemmer::stem(buffer);
        keyWords.push_back(buffer);             //Adding current keyword to vector of keywords
        node = tree.search(root, buffer);         //Searching index for keyword
        for(int x = 0; x < node->ids.size(); x++){
            viable.push_back(node->ids.at(x));      //Adding potentially viable papers to viable vector
        }
    }

    if(boolOperator == "AND") {             //Processing AND operator
        sort(viable.begin(), viable.end());         //Sorting paper ids
        for (int x = 1; x < viable.size(); x++) {       //Adding only duplicate values to results vector
            if (viable.at(x - 1) == viable.at(x))
                results.push_back(viable.at(x));
        }
        results.erase(unique(results.begin(), results.end()), results.end());   //Removing duplicate values from results
    }
    if(boolOperator == "OR"){               //Processing OR operator
        sort(viable.begin(), viable.end());         //Sorting paper ids
        viable.erase(unique(viable.begin(), viable.end()), viable.end());   //Removing duplicate values from results
        for(int x = 0; x < viable.size(); x++){     //Copying values from viable into results vector
            results.push_back(viable.at(x));
        }
    }
    if(!notWord.empty()) {                      //Processing NOT operator
        node = tree.search(root, notWord);      //Finding word to avoid and populating notWords vector
        for(int x = 0; x < node->ids.size(); x++){
            notWords.push_back(node->ids.at(x));
        }
    }
    vector<string>::iterator endRange;          //Modifies results to only contain values not found in notWords
    endRange = set_difference(results.begin(), results.end(), notWords.begin(), notWords.end(), results.begin());
    results.erase(endRange, results.end());

    if(!author.empty()) {                       //Processing AUTHOR operator
        vector<string> final;                       //final vector will hold ids after all other operators have been processed
        for(int x = 0; x < results.size(); x++){
            for(int y = 0; y < authors.getValue(results.at(x)).size(); y++){        //Parses authors at each key and adds current paper id to final vector if author is found
                if(author == authors.getValue(results.at(x)).at(y))
                    final.push_back(results.at(x));
            }
        }
        return final;           //Returns final vector if AUTHOR operator was used
    }
    return results;         //Returns results vector if either NOT or no operator was used
}


