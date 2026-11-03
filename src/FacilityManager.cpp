#include "FacilityManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

FacilityManager::FacilityManager() {}

void FacilityManager::loadFacilityFile(string filePath, vector<string>buildings) {
    int acceptedCount = 0;
    int rejectedCount = 0;

    ifstream inFile;
    inFile.open(filePath);

    if (inFile.is_open() == false) {
        cout << "ERROR - Could not open the file " << filePath << endl;
        cout << filePath << ": 0 accepted, 0 rejected. "
             << "Hash Capacity: " << idHashTable.getCapacity() << ", "
             << "entries: " << idHashTable.getEntries() << ", "
             << "collision: " << idHashTable.getCollisions() << endl;
        return;
    }

    string fullLine;
    int lineNumber = 0;

    while (getline(inFile, fullLine)) {
        lineNumber++;

        // skip blank line and comment lines
        if(isBlank(fullLine) || isComment(fullLine)) {
            continue;
        }

        // parse full line
        stringstream lineStream(fullLine);
        vector<string> fields;
        string field;

        while (getline(lineStream, field, '|')) {
            fields.push_back(trim(field));
        }
        string trimmed = trim(fullLine);
        if (trimmed[trimmed.length() - 1] == '|') {
            fields.push_back("");
        }

        // check field count
        if (fields.size() != 4) {
            cout << filePath << " expected 4 fields, line " 
                 << lineNumber << " has " << fields.size() << endl;
            rejectedCount++;
            continue;
        }

        string id = normalizeID(fields[0]);
        string name = fields[1];
        string building = fields[2];
        string category = fields[3];

        // check empty fields
        if (id == "" || name == "" || building == "" || category == "") {
            cout << filePath << " line " << lineNumber
                 << " has empty field" << endl;
            
            rejectedCount++;
            continue;
        }

        // check whether building in building.txt
        int buildingId = findIgnoreCase(buildings, building);
        if (buildingId == -1) {
            cout << filePath << " line " << lineNumber
                 << " has unknown building " << building << endl;
            rejectedCount++;
            continue;
        }
        // if found, use the exact name from buildings.txt
        building = buildings[buildingId];


        // check ID format
        if (isValidID(id) == false) {
            cout << filePath << " line " << lineNumber
                 << " has invalid ID " << id << endl;
            rejectedCount++;
            continue;
        }

        // check duplicate ID
        if (idHashTable.HashGet(id) != -1) {
            cout << filePath << " line " << lineNumber
                 << " has duplicate ID " << id << endl;
            
            rejectedCount++;
            continue;
        }

        Facility facility(id, name, building, category);
        facilities.push_back(facility);

        // index the facility in the vector to hash table
        int index = facilities.size()-1;
        idHashTable.HashInsert(id, index);
        acceptedCount++;
    }

    inFile.close();
    
    cout << filePath << ": " << acceptedCount << " accepted, "
         << rejectedCount << " rejected. " 
         << "Hash Capacity: " << idHashTable.getCapacity() << ", "
         << "entries: " << idHashTable.getEntries() << ", "
         << "collision: " << idHashTable.getCollisions() << endl;

}

void FacilityManager::displayFacility() {
    if (facilities.size() == 0) {
        cout << "No facilities found" << endl;
        return;
    }
    for (int i = 0; i < facilities.size(); i++) {
        cout << facilities[i].getID() << " | "
             << facilities[i].getName() << " | "
             << facilities[i].getBuilding() << " | "
             << facilities[i].getCategory() << endl;
    }
    cout << "Total facilities: " << facilities.size() << endl;
}

void FacilityManager::searchByID(string input) {
    string id = normalizeID(input);

    if (!isValidID(id)) {
        cout << "Invalid ID. The ID start with F followed by three digits" << endl;
        return;
    }

    // look up for the id in hash table
    int index = idHashTable.HashGet(id);

    if (index == -1) {
        cout << id << " is not found" << endl;
    }
    else {
        cout << id << " is found" << endl;
        cout << facilities[index].getID() << " | "
             << facilities[index].getName() << " | "
             << facilities[index].getBuilding() << " | "
             << facilities[index].getCategory() << endl;        
    }
}

vector<Facility> FacilityManager::getFacilities() {
    return facilities;
}
