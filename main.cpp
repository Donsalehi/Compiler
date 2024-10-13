#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <set>
#include <regex>

using namespace std;

// maintain the values of variables
map<string, int> variables;

//define valid variables
set<string> validVariables = {"A", "B", "C", "D"};

// evaluate an expression
int evaluate(string expr) {

    stringstream ss(expr);
    int result, num;
    char op;

    ss >> result;
    while (ss >> op >> num) {
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result /= num; break;
            default: throw invalid_argument("Invalid operator: the compiler only support +, -, *, / .");
        }
    }

    return result;
}

// process and execute each line of the program
void processLine(string line) {
    // remove spaces
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    // split the line into two parts: before and after '='
    size_t equalPos = line.find('=');
    if (equalPos == string::npos)
        throw invalid_argument("invalid format");
    string var = line.substr(0, equalPos);
    string expr = line.substr(equalPos + 1);

    // check the validity of the variable name
    if (validVariables.find(var) == validVariables.end()) {
        throw invalid_argument("invalid variable: " + var + ". only A, B, C and D are valid.");
    }

    //check if the expression follows one of the two allowed formats
    // first algorithm: operand = number
    regex format1(R"([A-D]=\d+)");
    // second algorithm: operand = operand operator operand
    regex format2(R"([A-D]=[A-D].+[A-D])");

    if (regex_match(line, format1)) {
        // if the format is: operand = number
        variables[var] = stoi(expr);
    } else if (regex_match(line, format2)) {
        // if the format is operand = operand operator operand
        // place variables in expression
        for (auto &v : variables) {
            size_t pos;
            while ((pos = expr.find(v.first)) != string::npos) {
                expr.replace(pos, 1, to_string(v.second));
            }
        }
        variables[var] = evaluate(expr); // evaluate and allocate of results
    } else {
        // If none of the formats match
        throw invalid_argument("invalid format:Your input can only follow one of the following 2 formats:\n1.operand = number\n2.operand = operand operator operand\n");
    }
}

int main() {

    string input, line;

    cout << "Please enter your commands (insert a blank line to end):\n";

    // get input from user
    while (true) {
        getline(cin, line);
        if (line.empty()) {
            break;
        }
        input += line + "\n";
    }

    stringstream ss(input);

    try {
        // compile input code to c++ language
        while (getline(ss, line)) {
            if (!line.empty()) {
                processLine(line);
            }
        }

        // print final value of A
        cout << "final value of A: " << variables["A"] << endl;

    } catch (const exception &e) {
        cerr << "error: " << e.what() << endl;
    }

    return 0;
}
