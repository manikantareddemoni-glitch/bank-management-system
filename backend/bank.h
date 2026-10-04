#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ============================================================================
//                         COMMON STRUCTURES (STRUCTS)
// ============================================================================

// 1. Structure for Date (Day, Month, Year)
struct Date {
    int day;
    int month;
    int year;
};

// 2. Structure for Address (City, State, Pincode)
struct Address {
    string city;
    string state;
    string pincode;
};

// 3. Structure for Customer Account
struct Customer {
    int accountNo;          // e.g. 1001, 1002
    string name;            // Customer Name
    string phone;           // 10-digit Phone Number
    string accountType;     // "SAVINGS" or "CURRENT"
    double balance;         // Current Balance in Rupees
    Date openedDate;        // Date structure inside Customer
    Address address;        // Address structure inside Customer
};

// 4. Structure for Transaction Record
struct Transaction {
    int txnId;              // Transaction ID: 1, 2, 3...
    int accountNo;          // Account number for this transaction
    string type;            // "OPENING", "DEPOSIT", "WITHDRAW"
    double amount;          // Amount deposited or withdrawn
    double balanceAfter;    // Balance after transaction
};

// 5. Structure for Stack (Student 4)
const int MAX_STACK = 50;
struct SimpleStack {
    int arr[MAX_STACK];
    int top;
};

// 6. Structure for Linear Queue (Student 5)
const int MAX_QUEUE = 50;
struct LinearQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
};

// 7. Structure for Circular Queue (Student 5)
struct CircularQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
    int count;
};

// ============================================================================
//                         GLOBAL VARIABLES
// ============================================================================
extern vector<Customer> customerList;
extern vector<Transaction> transactionList;
extern int nextAccountNo;
extern int nextTxnId;

// ============================================================================
//                         FUNCTION DECLARATIONS
// ============================================================================

// Student 1: Account Management
void seedInitialData();
void displayAllCustomers();
void addNewCustomer();
void sortAccountsByNumber();

// Student 2: Transactions & Search Algorithms
int linearSearchByPhone(const vector<Customer>& arr, string targetPhone);
int binarySearchByAccountNo(const vector<Customer>& arr, int targetAccNo);
void depositFunds();
void withdrawFunds();
void searchByAccountNo();
void searchByPhone();
void viewTransactionHistory();

// Student 3: Sorting & 2D Matrix Operations
void insertionSortByBalance(vector<Customer>& arr, bool ascending);
void sortCustomers();
void print2DMatrix(string title, int mat[2][2]);
void demoMatrixOperations();

// Student 4: String Algorithms & Stack ADT
string reverseString(string str);
void countCharacterFrequency(string str);
bool searchPattern(string text, string pattern);
void demoStringOperations();

void initStack(SimpleStack& s);
bool isStackEmpty(const SimpleStack& s);
bool isStackFull(const SimpleStack& s);
void push(SimpleStack& s, int val);
int pop(SimpleStack& s);
bool isBalancedParentheses(string expr);
void demoStack();

// Student 5: Queue ADT & Menu Controller
void initQueue(LinearQueue& q);
bool isQueueEmpty(const LinearQueue& q);
void enqueue(LinearQueue& q, int tokenNo);
int dequeue(LinearQueue& q);

void initCircularQueue(CircularQueue& cq);
void circularEnqueue(CircularQueue& cq, int tokenNo);
int circularDequeue(CircularQueue& cq);
void demoQueue();
void demoSimpleArrays();
void printMenu();

#endif // BANK_H
