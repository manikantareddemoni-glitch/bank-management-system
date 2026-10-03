#include "bank.h"

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   STUDENT 4: STRING ALGORITHMS & STACK ADT (MODULES V, VIII)
// ====================================================================================================
// ====================================================================================================
// Responsibilities:
// 1. In-place String Reversal algorithm
// 2. 256-bucket ASCII character frequency histogram analysis
// 3. Naive substring pattern matching algorithm
// 4. Array-based Stack Abstract Data Type (ADT) implementation (LIFO: Push, Pop, isFull, isEmpty)
// 5. Stack application: Balanced parentheses verification for financial expression syntax
// ====================================================================================================

/**
 * Student 4 Function: In-place String Reversal
 * Explanation: Swaps str[i] with str[n - 1 - i] moving inwards.
 */
string reverseString(string str) {
    int n = static_cast<int>(str.length());
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = temp;
    }
    return str;
}

/**
 * Student 4 Function: Character Frequency Counter
 * Explanation: Uses 256-bucket ASCII array to count each character.
 */
void countCharacterFrequency(string str) {
    int count[256] = {0};

    for (size_t i = 0; i < str.length(); i++) {
        unsigned char ch = str[i];
        count[ch]++;
    }

    cout << " Character Frequencies in \"" << str << "\":\n  ";
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0 && i != ' ') {
            cout << "'" << static_cast<char>(i) << "':" << count[i] << "  ";
        }
    }
    cout << "\n";
}

/**
 * Student 4 Function: Naive Pattern Search (Substring Matching)
 */
bool searchPattern(string text, string pattern) {
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());
    if (m > n) return false;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) return true;
    }
    return false;
}

/**
 * Student 4 Function: String Manipulation Demo
 */
void demoStringOperations() {
    cout << "\n=======================================================\n";
    cout << "  [STUDENT 4] MODULE V: STRING MANIPULATION DEMO       \n";
    cout << "=======================================================\n";

    string sample = "AURORA";
    cout << " Original String: " << sample << "\n";
    cout << " 1. Reversed String: " << reverseString(sample) << "\n";
    cout << " 2. Frequency Analysis:\n";
    countCharacterFrequency(sample);

    string text = "Bank Management System";
    string query = "Management";
    cout << " 3. Pattern Search in \"" << text << "\" for \"" << query << "\": ";
    if (searchPattern(text, query))
        cout << "[FOUND!]\n";
    else
        cout << "[NOT FOUND]\n";
}

/**
 * Student 4 Functions: Array-based Stack ADT (LIFO)
 */
void initStack(SimpleStack& s) {
    s.top = -1;
}

bool isStackEmpty(const SimpleStack& s) {
    return (s.top == -1);
}

bool isStackFull(const SimpleStack& s) {
    return (s.top == MAX_STACK - 1);
}

void push(SimpleStack& s, int val) {
    if (isStackFull(s)) {
        cout << " [ERROR] Stack Overflow!\n";
        return;
    }
    s.arr[++s.top] = val;
}

int pop(SimpleStack& s) {
    if (isStackEmpty(s)) {
        cout << " [ERROR] Stack Underflow!\n";
        return -1;
    }
    return s.arr[s.top--];
}

/**
 * Student 4 Function: Stack Application - Parenthesis Matching
 */
bool isBalancedParentheses(string expr) {
    SimpleStack s;
    initStack(s);

    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') {
            push(s, 1);
        } else if (expr[i] == ')') {
            if (isStackEmpty(s)) return false;
            pop(s);
        }
    }
    return isStackEmpty(s);
}

/**
 * Student 4 Function: Stack ADT Demo
 */
void demoStack() {
    cout << "\n=======================================================\n";
    cout << "  [STUDENT 4] MODULE VIII: STACK ADT DEMONSTRATION     \n";
    cout << "=======================================================\n";

    SimpleStack myStack;
    initStack(myStack);

    cout << " 1. Pushing elements onto Stack: 10, 20, 30\n";
    push(myStack, 10);
    push(myStack, 20);
    push(myStack, 30);

    cout << " 2. Popping elements (LIFO Order):\n";
    while (!isStackEmpty(myStack)) {
        cout << "    Popped: " << pop(myStack) << "\n";
    }

    cout << "\n 3. Stack Application: Parentheses Balance Check:\n";
    string expr1 = "(A + B) * (C - D)";
    string expr2 = "((A + B) * C";
    cout << "    Expression \"" << expr1 << "\": " << (isBalancedParentheses(expr1) ? "BALANCED" : "UNBALANCED") << "\n";
    cout << "    Expression \"" << expr2 << "\": " << (isBalancedParentheses(expr2) ? "BALANCED" : "UNBALANCED") << "\n";
}
