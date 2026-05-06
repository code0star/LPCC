#include <bits/stdc++.h>
using namespace std;

// Helper: Splits a line like "t1 = a + b" into tokens
vector<string> split(string str) {
    vector<string> tokens;
    stringstream ss(str);
    string word;
    while (ss >> word) tokens.push_back(word);
    return tokens;
}

// 1. CONSTANT FOLDING
vector<string> constantFolding(vector<string> instr) {
    vector<string> result;
    for (size_t i = 0; i < instr.size(); i++) {
        vector<string> v = split(instr[i]);
        // Pattern: target = num op num
        if (v.size() == 5 && isdigit(v[2][0]) && isdigit(v[4][0])) {
            int a = atoi(v[2].c_str());
            int b = atoi(v[4].c_str());
            int res = 0;
            if (v[3] == "+") res = a + b;
            else if (v[3] == "-") res = a - b;
            else if (v[3] == "*") res = a * b;
            else if (v[3] == "/") res = (b != 0) ? a / b : 0;
            
            // Reconstruct string manually for older C++
            stringstream ss;
            ss << v[0] << " = " << res;
            result.push_back(ss.str());
        } else {
            result.push_back(instr[i]);
        }
    }
    return result;
}

// 2. COMMON SUBEXPRESSION ELIMINATION
vector<string> commonSubexpression(vector<string> instr) {
    map<string, string> exprMap; 
    vector<string> result;
    for (size_t i = 0; i < instr.size(); i++) {
        string line = instr[i];
        size_t eq = line.find('=');
        if (eq == string::npos) {
            result.push_back(line);
            continue;
        }
        
        string lhs = line.substr(0, eq - 1);
        string rhs = line.substr(eq + 2);

        // Clean up trailing spaces for older string standards
        lhs.erase(lhs.find_last_not_of(" ") + 1);
        
        if (exprMap.count(rhs)) {
            result.push_back(lhs + " = " + exprMap[rhs]);
        } else {
            exprMap[rhs] = lhs;
            result.push_back(line);
        }
    }
    return result;
}

// 3. COPY PROPAGATION
vector<string> copyPropagation(vector<string> instr) {
    map<string, string> copies;
    vector<string> result;
    for (size_t i = 0; i < instr.size(); i++) {
        vector<string> v = split(instr[i]);
        if (v.size() == 3 && !isdigit(v[2][0])) {
            copies[v[0]] = v[2];
            result.push_back(instr[i]);
        } else {
            string newLine = "";
            for (size_t j = 0; j < v.size(); j++) {
                if (j != 0 && copies.count(v[j])) {
                    newLine += copies[v[j]] + " ";
                } else {
                    newLine += v[j] + " ";
                }
            }
            result.push_back(newLine);
        }
    }
    return result;
}

// 4. DEAD CODE ELIMINATION
vector<string> deadCodeElimination(vector<string> instr) {
    set<string> used;
    vector<string> result;
    // Backwards scan using traditional index
    for (int i = (int)instr.size() - 1; i >= 0; i--) {
        vector<string> v = split(instr[i]);
        if (v.empty()) continue;

        string lhs = v[0];
        
        // Skip unused temporaries
        if (lhs[0] == 't' && used.find(lhs) == used.end()) {
            continue; 
        }

        result.push_back(instr[i]);
        
        for (size_t j = 2; j < v.size(); j++) {
            if (!isdigit(v[j][0]) && v[j] != "+" && v[j] != "-" && v[j] != "*" && v[j] != "/") {
                used.insert(v[j]);
            }
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    vector<string> code;
    string line;
    
    cout << "Enter TAC (Empty line to start optimization):\n";
    while (getline(cin, line) && !line.empty()) {
        code.push_back(line);
    }

    code = constantFolding(code);
    code = commonSubexpression(code);
    code = copyPropagation(code);
    code = deadCodeElimination(code);

    cout << "\n--- Optimized Output ---\n";
    for (size_t i = 0; i < code.size(); i++) {
        cout << code[i] << endl;
    }

    return 0;
}