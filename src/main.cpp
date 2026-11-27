#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include "HashTable.h"
#include "FacilityManager.h"
#include "RuleHelper.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <cctype>

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
  cout << "3. Search facility by name" << endl;
  cout << "4. Search facility by category" << endl;
  cout << "5. View campus graph" << endl;
  cout << "6. Display graph counts and degrees" << endl;
  cout << "7. Find reachable buildings (BFS)" << endl;
  cout << "8. Find shortest path (BFS)" << endl;
  cout << "9. Add department" << endl;
  cout << "10. Search department" << endl;
  cout << "11. Display department hierarchy" << endl;
  cout << "12. Preorder traversal" << endl;
  cout << "13. Postorder traversal" << endl;
  cout << "14. Level-order traversal" << endl;
  cout << "15. Department statistics" << endl;
  cout << "16. Facility category report" << endl;
  cout << "17. Graph connectivity report" << endl;
  cout << "18. Exit" << endl;

}

// argc and argv allow an optional data directory to be passed down when running program
int main(int argc, char* argv[]){

  string dataDirectory = "data";

  if (argc > 1) {
    dataDirectory = argv[1];
  }

  // graph code
  int lineNumber = 0;
  
  //file input section, reading data from files and separating into strings in vectots
  vector<string> buildings;
  ifstream inFile;
  inFile.open(dataDirectory + "/buildings.txt");

  string line;
  int buildingsAccepted = 0;
  int buildingsRejected = 0;

  if (inFile.is_open() == false) {
        cout << "ERROR - Could not open " << dataDirectory
             << "/buildings.txt" << endl;
  }
  else {
      while(getline(inFile, line)){
        bool alreadyExist = false;
        bool isHeader = false;
        stringstream lineStream(line);
        lineNumber++;

        if (isBlank(line) || isComment(line)) {
          continue;
        }

        string buildingName = trim(line);
        char firstChar = lowercased(buildingName).at(0);
          if( (firstChar<97) || (firstChar>122) ){
            isHeader = true; //gets rid of first line w/ # - dont have to repeat for connections since already checkes if buildings exist there
          }
          if(isHeader) continue;
          for(int i = 0; i < buildings.size(); i++){
            if(lowercased(buildingName)==(lowercased(buildings[i]))){
              alreadyExist = true;
              cout <<"ERROR - buildings.txt line "<<lineNumber<<" duplicate record"<<endl;
            }
          }
          if(!alreadyExist) {
            buildings.push_back(buildingName);
            buildingsAccepted++;
          } 
          else {
            buildingsRejected++;
          }
          alreadyExist = false;
      }
  }
  
    inFile.close();
    lineNumber = 0;

    cout << "[" << dataDirectory << "/buildings.txt]: " 
         << buildingsAccepted << " accepted, "
         << buildingsRejected << " rejected" << endl;
  
  vector<string> connections;
  inFile.open(dataDirectory + "/connections.txt");
  int connectionsAccepted = 0;
  int connectionRejected = 0;

  if (inFile.is_open() == false) {
        cout << "ERROR - Could not open " << dataDirectory
             << "/connections.txt" << endl;
  }

  else {

    while(getline(inFile, line)){
      bool build1Exist = false;
      bool build2Exist = false;
      if(!line.empty() && line.back()=='\r'){
        line.pop_back();
      } //get rid of /r
      
      lineNumber++;

      if (isBlank(line) || isComment(line)) {
        continue;
      }

      stringstream linestream(line);
      
      string building1, building2;
      getline(linestream, building1, '|');
      getline(linestream, building2);
      building1 = trim(building1);
      building2 = trim(building2);
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
      if(connections.size()>=2){
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
        connectionsAccepted++;
      }
      else {
        if (!alreadyExist) {
          if (building1 == "" || building2 == "") {
            cout << "ERROR - connections.txt line " << lineNumber
                 << " wrong field count" << endl;
          }
          else if (lowercased(building1) == lowercased(building2)) {
            cout << "ERROR - connections.txt line " << lineNumber
                 << " self-connection " << building1 << endl;
          }
          else if (!build1Exist) {
            cout << "ERROR - connections.txt line " << lineNumber
                 << " unknown building " << building1 << endl;
          }
          else {
            cout << "ERROR - connections.txt line " << lineNumber
                 << " unknown building " << building2 << endl;
          }
      }
      connectionRejected++;
    }
  }
}

    inFile.close();
    lineNumber = 0;
    cout << "[" << dataDirectory << "/connections.txt]: "
         << connectionsAccepted << " accepted, "
         << connectionRejected << " rejected" << endl;

  // load facilities.txt
  FacilityManager facilityManager;
  facilityManager.loadFacilityFile( dataDirectory + "/facilities.txt", buildings);

  // Department Tree 
  DepartmentTree departmentTree;
  departmentTree.loadDepartments(dataDirectory + "/departments.txt");

  //creating and intializing graph
  Graph theGraph;
  theGraph.addBuildings(buildings);
  theGraph.addEdges(connections);
  
  // calling displayMenu()
  int choice = -1;

  do {
      cout << endl;
      displayMenu();

      string input;

      cout << "Enter choice: ";
      getline(cin, input);
      if (input.empty() && cin.eof()) {
        return 0;
      }

      
      if (input.empty()) {
          cout << "ERROR - Invalid Menu Input" << endl;
          continue;
      }

      bool validInput = true;

      for (int i = 0; i < input.length(); i++) {
      if (!isdigit(input[i])) {
          validInput = false;
      }
      }

      if (validInput == false || input.length()>2) {
        cout << "ERROR - Invalid Menu Input" << endl;
        continue;
      }

      choice = stoi(input);

      if (choice < 0 || choice > 18) {
        cout << "ERROR - Invalid Menu Input" << endl;
        continue;
      }
      
      switch (choice) {
        
        case 1: {
            facilityManager.displayFacility();
            break;
        }

        case 2: {
            string id;
            cout << "Enter facility ID: ";
            getline(cin, id);
            facilityManager.searchByID(id);            
            break;
        }
        
        case 3: {
          string name;
          cout << "Enter facility name: ";
          getline(cin, name);
          if(trim(name) == "") {
            cout << "ERROR - Facility name cannot be blank." << endl;
            break;
          }
          facilityManager.searchByName(name);
          break;
        }

        case 4: {
          string category;
          cout << "Enter category: ";
          getline(cin, category);
          if(trim(category) == "") {
            cout << "ERROR - Category cannot be blank." << endl;
            break;
          }
          facilityManager.searchByCategory(category);
          break;
        }
        
        case 5:
            theGraph.display(1);
            break;

        case 6:
            theGraph.display(0);
            break;

        case 7: {
            cout<<"Enter starting building name: ";
            string buildName;
            getline(cin, buildName);
            buildName = trim(buildName);
            if(buildName.empty()) {
                cout<<"ERROR - need starting building."<<endl; break;
            }
            theGraph.reachability(buildName);
            break;
            }

        case 8: //shortest path
            string buildNameStart, buildNameEnd;
            cout<<"Enter starting building name: ";
            getline(cin, buildNameStart);
            buildNameStart = trim(buildNameStart);
            cout<<"Enter ending building name: ";
            getline(cin, buildNameEnd);
            buildNameEnd = trim(buildNameEnd);
            if(buildNameStart.empty()) {
                cout<<"ERROR - need starting building."<<endl; break;
            }
            if(buildNameEnd.empty()) {
                cout<<"ERROR - need ending building."<<endl; break;
            }
            theGraph.shortestPath(buildNameStart, buildNameEnd);
            break;

        case 9: {
            string name;
          
            cout << "Enter department name: ";
            getline(cin, name);
            name = trim(name);

            if (name.empty()) {
                cout << "ERROR - Department name cannot be blank." << endl;
                break;
            }

            if (departmentTree.searchDepartment(name) != nullptr) {
                cout << "ERROR - Department name already exists." << endl;
                break;
            }
            
            if (departmentTree.isEmpty()) {
                departmentTree.addDepartment(name, "NONE");
                cout << name << " added as root department." << endl;
            }

            else {
                string parentName;

                cout << "Enter parent department: ";
                getline(cin, parentName);
                parentName = trim(parentName);

                if (parentName.empty()) {
                  cout << "ERROR - Parent department name cannot be blank." << endl;
                  break;
                }

                if (departmentTree.addDepartment(name, parentName)) {
                  cout << name << " added under " << parentName << "." << endl;
                }
            }

            break;
        }

        case 10: {
            string holdName;

            cout << "Search department: ";
            getline(cin, holdName);
            holdName = trim(holdName);

            if (holdName.empty()) {
              cout << "ERROR - Department cannot be blank" << endl;
              break;
            }

            departmentTree.displayDepartmentInfo(holdName);

            break;
        }
        
        case 11: 
            departmentTree.displayHierarchy();
            break;

        case 12:
            departmentTree.displayPreorder();
            break;

          case 13:
            departmentTree.displayPostorder();
            break;

          case 14:
            departmentTree.displayLevelorder();
            break;

          case 15:
            departmentTree.displayStatistics();
            break;

          case 16: {
            facilityManager.categoryReport();
            break;
          }

          case 17: //graph connectivity report
            
            break;

          case 0: //project requirement, option 0 exit cleanly

          case 18:
            cout << "Exiting the program." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
      }

  } while (choice != 18 && choice != 0);

  return 0;
}
