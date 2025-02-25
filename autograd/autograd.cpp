#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <numeric>
#include <algorithm>
using namespace std;

// define all possible token types in infix expression
enum TokenType {
    NUMBER,   // regular number
    VARIABLE, // variable, e.g., x, y, z, xx
    OPERATOR, // +, -, *, /, ^
    FUNCTION, // sin, cos, tan, log, ln, pow, exp
    LPAREN,   // ( will not present in the postfix expression
    RPAREN,   // ) will not present in the postfix expression
};

// token: the smallest unit in infix expression
// define the token structure
struct Token {
    TokenType type;           // token type, all possible types are defined in TokenType above
    string value;             // token value, the actual value of the token, e.g., "x", "2", "+", "sin" etc.
    int precedence;           // operator precedence
    bool isRightAssociative;  // operator associativity
    
    // constructor
    // default precedence = -1, the lowest precedence
    // default isRightAssc = false, left associative, read from left to right
    Token(TokenType type, string value, int precedence = -1, bool isRightAssc = false)
        : type(type), value(value), precedence(precedence), isRightAssociative(isRightAssc) {}
};

// define the expression tree node structure
struct TreeNode {
    Token token;      // token value, Token type
    TreeNode *left;   // left expression
    TreeNode *right;  // right expression

    // constructor
    TreeNode(Token token) : token(token), left(nullptr), right(nullptr) {}

    // destructor
    ~TreeNode() {
        delete left;
        delete right;
    }
};

// helper functions to check token properties

// get the arity of the function (number of arguments)
int getArity(const string& token) {
    // define all functions and their arities
    static const unordered_map<string, int> arities = {
        {"+", 2}, {"-", 2}, {"*", 2}, {"/", 2}, {"^", 2}, // "^" will be converted to "pow" function
        {"log", 2}, {"ln", 1},
        {"cos", 1}, {"sin", 1}, {"tan", 1},
        {"pow", 2}, {"exp", 1}
    };

    // default arity = 0, for numbers and variables
    if (arities.find(token) == arities.end()) return 0;
    // return the arity of the function if defined
    return arities.at(token);
}

// return precedence of the operator
int getPrecedence(const string& token) {
    // define all operators and their precedence
    static const unordered_map<string, int> precedences = {
        {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"^", 4},
    };

    // default precedence = -1, the lowest precedence
    if (precedences.find(token) == precedences.end()) return -1;
    // return the precedence of the operator if defined
    return precedences.at(token);
}

// return associativity of the operator
bool isRightAssociative(const string& token) {
    // list all right associative operators
    static const unordered_map<string, bool> rightAssociative = {
        {"^", true},
    };
    // return true if the operator is right associative
    return rightAssociative.find(token) != rightAssociative.end();
}

// check if the token is a function
bool isFunction(const string& token) {
    // list all functions
    static const unordered_map<string, bool> functions = {
        {"log", true}, {"ln", true},
        {"cos", true}, {"sin", true}, {"tan", true},
        {"pow", true}, {"exp", true}
    };
    // return true if the token is a function
    return functions.find(token) != functions.end();
}

// check if the token is an operator
bool isOperator(const string& token) {
    // list all operators
    static const unordered_map<string, bool> operators = {
        {"+", true}, {"-", true}, {"*", true}, {"/", true}, {"^", true},
    };
    // return true if the token is an operator
    return operators.find(token) != operators.end();
}

// helper functions END 

