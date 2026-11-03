#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <iomanip>
#include "Stack.h"

using namespace std;

// Check whether a character is an opening bracket
bool isOpening(char c) {
    return c == '(' || c == '[' || c == '{';
}

// Check whether a character is a closing bracket
bool isClosing(char c) {
    return c == ')' || c == ']' || c == '}';
}

// Check matching bracket types
bool isMatching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Check arithmetic operators
bool isOperator(char c) {
    return c == '+' || c == '-' ||
           c == '*' || c == '/';
}

// Operator precedence
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void stackDemo() {
    cout << "\n===== STACK DEMONSTRATION =====\n";

    Stack<int> s;
    int value;

    cout << "Pop on empty: ";

    if (!s.pop(value)) {
        cout << "false | size: " << s.size() << endl;
    }

    s.push(10);
    s.push(20);

    if (s.peek(value)) {
        cout << "Peek: " << value
             << " | size: " << s.size() << endl;
    }

    s.push(30);

    cout << "After push 30, size: "
         << s.size() << endl;

    while (s.pop(value)) {
        cout << "Popped: " << value << endl;
    }

    cout << "Peek on empty: ";

    if (!s.peek(value)) {
        cout << "false" << endl;
    }
}

bool checkBrackets(const string& expression,
                   string& message) {

    Stack<char> brackets;

    for (char c : expression) {

        if (isOpening(c)) {
            brackets.push(c);
        }

        else if (isClosing(c)) {

            char open;

            if (!brackets.pop(open)) {
                message = "Unmatched closing bracket.";
                return false;
            }

            if (!isMatching(open, c)) {
                message = "Mismatched brackets.";
                return false;
            }
        }
    }

    if (!brackets.isEmpty()) {
        message = "Unclosed opening bracket.";
        return false;
    }

    message = "Brackets are balanced.";
    return true;
}

void bracketMenu() {

    string expression;
    string message;

    cout << "\n===== CHECK BALANCED BRACKETS =====\n";
    cout << "Enter expression: ";

    getline(cin, expression);

    if (checkBrackets(expression, message)) {
        cout << "Result: " << message << endl;
    }
    else {
        cout << "Error: " << message << endl;
    }
}


