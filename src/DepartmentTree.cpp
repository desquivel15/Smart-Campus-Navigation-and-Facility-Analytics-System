#include "DepartmentTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

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

	while (getline(file, line)) {
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
	
			cout << "Invalid department record." << endl;
			rejected++;
			continue;
		}

		stringstream ss(line);

		string departmentName;
		string parentName;
		string extraField;
	
		getline(ss, departmentName, '|');
		getline(ss, parentName, '|');
		getline(ss, extraField);

		if (departmentName.empty() || parentName.empty() || !extraField.empty()) {
			cout << "Invalid department record." << endl;
			rejected++;
			continue;
		}

		// tree has root, so none would create a second root
		if (parentName == "NONE" && root != nullptr) {
			
			cout << "Invalid department record: multiple roots." << endl;
			rejected++;
			continue;
		}

		// tree has no root yet, so first department must say none
		if (root == nullptr && parentName != "NONE") {

			cout << "Invalid department record: root must have parent NONE." << endl;
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

	cout << "Accepted: " << accepted << endl;
	cout << "Rejected: " << rejected << endl;	

	return true;
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
