#include "CampusGraph.h"

Node::Node(string name){
  buildingName = name;
}
string Node::getBuildingName(){
  return buildingName;
}
void Node::addConnection(Node* otherBuilding){
  edges.push_back(otherBuilding);
}

void Graph::addBuildings(vector<string> uniqBuildings){  //might be broekn will make sure it works later
  for(int i = 0; i < uniqBuildings.size(); i++){
    Node newBuilding(uniqBuildings[i]);
    buildings.push_back(newBuilding);  //no checks needed here - done when reading buildings from file
  }
}

void Graph::addEdges(vector<string> uniqStrings){
  for(int i = 0; i < uniqStrings.size()/2; i++){ //for every 2 strings in uniqStrings there is a connections
    int index1, index2;
    for(int j = 0; j < buildings.size(); j++){
      if(buildings[j].getBuildingName()==uniqStrings[i]){
        index1 = j;
      }
      if(buildings[j].getBuildingName()==uniqStrings[i+1]){
        index2 = j;
      }
      //wont ever be the same because already checks for if same i=when reading in file
    }
    buildings[index1].addConnection(&buildings[index2]); 
    buildings[index2].addConnection(&buildings[index1]);
  }
}

void Graph::display(){

}

void Graph::BFS(Node goal){

}
