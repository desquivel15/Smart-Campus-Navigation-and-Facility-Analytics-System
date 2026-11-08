#include "CampusGraph.h"
#include "RuleHelper.h"
#include <iostream>
#include <queue>
#include <utility>
#include <climits>

struct shortestPathVertex{
    int distance;
    Node* prev;
};

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

void Graph::display(bool option){
    //option = 0, degrees
    //option = 1, adjaceny lust
    if(buildings.size()==0){
        cout<<"No buildings in graph."<<endl;
    } else {
        int sumGraphConnect = 0;
        for(int i = 0; i < buildings.size(); i++){
            cout<<buildings[i]->getBuildingName()<<" ";
            if(option == 0){ //DISPLAY  GRPAH COUNTS AND DEGREES
                if(buildings[i]->getNumConnections()==0){
                    cout<<"(no connections)"<<" degree = 0"<<endl;
                } else {
                    cout<<"("<<buildings[i]->getNumConnections()<<" connections) degree = ";
                    cout<<buildings[i]->getNumConnections()<<endl;
                }
            } else {//option = 1,, ADJACENY LIST, VIEW CAMPUS GRAPH
                cout<<": ";
                //print "no connection" instead of empty as requirement
                if(buildings[i]->getNumConnections()==0) {
                    cout << "(no connections)";
                }
                for(int j = 0; j < buildings[i]->getNumConnections(); j++){
                    cout<<buildings[i]->edges[j]->getBuildingName()<<", ";
                }
                if(buildings[i]->getNumConnections()>0) {
                    cout<<"\b\b ";
                }
                cout<<endl;
            }
            sumGraphConnect += buildings[i]->getNumConnections();
        }
        cout<<"Total buildings: "<<buildings.size();
        cout<<"; Total connections: "<<sumGraphConnect/2;
        if(option==0){
            cout<<"; Total connected parts: ";
            reachability(buildings[0]->getBuildingName(),1);
        }
        cout<<endl;
    }
}

void Graph::shortestPath(string start, string goal){
    //dijkstras shortest path algorithm
    //used zybooks as a reference for this
    if(start==goal){
        cout<<start<<" to "<<goal<<" (0 connections)."<<endl; return;
    }
    queue<pair<shortestPathVertex, Node*>> unvisQ;//dist,prev,curr
    Node* startN = NULL;
    bool goalexists = false;
    for(int i = 0; i < buildings.size(); i++){
        if(buildings[i]->getBuildingName()==start){
            startN = buildings[i];
        }
        if(buildings[i]->getBuildingName()==goal){
            goalexists = true;
        }
    }
    if(startN==NULL){
        cout<<"ERROR - invalid starting building"<<endl; return;
    }
    if(!goalexists){
        cout<<"ERROR - invalid destination building"<<endl; return;
    }
    shortestPathVertex one = {0,NULL};
    unvisQ.push(make_pair(one,startN));
    vector<pair<shortestPathVertex,Node*>>vectorized;
    vector<bool> visited(buildings.size(), false);
    queue<Node*> temp1;
    for(int i = 0; i < buildings.size(); i++){
        if(buildings[i]==startN){
            vectorized.push_back(make_pair(shortestPathVertex{0, NULL}, buildings[i]));
        } else {
            vectorized.push_back(make_pair(shortestPathVertex{INT_MAX, NULL}, buildings[i]));
        }
    }
    
    for(int i = 0; i < buildings.size(); i++){
        int minIndex = -1;
        int minDist = INT_MAX;
        for(int j = 0; j < vectorized.size(); j++){
            if(!visited[j] && (vectorized[j].first.distance<minDist) ){
                minDist = vectorized[j].first.distance;
                minIndex = j;
            }
        }
        if( (minIndex==-1)||(minDist == INT_MAX) ){
            break;
        }
        visited[minIndex] = true;
        Node* currNode = vectorized[minIndex].second;
        int currDist = vectorized[minIndex].first.distance;
        
        if(currNode->getBuildingName()==goal) break;
        
        for(int i = 0; i < currNode->edges.size(); i++){
            Node* adjNode = currNode->edges[i];
            int altPathDist = currDist + 1;
            int vindex = -1; //have to fnd
            for(int j = 0; j < vectorized.size(); j++){
                if(vectorized[j].second == adjNode){
                    vindex = j; break;
                }
            }
            
            if( (vindex!=-1) && (altPathDist<vectorized[vindex].first.distance) ){
                vectorized[vindex].first.distance = altPathDist;
                vectorized[vindex].first.prev = currNode;
            }
        }
    } //vectorized = dist, prev, cur
    pair<shortestPathVertex,Node*> tempend = {{0,NULL},NULL};
    for(int i = 0; i < vectorized.size(); i++){
        if(vectorized[i].second->getBuildingName()==goal){
            tempend.first = vectorized[i].first;
            tempend.second = vectorized[i].second;
        }
    } //set tempend to goal within vectorized
    if(tempend.first.distance==INT_MAX){
        cout<<start<<" to "<<goal<<": no path."<<endl; return;
    }
    vector<Node*> path; 
    Node* current = tempend.second;
    while(current!=NULL){
        path.push_back(current);
        Node* next = NULL;
        for(int i = 0; i < vectorized.size(); i++){
            //looks for the prev node
            //cant just loop from that cus not within actual node def
            if(vectorized[i].second == current){
                next = vectorized[i].first.prev;
                break; //so not useless looping
            }
        }
        current = next;
    }
    cout<<start<<" to "<<goal<<" ("<<tempend.first.distance<<" connections)."<<endl;
    for(int i = path.size()-1; i >= 0; i--){ //backwayrds
        cout<<path[i]->getBuildingName()<<endl;
    }
}

