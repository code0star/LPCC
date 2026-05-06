#include <bits/stdc++.h>
using namespace std;

// Function to return precedence of operators
int precedence(char op) {
   if (op == '^') return 3; // Highest priority
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void printTAC(stack<string>& operands, stack<char>& operators, int& tempCount) {
    string op2 = operands.top(); operands.pop();
    string op1 = operands.top(); operands.pop();
    char op = operators.top(); operators.pop();

    string tempVar = "t" + to_string(tempCount++);
    cout << tempVar << " = " << op1 << " " << op << " " << op2 << endl;
    operands.push(tempVar);
}

int main() {
    string exp;
    stack<string> operands;
    stack<char> operators;
    int tempCount = 1;

    cout << "Enter expression (e.g., a+b/c): ";
    cin >> exp;

    cout << "\nThree Address Code:\n";

    for (int i = 0; i < exp.length(); i++) {
        if (isalnum(exp[i])) {
            string val(1, exp[i]);
            operands.push(val);
        } 
        else if (exp[i] == '(') {
            operators.push('(');
        } 
        else if (exp[i] == ')') {
            while (operators.top() != '(') {
                printTAC(operands, operators, tempCount);
            }
            operators.pop(); // Remove '('
        } 
        else { // It's an operator (+, -, *, /,^)
            while (!operators.empty() && precedence(operators.top()) >= precedence(exp[i])) {
                printTAC(operands, operators, tempCount);
            }
            operators.push(exp[i]);
        }
    }

    // Process remaining operators in the stack
    while (!operators.empty()) {
        printTAC(operands, operators, tempCount);
    }

    return 0;
}