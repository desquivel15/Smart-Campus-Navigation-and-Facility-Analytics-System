#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H

#include "Facility.h"
#include "HashTable.h"
#include "RuleHelper.h"
#include <string>
#include <vector>
using namespace std;

class FacilityManager {
    private:
    vector<Facility> facilities;
    HashTable idHashTable;

    public:
    FacilityManager();

    void loadFacilityFile(string filePath, vector<string>buildings);

    void displayFacility();

    void searchByID(string id);

    void searchByName(string name);

    void searchByCategory(string category);

    void categoryReport();

    vector<Facility> getFacilities();
};

#endif
