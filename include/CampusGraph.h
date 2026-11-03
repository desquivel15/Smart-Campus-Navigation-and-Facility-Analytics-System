#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <vector>
#include <string>

using namespace std;

class Node{
  string buildingName;
  vector<Node*> edges;
  int sumEdges;
  public:
    Node(string name);
    string getBuildingName();
    int getNumConnections();
    void addConnection(Node* otherBuilding);
};

class Graph{
  vector<Node*> buildings; //i think this counts as an adjacency list
  public:
    void addBuildings(vector<string> uniqBuildings);
    void addEdges(vector<string> uniqStrings);
    void display();
    void BFS(Node goal);
};

#endif
