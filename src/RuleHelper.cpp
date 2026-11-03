#include "RuleHelper.h"
#include <cctype>
using namespace std;

/*
Case: match names without regard to capitalization. 
Comparing ASCII letters is sufficient.
*/
string toUpper(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = toupper(text[i]);
    }
    return text;
}


/*
Whitespace: trim whitespace around fields and user-entered names. 
Whitespace includes spaces, tabs, and the carriage return \r, so data files with Windows line endings must load correctly. 
Preserve spaces within names.
*/
string trim(string text) {
    int start = 0;
    int end = text.length() - 1;

    while (start <= end && (text[start]==' ' || text[start]=='\t' || text[start]=='\r')) {
        start++;
    }

    while (end >= start && (text[end]==' ' || text[end]=='\t' || text[end]=='\r')) {
        end--;
    }
    
    //return text between start and end
    return text.substr(start, end-start+1);
}


/*
Building references: a building named in connections.txt or facilities.txt is matched to buildings.txt ignoring case. 
Facilities and the graph display the building spelling from buildings.txt.
*/
int findIgnoreCase(vector<string>list, string name) {
    for (int i = 0; i < list.size(); ++i) {
        if (toUpper(list[i]) == toUpper(name)) {
            return i;
        }
    }
    return -1;
}


/*
Facility IDs are case-insensitive. After trimming, normalize every ID (from a file or typed by the user) to uppercase, 
and only then validate the format, hash, insert, compare, and display it. 
A valid ID is F followed by exactly three digits, such as F001. 
An ID written as f004 in a data file is accepted and stored as F004.
*/
string normalizeID(string id) {
    return toUpper(trim(id));
}

bool isValidID(string id) {
    // ID must be 4 chars and start with F
    if(id.length() != 4 || id[0] != 'F') {
        return false;
    }
    // After F are 3 digits
    for (int i = 1; i < 4; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}


/*
Comment lines: a line is a comment only if its first non-whitespace character is #. 
A # elsewhere in a line is ordinary text (for example, “Room #5”).
*/
// After trimming, check if the line is blank
bool isBlank (string line) {
    return trim(line) == "";
}

// Check if the first character is '#'
bool isComment(string line) {
    string trimmed = trim(line);
    return trimmed != "" && trimmed[0] == '#';
}

