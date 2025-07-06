#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class Parser {
    string expr;
    int pos;

    char peek() {
        if (pos < expr.length()) return expr[pos];
        return '\0';
    }

    char get() {
        return expr[pos++];
    }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

    int number() {
        skipWhitespace();
        int result = 0;
        while (isdigit(peek())) {
            result = result * 10 + (get() - '0');
        }
        return result;
    }

    int factor() {
        skipWhitespace();
        if (peek() == '(') {
            get();
            int result = expression();
            if (peek() == ')') get();
            return result;
        } else {
            return number();
        }
    }

    int term() {
        int result = factor();
        while (true) {
            skipWhitespace();
            char op = peek();
            if (op == '*') {
                get();
                result *= factor();
            } else if (op == '/') {
                get();
                int denom = factor();
                if (denom == 0) {
                    throw runtime_error("Division by zero");
                }
                result /= denom;
            } else {
                break;
            }
        }
        return result;
    }

    int expression() {
        int result = term();
        while (true) {
            skipWhitespace();
            char op = peek();
            if (op == '+') {
                get();
                result += term();
            } else if (op == '-') {
                get();
                result -= term();
            } else {
                break;
            }
        }
        return result;
    }

public:
    int parse(const string& input) {
        expr = input;
        pos = 0;
        return expression();
    }
};

int main() {
    Parser parser;
    string input;

    cout << "Enter expressions to evaluate (type 'exit' to quit):\n";
    while (true) {
        cout << ">>> ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            int result = parser.parse(input);
            cout << "Result: " << result << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
