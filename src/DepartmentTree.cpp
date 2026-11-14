#include "DepartmentTree.h"
#include "RuleHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <queue>

using namespace std;

string toLowerCase(string text) {

	for (char& c : text) {
		c = tolower(c);
	}

	return text;
}

// constructor
DepartmentTree::DepartmentTree() {
	root = nullptr;
}

// private recursive search
DepartmentNode* DepartmentTree::search(DepartmentNode* node, string name) {
	if (node == nullptr) {
		return nullptr;
	}

	else if (toLowerCase(node->name) == toLowerCase(name)) {
		return node;
	}

	for (DepartmentNode* child : node->children) {
		DepartmentNode* result = search(child, name);
		
		if (result != nullptr) {
			return result;
		}
	}

	return nullptr;
}

// public search
DepartmentNode* DepartmentTree::searchDepartment(string name) {
	return search(root, name);
}

// get depth of a department
int DepartmentTree::getDepth(DepartmentNode* node) {
	int depth = 0;

	while (node->parent != nullptr) {
		node = node->parent;	
		depth++;
	}

	return depth;
}

// add a department
bool DepartmentTree::addDepartment(string departmentName, string parentName) {

	// if tree is empty, first department becomes the root
	if (root == nullptr) {
	DepartmentNode* newNode = new DepartmentNode;
	newNode->name = departmentName;
	newNode->parent = nullptr;

	root = newNode;
	return true;
	}

	// no duplicates
	if(searchDepartment(departmentName) != nullptr) {
		cout << "Department already exists." << endl;
		return false;
	}

	// find requested parent
	DepartmentNode* parentNode = searchDepartment(parentName);

	// parent must exist
	if (parentNode == nullptr) {
		cout << "Parent department not found." << endl;
		return false;
	}

	DepartmentNode* newNode = new DepartmentNode;
	newNode->name = departmentName;
	newNode->parent = parentNode;

	parentNode->children.push_back(newNode);

	return true;
}

void DepartmentTree::preorder(DepartmentNode* node) {
	
	if (node == nullptr) {
		return;
	}
	
	cout << node->name << endl;

	for (DepartmentNode* child : node->children) {
		preorder(child);
	}
}

void DepartmentTree::displayPreorder() {
	
	if (root == nullptr) {
		cout << "No departments are loaded." << endl;
		return;
	}

	preorder(root);
}

void DepartmentTree::postorder(DepartmentNode* node) {
	if (node == nullptr) {
		return;
	}
	for (DepartmentNode* child : node->children) {
		postorder(child);
	}
	
	cout << node->name << endl;
}

void DepartmentTree::displayPostorder() {

	if (root == nullptr) {
		cout << "No departments are loaded." << endl;
		return;
	}

	postorder(root);
}

void DepartmentTree::displayLevelorder() {

	if (root == nullptr) {
		cout << "No departments are loaded." << endl;
		return;
	}

	queue<DepartmentNode*> nodes;
	nodes.push(root);

	while(!nodes.empty()) {
		DepartmentNode* current = nodes.front();
		nodes.pop();

		cout << current->name << endl;

		for (DepartmentNode* child : current->children) {
			nodes.push(child);
		}
	}
}

void DepartmentTree::displayHierarchy(DepartmentNode* node) {
	
	if (node == nullptr) {
		return;
	}

	int depth = getDepth(node);

	for (int i = 0; i < depth; i++) {
		cout << "    ";
	}

	cout << node->name << endl;

	for (DepartmentNode* child : node->children) {
		displayHierarchy(child);
	}
}

void DepartmentTree::displayHierarchy() {
	
	if (root == nullptr) {
		cout << "No departments are loaded." << endl;
		return;
	}
	
	displayHierarchy(root);
}

