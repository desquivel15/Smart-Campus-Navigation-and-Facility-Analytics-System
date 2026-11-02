#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include "HashTable.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 

using namespace std;

string lowercased(string given){
  string returned;
  for(int i = 0; i < given.size(); i++){
    if( (given[i] > 64) && (given[i] < 91) ){
        returned += given[i]+32;
    }
  }
  return returned;
}

int main(){
  int lineNumber;
  //file input section, reading data from files and separating into strings in vectots
  vector<string> buildings;
  ifstream inFile;
  inFile.open("data/buildings.txt");
  if (inFile.is_open() == false) {
        cout << "ERROR - File could not open.";
        return 1;
  }
  string line;
  bool alreadyExist = false;
  while(getline(inFile, line)){
    stringstream lineStream(line);
    lineNumber++;
    string buildingName = line;
    for(int i = 0; i < buildings.size(); i++){
      if(lowercased(buildingName)==(lowercased(buildings[i]))){
        alreadyExist = true;
        cout<<"ERROR - buildings.txt line "<<lineNumber<<" duplicate record"<<endl;
      }
    }
    if(!alreadyExist)  buildings.push_back(line);
  }
  inFile.close();
  lineNumber = 0;
  
  vector<string> connections;
  inFile.open("data/connections.txt");
   if (inFile.is_open() == false) {
        cout << "ERROR - File could not open.";
        return 1;
  }
  alreadyExist = false;
  bool buildsExist = true;
  while(getline(inFile, line)){
    stringstream linestream(line);
    line.pop_back(); //get rid of /r
    lineNumber++;
    string building1, building2;
    getline(linestream, building1, '|');
    getline(linestream, building2);
    building2.pop_back(); //get rid of carrier for now
    for(int i = 0; i < buildings.size(); i++){
      if( lowercased(building1) == lowercased(buildings[i]) ){
        buildsExist = true;
      }
      if( lowercased(building2) == lowercased(buildings[i]) ){
        buildsExist = true;
      }
      if( lowercased(building1) == lowercased(building2) ){ 
        buildsExist = false; 
      }
    }
    for(int i = 0; i < connections.size(); i++){
        string temp = building1+"|"+building2;
        string reverse = building2+"|"+building1;
        if( (connections[i] == temp) || (connections[i] == reverse) ){
            alreadyExist = true;
            cout<<"ERROR - connections.txt line "<<lineNumber<<" duplicate record"<<endl;
        }
    }
    if(buildsExist && !alreadyExist){
      connections.push_back(line); //entire line, can resplit connections later 
    }
  }
  lineNumber = 0;
  
    
  return 0;
}
/*
    // variable for getline to read the full line
    string fullLine;

    // loop so each line can be read inside file
    while (getline(inFile, fullLine)) {
        // parse full line here
        stringstream lineStream(fullLine);

        string resourceID;
        string resourceName;
        string resourceType;
        string availabilityText;
        bool availabilityStatus;

       getline(lineStream, resourceID, '|');
       getline(lineStream, resourceName, '|');
       getline(lineStream, resourceType, '|');
       getline(lineStream, availabilityText);

       if (availabilityText == "Available") {
            availabilityStatus = true;
       }
       else if (availabilityText == "Unavailable") {
            availabilityStatus = false;
       }
       else {
            cout << "ERROR - Wrong Input.";
            return 1;
       }

       // Resource object from parsed file data
       Resource resource(resourceID, resourceName, resourceType, availabilityStatus);

       // add to vector
       resources.push_back(resource);

    }
    
    ifstream reservationFile;
    reservationFile.open("data/reservations.txt");

    if (reservationFile.is_open() == false) {
        cout << "ERROR - File could not open.";
        return 1;
    }

    string reservationLine;
    ReservationManager reservationManager;

    while (getline(reservationFile, reservationLine)) {
        stringstream lineStream(reservationLine);

        string reservationID;
        string studentId;
        string studentName;
        string resourceId;
        string date;

       getline(lineStream, reservationID, '|');
       getline(lineStream, studentId, '|');
       getline(lineStream, studentName, '|');
       getline(lineStream, resourceId, '|');
       getline(lineStream, date);

       reservationManager.createNode(reservationID, studentId, studentName, resourceId, date);
    }
    reservationFile.close();

*/