bool infixToPostfix(const string& expression,
                    vector<string>& postfix,
                    string& error) {

    postfix.clear();

    string bracketMessage;

    if (!checkBrackets(expression, bracketMessage)) {
        error = bracketMessage;
        return false;
    }

    Stack<char> operators;

    bool expectOperand = true;
    bool hasToken = false;

    int i = 0;

    while (i < (int)expression.length()) {

        char c = expression[i];

        // Ignore spaces
        if (isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        // Read multi-digit numbers
        if (isdigit(static_cast<unsigned char>(c))) {

            if (!expectOperand) {
                error = "Missing operator between operands.";
                return false;
            }

            string number;

            while (i < (int)expression.length() &&
                   isdigit(static_cast<unsigned char>(expression[i]))) {

                number += expression[i];
                i++;
            }

            postfix.push_back(number);

            expectOperand = false;
            hasToken = true;

            continue;
        }

        // Opening bracket
        if (isOpening(c)) {

            if (!expectOperand) {
                error = "Implicit multiplication is not allowed.";
                return false;
            }

            operators.push(c);

            expectOperand = true;
            hasToken = true;

            i++;
            continue;
        }

        // Closing bracket
        if (isClosing(c)) {

            if (expectOperand) {
                error = "Empty group or missing operand.";
                return false;
            }

            char top;

            while (operators.peek(top) && !isOpening(top)) {

                operators.pop(top);
                postfix.push_back(string(1, top));
            }

            if (!operators.pop(top)) {
                error = "Missing opening bracket.";
                return false;
            }

            expectOperand = false;

            i++;
            continue;
        }

        // Arithmetic operator
        if (isOperator(c)) {

            if (expectOperand) {
                error = "Invalid operator sequence or missing operand.";
                return false;
            }

            char top;

            while (operators.peek(top) &&
                   !isOpening(top) &&
                   precedence(top) >= precedence(c)) {

                operators.pop(top);
                postfix.push_back(string(1, top));
            }

            operators.push(c);

            expectOperand = true;

            i++;
            continue;
        }

        // Reject unsupported characters
        error = string("Unsupported character: ") + c;
        return false;
    }

    // Reject empty expressions
    if (!hasToken) {
        error = "Expression cannot be empty.";
        return false;
    }

    // Reject trailing operators
    if (expectOperand) {
        error = "Expression ends with a missing operand.";
        return false;
    }

    // Pop remaining operators
    char top;

    while (!operators.isEmpty()) {

        operators.pop(top);

        if (isOpening(top)) {
            error = "Unclosed opening bracket.";
            return false;
        }

        postfix.push_back(string(1, top));
    }

    return true;
}


bool evaluatePostfix(const vector<string>& postfix,
                     double& result,
                     string& error) {

    Stack<double> values;

    try {

        for (const string& token : postfix) {

            // Number token
            if (!token.empty() &&
                isdigit(static_cast<unsigned char>(token[0]))) {

                size_t position = 0;

                double number = stod(token, &position);

                if (position != token.length() ||
                    !isfinite(number)) {

                    error = "Invalid numeric token.";
                    return false;
                }

                values.push(number);
            }

            // Operator token
            else if (token.length() == 1 &&
                     isOperator(token[0])) {

                double right, left;

                if (!values.pop(right) ||
                    !values.pop(left)) {

                    error = "Not enough operands.";
                    return false;
                }

                double answer = 0;

                switch (token[0]) {

                    case '+':
                        answer = left + right;
                        break;

                    case '-':
                        answer = left - right;
                        break;

                    case '*':
                        answer = left * right;
                        break;

                    case '/':
                        if (right == 0) {
                            error = "Division by zero.";
                            return false;
                        }

                        answer = left / right;
                        break;
                }

                if (!isfinite(answer)) {
                    error = "Non-finite calculation result.";
                    return false;
                }

                values.push(answer);
            }

            else {
                error = "Invalid postfix token.";
                return false;
            }
        }

    }
    catch (const exception&) {
        error = "Numeric conversion error.";
        return false;
    }

    // Exactly one result must remain
    if (values.size() != 1) {
        error = "Invalid expression: extra or missing operands.";
        return false;
    }

    values.pop(result);

    return true;
}

void expressionMenu() {

    string expression;
    string error;

    vector<string> postfix;

    double result;

    cout << "\n===== INFIX TO POSTFIX EVALUATOR =====\n";

    cout << "Enter arithmetic expression: ";
    getline(cin, expression);

    cout << "\nInput: " << expression << endl;

    // Check brackets
    string bracketMessage;

    if (!checkBrackets(expression, bracketMessage)) {
        cout << "Bracket Status: " << bracketMessage << endl;
        cout << "Error: Invalid expression." << endl;
        return;
    }

    cout << "Bracket Status: Balanced" << endl;

    // Convert infix to postfix
    if (!infixToPostfix(expression, postfix, error)) {
        cout << "Error: " << error << endl;
        return;
    }

    cout << "Postfix Expression: ";

    for (int i = 0; i < (int)postfix.size(); i++) {

        cout << postfix[i];

        if (i < (int)postfix.size() - 1) {
            cout << " ";
        }
    }

    cout << endl;

    // Evaluate postfix
    if (!evaluatePostfix(postfix, result, error)) {
        cout << "Error: " << error << endl;
        return;
    }

    cout << fixed << setprecision(6);
    cout << "Result: " << result << endl;
}

int main() {

    string choice;

    cout << "====================================\n";
    cout << " CC105 WEEK 8 STACK LABORATORY\n";
    cout << "====================================\n";

    while (true) {

        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Check Balanced Brackets\n";
        cout << "2. Convert and Evaluate Expression\n";
        cout << "3. Run Stack Demonstration\n";
        cout << "0. Exit\n";
        cout << "===============================\n";

        cout << "Enter choice: ";

        getline(cin, choice);

        if (choice == "1") {
            bracketMenu();
        }

        else if (choice == "2") {
            expressionMenu();
        }

        else if (choice == "3") {
            stackDemo();
        }

        else if (choice == "0") {
            cout << "\nThank you for using the program!\n";
            break;
        }

        else {
            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}
