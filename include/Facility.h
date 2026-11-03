/*
Facility: Campus resource with an ID, name, building, and category.

Facility IDs are case-insensitive. After trimming, normalize every ID (from a file or typed by the user) to uppercase, 
and only then validate the format, hash, insert, compare, and display it. 
A valid ID is F followed by exactly three digits, such as F001. An ID written as f004 in a data file is accepted and stored as F004.

*/
#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using namespace std;

class Facility {
    private:
    string id;
    string name;
    string building;
    string category;

    public:
    Facility();
    Facility(const string& id, const string& name, 
             const string& building, const string& category);
    
    
    string getID() const;
    string getName() const;
    string getBuilding() const;
    string getCategory() const;

};

#endif