// process infix expression, convert it to postfix expression, and get the list of variables
// vector<Token>: postfix expression
// vector<string>: list of variables
pair<vector<Token>, vector<string>> processInfix(const string& infix) {
    vector<Token> output;      // postfix expression token list
    vector<string> variables;  // list of variables
    stack<Token> stack;        // operator and function stack
    size_t i = 0;              // index of the current character in the infix expression

    // special handling for the "^" operator, convert it to "pow" function
    // process fun^num(exp) as fun(pow(fun(exp), num))), e.g., sin^2(x) = (sin(x))^2
    bool powFunction = false;     // flag to indicate if the operator is "^"
    string powFunctionExp = "";   // expression for the "^" operator / pow function

    while (i < infix.length()) {
        if (isdigit(infix[i])) {  // Handle numbers
            string number;  // collect consecutive digits
            while (i < infix.length() && isdigit(infix[i])) {
                number += infix[i++];
            }
            output.emplace_back(NUMBER, number);
        } else if (isalpha(infix[i])) {  // Handle variables and functions
            string identifier;  // collect consecutive letters
            while (i < infix.length() && isalpha(infix[i])) {
                identifier += infix[i++];
            }

            if (isFunction(identifier)) {
                stack.emplace(FUNCTION, identifier);        // FUNCTION: Push function onto the stack
            } else {
                output.emplace_back(VARIABLE, identifier);  // VARIABLE: Add to the output list

                // Add to the list of variables if not present in the variable list
                bool found = false;
                for (const auto& var : variables) {
                    if (var == identifier) {
                        found = true;
                        break;
                    }
                }
                if (!found) variables.emplace_back(identifier);
            }
        } else if (isOperator(string(1, infix[i]))) {  // Handle operators. string(1, infix[i]): convert char to string
            int precedence = getPrecedence(string(1, infix[i]));
            bool isRightAssc = isRightAssociative(string(1, infix[i]));

            // Pop operators from the stack with higher precedence or equal precedence and left associativity
            // "^" is right associative, 2^3^4 = 2^(3^4)
            while (!stack.empty() && ((stack.top().type == OPERATOR && stack.top().precedence > precedence) ||
                                      (stack.top().precedence == precedence && !isRightAssc))) {
                // special handling for the "^" operator
                if (stack.top().value == "^") {
                    // convert to pow funtion, unify the same operator and function
                    output.emplace_back(FUNCTION, "pow");  // FUNCTION "pow" for output
                } else {
                    // simply add the operator to the output list if not "^"
                    output.emplace_back(stack.top());
                }
                stack.pop();
            }
            // Push the current operator onto the stack
            stack.emplace(OPERATOR, string(1, infix[i]), precedence, isRightAssc);
            i++;
        } else if (infix[i] == '(') {
            // process fun^num(exp) as fun(pow(fun(exp), num))), e.g., sin^2(x) = (sin(x))^2
            // no need to process fun^(exp1)(exp2) as fun(pow(fun(exp2), (exp1))))
            if (stack.size() > 1 && stack.top().value == "^") {
                stack.pop();
                if (stack.top().type == FUNCTION) {
                    powFunction = true;
                    powFunctionExp = output.back().value;  // save the exp for "^" operator (the last output element)
                    output.pop_back();
                } else {
                    stack.emplace(OPERATOR, "^");  // OPERATOR "^" for stack
                }
            }
            stack.emplace(LPAREN, "(");
            i++;
        } else if (infix[i] == ')') {
            // Pop until left parenthesis is found
            while (!stack.empty() && stack.top().type != LPAREN) {
                if (stack.top().value == "^") {
                    // convert to pow funtion
                    output.emplace_back(FUNCTION, "pow");
                } else {
                    // simply add the operator to the output list if not "^"
                    output.emplace_back(stack.top());
                }
                stack.pop();
            }
            stack.pop(); // Pop the left parenthesis
            if (!stack.empty() && stack.top().type == FUNCTION) {
                // // If the token at the top of the stack is a function token, pop it onto the output queue.
                output.emplace_back(stack.top());
                stack.pop(); // Pop the function token

                // special handling for the "^" operator, convert it to "pow" function
                if (powFunction) {
                    output.emplace_back(NUMBER, powFunctionExp);
                    output.emplace_back(FUNCTION, "pow");
                    powFunction = false;
                    powFunctionExp = "";
                }
            }
            i++;
        } else if (infix[i] == ',') {
            // Pop until left parenthesis is found
            while (!stack.empty() && stack.top().type != LPAREN) {
                if (stack.top().value == "^") {
                    // convert to pow funtion
                    output.emplace_back(FUNCTION, "pow");
                } else {
                    // simply add the operator to the output list if not "^"
                    output.emplace_back(stack.top());
                }
                stack.pop();
            }
            i++;
        } else {
            i++;  // Ignore other characters (spaces, etc.)
        }
    }

    // Pop remaining operators
    while (!stack.empty()) {
        if (stack.top().value == "^") {
            // convert to pow funtion
            output.emplace_back(FUNCTION, "pow");
        } else {
            // simply add the operator to the output list if not "^"
            output.emplace_back(stack.top());
        }
        stack.pop();
    }
    // return the postfix expression and the list of variables
    return make_pair(output, variables);
}

