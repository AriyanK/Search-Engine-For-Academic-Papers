#include <fstream>
#include "QueryProcessor.h"
#include "include/rapidjson/document.h"
using namespace std;
using namespace rapidjson;

void QueryProcessor::runProcessor(char** argv) {        //Displays UI and processes user input
    index.makeTree(argv);               //Indexes words into AVLTree

    string option, query, firstWord;  //Declaring variables for file input
    stringstream ss;

    while(option != to_string(3)){          //Loop for GUI
        cout << endl << "Select an option:" << endl << "(1) Search" << endl << "(2) Stats" << endl << "(3) Exit" << endl;
        getline(cin, option);           //Reading user's input

        if(option == to_string(1)) {            //Search option is selected
            cout << "Enter search query: " << endl;
            getline(cin, query);            //Reads search query
            ss.clear();
            ss.str(query);
            ss >> firstWord;
            if (firstWord == "AND" || firstWord == "OR"){           //Calls appropriate search function based on first word in query
                printData(index.searchAndOr(query));
            }
            else{
                printData(index.searchQuery(query));
            }
        }
        if(option == to_string(2)){                         //Prints relevant statistics of the engine
            cout << "Engine stats: " << endl;
            cout << endl << "Individual articles indexed: " << index.totalArticles - 1 << endl;
            cout << "Unique words indexed: " << index.totalWords << endl;
            cout << "Average number of words indexed per article: " << index.totalWords/(index.totalArticles - 1) << endl;
            cout << "Total number of authors: " << index.totalAuthors << endl;
        }
    }
}

void QueryProcessor::printData(vector<string> results) {        //Prints data about papers given a list of viable ids
    string option, buffer, wholeFile;       //Variables for file input
    Document doc;
    ifstream input;
    int x;

    cout << endl << "Select a paper to view: " << endl;
    for(x = 0; x < results.size(); x++) {
        if(x == 15)
            break;
        input.open(index.path + "/" + results.at(x) + ".json");     //Opens and parses each paper in the results vector
        while(getline(input, buffer))
            wholeFile += buffer;
        input.close();
        doc.Parse(wholeFile.c_str());
        wholeFile = "";

        cout << x + 1 << ". " << doc["metadata"]["title"].GetString() << "; Author: " << doc["metadata"]["authors"][0]["last"].GetString() << endl;     //Prints metadata of each paper
    }
    cout << x + 1 << ". Return" << endl;
    cin >> option;
    if(option == to_string(x + 1)) {    //Exits function if user doesn't select a paper
        return;
    }
    input.open(index.path + "/" + results.at(stoi(option, nullptr) - 1) + ".json");
    while(getline(input, buffer))
        wholeFile += buffer;                                //Opens and parses file specified by user
    input.close();
    doc.Parse(wholeFile.c_str());

    for(int y = 0; x < doc["body_text"].Size(); y++){           //Prints preview of the body text
        if(y == 2)
            break;
        cout << doc["body_text"][y]["text"].GetString() << endl;
    }

}

void QueryProcessor::rankTop15() {

}
