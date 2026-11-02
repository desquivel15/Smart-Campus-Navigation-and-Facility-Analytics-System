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
  static vector<Node> buildings; //i think this counts as an adjacency list, only ONE so trying to use static keyword will change if break
  public:
    void addBuildings(vector<string> uniqBuildings);
    void addEdges(vector<string> uniqStrings);
    void display();
    void BFS(Node goal);
};

#endif
