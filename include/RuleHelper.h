#ifndef RULE_HELPER_H
#define RULE_HELPER_H

#include <string>
#include <vector>
using namespace std;

/*
Case: match names without regard to capitalization. 
Comparing ASCII letters is sufficient.
*/
string toUpper(string text);


/*
Whitespace: trim whitespace around fields and user-entered names. 
Whitespace includes spaces, tabs, and the carriage return \r, so data files with Windows line endings must load correctly. 
Preserve spaces within names.
*/
string trim(string text);


/*
Building references: a building named in connections.txt or facilities.txt is matched to buildings.txt ignoring case. 
Facilities and the graph display the building spelling from buildings.txt.
*/
int findIgnoreCase(vector<string>list, string name);

/*
Facility IDs are case-insensitive. After trimming, normalize every ID (from a file or typed by the user) to uppercase, 
and only then validate the format, hash, insert, compare, and display it. 
A valid ID is F followed by exactly three digits, such as F001. 
An ID written as f004 in a data file is accepted and stored as F004.
*/
string normalizeID(string id);
bool isValidID(string id);


/*
Comment lines: a line is a comment only if its first non-whitespace character is #. 
A # elsewhere in a line is ordinary text (for example, “Room #5”).
*/
// After trimming, check if the line is blank
bool isBlank (string line);

// Check if the first character is '#'
bool isComment(string line);


#endif