// build the expression tree from the postfix expression
TreeNode* buildExpressionTree(const vector<Token>& postfix) {
    stack<TreeNode* > stack;  // stack to store the tree root nodes

    for (const auto& token : postfix) {
        if (token.type == NUMBER || token.type == VARIABLE) {
            stack.push(new TreeNode(token));  // Push numbers and variables child onto the stack
        } else if (token.type == FUNCTION || token.type == OPERATOR) {
            TreeNode* node = new TreeNode(token);

            // right child is always the top of the stack for binary operators/functions
            if (getArity(token.value) == 2 && !stack.empty()) {
                node->right = stack.top();
                stack.pop();
            }

            // left child is then the top of the stack for all operators/functions
            if (!stack.empty()) {
                node->left = stack.top();
                stack.pop();
            }

            // special handling for unary operators (+, -)
            if ((token.value == "+" || token.value == "-") && node->left == nullptr) {
                // convert positive/negative unary to binary
                node->left = new TreeNode(Token(NUMBER, "0"));
            }
            // check if the arity number is correct
            if (node->left == nullptr || (node->right == nullptr && getArity(token.value) == 2)) {
                cerr << "Invalid expression: incorrect arity number \n";
                // Cleanup
                while (!stack.empty()) {
                    cerr << stack.top()->token.value << " ";
                    delete stack.top();
                    stack.pop();
                }
                cerr << endl;
                return nullptr;
            }
            
            stack.push(node);
        }
    }

    // The final tree should have only one node at the top of the stack
    if (stack.size() != 1) {
        cerr << "Invalid expression: incorrect tokens \n";
        // Cleanup
        while (!stack.empty()) {
            cerr << stack.top()->token.value << " ";
            delete stack.top();
            stack.pop();
        }
        cerr << endl;
        return nullptr;
    }

    // Return the root of the expression tree, the only node in the stack
    return stack.top();
}

