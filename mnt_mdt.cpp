#include <bits/stdc++.h>

using namespace std;

// Data Structures
vector<string> allLines;   // Storage for raw input
vector<string> MDT;        // Macro Definition Table
map<string, int> MNT;      // Macro Name -> Index in MDT

int main() {
    string line;
    cout << "Enter Program (Type 'END' to finish):\n";

    // STEP 1: COLLECT WHOLE INPUT
    while (getline(cin, line)) {
        allLines.push_back(line);
        if (line == "END") break;
    }

    // STEP 2: PASS 1 - Build MNT and MDT
    for (int i = 0; i < allLines.size(); i++) {
        if (allLines[i] == "MACRO") {
            i++; // Prototype line (e.g., INCR &A &B)
            stringstream ss(allLines[i]);
            string mName;
            ss >> mName;

            MNT[mName] = MDT.size(); // Save starting position

            // Save the body until MEND
            while (allLines[i] != "MEND") {
                MDT.push_back(allLines[i]);
                i++;
            }
            MDT.push_back("MEND");
        }
    }

    // STEP 3: PASS 2 - Expansion
    cout << "\n--- EXPANDED CODE ---\n";
    bool inDefinition = false;

    for (int i = 0; i < allLines.size(); i++) {
        // Skip macro definitions in output
        if (allLines[i] == "MACRO") { inDefinition = true; continue; }
        if (allLines[i] == "MEND") { inDefinition = false; continue; }
        if (inDefinition) continue;

        stringstream ss(allLines[i]);
        string firstWord;
        ss >> firstWord;

        if (MNT.count(firstWord)) {
            // It's a Macro Call!
            int mdtIdx = MNT[firstWord];
            
            // 1. Map Actual Args to Formal placeholders
            map<string, string> argMap;
            vector<string> actuals;
            string temp;
            
            // Get values from source line (e.g., VAL1 VAL2)
            while (ss >> temp) actuals.push_back(temp);

            // Get placeholders from MDT Prototype (e.g., &A &B)
            stringstream ssProto(MDT[mdtIdx]);
            string formal;
            ssProto >> formal; // Skip macro name
            int k = 0;
            while (ssProto >> formal) {
                argMap[formal] = actuals[k++];
            }

            // 2. Output the body with replacements
            for (int j = mdtIdx + 1; MDT[j] != "MEND"; j++) {
                stringstream ssBody(MDT[j]);
                string word;
                while (ssBody >> word) {
                    if (argMap.count(word)) 
                        cout << argMap[word] << " ";
                    else 
                        cout << word << " ";
                }
                cout << endl;
            }
        } else {
            // Normal assembly line (START, MOV, etc.)
            cout << allLines[i] << endl;
        }
    }

    return 0;
}
