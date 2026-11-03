#include "HashTable.h"

// Initialize empty bucket
HashTable::HashTable(int capacity) {
    hashTable.resize(capacity, nullptr);
    entries = 0;
    collisions = 0;
}

// Delete items in every bucket
HashTable::~HashTable() {
    for(int i = 0; i < hashTable.size(); i++) {
        HashItem* currentItem = hashTable[i];
        while (currentItem != nullptr) {
            HashItem* nextItem = currentItem->next;
            delete currentItem;
            currentItem = nextItem;
        }
    }
}

// Hash function
int HashTable::Hash(string key) {
    int hashCode = 0;
    for(int i = 0; i < key.length(); i++) {
        hashCode = hashCode * 27 + key[i];
    }
    return hashCode;
}

// Hash Insert: insert key/value pair
bool HashTable::HashInsert(string key, int value) {
    // Hash the key to get bucket index
    int bucketIndex = Hash(key) % hashTable.size();

    // Search the key in the linked list
    HashItem* currentItem = hashTable[bucketIndex];
    HashItem* previousItem = nullptr;
    while (currentItem != nullptr) {
        // If the key already exists, return false, table is not changed
        if (key == currentItem->key) {
            return false;
        }

        // If they key is not found, the previousItem is the last item
        previousItem = currentItem;
        currentItem = currentItem->next;
    }

        // Then, allocate new item
        HashItem* newItem = new HashItem;
        newItem->key = key;
        newItem->value = value;
        newItem->next = nullptr;

        // Append new item to the linked list
           // Append into empty bucket
        if(hashTable[bucketIndex] == nullptr) {
            hashTable[bucketIndex] = newItem;
        }
           // Append into non-empty bucket
        else {
            previousItem->next = newItem;
            collisions++;
        }
        entries++;
        return true;
}

// Remove a key/value pair
bool HashTable::HashRemove(string key) {
    // Hash the key the get bucket index
    int bucketIndex = Hash(key) % hashTable.size();

    // Search bucket linked list for the key
    HashItem* currentItem = hashTable[bucketIndex];
    HashItem* previousItem = nullptr;
    while (currentItem != nullptr) {
        // if the key is found, remove the item from the bucket linked list
        if (key == currentItem->key) {
            // case 1: remove when it is the first item
            if (previousItem == nullptr) {
                hashTable[bucketIndex] = currentItem->next;
            }
            // case 2: remove when it is not the first item
            else {
                previousItem->next = currentItem->next;
            }
            delete currentItem;
            entries--;
            return true;
        }
        // if the key is not found, continue to transverse
        previousItem = currentItem;
        currentItem = currentItem->next;
    }

    // if the key is totally not found, return false
    return false;
}

// Search for the key and Return corresponding value
int HashTable::HashGet(string key) {
    // Hash the key to get the bucket index
    int bucketIndex = Hash(key) % hashTable.size();

    // Search the key in the bucket linked list
    HashItem* item = hashTable[bucketIndex];
    while (item != nullptr) {
        // return value when the key found
        if (key == item->key) {
            return item->value;
        }
        item = item->next;
    }
        // return -1 when the key not found 
    return -1;
}

// Diagnostic report
int HashTable::getCapacity() {
    return hashTable.size();
}

int HashTable::getEntries() {
    return entries;
}

int HashTable::getCollisions() {
    return collisions;
}