// recursively build differential (of variable) tree from the expression tree
TreeNode* buildDiffTree(TreeNode* root, const string& variable) {
    // boundary condition
    if (root == nullptr) return nullptr;
    
    switch (root->token.type) {
        case NUMBER: {
            // Derivative of a constant is 0.
            return new TreeNode(Token(NUMBER, "0"));
        }
        case VARIABLE: {
            // Derivative of the variable with respect to itself is 1, otherwise 0.
            return new TreeNode(Token(NUMBER, root->token.value == variable ? "1" : "0"));
        }
        case OPERATOR: {
            // Handle binary operators (+, -, *, /)
            TreeNode* finalDerivative = nullptr;  // final derivative tree
            TreeNode* leftDerivative = buildDiffTree(root->left, variable);  // left child derivative
            TreeNode* rightDerivative = nullptr;  // right child derivative (if exists)
            if (root->right) rightDerivative = buildDiffTree(root->right, variable);
            
            if (root->token.value == "+" || root->token.value == "-") {
                // (f(x) +/- g(x))' = f'(x) +/- g'(x)
                finalDerivative = new TreeNode(root->token);
                finalDerivative->left = leftDerivative;
                finalDerivative->right = rightDerivative;
            } else if (root->token.value == "*") {
                // (f(x) * g(x))' = f'(x) * g(x) + f(x) * g'(x)
                finalDerivative = new TreeNode(Token(OPERATOR, "+", 2, false));
                finalDerivative->left = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left->left = leftDerivative;
                finalDerivative->left->right = root->right;
                finalDerivative->right = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->right->left = rightDerivative;
                finalDerivative->right->right = root->left;
            } else if (root->token.value == "/") {
                // (f(x) / g(x))' = (f'(x) * g(x) - f(x) * g'(x)) / (g(x))^2
                finalDerivative = new TreeNode(Token(OPERATOR, "/", 3, false));
                finalDerivative->left = new TreeNode(Token(OPERATOR, "-", 2, false));
                finalDerivative->left->left = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left->left->left = leftDerivative;
                finalDerivative->left->left->right = root->right;
                finalDerivative->left->right = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left->right->left = root->left;
                finalDerivative->left->right->right = rightDerivative;
                finalDerivative->right = new TreeNode(Token(FUNCTION, "pow"));
                finalDerivative->right->left = root->right;
                finalDerivative->right->right = new TreeNode(Token(NUMBER, "2"));
            }
            return finalDerivative;
        }
        case FUNCTION: {
            TreeNode* finalDerivative = nullptr;  // final derivative tree

            if (root->token.value != "log" && root->token.value != "pow") {  // Handle unary functions
                // apply chain rule: f'(g(x)) = f'(g(x)) * g'(x)
                finalDerivative = new TreeNode(Token(OPERATOR, "*", 3, false));
                TreeNode* outerDerivative = nullptr;                              // outer function derivative
                TreeNode* innerDerivative = buildDiffTree(root->left, variable);  // inner function derivative

                // process outer function derivative
                if (root->token.value == "sin") {
                    // (sin(f(x)))' = cos(f(x)) * f'(x)
                    outerDerivative = new TreeNode(Token(FUNCTION, "cos"));
                    outerDerivative->left = root->left; 
                } else if (root->token.value == "cos") {
                    // (cos(f(x)))' = -sin(f(x)) * f'(x)
                    outerDerivative = new TreeNode(Token(OPERATOR, "-", 2, false));
                    outerDerivative->left = new TreeNode(Token(NUMBER, "0"));
                    TreeNode* sinNode = new TreeNode(Token(FUNCTION, "sin"));
                    sinNode->left = root->left;
                    outerDerivative->right = sinNode;
                } else if (root->token.value == "tan") {
                    // (tan(f(x)))' = 1/(cos^2(f(x))) * f'(x)
                    outerDerivative = new TreeNode(Token(OPERATOR, "/", 3, false));
                    outerDerivative->left = new TreeNode(Token(NUMBER, "1"));
                    outerDerivative->right = new TreeNode(Token(FUNCTION, "pow"));
                    outerDerivative->right->left = new TreeNode(Token(FUNCTION, "cos"));
                    outerDerivative->right->left->left = root->left;
                    outerDerivative->right->right = new TreeNode(Token(NUMBER, "2"));
                } else if (root->token.value == "ln") {
                    // (ln(f(x)))' = 1/f(x) * f'(x)
                    outerDerivative = new TreeNode(Token(OPERATOR, "/", 3, false));
                    outerDerivative->left = new TreeNode(Token(NUMBER, "1"));
                    outerDerivative->right = root->left;
                } else if (root->token.value == "exp") {
                    // (exp(f(x)))' = exp(f(x)) * f'(x)
                    outerDerivative = new TreeNode(Token(FUNCTION, "exp"));
                    outerDerivative->left = root->left;
                }

                // Combine derivative with the derivative of the inner function for chain rule
                finalDerivative->left = outerDerivative;
                finalDerivative->right = innerDerivative;
            } else if (root->token.value == "log") {
                // (log(f(x),g(x)))' = (ln(f(x))*g'(x)/g(x) - ln(g(x))*f'(x)/f(x)) / ((ln(f(x)))^2)
                TreeNode* baseDerivative = buildDiffTree(root->left, variable);  // f'(x)
                TreeNode* naturalDerivative = buildDiffTree(root->right, variable);  // g'(x)
                finalDerivative = new TreeNode(Token(OPERATOR, "/", 3, false));
                finalDerivative->left = new TreeNode(Token(OPERATOR, "-", 2, false));
                finalDerivative->left->left = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left->left->left = new TreeNode(Token(FUNCTION, "ln"));
                finalDerivative->left->left->left->left = root->left;
                finalDerivative->left->left->right = new TreeNode(Token(OPERATOR, "/", 3, false));
                finalDerivative->left->left->right->left = naturalDerivative;
                finalDerivative->left->left->right->right = root->right;
                finalDerivative->left->right = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left->right->left = new TreeNode(Token(FUNCTION, "ln"));
                finalDerivative->left->right->left->left = root->right;
                finalDerivative->left->right->right = new TreeNode(Token(OPERATOR, "/", 3, false));
                finalDerivative->left->right->right->left = baseDerivative;
                finalDerivative->left->right->right->right = root->left;
                finalDerivative->right = new TreeNode(Token(FUNCTION, "pow"));
                finalDerivative->right->left = new TreeNode(Token(FUNCTION, "ln"));
                finalDerivative->right->left->left = root->left;
                finalDerivative->right->right = new TreeNode(Token(NUMBER, "2"));
            } else if (root->token.value == "pow") {
                // (f(x)^g(x))' = f(x)^g(x) * (g(x)*f'(x)/f(x) + g'(x)*ln(f(x))
                TreeNode* baseDerivative = buildDiffTree(root->left, variable);  // f'(x)
                TreeNode* exponentDerivative = buildDiffTree(root->right, variable);  // g'(x)
                finalDerivative = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->left = new TreeNode(Token(FUNCTION, "pow"));
                finalDerivative->left->left = root->left;
                finalDerivative->left->right = root->right;
                finalDerivative->right = new TreeNode(Token(OPERATOR, "+", 2, false));
                finalDerivative->right->left = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->right->left->left = root->right;
                finalDerivative->right->left->right = new TreeNode(Token(OPERATOR, "/", 3, false));
                finalDerivative->right->left->right->left = baseDerivative;
                finalDerivative->right->left->right->right = root->left;
                finalDerivative->right->right = new TreeNode(Token(OPERATOR, "*", 3, false));
                finalDerivative->right->right->left = exponentDerivative;
                finalDerivative->right->right->right = new TreeNode(Token(FUNCTION, "ln"));
                finalDerivative->right->right->right->left = root->left;
            }
            return finalDerivative;
        }
        default:
            break;
    }

    // Should not reach here for well-formed expressions
    return nullptr;  
}

