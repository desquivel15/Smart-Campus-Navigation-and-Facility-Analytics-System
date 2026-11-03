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

		void preorder(DepartmentNode* node);
		void displayHierarchy(DepartmentNode* node);

	public:
		DepartmentTree();
		
		bool addDepartment(string departmentName, string parentName);

		DepartmentNode* searchDepartment(string name);

		void displayPreorder();
		void displayHierarchy();
		void displayDepartmentInfo(string name);
		bool loadDepartments(string filename);
		bool isEmpty();

};


#endif