#include "CampusGraph.h"
#include <iostream>
#include <queue>
#include <utility>

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

void Graph::shortestPath(string start, string goal){ //prompt for goal IN MAIN
    queue<pair<int,Node*>> q; //int is distance
    vector<Node*> visited;
    Node* first = NULL;
    for(int i = 0; i < buildings.size(); i++){
        if(buildings[i]->getBuildingName()==start){
            first = buildings[i];
            break;
        }
    }
    if(first==NULL){
        cout<<"ERROR - invalid starting building"<<endl; return;
    }
    cout<<"From "<<start<<" to "<<goal;
    if(start==goal){
        cout<<", 0 connections."<<endl; return;
    } else cout<<endl;
    
    q.push(make_pair(0, first));
    visited.push_back(first);
    while(!q.empty()){ //start does exist and has at least 1 edge.
        Node* curr = q.front().second;
        int dist = q.front().first;
        q.pop();
        cout<<"At "<<curr->getBuildingName()<<" distance "<<dist<<endl;
        if(curr->getBuildingName()==goal){
            cout<<dist<<" connections."<<endl; return;
        }
        for(int i = 0; i < curr->edges.size(); i++){
            bool alreadyVisited = false;
            for(int j = 0; j < visited.size(); j++){
                if(curr->edges[i]==visited[j]){
                    alreadyVisited = true;
                }
            }
            if(!alreadyVisited){
                visited.push_back(curr->edges[i]);
                q.push(make_pair(dist+1,curr->edges[i]));
            }
        }
    }
    cout<<"No path found between "<<start<<" and "<<goal<<endl;
}

void Graph::reachability(string start){ //prompt for start IN MAIN
    queue<pair<int,Node*>> q; //int is distance
    vector<Node*> visited;
    Node* first = NULL;
    for(int i = 0; i < buildings.size(); i++){
        if(buildings[i]->getBuildingName()==start){
            first = buildings[i];
            break;
        }
    }
    if(first==NULL){
        cout<<"ERROR - invalid starting building"<<endl; return;
    }
    cout<<"From "<<start<<endl;
    q.push(make_pair(0, first));
    visited.push_back(first);
    int dist = 0;
    while(!q.empty()){  
        Node* curr = q.front().second;
        dist = q.front().first;
        q.pop();
        cout<<"At "<<curr->getBuildingName()<<" distance "<<dist<<endl;
        for(int i = 0; i < curr->edges.size(); i++){
            bool alreadyVisited = false;
            for(int j = 0; j < visited.size(); j++){
                if(curr->edges[i]==visited[j]){
                    alreadyVisited = true;
                }
            }
            if(!alreadyVisited){
                visited.push_back(curr->edges[i]);
                q.push(make_pair(dist+1,curr->edges[i]));
            }
        }
    }
    cout<<"Reachable: "<<dist<<" out of "<<buildings.size()<<endl;
}

