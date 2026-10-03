#include "SyllabusDS.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stack>
#include <cctype>

using namespace std;

// =========================================================================
// MODULE IV: 2D NUMERIC ARRAYS & MATRIX OPERATIONS
// =========================================================================
BranchMatrix matrixAdd(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix result;
    result.rows = A.rows;
    result.cols = A.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            result.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }
    return result;
}

BranchMatrix matrixSubtract(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix result;
    result.rows = A.rows;
    result.cols = A.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            result.data[i][j] = A.data[i][j] - B.data[i][j];
        }
    }
    return result;
}

BranchMatrix matrixMultiply(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix result;
    result.rows = A.rows;
    result.cols = B.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            result.data[i][j] = 0;
            for (int k = 0; k < A.cols; ++k) {
                result.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return result;
}

BranchMatrix matrixTranspose(const BranchMatrix& A) {
    BranchMatrix result;
    result.rows = A.cols;
    result.cols = A.rows;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            result.data[j][i] = A.data[i][j];
        }
    }
    return result;
}

void printMatrix(const string& title, const BranchMatrix& M) {
    cout << "\n--- " << title << " (" << M.rows << "x" << M.cols << ") ---\n";
    for (int i = 0; i < M.rows; ++i) {
        cout << " [ ";
        for (int j = 0; j < M.cols; ++j) {
            cout << setw(8) << M.data[i][j] << " ";
        }
        cout << "]\n";
    }
}

// =========================================================================
// MODULE V: STRING ARRAYS, FREQUENCY, TOKENIZATION, PATTERN MATCHING
// =========================================================================
void analyzeCharacterFrequency(const string& text, int freq[256]) {
    for (int i = 0; i < 256; ++i) freq[i] = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        freq[c]++;
    }
}

string reverseString(const string& input) {
    string rev = input;
    int n = static_cast<int>(rev.length());
    for (int i = 0; i < n / 2; ++i) {
        char temp = rev[i];
        rev[i] = rev[n - 1 - i];
        rev[n - 1 - i] = temp;
    }
    return rev;
}

vector<string> tokenizeString(const string& input, char delimiter) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool patternMatchNaive(const string& text, const string& pattern) {
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());
    if (m == 0) return true;
    if (m > n) return false;

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) return true;
    }
    return false;
}

// =========================================================================
// MODULE VIII: STACK ADT (ARRAY IMPLEMENTATION & APPLICATIONS)
// =========================================================================
void stackInit(ArrayStack& s) {
    s.top = -1;
}

bool stackIsEmpty(const ArrayStack& s) {
    return (s.top == -1);
}

bool stackIsFull(const ArrayStack& s) {
    return (s.top == STACK_CAPACITY - 1);
}

bool stackPush(ArrayStack& s, const Transaction& item) {
    if (stackIsFull(s)) return false;
    s.top++;
    s.items[s.top] = item;
    return true;
}

bool stackPop(ArrayStack& s, Transaction& itemOut) {
    if (stackIsEmpty(s)) return false;
    itemOut = s.items[s.top];
    s.top--;
    return true;
}

bool stackPeek(const ArrayStack& s, Transaction& itemOut) {
    if (stackIsEmpty(s)) return false;
    itemOut = s.items[s.top];
    return true;
}

// Parenthesis Matching Application
bool checkParenthesisMatching(const string& expression) {
    char charStack[100];
    int top = -1;

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        if (ch == '(' || ch == '{' || ch == '[') {
            if (top >= 99) return false; // Stack overflow
            charStack[++top] = ch;
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (top == -1) return false; // Stack underflow (unmatched closing)
            char topChar = charStack[top--];
            if ((ch == ')' && topChar != '(') ||
                (ch == '}' && topChar != '{') ||
                (ch == ']' && topChar != '[')) {
                return false; // Mismatched brackets
            }
        }
    }
    return (top == -1); // Must be empty at end
}

// Helper for infix evaluation operator precedence
static int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

static int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? (a / b) : 0;
    }
    return 0;
}

int evaluateInfixExpression(const string& expression) {
    int values[100];
    int valTop = -1;
    char ops[100];
    int opsTop = -1;

    for (size_t i = 0; i < expression.length(); ++i) {
        if (expression[i] == ' ') continue;

        if (expression[i] == '(') {
            ops[++opsTop] = '(';
        } else if (isdigit(expression[i])) {
            int val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            i--;
            values[++valTop] = val;
        } else if (expression[i] == ')') {
            while (opsTop != -1 && ops[opsTop] != '(') {
                int val2 = values[valTop--];
                int val1 = values[valTop--];
                char op = ops[opsTop--];
                values[++valTop] = applyOp(val1, val2, op);
            }
            if (opsTop != -1) opsTop--; // Pop '('
        } else { // Operator (+, -, *, /)
            while (opsTop != -1 && precedence(ops[opsTop]) >= precedence(expression[i])) {
                int val2 = values[valTop--];
                int val1 = values[valTop--];
                char op = ops[opsTop--];
                values[++valTop] = applyOp(val1, val2, op);
            }
            ops[++opsTop] = expression[i];
        }
    }

    while (opsTop != -1) {
        int val2 = values[valTop--];
        int val1 = values[valTop--];
        char op = ops[opsTop--];
        values[++valTop] = applyOp(val1, val2, op);
    }

    return (valTop != -1) ? values[valTop] : 0;
}

// =========================================================================
// MODULE IX: QUEUE ADT (ARRAY IMPLEMENTATIONS)
// =========================================================================
void queueInit(ArrayQueue& q) {
    q.front = 0;
    q.rear = -1;
}

