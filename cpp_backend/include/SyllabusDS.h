#ifndef SYLLABUS_DS_H
#define SYLLABUS_DS_H

#include "Customer.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <queue>
using namespace std;

// =========================================================================
// MODULE IV: 2D NUMERIC ARRAYS & MATRIX OPERATIONS
// =========================================================================
const int MAX_MATRIX_DIM = 10;

struct BranchMatrix {
    int rows;
    int cols;
    long long data[MAX_MATRIX_DIM][MAX_MATRIX_DIM];
};

BranchMatrix matrixAdd(const BranchMatrix& A, const BranchMatrix& B);
BranchMatrix matrixSubtract(const BranchMatrix& A, const BranchMatrix& B);
BranchMatrix matrixMultiply(const BranchMatrix& A, const BranchMatrix& B);
BranchMatrix matrixTranspose(const BranchMatrix& A);
void printMatrix(const string& title, const BranchMatrix& M);

// =========================================================================
// MODULE V: STRING ARRAYS, FREQUENCY, TOKENIZATION, PATTERN MATCHING
// =========================================================================
void analyzeCharacterFrequency(const string& text, int freq[256]);
string reverseString(const string& input);
vector<string> tokenizeString(const string& input, char delimiter);
bool patternMatchNaive(const string& text, const string& pattern);

// =========================================================================
// MODULE VIII: STACK ADT (ARRAY IMPLEMENTATION & APPLICATIONS)
// =========================================================================
const int STACK_CAPACITY = 100;

struct ArrayStack {
    Transaction items[STACK_CAPACITY];
    int top;
};

void stackInit(ArrayStack& s);
bool stackIsEmpty(const ArrayStack& s);
bool stackIsFull(const ArrayStack& s);
bool stackPush(ArrayStack& s, const Transaction& item);
bool stackPop(ArrayStack& s, Transaction& itemOut);
bool stackPeek(const ArrayStack& s, Transaction& itemOut);

// Stack Applications: Parenthesis Matching & Infix Expression Evaluation
bool checkParenthesisMatching(const string& expression);
int evaluateInfixExpression(const string& expression);

// =========================================================================
// MODULE IX: QUEUE ADT (ARRAY IMPLEMENTATIONS: LINEAR, CIRCULAR, PRIORITY)
// =========================================================================
const int QUEUE_CAPACITY = 100;

struct ArrayQueue {
    int tokens[QUEUE_CAPACITY];
    int front;
    int rear;
};

void queueInit(ArrayQueue& q);
bool queueIsEmpty(const ArrayQueue& q);
bool queueIsFull(const ArrayQueue& q);
bool queueEnqueue(ArrayQueue& q, int token);
bool queueDequeue(ArrayQueue& q, int& tokenOut);

struct CircularQueue {
    int tokens[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
};

void circularQueueInit(CircularQueue& cq);
bool circularQueueIsEmpty(const CircularQueue& cq);
bool circularQueueIsFull(const CircularQueue& cq);
bool circularQueueEnqueue(CircularQueue& cq, int token);
bool circularQueueDequeue(CircularQueue& cq, int& tokenOut);

struct PriorityToken {
    int token;
    int priority; // Higher value = higher service priority
    string customerName;
};

struct PriorityQueue {
    PriorityToken tokens[QUEUE_CAPACITY];
    int count;
};

void priorityQueueInit(PriorityQueue& pq);
bool priorityQueueEnqueue(PriorityQueue& pq, int token, int priority, string customerName);
bool priorityQueueDequeue(PriorityQueue& pq, PriorityToken& tokenOut);

// =========================================================================
// MODULE X: STL FUNDAMENTALS & CONTAINERS DEMONSTRATION
// =========================================================================
void demonstrateSTLContainers(const vector<Customer>& customers);

#endif // SYLLABUS_DS_H
