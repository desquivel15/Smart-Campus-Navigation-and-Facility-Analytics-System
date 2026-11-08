# Smart-Campus-Navigation-and-Facility-Analytics-System
A C++ program that loads campus data from text files and lets a user locate facilities, explore building connections, find reachable buildings using BFS, browse a department hierarchy, and view graph information.

# Link to GitHub Repository
https://github.com/desquivel15Smart-Campus-Navigation-and-Facility-Analytics-System.git

# Build Instructions

Compile the program using g++:

g++ -Iinclude src/main.cpp src/CampusGraph.cpp src/DepartmentTree.cpp src/Facility.cpp src/HashTable.cpp src/FacilityManager.cpp src/RuleHelper.cpp -o campus

# Run Instructions

Run the program using the default data directory:

./campus

The program loads its input files from the data/ directory by default.
To use a different data directory, provide the directory as a command-line argument:

./campus [data-directory]

Example:
./campus testdata/case1

# Directory Layout

include/ 
- CampusGraph.h
- DepartmentTree.h
- Facility.h
- FacilityManager.h
- HashTable.h
- RuleHelper.h

src/
- CampusGraph.cpp
- DepartmentTree.cpp
- Facility.cpp
- FacilityManager.cpp
- HashTable.cpp
- main.cpp
- RuleHelper.cpp

data/ 
- buildings.txt
- connections.txt
- facilities.txt
- departments.txt

README.txt
- Project purpose, build/run instructions, and directory layout.

CONTRIBUTIONS.txt
- Team member contributions and responsibilities.

design_document.txt
- Project design and implementation decisions.

test_results.txt
- Results of the required Milestone 1 tests.

.gitignore 
- Specifies files that are excluded from the Git repository
