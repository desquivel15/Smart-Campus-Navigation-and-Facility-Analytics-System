#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <vector>
#include <string>

using namespace std;

class Node{
  string buildingName;
  vector<Node*> edges;
  public:
    Node(string name);
    string getBuildingName();
    void addConnection(Node* otherBuilding);
};

class Graph{
  vector<Node> buildings; //i think this counts as an adjacency list,
  public:
    void addBuildings(vector<string> uniqBuildings);
    void addEdges(vector<string> uniqStrings);
    void display();
    void BFS(Node goal);
};

#endif
