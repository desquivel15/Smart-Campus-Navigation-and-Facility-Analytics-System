#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

using namespace std;

struct DepartmentNode {

	string name;
	DepartmentNode* parent;
	vector<DepartmentNode*> children;

};

class DepartmentTree {

	private:
		DepartmentNode* root;
		DepartmentNode* search(DepartmentNode* node, string name);
		int getDepth(DepartmentNode* node);
		int countDepartments(DepartmentNode* node);
		int countLeaves(DepartmentNode* node);
		int getHeight(DepartmentNode* node);

		void preorder(DepartmentNode* node);
		void postorder(DepartmentNode* node);
		void displayHierarchy(DepartmentNode* node);

	public:
		DepartmentTree();
		
		bool addDepartment(string departmentName, string parentName);

		DepartmentNode* searchDepartment(string name);

		void displayPreorder();
		void displayPostorder();
		void displayLevelorder();
		void displayHierarchy();
		void displayStatistics();
		void displayDepartmentInfo(string name);
		bool loadDepartments(string filename);
		bool isEmpty();

};


#endif