bool DepartmentTree::loadDepartments(string filename) {
	
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "ERROR - Could not open " << filename << endl;
		return false;
	}
	
	string line;
	int accepted = 0;
	int rejected = 0;

	int lineNumber = 0;

	while (getline(file, line)) {

		lineNumber++;

		line = trim(line);
		if (line.empty() || line[0] == '#') {
			continue;	
		}

		int delimiterCount = 0;
		
		for (char c : line) {

			if (c == '|') {
				delimiterCount++;
			}
		}

		if (delimiterCount != 1) {
			cout << filename << " line " << lineNumber  << ": wrong field count " << endl;
			rejected++;
			continue;
		}

		stringstream ss(line);

		string departmentName;
		string parentName;
	
		getline(ss, departmentName, '|');
		departmentName = trim(departmentName);
		
		getline(ss, parentName, '|');
		parentName = trim(parentName);

		if (departmentName.empty() || parentName.empty()) {
			cout << filename << " line " << lineNumber << ": empty field" << endl;
			rejected++;
			continue;
		}

		// tree has root, so none would create a second root
		if (parentName == "NONE" && root != nullptr) {
			
			cout << filename << " line " << lineNumber << ": multiple roots." << endl;
			rejected++;
			continue;
		}

		// tree has no root yet, so first department must say none
		if (root == nullptr && parentName != "NONE") {

			cout << filename << " line " << lineNumber << ": root must have parent NONE." << endl;
			rejected++;
			continue;
		}

		if (searchDepartment(departmentName) != nullptr) {
			cout << filename << " line " << lineNumber << ": duplicate department" << endl;
			rejected++;
			continue;
		}

		if (parentName != "NONE" && searchDepartment(parentName) == nullptr) {
			cout << filename << " line " << lineNumber << ": parent department not found" << endl;
			rejected++;
			continue;
		}
  
		bool added = addDepartment(departmentName, parentName);

		if (added) {
			accepted++;
		}
		else {
			rejected++;
		}
	}

	file.close();

	cout << "[" << filename << "]: " << accepted << " accepted, " << rejected << " rejected" << endl;
	
	return true;
}

int DepartmentTree::countDepartments(DepartmentNode* node) {
	if (node == nullptr) {
		return 0;
	}

	int count = 1;

	for (DepartmentNode* child : node->children) {
		count += countDepartments(child);
	}

	return count;
}

int DepartmentTree::countLeaves(DepartmentNode* node) {
	if (node == nullptr) {
		return 0;
	}

	if (node->children.empty()) {
		return 1;
	}

	int leaves = 0;

	for (DepartmentNode* child : node->children) {
		leaves += countLeaves(child);
	}

	return leaves;
}

int DepartmentTree::getHeight(DepartmentNode* node) {

	if (node == nullptr) {
		return 0;
	}
	
	if (node->children.empty()) {
		return 0;
	}

	int maxHeight = 0;

	for (DepartmentNode* child : node->children) {
		int childHeight = 1 + getHeight(child);

		if (childHeight > maxHeight) {
			maxHeight = childHeight;
		}
	}

	return maxHeight;
}

void DepartmentTree::displayStatistics() {

	if (root == nullptr) {
		cout << "Total Departments: 0" << endl;
		cout << "Leaves: 0" << endl;
		cout << "Tree Height: N/A" << endl;
		return;
	}
	
	else {
		cout << "Total Departments: " << countDepartments(root) << endl;
		cout << "Leaves: " << countLeaves(root) << endl;
		cout << "Tree Height: " << getHeight(root) << endl;
		return;
	}
}

bool DepartmentTree::isEmpty() {
	return root == nullptr;
}

void DepartmentTree::displayDepartmentInfo(string name) {

	DepartmentNode* node = searchDepartment(name);

	if (node == nullptr) {
		cout << "Department not found." << endl;
		return;
	}

	cout << "Department: " << node->name << endl;

	if (node->parent == nullptr) {
		cout << "Parent: None" << endl;
	}

	else {
		cout << "Parent: " << node->parent->name << endl;
	}

	cout << "Children: " << node->children.size() << endl;
	cout << "Depth: " << getDepth(node) << endl;

}
