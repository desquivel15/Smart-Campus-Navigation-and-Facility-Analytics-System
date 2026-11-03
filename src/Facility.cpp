#include "Facility.h"
using namespace std;

Facility::Facility() {}

Facility::Facility(const string& id, const string& name, 
             const string& building, const string& category) {
    this->id = id;
    this->name = name;
    this->building = building;
    this->category = category;
}

string Facility::getID() const {return id;}
string Facility::getName() const {return name;}
string Facility::getBuilding() const {return building;}
string Facility::getCategory() const {return category;}
