#include <bits/stdc++.h>
using namespace std;

string without;
int p = 0;

bool isNumber(const string& s) {
    if (s.empty()) {
        return false;
    }

    bool hasDecimalPoint = false;
    int start = (s[0] == '-') ? 1 : 0; // Check for negative numbers

    for (int i = start; i < s.length(); i++) {
        if (s[i] == '.') {
            if (hasDecimalPoint) return false; // Multiple decimal points, invalid number
            hasDecimalPoint = true; // Mark that we found a decimal point
        } else if (!isdigit(s[i])) {
            return false; // Non-digit character found
        }
    }
    return true;
}

// Function to check if a number is a float
bool isFloat(const string& s) {
    return s.find('.') != string::npos; // Check if the number has a decimal point
}

int main() {
    ifstream input("input.txt");
    ofstream output("output.txt");
    int flag = 0, i;
    string temp;
    const string keyword[] = {"int", "float", "if", "char", "return", "string", "char"};
    const char symbol[] = {',', '.', ';', '<', '>', '=', '#', ','};
    const string symbolName[] = {"comma", "point", "semicolon", "less than", "greater than", "equal", "hash", "comma"};
    char fundsym[100];

    int sym_count = 0;
    string found_keyword[100];
    int found_count = 0;
    map<string, string> variable;
    map<string, string> number;

    if (input.is_open()) {
        while (getline(input, temp)) {
            without.clear(); // Clear `without` for each new line

            for (i = 0; i < temp.size(); i++) {
                if (temp[i] == '/' && temp[i + 1] == '/') {
                    break; // Skip single line comments
                } else if (temp[i] == '/' && temp[i + 1] == '*') {
                    flag = 1;
                    i++;
                } else if (((i + 1) < temp.size()) && temp[i] == '*' && temp[i + 1] == '/') {
                    flag = 0;
                    i++;
                    i++;
                }

                if (flag == 0 && i < temp.size()) {
                    without += temp[i];
                }
            }

            output << without << "\n"; // Output processed line

            string word;
            string current_type = "";

            // Processing each character in the line
            for (int j = 0; j < without.length(); j++) {
                char c = without[j];

                // Check if the character is part of a word (alpha-numeric)
                if (isalnum(c) || c == '_') { // allows variables like a1, _abc, etc.
                    word += c;
                } else {
                    if (!word.empty()) {
                        // Skip function names
                        if (j < without.length() && without[j] == '(') {
                            word.clear();
                            continue;
                        }

                        // Check if it's a keyword
                        bool is_type = false;
                        for (int k = 0; k < 8; k++) {
                            if (word == keyword[k]) {
                                is_type = true;
                                current_type = word;

                                // Avoid duplicates
                                if (find(found_keyword, found_keyword + found_count, word) == found_keyword + found_count) {
                                    found_keyword[found_count++] = word;
                                }
                            }
                        }

                        // If it's not a keyword, treat it as a variable
                        if (!is_type && !current_type.empty()) {
                            if (!isNumber(word)) {
                                if (variable.find(word) == variable.end()) {
                                    variable[word] = current_type;
                                }
                            }
                        }

                        word.clear();
                    }
                }
            }

            // Process numbers in the line
            string word1;
            for (int j = 0; j < without.length(); j++) {
                char c = without[j];

                // Check if the character is part of a number (digits or decimal point)
                if (isdigit(c) || (c == '.' && word1.find('.') == string::npos)) {
                    word1 += c; // Build the number string
                } else {
                    // If we encounter a non-digit/non-decimal character, check if we have a valid number
                    if (isNumber(word1) && word1 != ".") {
                        number[word1] = isFloat(word1) ? "float" : "int"; // Store the number with its type
                    }
                    word1.clear(); // Clear for the next potential number
                }
            }

            // Check the last word if it ends with a number
            if (isNumber(word1) && word1 != ".") {
                number[word1] = isFloat(word1) ? "float" : "int"; // Store the last number with its type
            }

            // Identifying symbols
            for (int k = 0; k < without.length(); k++) {
                char c = without[k];
                for (int l = 0; l < 8; l++) {
                    if (c == symbol[l]) {
                        fundsym[sym_count++] = c;
                    }
                }
            }
        }
    }

    input.close();

    // Output found keywords
    output << "\nKeyword \n";
    for (int j = 0; j < found_count; j++) {
        output << found_keyword[j] << " ";
    }

    // Output symbols
    output << "\nSymbol lists \n";
    for (int j = 0; j < sym_count; j++) {
        for (int k = 0; k < 8; k++) {
            if (symbol[k] == fundsym[j]) {
                output << symbol[k] << "  " << symbolName[k] << "\n";
            }
        }
    }

    // Output variables with types
    output << "Variable list with type \n";
    for (auto& it : variable) {
        output << "Variable: " << it.first << " Type: " << it.second << endl;
    }

    // Output number list
    output << "NUMBER LIST\n";
    for (auto& it : number) {
        output << "Number: " << it.first << " Type: " << it.second << endl;
    }

    output.close();
    return 0;
}