bool queueIsEmpty(const ArrayQueue& q) {
    return (q.rear < q.front);
}

bool queueIsFull(const ArrayQueue& q) {
    return (q.rear == QUEUE_CAPACITY - 1);
}

bool queueEnqueue(ArrayQueue& q, int token) {
    if (queueIsFull(q)) return false;
    q.rear++;
    q.tokens[q.rear] = token;
    return true;
}

bool queueDequeue(ArrayQueue& q, int& tokenOut) {
    if (queueIsEmpty(q)) return false;
    tokenOut = q.tokens[q.front];
    q.front++;
    return true;
}

// Circular Queue
void circularQueueInit(CircularQueue& cq) {
    cq.front = 0;
    cq.rear = -1;
    cq.count = 0;
}

bool circularQueueIsEmpty(const CircularQueue& cq) {
    return (cq.count == 0);
}

bool circularQueueIsFull(const CircularQueue& cq) {
    return (cq.count == QUEUE_CAPACITY);
}

bool circularQueueEnqueue(CircularQueue& cq, int token) {
    if (circularQueueIsFull(cq)) return false;
    cq.rear = (cq.rear + 1) % QUEUE_CAPACITY;
    cq.tokens[cq.rear] = token;
    cq.count++;
    return true;
}

bool circularQueueDequeue(CircularQueue& cq, int& tokenOut) {
    if (circularQueueIsEmpty(cq)) return false;
    tokenOut = cq.tokens[cq.front];
    cq.front = (cq.front + 1) % QUEUE_CAPACITY;
    cq.count--;
    return true;
}

// Priority Queue
void priorityQueueInit(PriorityQueue& pq) {
    pq.count = 0;
}

bool priorityQueueEnqueue(PriorityQueue& pq, int token, int priority, string customerName) {
    if (pq.count >= QUEUE_CAPACITY) return false;
    
    PriorityToken item;
    item.token = token;
    item.priority = priority;
    item.customerName = customerName;

    // Insertion sort based on priority (descending)
    int i = pq.count - 1;
    while (i >= 0 && pq.tokens[i].priority < priority) {
        pq.tokens[i + 1] = pq.tokens[i];
        i--;
    }
    pq.tokens[i + 1] = item;
    pq.count++;
    return true;
}

bool priorityQueueDequeue(PriorityQueue& pq, PriorityToken& tokenOut) {
    if (pq.count == 0) return false;
    tokenOut = pq.tokens[0];
    for (int i = 1; i < pq.count; ++i) {
        pq.tokens[i - 1] = pq.tokens[i];
    }
    pq.count--;
    return true;
}

// =========================================================================
// MODULE X: STL CONTAINERS & ITERATORS DEMONSTRATION
// =========================================================================
void demonstrateSTLContainers(const vector<Customer>& customers) {
    cout << "\n=======================================================\n";
    cout << "  MODULE X: STL FUNDAMENTALS & CONTAINERS DEMO\n";
    cout << "=======================================================\n";

    // 1. std::pair & std::vector with dynamic resizing
    cout << "\n[1] std::vector & std::pair (Customer Account & Balance mapping):\n";
    vector<pair<int, long long>> accountBalancePairs;
    for (size_t i = 0; i < customers.size(); ++i) {
        accountBalancePairs.push_back(make_pair(customers[i].accountNo, customers[i].balancePaise));
    }
    cout << "  Populated " << accountBalancePairs.size() << " account-balance pairs in std::vector.\n";

    // 2. Iterators demonstration
    cout << "\n[2] std::vector Iterators traversal:\n";
    cout << "  Using const_iterator to list account numbers: ";
    for (vector<pair<int, long long>>::const_iterator it = accountBalancePairs.begin(); it != accountBalancePairs.end(); ++it) {
        cout << "#" << it->first << " ";
    }
    cout << "\n";

    // 3. std::deque operations
    cout << "\n[3] std::deque (Double-ended Queue) teller counter:\n";
    deque<string> tellerDeque;
    tellerDeque.push_back("Token #101 (Regular)");
    tellerDeque.push_back("Token #102 (Regular)");
    tellerDeque.push_front("Token #VIP-1 (Priority Teller)"); // Front push
    cout << "  Front of Deque: " << tellerDeque.front() << "\n";
    cout << "  Back of Deque:  " << tellerDeque.back() << "\n";
    tellerDeque.pop_front();
    cout << "  Deque size after pop_front: " << tellerDeque.size() << "\n";

    // 4. std::set (Unique Phone numbers registry)
    cout << "\n[4] std::set (Unique customer phone numbers tracking):\n";
    set<string> uniquePhones;
    for (size_t i = 0; i < customers.size(); ++i) {
        uniquePhones.insert(customers[i].phone);
    }
    cout << "  Unique phone count in std::set: " << uniquePhones.size() << "\n";

    // 5. std::map (Fast Key-Value Lookup by Account Number)
    cout << "\n[5] std::map (Account Number -> Customer Struct O(log n) Lookup):\n";
    map<int, Customer> customerMap;
    for (size_t i = 0; i < customers.size(); ++i) {
        customerMap[customers[i].accountNo] = customers[i];
    }
    cout << "  Map size: " << customerMap.size() << " records.\n";
    if (!customers.empty()) {
        int sampleAcc = customers[0].accountNo;
        map<int, Customer>::iterator mapIt = customerMap.find(sampleAcc);
        if (mapIt != customerMap.end()) {
            cout << "  Map Lookup found Account #" << sampleAcc << ": " << mapIt->second.name << "\n";
        }
    }
    cout << "=======================================================\n";
}
