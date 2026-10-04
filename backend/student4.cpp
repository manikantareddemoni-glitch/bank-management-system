#include "bank.h"

// ============================================================================
//   STUDENT 4: STRING ALGORITHMS & STACK ABSTRACT DATA TYPE (ADT)
// ============================================================================
// Key Topics Covered:
// 1. String Manipulation (Reversing a string with a simple for loop)
// 2. Character Frequency Counting (Using a basic 256 array)
// 3. Substring Pattern Matching (Using nested for and while loops)
// 4. Stack Data Structure (LIFO - Last In First Out):
//    - Array-based implementation (top index, push, pop, isEmpty, isFull)
// 5. Stack Application: Balanced Parentheses Checking
// ============================================================================

/**
 * Function: Reverse a String
 * Explanation:
 * - Uses a for loop up to length / 2.
 * - Swaps str[i] with str[length - 1 - i] using a temporary character variable.
 */
string reverseString(string str) {
    int n = (int)str.length();
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = temp;
    }
    return str;
}

/**
 * Function: Count Character Frequency in a String
 * Explanation:
 * - Uses an integer count array of size 256 initialized to 0.
 * - Loops through the string and increments count[(unsigned char)str[i]].
 * - Prints characters that appeared more than 0 times.
 */
void countCharacterFrequency(string str) {
    int count[256] = {0};
    int n = (int)str.length();

    // Count each character
    for (int i = 0; i < n; i++) {
        unsigned char ch = str[i];
        count[ch]++;
    }

    // Print frequencies
    cout << "Character counts in \"" << str << "\":\n  ";
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0 && i != ' ') {
            cout << "'" << (char)i << "': " << count[i] << "   ";
        }
    }
    cout << "\n";
}

/**
 * Function: Pattern Matching / Substring Search
 * Explanation:
 * - Checks if pattern exists in text.
 * - Loops through text and compares characters of pattern one by one.
 */
bool searchPattern(string text, string pattern) {
    int n = (int)text.length();
    int m = (int)pattern.length();

    if (m > n) return false;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            return true; // Full pattern matched
        }
    }
    return false; // Not found
}

/**
 * Function: String Operations Demo
 */
void demoStringOperations() {
    cout << "\n===============================================================\n";
    cout << "       [STUDENT 4] STRING OPERATIONS & PATTERN SEARCH DEMO     \n";
    cout << "===============================================================\n";

    string word = "BANKING";
    cout << "1. Original String: " << word << "\n";
    cout << "   Reversed String: " << reverseString(word) << "\n";

    cout << "\n2. Frequency Analysis:\n";
    countCharacterFrequency("SUCCESS");

    string text = "Bank Management System";
    string searchWord = "System";
    cout << "\n3. Substring Search in \"" << text << "\" for \"" << searchWord << "\": ";
    if (searchPattern(text, searchWord)) {
        cout << "[FOUND!]\n";
    } else {
        cout << "[NOT FOUND]\n";
    }
}

// ============================================================================
// STACK ADT IMPLEMENTATION (LIFO - Last In, First Out)
// ============================================================================

/**
 * Function: Initialize Stack
 */
void initStack(SimpleStack& s) {
    s.top = -1;
}

/**
 * Function: Check if Stack is Empty
 */
bool isStackEmpty(const SimpleStack& s) {
    if (s.top == -1) {
        return true;
    }
    return false;
}

/**
 * Function: Check if Stack is Full
 */
bool isStackFull(const SimpleStack& s) {
    if (s.top == MAX_STACK - 1) {
        return true;
    }
    return false;
}

/**
 * Function: Push an Element onto Stack
 */
void push(SimpleStack& s, int val) {
    if (isStackFull(s)) {
        cout << "\n[ERROR] Stack Overflow (Stack is full)!\n";
        return;
    }
    s.top = s.top + 1;
    s.arr[s.top] = val;
}

/**
 * Function: Pop an Element from Stack
 */
int pop(SimpleStack& s) {
    if (isStackEmpty(s)) {
        cout << "\n[ERROR] Stack Underflow (Stack is empty)!\n";
        return -1;
    }
    int val = s.arr[s.top];
    s.top = s.top - 1;
    return val;
}

/**
 * Function: Stack Application - Check Balanced Parentheses
 * Explanation:
 * - Pushes 1 to stack when '(' is found.
 * - Pops from stack when ')' is found.
 * - If stack is empty at the end, parentheses are balanced.
 */
bool isBalancedParentheses(string expr) {
    SimpleStack s;
    initStack(s);
    int n = (int)expr.length();

    for (int i = 0; i < n; i++) {
        if (expr[i] == '(') {
            push(s, 1);
        } else if (expr[i] == ')') {
            if (isStackEmpty(s)) {
                return false;
            }
            pop(s);
        }
    }
    return isStackEmpty(s);
}

/**
 * Function: Stack ADT Demo
 */
void demoStack() {
    cout << "\n===============================================================\n";
    cout << "       [STUDENT 4] STACK ADT & APPLICATION DEMO               \n";
    cout << "===============================================================\n";

    SimpleStack myStack;
    initStack(myStack);

    cout << "1. Pushing numbers onto Stack: 10, 20, 30\n";
    push(myStack, 10);
    push(myStack, 20);
    push(myStack, 30);

    cout << "\n2. Popping from Stack (LIFO Order - Last In First Out):\n";
    while (!isStackEmpty(myStack)) {
        cout << "   Popped: " << pop(myStack) << "\n";
    }

    cout << "\n3. Checking Parentheses in Expressions:\n";
    string expr1 = "((A + B) * (C - D))";
    string expr2 = "((A + B) * C";
    cout << "   Expression \"" << expr1 << "\": " << (isBalancedParentheses(expr1) ? "BALANCED" : "UNBALANCED") << "\n";
    cout << "   Expression \"" << expr2 << "\": " << (isBalancedParentheses(expr2) ? "BALANCED" : "UNBALANCED") << "\n";
}
