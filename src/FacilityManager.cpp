#include "FacilityManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

FacilityManager::FacilityManager() {}

void FacilityManager::loadFacilityFile(string filePath, vector<string>buildings) {
    int acceptedCount = 0;
    int rejectedCount = 0;

    ifstream inFile;
    inFile.open(filePath);

    if (inFile.is_open() == false) {
        cout << "ERROR - Could not open the file " << filePath << endl;
        cout << "[" << filePath << "]: 0 accepted, 0 rejected. "
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
    
    cout << "[" << filePath << "]: " << acceptedCount << " accepted, "
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

void FacilityManager::searchByName(string input) {
    string name = trim(input);
    int nameMatch = 0;
    
    for (int i = 0; i < facilities.size(); i++) {
        // compare by ignoring case and outer whitespace
        if (toUpper(facilities[i].getName()) == toUpper(name)) {
            cout << facilities[i].getID() << " | "
                 << facilities[i].getName() << " | "
                 << facilities[i].getBuilding() << " | "
                 << facilities[i].getCategory() << endl;
            nameMatch++;
        }
    }

    if (nameMatch == 0) {
        cout << "The facility name " << name << " is not found." << endl;
    }
    cout << "Total facility name matches: " << nameMatch << endl;
}

void FacilityManager::searchByCategory(string input) {
    string category = trim(input);
    int categoryMatch = 0;

    for (int i = 0; i < facilities.size(); i++) {
        if(toUpper(facilities[i].getCategory()) == toUpper(category)) {
            cout << facilities[i].getID() << " | "
                 << facilities[i].getName() << " | "
                 << facilities[i].getBuilding() << " | "
                 << facilities[i].getCategory() << endl;
            categoryMatch++;            
        }
    }

    if (categoryMatch == 0) {
        cout << "The facility in category " << category << " is not found." << endl;
    }
    cout << "Total facility category matches: " << categoryMatch << endl;
}

/*
Facility category report: 
for each distinct category, display facility count and percentage of all accepted facilities (one decimal place). 
Group category names ignoring capitalization and preserve the spelling of the first accepted category. 
Display the total count. If no facilities exist, state that none are available and show total zero; never divide by zero
*/
void FacilityManager::categoryReport() {
    int total = facilities.size();

    //when no facilities
    if(total == 0) {
        cout << "No facilities are available." << endl;
        cout << "Total: 0" << endl;
        return;
    }

    //when having facilities: create 2 vectors to store category names and their counts
    vector<string> categoryNames;
    vector<int> counts;

    for(int i = 0; i < total; i++) {
        string category = facilities[i].getCategory();

        //use findIgnoreCase to return the index of the category in vector
        int index = findIgnoreCase(categoryNames, category);
        //if the category is not in the current vector, add it
        if(index == -1) {
            categoryNames.push_back(category);
            counts.push_back(1);
        }
        //if already in the vector, increase its count
        else {
            counts[index]++;
        }
    }
    
    //display category category name, facility counts, and percentage of accepted facilities
    cout << fixed << setprecision(1);

    for (int i = 0; i < categoryNames.size(); i++) {
        float percent = counts[i] * 100.0 / total;
        cout << categoryNames[i] << ": " << counts[i]
             << " (" << percent << "%)" << endl;
    }

    cout << "Total: " << total << endl;
}

vector<Facility> FacilityManager::getFacilities() {
    return facilities;
}
