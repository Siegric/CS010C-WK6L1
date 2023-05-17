#include <stack>
#include <sstream>
#include <fstream>

#include "arithmeticExpression.h"

// Constructor
arithmeticExpression::arithmeticExpression(const string& expression) {
    infixExpression = expression;
    root = nullptr;
}

// Build the expression tree
void arithmeticExpression::buildTree() {
    string postfixExpression = infix_to_postfix();
    stack<TreeNode*> s;
    char key = 'a';

    // Iterate over each character in the postfix expression
    for (char c : postfixExpression) {
        if (isalnum(c)) { // If c is an operand (letter or digit)
            // Create a new TreeNode for the operand and push it onto the stack
            TreeNode* newNode = new TreeNode(c, key++);
            s.push(newNode);
        } else { // If c is an operator (+, -, *, /)
            // Pop the top two nodes from the stack and create a new TreeNode with the operator
            // Set the popped nodes as the left and right children of the new node
            TreeNode* rightNode = s.top();
            s.pop();
            TreeNode* leftNode = s.top();
            s.pop();
            TreeNode* opNode = new TreeNode(c, key++);
            opNode->left = leftNode;
            opNode->right = rightNode;
            s.push(opNode);
        }
    }

    // The final node remaining on the stack is the root of the expression tree
    root = s.top();
}

// Perform infix traversal of the expression tree
void arithmeticExpression::infix() {
    infix(root);
}

// Perform prefix traversal of the expression tree
void arithmeticExpression::prefix() {
    prefix(root);
}

// Perform postfix traversal of the expression tree
void arithmeticExpression::postfix() {
    postfix(root);
}

// Visualize the expression tree and save the dot file and corresponding image file
void arithmeticExpression::visualizeTree(const string& outputFilename) {
    ofstream outFS(outputFilename);
    if (!outFS.is_open()) {
        cout << "Error opening " << outputFilename << endl;
        return;
    }
    outFS << "digraph G {" << endl;
    visualizeTree(outFS, root);
    outFS << "}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0, outputFilename.size() - 4) + ".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

// Return the priority of an operator
int arithmeticExpression::priority(char op) {
    int priority = 0;
    if (op == '(') {
        priority = 3;
    } else if (op == '*' || op == '/') {
        priority = 2;
    } else if (op == '+' || op == '-') {
        priority = 1;
    }
    return priority;
}

// Convert the infix expression to postfix notation
string arithmeticExpression::infix_to_postfix() {
    stack<char> s;
    ostringstream oss;
    char c;

    // Iterate over each character in the infix expression
    for (unsigned i = 0; i < infixExpression.size(); ++i) {
        c = infixExpression.at(i);
        if (c == ' ') {
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') { // c is an operator
            if (c == '(') {
                s.push(c);
            } else if (c == ')') {
                // Pop operators from the stack and append them to the output until a matching '(' is found
                while (s.top() != '(') {
                    oss << s.top();
                    s.pop();
                }
                s.pop(); // Pop the '(' from the stack
            } else {
                // Pop operators from the stack and append them to the output as long as they have higher or equal priority
                while (!s.empty() && priority(c) <= priority(s.top())) {
                    if (s.top() == '(') {
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c); // Push the current operator onto the stack
            }
        } else { // c is an operand (letter or digit)
            oss << c; // Append the operand to the output
        }
    }

    // Append any remaining operators from the stack to the output
    while (!s.empty()) {
        oss << s.top();
        s.pop();
    }

    return oss.str();
}

// Perform infix traversal of the expression tree recursively
void arithmeticExpression::infix(TreeNode* node) {
    if (node != nullptr) {
        if (node->left != nullptr || node->right != nullptr) {
            cout << "(";
        }
        infix(node->left);
        cout << node->data;
        infix(node->right);
        if (node->left != nullptr || node->right != nullptr) {
            cout << ")";
        }
    }
}

// Perform prefix traversal of the expression tree recursively
void arithmeticExpression::prefix(TreeNode* node) {
    if (node != nullptr) {
        cout << node->data;
        prefix(node->left);
        prefix(node->right);
    }
}

// Perform postfix traversal of the expression tree recursively
void arithmeticExpression::postfix(TreeNode* node) {
    if (node != nullptr) {
        postfix(node->left);
        postfix(node->right);
        cout << node->data;
    }
}

// Visualize the expression tree recursively and generate the dot file
void arithmeticExpression::visualizeTree(ofstream& outFS, TreeNode* node) {
    if (node != nullptr) {
        outFS << "  " << node->key << " [label=\"" << node->data << "\"];" << endl;
        if (node->left != nullptr) {
            outFS << "  " << node->key << " -> " << node->left->key << ";" << endl;
            visualizeTree(outFS, node->left);
        }
        if (node->right != nullptr) {
            outFS << "  " << node->key << " -> " << node->right->key << ";" << endl;
            visualizeTree(outFS, node->right);
        }
    }
}