// apply some rules to simplify a expression tree
// refreshChild: whether to refresh the child nodes
// the child is always simplified first since recursive algorithm
TreeNode* simplifyDiffTree(TreeNode* root, bool refreshChild = true) {
    // boundary condition, (NULL, NUMBER, VARIABLE)
    if (root == nullptr || root->token.type == NUMBER || root->token.type == VARIABLE) return root;

    if (refreshChild) {
        root->left = simplifyDiffTree(root->left);
        root->right = simplifyDiffTree(root->right);
    }

    TreeNode* simplifiedTree = root;
    bool isModified = false;  // flag to indicate if the tree is modified, true to simplify again
    if (simplifiedTree->token.type == FUNCTION) {
        if (simplifiedTree->token.value == "pow") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // pow(0, x) = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            } else if (simplifiedTree->right->token.type == NUMBER &&
                       simplifiedTree->right->token.value == "0") {
                // pow(x, 0) = 1
                simplifiedTree = new TreeNode(Token(NUMBER, "1")); isModified = true;
            }
        } else if (simplifiedTree->token.value == "exp") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // exp(0) = 1
                simplifiedTree = new TreeNode(Token(NUMBER, "1")); isModified = true;
            }
        } else if (simplifiedTree->token.value == "log" ||
                   simplifiedTree->token.value == "ln") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "1") {
                // log(1, x) = 0, ln(1) = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            }
        } else if (simplifiedTree->token.value == "sin" ||
                   simplifiedTree->token.value == "tan") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // sin(0) = 0, tan(0) = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            }
        }
    }
    if (simplifiedTree->token.type == OPERATOR) {
        if (simplifiedTree->token.value == "+") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // 0 + x = x
                simplifiedTree = simplifiedTree->right; isModified = true;
            } else if (simplifiedTree->right->token.type == NUMBER &&
                       simplifiedTree->right->token.value == "0") {
                // x + 0 = x
                simplifiedTree = simplifiedTree->left; isModified = true;
            } else if (simplifiedTree->left->token.type == NUMBER &&
                       simplifiedTree->right->token.type == NUMBER) {
                // a + b = a+b
                simplifiedTree = new TreeNode(Token(NUMBER, to_string(stoi(simplifiedTree->left->token.value)
                                                                     +stoi(simplifiedTree->right->token.value))));
                isModified = true;
            } else if (simplifiedTree->left->token.type == VARIABLE &&
                       simplifiedTree->right->token.type == VARIABLE &&
                       simplifiedTree->left->token.value == simplifiedTree->right->token.value) {
                // x + x = 2*x
                simplifiedTree = new TreeNode(Token(OPERATOR, "*", 3, false));
                simplifiedTree->left = new TreeNode(Token(NUMBER, "2"));
                simplifiedTree->right = simplifiedTree->left;
                isModified = true;
            } else if (simplifiedTree->right->token.type == OPERATOR &&
                       simplifiedTree->right->token.value == "-" &&
                       simplifiedTree->right->left->token.type == NUMBER &&
                       simplifiedTree->right->left->token.value == "0") {
                // x + (-y) = x-y
                TreeNode* simplifiedTreeTemp = new TreeNode(Token(OPERATOR, "-", 2, false));
                simplifiedTreeTemp->left = simplifiedTree->left;
                simplifiedTreeTemp->right = simplifiedTree->right->right;
                simplifiedTree = simplifiedTreeTemp;
                isModified = true;
            }
        } else if (simplifiedTree->token.value == "-") {
            if (simplifiedTree->right->token.type == NUMBER &&
                simplifiedTree->right->token.value == "0") {
                // x - 0 = x
                simplifiedTree = simplifiedTree->left; isModified = true;
            } else if (simplifiedTree->left->token.type == NUMBER &&
                       simplifiedTree->right->token.type == NUMBER) {
                // a - b = a-b
                simplifiedTree = new TreeNode(Token(NUMBER, to_string(stoi(simplifiedTree->left->token.value)
                                                                     -stoi(simplifiedTree->right->token.value))));
                isModified = true;
            } else if (simplifiedTree->left->token.type == VARIABLE &&
                       simplifiedTree->right->token.type == VARIABLE &&
                       simplifiedTree->left->token.value == simplifiedTree->right->token.value) {
                // x - x = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            } else if (simplifiedTree->right->token.type == OPERATOR &&
                       simplifiedTree->right->token.value == "-" &&
                       simplifiedTree->right->left->token.type == NUMBER &&
                       simplifiedTree->right->left->token.value == "0") {
                // x - (-y) = x+y
                TreeNode* simplifiedTreeTemp = new TreeNode(Token(OPERATOR, "+", 2, false));
                simplifiedTreeTemp->left = simplifiedTree->left;
                simplifiedTreeTemp->right = simplifiedTree->right->right;
                simplifiedTree = simplifiedTreeTemp;
                isModified = true;
            }
        } else if (simplifiedTree->token.value == "*") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // 0 * x = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            } else if (simplifiedTree->right->token.type == NUMBER &&
                       simplifiedTree->right->token.value == "0") {
                // x * 0 = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            } else if (simplifiedTree->left->token.type == NUMBER &&
                       simplifiedTree->left->token.value == "1") {
                // 1 * x = x
                simplifiedTree = simplifiedTree->right; isModified = true;
            } else if (simplifiedTree->right->token.type == NUMBER &&
                       simplifiedTree->right->token.value == "1") {
                // x * 1 = x
                simplifiedTree = simplifiedTree->left; isModified = true;
            } else if (simplifiedTree->left->token.type == NUMBER &&
                       simplifiedTree->right->token.type == NUMBER) {
                // a * b = a*b
                simplifiedTree = new TreeNode(Token(NUMBER, to_string(stoi(simplifiedTree->left->token.value)
                                                                     *stoi(simplifiedTree->right->token.value))));
                isModified = true;
            } else if (simplifiedTree->left->token.type == VARIABLE &&
                       simplifiedTree->right->token.type == VARIABLE &&
                       simplifiedTree->left->token.value == simplifiedTree->right->token.value) {
                // x * x = x^2
                simplifiedTree = new TreeNode(Token(FUNCTION, "pow"));
                simplifiedTree->left = simplifiedTree->left;
                simplifiedTree->right = new TreeNode(Token(NUMBER, "2"));
                isModified = true;
            } else if (simplifiedTree->right->token.type == OPERATOR &&
                       simplifiedTree->right->token.value == "/" && 
                       simplifiedTree->right->left->token.type == NUMBER &&
                       simplifiedTree->right->left->token.value == "1") {
                // a*(1/b) = a/b
                TreeNode* simplifiedTreeTemp = new TreeNode(Token(OPERATOR, "/", 3, false));
                simplifiedTreeTemp->left = simplifiedTree->left;
                simplifiedTreeTemp->right = simplifiedTree->right->right;
                simplifiedTree = simplifiedTreeTemp;
                isModified = true;
            } else if (simplifiedTree->left->token.type == OPERATOR &&
                       simplifiedTree->left->token.value == "/" && 
                       simplifiedTree->left->left->token.type == NUMBER &&
                       simplifiedTree->left->left->token.value == "1") {
                // (1/a)*b = b/a
                TreeNode* simplifiedTreeTemp = new TreeNode(Token(OPERATOR, "/", 3, false));
                simplifiedTreeTemp->left = simplifiedTree->right;
                simplifiedTreeTemp->right = simplifiedTree->left->right;
                simplifiedTree = simplifiedTreeTemp;
                isModified = true;
            }
        } else if (simplifiedTree->token.value == "/") {
            if (simplifiedTree->left->token.type == NUMBER &&
                simplifiedTree->left->token.value == "0") {
                // 0 / x = 0
                simplifiedTree = new TreeNode(Token(NUMBER, "0")); isModified = true;
            } else if (simplifiedTree->right->token.type == NUMBER &&
                       simplifiedTree->right->token.value == "1") {
                // x / 1 = x
                simplifiedTree = simplifiedTree->left; isModified = true;
            } else if (simplifiedTree->left->token.type == NUMBER &&
                       simplifiedTree->right->token.type == NUMBER) {
                // a / b = a' / b' (integer division)
                int leftValue = stoi(simplifiedTree->left->token.value);
                int rightValue = stoi(simplifiedTree->right->token.value);
                int gcd = std::gcd(leftValue, rightValue);
                simplifiedTree->left->token.value = to_string(leftValue / gcd);
                simplifiedTree->right->token.value = to_string(rightValue / gcd);
                isModified = true;
            } else if (simplifiedTree->left->token.type == VARIABLE &&
                       simplifiedTree->right->token.type == VARIABLE &&
                       simplifiedTree->left->token.value == simplifiedTree->right->token.value) {
                // x / x = 1
                simplifiedTree = new TreeNode(Token(NUMBER, "1")); isModified = true;
            } else if (simplifiedTree->right->token.type == OPERATOR &&
                       simplifiedTree->right->token.value == "/") {
                // a/(b/c) = (a*c)/b
                TreeNode* simplifiedTreeTemp = new TreeNode(Token(OPERATOR, "/", 3, false));
                simplifiedTreeTemp->left = new TreeNode(Token(OPERATOR, "*", 3, false));
                simplifiedTreeTemp->left->left = simplifiedTree->left;
                simplifiedTreeTemp->left->right = simplifiedTree->right->right;
                simplifiedTreeTemp->right = simplifiedTree->right->left;
                simplifiedTree = simplifiedTreeTemp;
                isModified = true;
            }
        }
        // TODO: more simplification rules ...
    }
    // if the tree is modified, simplify again to ensure full simplification
    if (isModified) simplifiedTree = simplifyDiffTree(simplifiedTree, false);
    return simplifiedTree;
}

