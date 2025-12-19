#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <algorithm>
#include <cmath>
using namespace std;

bool is_math_function(const string& token) {
    return token == "sin" or token == "cos" or token == "tg" or token == "ctg" or token == "exp";
}

int precedence(const string& op) {
    if (op == "+" or op == "-") return 1;
    if (op == "*" or op == "/") return 2;
    if (op == "^") return 3;
    if (op == "_") return 4;
    if (is_math_function(op)) return 5;
    return 0;
}

vector<string> tokenize(string expr) {
    vector<string> tokens;
    string current;

    expr.erase(remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];

        if (isalpha(c)) {
            current = c;
            while (i + 1 < expr.length() and isalpha(expr[i + 1])) {
                current += expr[++i];
            }

            if (is_math_function(current)) {
                tokens.push_back(current);
            }
            continue;
        }

        if (isdigit(c) or c == '.') {
            current = c;
            while (i + 1 < expr.length() and (isdigit(expr[i + 1]) or expr[i + 1] == '.')) {
                current += expr[++i];
            }
            tokens.push_back(current);
            continue;
        }

        if (c == '-') {
            if (tokens.empty() or tokens.back() == "(" or (!tokens.empty() and is_math_function(tokens.back())) or (!tokens.empty() and (tokens.back() == "+" || tokens.back() == "-" or tokens.back() == "*" or tokens.back() == "/"))) {
                tokens.push_back("_");
            } else {
                tokens.push_back("-");
            }
            continue;
        }

        if (c == '+' or c == '*' or c == '/' or c == '^' or c == '(' or c == ')') {
            tokens.push_back(string(1, c));
        }
    }

    return tokens;
}

vector<string> to_postfix(const vector<string>& tokens) {
    vector<string> output;
    stack<string> operators;

    for (const string& token : tokens) {
        if (isdigit(token[0]) or token[0] == '.' or (token.length() > 1 and isdigit(token[1]))) {
            output.push_back(token);
        }
        else if (is_math_function(token)) {
            operators.push(token);
        }
        else if (token == "(") {
            operators.push(token);
        }
        else if (token == ")") {
            while (!operators.empty() and operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop();

            if (!operators.empty() and is_math_function(operators.top())) {
                output.push_back(operators.top());
                operators.pop();
            }
        }
        else {
            while (!operators.empty() and operators.top() != "(" and
                   precedence(operators.top()) >= precedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

float operation(float x, float y, const string& op){
        if (op == "+") return y + x;
        if (op == "-") return y - x;
        if (op == "*") return y * x;
        if (op == "/") return y / x;
        if (op == "^") return pow(y, x);
        if (op == "_") return -x;
        if (op == "sin") return sin(x);
        if (op == "cos") return cos(x);
        if (op == "tg") return tan(x);
        if (op == "ctg") return 1/tan(x);
        if (op == "exp") return exp(x);
        return 0;
};

int main() {
    cout << "Enter: " << endl;
    string expression;
    getline(cin, expression);

    vector<string> tokens = tokenize(expression);

    cout << "Tokens: ";
    for (const string& token : tokens) {
        cout << token << " ";
    }
    cout << endl;

    vector<string> postfix = to_postfix(tokens);

    cout << "Postfix: ";
    for (const string& token : postfix) {
        cout << token << " ";
    }
    cout << endl;

    stack<float> values;

    for (int i = 0; i < postfix.size(); i++) {
        if (isdigit(postfix[i][0]) or postfix[i][0] == '.' or (postfix[i].length() > 1 && isdigit(postfix[i][1]))) {
            values.push(stof(postfix[i]));
        }
        else if (is_math_function(postfix[i]) or postfix[i] == "_") {
            if (values.empty()) {
                cout << "error" << endl;
                return 1;
            }

            float x = values.top();
            values.pop();

            float result = operation(x, 0, postfix[i]);
            values.push(result);
        }
        else if (postfix[i] == "+" or postfix[i] == "-" or postfix[i] == "*" or postfix[i] == "/" or postfix[i] == "^") {
            if (values.size() < 2) {
                cout << "error" << endl;
                return 1;
            }

            float x = values.top();
            values.pop();
            float y = values.top();
            values.pop();

            float result = operation(x, y, postfix[i]);
            values.push(result);
        }
        else {
            cout << "error" << endl;
            return 1;
        }
    }

    if (values.size() != 1) {
        cout << "error" << endl;
        return 1;
    }

    cout << "Result: " << values.top() << endl;

    return 0;
}