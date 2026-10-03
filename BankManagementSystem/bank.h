#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <deque>
#include <set>
#include <map>

// ====================================================================================================
//                               COMMON DATA STRUCTURES & DEFINITIONS
// ====================================================================================================

/**
 * Date Structure (Nested within Customer)
 */
struct Date {
    int day;   // 1 - 31
    int month; // 1 - 12
    int year;  // e.g. 2026
};

/**
 * Address Structure (Nested within Customer)
 */
struct Address {
    std::string city;    // City name
    std::string state;   // State name
    std::string pincode; // 6-digit PIN code
};

/**
 * Customer Structure - Core bank account entity
 */
struct Customer {
    int accountNo;          // Unique account number (e.g. 1001, 1002)
    std::string name;       // Customer full name
    std::string phone;      // 10-digit registered phone number
    std::string accountType;// "SAVINGS" or "CURRENT"
    double balance;         // Account balance in Rupees
    Date openedDate;        // Nested Date struct
    Address address;        // Nested Address struct
};

/**
 * Transaction Structure - Audit ledger log entry
 */
struct Transaction {
    int txnId;              // Unique transaction ID
    int accountNo;          // Related customer account number
    std::string type;       // "OPENING", "DEPOSIT", "WITHDRAW"
    double amount;          // Transaction amount in Rupees
    double balanceAfter;    // Updated balance after transaction
};

/**
 * Stack ADT Structure (Student 4)
 */
const int MAX_STACK = 50;
struct SimpleStack {
    int arr[MAX_STACK];
    int top;
};

/**
 * Linear Queue Structure (Student 5)
 */
const int MAX_QUEUE = 50;
struct LinearQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
};

/**
 * Circular Queue Structure (Student 5)
 */
struct CircularQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
    int count;
};

// ====================================================================================================
//                               GLOBAL IN-MEMORY STATE (EXTERN DECLARATIONS)
// ====================================================================================================

extern std::vector<Customer> customerList;
extern std::vector<Transaction> transactionList;
extern int nextAccountNo;
extern int nextTxnId;

// ====================================================================================================
//                               STUDENT FUNCTION PROTOTYPES
// ====================================================================================================

// --- STUDENT 1: Account Management & Data Structures ---
void seedInitialData();
void displayAllCustomers();
void addNewCustomer();

// --- STUDENT 2: Financial Transactions & Search Algorithms ---
int linearSearchByPhone(const std::vector<Customer>& arr, std::string targetPhone);
int binarySearchByAccountNo(const std::vector<Customer>& arr, int targetAccNo);
void depositFunds();
void withdrawFunds();
void searchByAccountNo();
void searchByPhone();
void viewTransactionHistory();

// --- STUDENT 3: Sorting Algorithms & 2D Matrix Operations ---
void insertionSortByBalance(std::vector<Customer>& arr, bool ascending);
void sortCustomers();
void print2DMatrix(std::string title, int mat[2][2]);
void demoMatrixOperations();

// --- STUDENT 4: String Algorithms & Stack ADT ---
std::string reverseString(std::string str);
void countCharacterFrequency(std::string str);
bool searchPattern(std::string text, std::string pattern);
void demoStringOperations();

void initStack(SimpleStack& s);
bool isStackEmpty(const SimpleStack& s);
bool isStackFull(const SimpleStack& s);
void push(SimpleStack& s, int val);
int pop(SimpleStack& s);
bool isBalancedParentheses(std::string expr);
void demoStack();

// --- STUDENT 5: Queue ADT, STL Containers & Driver Controller ---
void initQueue(LinearQueue& q);
bool isQueueEmpty(const LinearQueue& q);
void enqueue(LinearQueue& q, int tokenNo);
int dequeue(LinearQueue& q);

void initCircularQueue(CircularQueue& cq);
void circularEnqueue(CircularQueue& cq, int tokenNo);
int circularDequeue(CircularQueue& cq);

void demoQueue();
void demoSTLContainers();
void printMenu();

#endif // BANK_H