// generate the final expression string from the expression tree
// based on inorder traversal
// MMAStyle: whether to generate the expression in Mathematica style (for testing)
string treeToExpression(TreeNode* root, bool MMAStyle = false) {
    if (root == nullptr) return "";

    string left = treeToExpression(root->left, MMAStyle);
    string right = treeToExpression(root->right, MMAStyle);

    string result = "";
    if (root->token.type == FUNCTION) {
        if (MMAStyle) {  // Mathematica style: func[x], func[x,y]
            // mapping from the function name to the Mathematica function name
            unordered_map<string, string> mmaFunc = {
                {"pow", "Power"},
                {"exp", "Exp"},
                {"ln", "Log"},
                {"log", "Log"},
                {"sin", "Sin"},
                {"cos", "Cos"},
                {"tan", "Tan"},
            };
            if (getArity(root->token.value) == 1) {
                result = mmaFunc[root->token.value] + "[" + left + "]";
            } else if (getArity(root->token.value) == 2) {
                result = mmaFunc[root->token.value] + "[" + left + "," + right + "]";
            }
        } else {
            if (getArity(root->token.value) == 1) {  // unary functions f(x)
                result = root->token.value + "(" + left + ")";
            } else if (getArity(root->token.value) == 2) {  // binary functions f(x,y)
                // result = root->token.value + "(" + left + "," + right + ")";
                if (root->token.value == "pow") {  // pow(a,b) = (a)^(b)
                    // flag to indicate if parentheses are needed for the base and exponent
                    bool baseParen = false, expParen = false;
                    // if the base or exponent is a function or operator, parentheses are needed
                    if (root->left->token.type == FUNCTION || root->left->token.type == OPERATOR) {
                        baseParen = true;
                    }
                    // if the base or exponent is a function or operator, parentheses are needed
                    if (root->right->token.type == FUNCTION || root->right->token.type == OPERATOR) {
                        expParen = true;
                    }
                    result = (baseParen?"("+left+")":left) + "^" + (expParen?"("+right+")":right);
                }
                else {
                    result = root->token.value + "(" + left + "," + right + ")";
                }
            }
        }
    } else if (root->token.type == OPERATOR) {
        bool leftParen = false, rightParen = false;  // flag to indicate if parentheses are needed
        int parentPrecedence = root->token.precedence;

        if (root->left->token.type == OPERATOR) {
            int leftPrecedence = root->left->token.precedence;
            if (leftPrecedence < parentPrecedence) {  // left child has lower precedence
                leftParen = true;
            }
        }
        if (root->right->token.type == OPERATOR) {
            int rightPrecedence = root->right->token.precedence;
            // right child has lower precedence or equal precedence and parent is "-" or "/"
            if (rightPrecedence < parentPrecedence ||
                (rightPrecedence == parentPrecedence && (root->token.value == "-" || root->token.value == "/"))) {
                rightParen = true;
            }
        }

        // leftParen = true; rightParen = true;

        // special handling for unary operators -NUM
        if (root->token.value == "-" && 
            root->left->token.type == NUMBER &&
            root->left->token.value == "0") {
            result = "-" + (rightParen?"("+right+")":right);
        }
        else {  // left operator right
            result = (leftParen?"("+left+")":left) + root->token.value + (rightParen?"("+right+")":right);
        }
    } else {  // NUMBER or VARIABLE
        result = root->token.value;
    }

    return result;
}

