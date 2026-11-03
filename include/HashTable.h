#ifndef HASH_TABLE_H
#define HASH_TABLE_H

using namespace std;
#include <string>
#include <vector>

struct HashItem {
    string key;
    int value;
    HashItem* next;
};

class HashTable {
    public:
    static const int default_capacity = 15;
    
    HashTable(int capacity = default_capacity);
    ~HashTable();

    // Insert key/value pair. Return false if the key already existed
    bool HashInsert(string key, int value);

    // Remove the key. Return false if the key is not found
    bool HashRemove(string key);

    // Return value for they key
    int HashGet(string key);

    // Diagnostic report
    int getCapacity();
    int getEntries();
    int getCollisions();

    private:
    int Hash(string key);

    vector <HashItem*> hashTable;
    int entries;
    int collisions;
};
#endif