void Graph::reachability(string start, bool option){ //prompt for start IN MAIN
    //OKAY. so 1 part is the printing reachability part aka option 0
    //second part (option = 1) is figuring out amount of connnected parts.
    //second part doesnt actually need starting but is useful
    // so not excluding - this functoin wont be called in main
    // will be called in parts of display 
    queue<pair<int,Node*>> q; //int is distance
    vector<Node*> visited;
    Node* first = NULL;
    for(int i = 0; i < buildings.size(); i++){
        if(toUpper(buildings[i]->getBuildingName())==toUpper(trim(start))) { //ignore case and outer spaces
            first = buildings[i];
            break;
        }
    }
    if(first==NULL){
        cout<<"ERROR - invalid starting building"<<endl; return;
    }
    if(!option){
        cout<<"From "<<start<<endl;
    }
    q.push(make_pair(0, first));
    visited.push_back(first);
    int dist = 0;
    while(!q.empty()){  
        Node* curr = q.front().second;
        dist = q.front().first;
        q.pop();
        if(!option){
            cout<<"At "<<curr->getBuildingName()<<" (distance "<<dist<<")"<<endl;
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
    if(!option){
        cout<<"Reachable: "<<visited.size()<<" out of "<<buildings.size()<<endl; return;
    }
    int connectedSum = 1;
    if(visited.size()!=buildings.size()){//not less than cus no way its vis bigger than buil
        //visited is connected part one
        vector<Node*> notMain;
        for(int i = 0; i < buildings.size(); i++){//inefficient
            bool isInBoth = false;
            for(int j = 0; j < visited.size(); j++){
                if(visited[j]->getBuildingName()==buildings[i]->getBuildingName()){
                    isInBoth = true;
                }
            }
            if(!isInBoth){
                notMain.push_back(buildings[i]); //adds unconnected into main.
                //recusion would be good were but seems extra :/
            }
        }
        queue<Node*> q; //dont need dist anymore
        Node* first = notMain[0];
        q.push(first);
        visited.push_back(first);
        while(!q.empty()){  
            Node* curr = q.front();
            q.pop();
            for(int i = 0; i < curr->edges.size(); i++){
                bool alreadyVisited = false;
                for(int j = 0; j < visited.size(); j++){
                    if(curr->edges[i]==visited[j]){
                        alreadyVisited = true;
                    }
                }
                if(!alreadyVisited){
                    visited.push_back(curr->edges[i]);
                    q.push(curr->edges[i]);
                }  
            }
        }
        connectedSum++;
        cout<<connectedSum;
    }
    else {
        cout << connectedSum; //whole graph is connected
    }
}