// (for debug in Step 2) postorder traversal of the expression tree 
// void postorderTraversal(TreeNode* root) {
//     if (root == nullptr) return;
//     postorderTraversal(root->left);
//     postorderTraversal(root->right);
//     std::cout << root->token.value << " ";
// }

// differentiating the expression based on all the variables
void diffExpression(const string& infix) {
    // Step 0: print the expression to be tested
    cout << ">>> Testing expression: " << infix << endl;

    // Step 1: Convert infix to postfix notation
    auto [postfix, variables] = processInfix(infix);
    sort(variables.begin(), variables.end());
    // debug info: postfix expression
    // cout << "Postfix notation: ";
    // for (const auto& token : postfix) {
    //     cout << token.value << " ";
    // }
    // cout << endl;

    // Step 2: Build expression tree
    TreeNode* root = buildExpressionTree(postfix);
    if (root == nullptr) {
        cerr << "Failed to build expression tree.\n";
        return;
    }
    // debug info: postorder traversal of the expression tree (same as postfix expression)
    // cout << "Postorder traversal: ";
    // postorderTraversal(root);
    // cout << endl;

    for (const auto& variable : variables) {
        // Step 3: Build the Diff tree
        TreeNode* diffRoot = buildDiffTree(root, variable);
        // cout << variable << ": " << treeToExpression(diffRoot) << endl;

        // Step 4: Simplify the derivative tree
        diffRoot = simplifyDiffTree(diffRoot);
        cout << variable << ": " << treeToExpression(diffRoot) << endl;

        // Step 5 (debug info): Verify the result using Mathematica
        // you can run it online at https://tio.run/#mathematica
        cout << "Mathematica verifying code:\nFullSimplify[D[" << treeToExpression(root, true) << "," << variable << "]-(" << treeToExpression(diffRoot, true) << ")]===0" << endl<< endl;
    }

    // Cleanup
    delete root;
}

