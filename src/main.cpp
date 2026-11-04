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
    } else {
        returned += given[i];
    }
  }
  return returned;
}

void displayMenu() {

  cout << "1. View all facilities" << endl;
  cout << "2. Search facility by ID" << endl;
  cout << "3. View campus graph" << endl;
  cout << "4. Display graph counts and degrees" << endl;
  cout << "5. Find reachable buildings (BFS)" << endl;
  cout << "6. Add department" << endl;
  cout << "7. Search department" << endl;
  cout << "8. Display department hierarchy" << endl;
  cout << "9. Run preorder traversal" << endl;
  cout << "0. Exit" << endl;

}

int main(){

  // graph code
  int lineNumber = 0;
  
  //file input section, reading data from files and separating into strings in vectots
  vector<string> buildings;
  ifstream inFile;
  inFile.open("data/buildings.txt");
  if (inFile.is_open() == false) {
        cout << "ERROR - File could not open.";
        return 1;
  }
  string line;
  while(getline(inFile, line)){
    bool alreadyExist = false;
    bool isHeader = false;
    stringstream lineStream(line);
    lineNumber++;
    string buildingName = line;
    char firstChar = lowercased(buildingName).at(0);
    if( (firstChar<97) || (firstChar>122) ){
      isHeader = true; //gets rid of first line w/ # - dont have to repeat for connections since already checkes if buildings exist there
    }
    if(isHeader) continue;
    for(int i = 0; i < buildings.size(); i++){
      if(lowercased(buildingName)==(lowercased(buildings[i]))){
        alreadyExist = true;
        cout<<"ERROR - buildings.txt line "<<lineNumber<<" duplicate record"<<endl;
      }
    }
    if(!alreadyExist)  buildings.push_back(line);
    alreadyExist = false;
  }
  inFile.close();
  lineNumber = 0;
  
  vector<string> connections;
  inFile.open("data/connections.txt");
  if (inFile.is_open() == false) {
    cout << "ERROR - File could not open.";
    return 1;
  }
  while(getline(inFile, line)){
    bool build1Exist = false;
    bool build2Exist = false;
    stringstream linestream(line);
    if(!line.empty() && line.back()=='\r'){
      line.pop_back();
    } //get rid of /r
    lineNumber++;
    string building1, building2;
    getline(linestream, building1, '|');
    getline(linestream, building2);
    for(int i = 0; i < buildings.size(); i++){
      if( lowercased(building1) == lowercased(buildings[i]) ){
        build1Exist = true;
      }
      if( lowercased(building2) == lowercased(buildings[i]) ){
        build2Exist = true;
      }
      if( lowercased(building1) == lowercased(building2) ){ 
        build1Exist = false;
        build2Exist = false;
      }
    }
    bool alreadyExist = false;
    if(connections.size()>2){
      for(int i = 0; i < connections.size()-1; i+=2){
        if( (connections[i]==building1) && (connections[i+1]==building2) ){
          alreadyExist = true;
          cout<<"ERROR - connections.txt line "<<lineNumber<<" duplicate record"<<endl;
        }
        if( (connections[i+1]==building1) && (connections[i]==building2) ){
          alreadyExist = true;
          cout<<"ERROR - connections.txt line "<<lineNumber<<" duplicate record"<<endl;
        }
      }
    }
    if(build1Exist && build2Exist && !alreadyExist){ //BOTH buildings that are connected get added here - when adding to graph accesses both
      connections.push_back(building1);
      connections.push_back(building2);
    }
  }
  lineNumber = 0;

  // Department Tree 
  DepartmentTree departmentTree;

  departmentTree.loadDepartments("data/departments.txt");

  //creating and intializing graph
  Graph theGraph;
  theGraph.addBuildings(buildings);
  theGraph.addEdges(connections);
  
  // calling displayMenu()
  int choice;

  do {
      displayMenu();

      string input;

      cout << "Enter choice: ";
      getline(cin, input);

      if (input.empty() || input.length() != 1) {
        cout << "ERROR - Invalid Menu Input" << endl;
        continue;
      }

      if (!isdigit(input[0])) {
        cout << "ERROR - Invalid Menu Input" << endl;
        continue;
      }

      choice = stoi(input);

      switch (choice) {

        case 0:
            break;

        case 1:
            break;

        case 2:
            break;
        
        case 3:
            theGraph.display();
            break;

        case 4:
            cout<<"Enter starting building name: ";
            string buildName;
            getline(cin, buildName);
            if(buildName.empty(){
                cout<<"ERROR - need starting building."<<endl; break;
            }
            theGraph.reachability(buildName);
            break;

        case 5:
            break;

        case 6: {
            string name;
          
            cout << "Enter department name: ";
            getline(cin, name);

            if (name.empty()) {
                cout << "ERROR - Department name cannot be blank." << endl;
                break;
            }
            
            if (departmentTree.isEmpty()) {
                departmentTree.addDepartment(name, "NONE");
            }

            else {
                string parentName;

                cout << "Enter parent department: ";
                getline(cin, parentName);

                departmentTree.addDepartment(name, parentName);
            }

            break;
        }

        case 7: {
            string holdName;

            cout << "Search department: ";
            getline(cin, holdName);

            if (holdName.empty()) {
              cout << "ERROR - Department cannot be blank" << endl;
              break;
            }

            departmentTree.displayDepartmentInfo(holdName);

            break;
        }
        
        case 8: 
            departmentTree.displayHierarchy();
            break;

        case 9:
            departmentTree.displayPreorder();
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
      }

  } while (choice != 0);
  




  return 0;
}

