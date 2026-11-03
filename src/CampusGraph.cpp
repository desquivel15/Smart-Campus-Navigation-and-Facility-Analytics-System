#include "CampusGraph.h"
#include <iostream>

Node::Node(string name){
  buildingName = name;
  sumEdges = 0;
}
string Node::getBuildingName(){
  return buildingName;
}
int Node::getNumConnections(){
    return sumEdges;
}
void Node::addConnection(Node* otherBuilding){
  edges.push_back(otherBuilding);
  sumEdges++;
}

void Graph::addBuildings(vector<string> uniqBuildings){
  for(int i = 0; i < uniqBuildings.size(); i++){
    Node* newBuilding = new Node(uniqBuildings[i]);
    buildings.push_back(newBuilding);  //no checks needed here - done when reading buildings from file
  }
}

void Graph::addEdges(vector<string> uniqStrings){
  for(int i = 0; i < uniqStrings.size(); i+=2){ //for every 2 strings in uniqStrings there is a connections
    int index1 = 0;
    int index2 = 0;
    for(int j = 0; j < buildings.size(); j++){
      if(buildings[j]->getBuildingName()==uniqStrings[i]){
        index1 = j;
      }
      if(buildings[j]->getBuildingName()==uniqStrings[i+1]){
        index2 = j;
      }
      //wont ever be the same because already checks for if same i=when reading in file
    }
    buildings[index1]->addConnection(buildings[index2]);
    buildings[index2]->addConnection(buildings[index1]);
  }
}

void Graph::display(){
    if(buildings.size()==0){
        cout<<"No buildings in graph."<<endl;
    } else {
        int sumGraphConnect = 0;
        for(int i = 0; i < buildings.size(); i++){
            cout<<buildings[i]->getBuildingName()<<" ";
            if(buildings[i]->getNumConnections()==0){
                cout<<"(no connections)"<<" degree = 0"<<endl;
            } else {
                cout<<"("<<buildings[i]->getNumConnections()<<" connections) degree = ";
                cout<<buildings[i]->getNumConnections()<<endl;
                sumGraphConnect += buildings[i]->getNumConnections();
            }
        }
        cout<<"Total buildings: "<<buildings.size();
        cout<<"; Total connections: "<<sumGraphConnect/2<<endl;
    }
}

void Graph::BFS(Node goal){

}