int main() {
    // Example infix expression to test
    static const vector<string> testCases = {
        // basic expressions
        "2*a+3*b",
        "-2*a-3*b",
        "a*b","a/b",
        "a^b","exp(a)","pow(a,b)",
        "ln(a)","log(a,b)",
        "sin(x)+cos(x)+tan(x)",
        "sin(x)","cos(x)","tan(x)",

        // complex expressions
        "a+b^c*d",
        "a*10*b+2^a/a",
        "(x+(y^3-z))^2^x",
        "xx^2/xy*xy+a^a",  // multi-character variables
        "sin^2(x)^2",      // special "^" and right associative
        "log(a,b)/log(c,a)",
        "sin(x)*cos(y)+tan(z)/ln(x)",
        "x*ln(2/y)+y*cos(2*x)+sin(y)",

        // ultimate complex expressions
        "ln^2(x) * sin^2(y)^2 + exp(z^2) - log(3, x*y)",
        "sin^2(x^2) + cos^2(x)^2 - exp(ln^2(y))",
        "(tan^2(x))^2 + (exp(sin^2(y)))^2 - 2^ln^2(z)",
        "log(2^2, x^2) * ln^2(y*z) + exp^2(ln^2(x*y))",
        "sin^2(x)^2 + cos^2(y^2) - tan^2(z)^2 + ln^2(x*y*z) * exp^2(y^2) - log(7, x+y)^2",
        "log(cos(x)+4,exp(tan(y))+1)*((2^x-y^3)/(sin(z)+3))+sin(ln(y^2+2))*(tan^2(x)+cos(z^2/2))^(pow(y-2,2))+exp(sin(z))-2*ln(x+1)",
    };

    for (const auto& test : testCases) {
        diffExpression(test);
        cout << endl;
    }

    return 0;
}