#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

// --- Data Structures ---
struct Opcode { string type; int code; };
struct Literal { string value; int address; };

map<string, Opcode> opt = {
    {"STOP", {"IS", 0}}, {"ADD", {"IS", 1}}, {"SUB", {"IS", 2}},
    {"MOVER", {"IS", 4}}, {"BC", {"IS", 7}},
    {"START", {"AD", 1}}, {"END", {"AD", 2}}, {"LTORG", {"AD", 3}}
};

map<string, int> reg = {{"AREG", 1}, {"BREG", 2}};
map<string, int> cond = {{"NE", 1}};

map<string, int> symtab;
vector<Literal> littab;
vector<int> pooltab = {0};
vector<string> ic;
int LC = 0;

// --- Helper Functions ---

void assignLiterals() {
    for (int i = pooltab.back(); i < littab.size(); i++) {
        littab[i].address = LC++;
    }
    pooltab.push_back(littab.size());
}

int getLiteralID(string val) {
    littab.push_back({val, -1});
    return littab.size();
}

int getSymbolID(string name) {
    int id = 1;
    // Updated for compatibility: using item.first
    for (auto const& item : symtab) {
        if (item.first == name) return id;
        id++;
    }
    symtab[name] = -1; 
    return id;
}

// --- Main Logic ---
int main() {
    vector<string> prog = {
        "START 100", 
        "MOVER AREG ='5'", 
        "ADD BREG ='1'",
        "LTORG", 
        "LOOP SUB AREG ='1'", 
        "BC NE LOOP",
        "STOP", 
        "END"
    };

    for (int i = 0; i < prog.size(); ++i) {
        stringstream ss(prog[i]);
        string word;
        vector<string> parts;
        while (ss >> word) parts.push_back(word);

        if (parts[0] == "START") {
            LC = stoi(parts[1]);
            ic.push_back("(AD,1) (C," + to_string(LC) + ")");
        } 
        else if (parts[0] == "LTORG" || parts[0] == "END") {
            assignLiterals();
            ic.push_back("(AD," + to_string(opt[parts[0]].code) + ")");
        } 
        else {
            if (opt.find(parts[0]) == opt.end()) {
                symtab[parts[0]] = LC;
                parts.erase(parts.begin()); 
            }

            Opcode op = opt[parts[0]];
            string s = to_string(LC) + " (" + op.type + "," + to_string(op.code) + ")";

            for (int j = 1; j < parts.size(); j++) {
                string x = parts[j];
                if (reg.count(x)) s += " (" + to_string(reg[x]) + ")";
                else if (cond.count(x)) s += " (" + to_string(cond[x]) + ")";
                else if (x.find("='") != string::npos) s += " (L," + to_string(getLiteralID(x)) + ")";
                else s += " (S," + to_string(getSymbolID(x)) + ")";
            }
            ic.push_back(s);
            LC++;
        }
    }

    // --- Output Section ---
    
    cout << "\nSYMTAB:\n";
    int idx = 1;
    // Updated for compatibility: using item.first and item.second
    for (auto const& item : symtab) {
        cout << idx++ << " " << item.first << "\t" << item.second << endl;
    }

    cout << "\nLITTAB:\n";
    for (int i = 0; i < littab.size(); i++) {
        cout << i + 1 << " " << littab[i].value << "\t" << littab[i].address << endl;
    }

    cout << "\nPOOLTAB:\n";
    for (int i = 0; i < pooltab.size() - 1; i++) {
        cout << i + 1 << " #" << pooltab[i] + 1 << endl;
    }

    cout << "\nINTERMEDIATE CODE:\n";
    for (int i = 0; i < ic.size(); i++) {
        cout << ic[i] << endl;
    }

    return 0;
}