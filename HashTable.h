#ifndef SEARCH_ENGINE_HASHTABLE_H
#define SEARCH_ENGINE_HASHTABLE_H
#include <vector>
#include <iostream>
using namespace std;

#define DEBUG false
#define DEBUG2 true

template <typename K, typename V>

///Hash Table indexes authors
class HashTable{
private:

    ///Entry contains key-value pair
    struct Entry{
        K key;
        V value;
        Entry(K k, V v):key(k), value(v) {}     ///Basic constructor for Entry
    };
    vector<Entry>* table = nullptr;         ///Contains vectors of key-value pairs
    int tableSize = 0;          ///Holds size of hash table
public:
    HashTable(){            ///Constructor that initializes table at size 100
        tableSize = 100;
        table = new vector<Entry>[tableSize];
    }
    void insert(const K& theKey, const V& theValue){    ///Inserts key-value pair into table
        Entry e(theKey, theValue);
        auto hashValue = hash<K>{}(theKey);
        int location = hashValue % tableSize;
        if(DEBUG) cout << theKey << ": "
                        << hashValue << endl;
        table[location].push_back(e);
    }

    V& getValue(const K& theKey){           ///Returns value given corresponding key
        auto location = hash<K>{}(theKey) % tableSize;
        for(auto &c : table[location]){
            if(c.key == theKey)
                return c.value;
        }
    }

};



#endif //SEARCH_ENGINE_HASHTABLE_H
