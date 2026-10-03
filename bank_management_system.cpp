/**
 * ====================================================================================================
 *                         AURORA HIGHER EDUCATION AND RESEARCH ACADEMY
 *                             CS207: DATA STRUCTURE USING C++
 *                               BANK MANAGEMENT SYSTEM
 * ====================================================================================================
 * 
 * SYLLABUS TOPICS COVERED IN THIS CODE:
 * ----------------------------------------------------------------------------------------------------
 * - Module I & II:  Variables, Data Types, cin/cout Streams, Control Statements (if-else, switch, loops)
 *                   and Functions with Pass-by-Reference.
 * - Module III:     1D Arrays & Hand-rolled Algorithms (Linear Search, Binary Search, Insertion Sort).
 * - Module IV:      2D Arrays & Matrix Operations (Addition, Subtraction, Multiplication, Transpose).
 * - Module V:       String Manipulation (Length, Traversal, In-place Reversal, Character Frequency, Pattern Matching).
 * - Module VI:      User-defined Structures (struct) and Nested Structures (Date, Address inside Customer).
 * - Module VII:     Time Complexity Analysis (Big-O Notations: O(1), O(log n), O(n), O(n^2)).
 * - Module VIII:    Stack ADT using Array (LIFO: push, pop, isEmpty, isFull) & Parentheses Matching.
 * - Module IX:      Queue ADT using Array (FIFO: Linear Queue, Circular Queue with modulo wraparound).
 * - Module X:       Standard Template Library (STL: vector, pair, deque, set, map, iterators).
 * ====================================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <set>
#include <deque>

using namespace std;

// ====================================================================================================
// SECTION 1 (MODULE VI): STRUCTURES & NESTED STRUCTURES
// ====================================================================================================

/**
 * 1. Date Structure
 * Explanation: Stores calendar date (day, month, year).
 * Concept: Used as a nested member inside Customer struct.
 */
struct Date {
    int day;
    int month;
    int year;
};

/**
 * 2. Address Structure
 * Explanation: Stores customer residential address details.
 * Concept: Nested structure inside Customer struct.
 */
struct Address {
    string city;
    string state;
    string pincode;
};

/**
 * 3. Customer Structure
 * Explanation: Holds all details of a single bank customer account.
 * Concept: Composite structure combining primitive types (int, string, double)
 *          and nested custom structures (Date, Address).
 */
struct Customer {
    int accountNo;          // Unique account number (e.g. 1001, 1002)
    string name;            // Customer full name
    string phone;           // 10-digit mobile number
    string accountType;     // "SAVINGS" or "CURRENT"
    double balance;         // Account balance in Rupees (e.g. 5000.00)
    Date openedDate;        // Nested Date structure
    Address address;        // Nested Address structure
};

/**
 * 4. Transaction Structure
 * Explanation: Stores history log of each deposit or withdrawal.
 */
struct Transaction {
    int txnId;              // Unique transaction ID (1, 2, 3...)
    int accountNo;          // Related account number
    string type;            // "OPENING", "DEPOSIT", "WITHDRAW"
    double amount;          // Transaction amount
    double balanceAfter;    // Updated balance after the transaction
};

// Global in-memory storage using std::vector (Module X & Module III)
vector<Customer> customerList;
vector<Transaction> transactionList;
int nextAccountNo = 1007;   // Auto-increment counter for new account numbers
int nextTxnId = 7;          // Auto-increment counter for new transactions

// ====================================================================================================
// SECTION 2 (MODULE III & VII): SEARCHING & SORTING ALGORITHMS
// ====================================================================================================

/**
 * 1. Linear Search Algorithm (Unsorted search by phone number)
 * -------------------------------------------------------------
 * Explanation:
 *   - Starts at index 0 and checks each customer one-by-one.
 *   - If phone number matches, it returns the index.
 *   - If loop finishes without match, returns -1.
 * 
 * Complexity (Module VII):
 *   - Time Complexity:  O(n)  [Scans all n elements in worst case]
 *   - Space Complexity: O(1)  [No extra memory used]
 */
int linearSearchByPhone(const vector<Customer>& arr, string targetPhone) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i].phone == targetPhone) {
            return i; // Found at index i
        }
    }
    return -1; // Not found
}

/**
 * 2. Binary Search Algorithm (Sorted search by Account Number)
 * ------------------------------------------------------------
 * Explanation:
 *   - Precondition: The array must already be sorted by accountNo!
 *   - Calculates mid = (low + high) / 2.
 *   - If target == arr[mid], found!
 *   - If target > arr[mid], search in the right half (low = mid + 1).
 *   - If target < arr[mid], search in the left half (high = mid - 1).
 * 
 * Complexity (Module VII):
 *   - Time Complexity:  O(log n) [Divides search space in half each step]
 *   - Space Complexity: O(1)     [Iterative constant space]
 */
int binarySearchByAccountNo(const vector<Customer>& arr, int targetAccNo) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid].accountNo == targetAccNo) {
            return mid; // Found at index mid
        }
        else if (arr[mid].accountNo < targetAccNo) {
            low = mid + 1; // Move to right half
        }
        else {
            high = mid - 1; // Move to left half
        }
    }
    return -1; // Not found
}

/**
 * 3. Insertion Sort Algorithm (Sorting customer records by balance)
 * ------------------------------------------------------------------
 * Explanation:
 *   - Divides the array into sorted and unsorted parts.
 *   - Takes one element (key) from unsorted part and shifts larger
 *     elements one position ahead to make room for insertion.
 * 
 * Complexity (Module VII):
 *   - Time Complexity:  O(n^2) worst/average case, O(n) best case (already sorted).
 *   - Space Complexity: O(1) in-place sort.
 */
void insertionSortByBalance(vector<Customer>& arr, bool ascending) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        Customer key = arr[i]; // Element to be inserted
        int j = i - 1;

        if (ascending) {
            // Shift elements greater than key to the right
            while (j >= 0 && arr[j].balance > key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        } else {
            // Shift elements smaller than key to the right (Descending)
            while (j >= 0 && arr[j].balance < key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        }
        arr[j + 1] = key; // Place key at its correct position
    }
}

// ====================================================================================================
// SECTION 3 (MODULE IV): 2D ARRAYS & MATRIX OPERATIONS
// ====================================================================================================

/**
 * Matrix Helper Functions:
 * Demonstrates basic 2x2 matrix operations (Addition, Subtraction, Multiplication, Transpose)
 * representing financial branch transaction data.
 */

// Function to print a 2x2 matrix
void print2DMatrix(string title, int mat[2][2]) {
    cout << "\n--- " << title << " (2x2 Matrix) ---\n";
    for (int i = 0; i < 2; i++) {
        cout << "  [ ";
        for (int j = 0; j < 2; j++) {
            cout << setw(6) << mat[i][j] << " ";
        }
        cout << "]\n";
    }
}

// Demonstration of Matrix Operations
void demoMatrixOperations() {
    cout << "\n=======================================================\n";
    cout << "  MODULE IV: 2D NUMERIC ARRAY & MATRIX DEMONSTRATION  \n";
    cout << "=======================================================\n";

    // Branch 1 & Branch 2 quarterly cash flows (2x2 matrices)
    int A[2][2] = { {1000, 2000}, {1500, 2500} };
    int B[2][2] = { {500,  1200}, {800,  1400} };

    print2DMatrix("Branch A Cash Flow", A);
    print2DMatrix("Branch B Cash Flow", B);

    // 1. Matrix Addition: C[i][j] = A[i][j] + B[i][j]
    int Sum[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Sum[i][j] = A[i][j] + B[i][j];
        }
    }
    print2DMatrix("1. Matrix Addition (A + B)", Sum);

    // 2. Matrix Subtraction: Diff[i][j] = A[i][j] - B[i][j]
    int Diff[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Diff[i][j] = A[i][j] - B[i][j];
        }
    }
    print2DMatrix("2. Matrix Subtraction (A - B)", Diff);

    // 3. Matrix Multiplication: Mult[i][j] = Sum(A[i][k] * B[k][j])
    int Mult[2][2] = { {0, 0}, {0, 0} };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Mult[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    print2DMatrix("3. Matrix Multiplication (A * B)", Mult);

    // 4. Matrix Transpose: Trans[j][i] = A[i][j] (Swap rows and columns)
    int Trans[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Trans[j][i] = A[i][j];
        }
    }
    print2DMatrix("4. Matrix Transpose of A (Rows <-> Columns)", Trans);
}

// ====================================================================================================
// SECTION 4 (MODULE V): STRING MANIPULATION & ALGORITHMS
// ====================================================================================================

/**
 * 1. In-place String Reversal
 * Explanation: Swaps character at start (i) with character at end (len - 1 - i)
 * until it reaches the middle.
 */
string reverseString(string str) {
    int n = str.length();
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = temp;
    }
    return str;
}

/**
 * 2. Character Frequency Counter
 * Explanation: Uses an array of size 256 (ASCII table) to count occurrences of each character.
 */
void countCharacterFrequency(string str) {
    int count[256] = {0}; // Initialize all 256 character counts to 0

    for (int i = 0; i < str.length(); i++) {
        unsigned char ch = str[i];
        count[ch]++;
    }

    cout << " Character Frequencies in \"" << str << "\":\n  ";
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0 && i != ' ') {
            cout << "'" << (char)i << "':" << count[i] << "  ";
        }
    }
    cout << "\n";
}

/**
 * 3. Simple Pattern Search (Substring Matching)
 * Explanation: Slides pattern over text to check if substring exists.
 */
bool searchPattern(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m > n) return false;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) return true; // Found match!
    }
    return false;
}

// String Demo function
void demoStringOperations() {
    cout << "\n=======================================================\n";
    cout << "      MODULE V: STRING MANIPULATION DEMONSTRATION      \n";
    cout << "=======================================================\n";

    string sample = "AURORA";
    cout << " Original String: " << sample << "\n";
    cout << " 1. Reversed String: " << reverseString(sample) << "\n";
    cout << " 2. Frequency Analysis:\n";
    countCharacterFrequency(sample);

    string text = "Bank Management System";
    string query = "Management";
    cout << " 3. Pattern Search in \"" << text << "\" for \"" << query << "\": ";
    if (searchPattern(text, query)) {
        cout << "[FOUND!]\n";
    } else {
        cout << "[NOT FOUND]\n";
    }
}

// ====================================================================================================
// SECTION 5 (MODULE VIII): STACK ADT & APPLICATION
// ====================================================================================================

/**
 * Array Stack Implementation (LIFO - Last In First Out)
 * Explanation:
 *   - 'top' pointer starts at -1 (empty).
 *   - push(): increments top and inserts element.
 *   - pop(): removes and returns top element, decrements top.
 */
const int MAX_STACK = 50;

struct SimpleStack {
    int arr[MAX_STACK];
    int top;
};

void initStack(SimpleStack& s) {
    s.top = -1; // -1 means empty stack
}

bool isStackEmpty(const SimpleStack& s) {
    return (s.top == -1);
}

bool isStackFull(const SimpleStack& s) {
    return (s.top == MAX_STACK - 1);
}

void push(SimpleStack& s, int val) {
    if (isStackFull(s)) {
        cout << " [ERROR] Stack Overflow! Stack is full.\n";
        return;
    }
    s.top++;
    s.arr[s.top] = val;
}

int pop(SimpleStack& s) {
    if (isStackEmpty(s)) {
        cout << " [ERROR] Stack Underflow! Stack is empty.\n";
        return -1;
    }
    int val = s.arr[s.top];
    s.top--;
    return val;
}

/**
 * Stack Application: Parenthesis Matching Validator
 * Explanation:
 *   - Scans string: pushes '(' onto stack.
 *   - When ')' is found, pops from stack.
 *   - If stack is empty at the end, parentheses are balanced!
 */
bool isBalancedParentheses(string expr) {
    SimpleStack s;
    initStack(s);

    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') {
            push(s, 1);
        } else if (expr[i] == ')') {
            if (isStackEmpty(s)) return false; // Closing without opening
            pop(s);
        }
    }
    return isStackEmpty(s); // Must be empty for balanced string
}

void demoStack() {
    cout << "\n=======================================================\n";
    cout << "           MODULE VIII: STACK ADT DEMONSTRATION        \n";
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

// ====================================================================================================
// SECTION 6 (MODULE IX): QUEUE ADT (LINEAR & CIRCULAR QUEUE)
// ====================================================================================================

/**
 * 1. Linear Queue Implementation (FIFO - First In First Out)
 */
const int MAX_QUEUE = 50;

struct LinearQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
};

void initQueue(LinearQueue& q) {
    q.front = 0;
    q.rear = -1;
}

bool isQueueEmpty(const LinearQueue& q) {
    return (q.rear < q.front);
}

void enqueue(LinearQueue& q, int tokenNo) {
    if (q.rear == MAX_QUEUE - 1) {
        cout << " [ERROR] Queue is Full!\n";
        return;
    }
    q.rear++;
    q.arr[q.rear] = tokenNo;
}

int dequeue(LinearQueue& q) {
    if (isQueueEmpty(q)) {
        cout << " [ERROR] Queue is Empty!\n";
        return -1;
    }
    int val = q.arr[q.front];
    q.front++;
    return val;
}

/**
 * 2. Circular Queue (Modulo wraparound to prevent memory waste)
 */
struct CircularQueue {
    int arr[MAX_QUEUE];
    int front;
    int rear;
    int count;
};

void initCircularQueue(CircularQueue& cq) {
    cq.front = 0;
    cq.rear = -1;
    cq.count = 0;
}

void circularEnqueue(CircularQueue& cq, int tokenNo) {
    if (cq.count == MAX_QUEUE) {
        cout << " [ERROR] Circular Queue is Full!\n";
        return;
    }
    cq.rear = (cq.rear + 1) % MAX_QUEUE; // Wrap around using modulo %
    cq.arr[cq.rear] = tokenNo;
    cq.count++;
}

int circularDequeue(CircularQueue& cq) {
    if (cq.count == 0) {
        cout << " [ERROR] Circular Queue is Empty!\n";
        return -1;
    }
    int val = cq.arr[cq.front];
    cq.front = (cq.front + 1) % MAX_QUEUE; // Wrap around using modulo %
    cq.count--;
    return val;
}

void demoQueue() {
    cout << "\n=======================================================\n";
    cout << "           MODULE IX: QUEUE ADT DEMONSTRATION          \n";
    cout << "=======================================================\n";

    cout << " 1. Linear Queue for Bank Teller Counter (FIFO):\n";
    LinearQueue lq;
    initQueue(lq);
    enqueue(lq, 101);
    enqueue(lq, 102);
    enqueue(lq, 103);
    cout << "    Enqueued Token #101, #102, #103\n";
    cout << "    Servicing Customers:\n";
    while (!isQueueEmpty(lq)) {
        cout << "    Serving Token #" << dequeue(lq) << "\n";
    }

    cout << "\n 2. Circular Queue with Modulo Wraparound:\n";
    CircularQueue cq;
    initCircularQueue(cq);
    circularEnqueue(cq, 201);
    circularEnqueue(cq, 202);
    cout << "    Enqueued Token #201, #202 in Circular Queue\n";
    cout << "    Dequeued Token #" << circularDequeue(cq) << "\n";
}

// ====================================================================================================
// SECTION 7 (MODULE X): STL CONTAINERS DEMONSTRATION
// ====================================================================================================

void demoSTLContainers() {
    cout << "\n=======================================================\n";
    cout << "       MODULE X: STL CONTAINERS & ITERATORS DEMO       \n";
    cout << "=======================================================\n";

    // 1. std::vector & std::pair
    cout << "\n 1. std::vector & std::pair (AccountNo, Balance):\n";
    vector<pair<int, double>> accountPairs;
    for (int i = 0; i < customerList.size(); i++) {
        accountPairs.push_back(make_pair(customerList[i].accountNo, customerList[i].balance));
    }
    for (int i = 0; i < accountPairs.size(); i++) {
        cout << "    Account #" << accountPairs[i].first << " -> Rs. " << accountPairs[i].second << "\n";
    }

    // 2. std::deque (Double ended queue)
    cout << "\n 2. std::deque (VIP Front Push & Regular Back Push):\n";
    deque<string> tellerLine;
    tellerLine.push_back("Regular Customer #1");
    tellerLine.push_back("Regular Customer #2");
    tellerLine.push_front("VIP Customer (Emergency)");
    cout << "    Front of Deque: " << tellerLine.front() << "\n";
    cout << "    Back of Deque:  " << tellerLine.back() << "\n";

    // 3. std::set (Unique elements)
    cout << "\n 3. std::set (Unique registered phone numbers):\n";
    set<string> uniquePhones;
    for (int i = 0; i < customerList.size(); i++) {
        uniquePhones.insert(customerList[i].phone);
    }
    cout << "    Total unique phone numbers in database: " << uniquePhones.size() << "\n";

    // 4. std::map (Fast Key-Value Lookup)
    cout << "\n 4. std::map (AccountNo -> Customer Name Map):\n";
    map<int, string> nameMap;
    for (int i = 0; i < customerList.size(); i++) {
        nameMap[customerList[i].accountNo] = customerList[i].name;
    }
    map<int, string>::iterator it = nameMap.find(1001);
    if (it != nameMap.end()) {
        cout << "    Map found Account #1001: " << it->second << "\n";
    }
}

// ====================================================================================================
// SECTION 8: BANK MANAGEMENT OPERATIONS (ADD, DEPOSIT, WITHDRAW, SEARCH, DISPLAY)
// ====================================================================================================

// Seeds initial demo customer records
void seedInitialData() {
    customerList.clear();
    transactionList.clear();

    Customer c1 = { 1001, "Aarav Sharma", "9876543210", "SAVINGS", 25000.50, {1, 1, 2026}, {"Hyderabad", "Telangana", "500001"} };
    Customer c2 = { 1002, "Priya Patel",  "9823456789", "SAVINGS", 1000.00,  {1, 1, 2026}, {"Hyderabad", "Telangana", "500002"} };
    Customer c3 = { 1003, "Rohan Verma",  "9712345678", "CURRENT", 75000.00, {1, 1, 2026}, {"Hyderabad", "Telangana", "500003"} };
    Customer c4 = { 1004, "Ananya Iyer",  "9601234567", "SAVINGS", 650.75,   {1, 1, 2026}, {"Hyderabad", "Telangana", "500034"} };
    Customer c5 = { 1005, "Vikram Malhotra","9543210987","CURRENT",150000.00,{1, 1, 2026}, {"Hyderabad", "Telangana", "500033"} };
    Customer c6 = { 1006, "Sanya Gupta",  "9432109876", "SAVINGS", 500.00,   {1, 1, 2026}, {"Hyderabad", "Telangana", "500081"} };

    customerList.push_back(c1);
    customerList.push_back(c2);
    customerList.push_back(c3);
    customerList.push_back(c4);
    customerList.push_back(c5);
    customerList.push_back(c6);

    for (int i = 0; i < customerList.size(); i++) {
        Transaction t = { i + 1, customerList[i].accountNo, "OPENING", customerList[i].balance, customerList[i].balance };
        transactionList.push_back(t);
    }
}

// Function to print tabular customer details
void displayAllCustomers() {
    if (customerList.empty()) {
        cout << " [!] No customer records found.\n";
        return;
    }

    cout << "\n--------------------------------------------------------------------------------------\n";
    cout << left 
         << setw(12) << "Acc No"
         << setw(20) << "Customer Name"
         << setw(15) << "Phone Number"
         << setw(12) << "Type"
         << setw(15) << "Balance (Rs.)"
         << setw(12) << "City" << "\n";
    cout << "--------------------------------------------------------------------------------------\n";

    for (int i = 0; i < customerList.size(); i++) {
        cout << left 
             << setw(12) << customerList[i].accountNo
             << setw(20) << customerList[i].name
             << setw(15) << customerList[i].phone
             << setw(12) << customerList[i].accountType
             << right << setw(13) << fixed << setprecision(2) << customerList[i].balance << "  "
             << left << setw(12) << customerList[i].address.city << "\n";
    }
    cout << "--------------------------------------------------------------------------------------\n";
    cout << " Total records: " << customerList.size() << "\n";
}

// Add New Customer
void addNewCustomer() {
    cout << "\n--- ADD NEW CUSTOMER ACCOUNT ---\n";
    Customer newCust;
    newCust.accountNo = nextAccountNo++;

    cout << " Enter Customer Name: ";
    cin.ignore();
    getline(cin, newCust.name);

    cout << " Enter 10-digit Phone Number: ";
    cin >> newCust.phone;

    // Check duplicate phone via Linear Search
    if (linearSearchByPhone(customerList, newCust.phone) != -1) {
        cout << " [ERROR] Phone number already registered to another account!\n";
        return;
    }

    cout << " Select Account Type (1 = SAVINGS, 2 = CURRENT): ";
    int typeChoice;
    cin >> typeChoice;
    newCust.accountType = (typeChoice == 1) ? "SAVINGS" : "CURRENT";

    cout << " Enter Initial Deposit Amount (Rs.): ";
    cin >> newCust.balance;

    if (newCust.accountType == "SAVINGS" && newCust.balance < 500.0) {
        cout << " [ERROR] SAVINGS account requires a minimum initial deposit of Rs. 500.00!\n";
        return;
    }

    newCust.openedDate = { 3, 10, 2026 };
    newCust.address = { "Hyderabad", "Telangana", "500001" };

    // Insert into customer list
    customerList.push_back(newCust);

    // Keep vector sorted by account number so Binary Search works properly!
    sort(customerList.begin(), customerList.end(), [](const Customer& a, const Customer& b) {
        return a.accountNo < b.accountNo;
    });

    // Record Opening Transaction
    Transaction t = { nextTxnId++, newCust.accountNo, "OPENING", newCust.balance, newCust.balance };
    transactionList.push_back(t);

    cout << " [OK] Account created successfully! Allocated Account Number: " << newCust.accountNo << "\n";
}

// Deposit Money
void depositFunds() {
    cout << "\n--- DEPOSIT FUNDS ---\n";
    int accNo;
    double amount;

    cout << " Enter Account Number: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx == -1) {
        cout << " [ERROR] Account number " << accNo << " not found!\n";
        return;
    }

    cout << " Enter Amount to Deposit (Rs.): ";
    cin >> amount;

    if (amount <= 0) {
        cout << " [ERROR] Deposit amount must be greater than 0!\n";
        return;
    }

    customerList[idx].balance += amount;

    // Log transaction
    Transaction t = { nextTxnId++, accNo, "DEPOSIT", amount, customerList[idx].balance };
    transactionList.push_back(t);

    cout << " [OK] Deposit successful! Updated Balance: Rs. " << fixed << setprecision(2) << customerList[idx].balance << "\n";
}

// Withdraw Money
void withdrawFunds() {
    cout << "\n--- WITHDRAW FUNDS ---\n";
    int accNo;
    double amount;

    cout << " Enter Account Number: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx == -1) {
        cout << " [ERROR] Account number " << accNo << " not found!\n";
        return;
    }

    cout << " Enter Amount to Withdraw (Rs.): ";
    cin >> amount;

    if (amount <= 0) {
        cout << " [ERROR] Withdrawal amount must be greater than 0!\n";
        return;
    }

    if (amount > customerList[idx].balance) {
        cout << " [ERROR] Insufficient balance! Current Balance: Rs. " << customerList[idx].balance << "\n";
        return;
    }

    double newBal = customerList[idx].balance - amount;
    if (customerList[idx].accountType == "SAVINGS" && newBal < 500.0) {
        cout << " [ERROR] Withdrawal rejected: SAVINGS account balance cannot fall below Rs. 500.00!\n";
        return;
    }

    customerList[idx].balance = newBal;

    // Log transaction
    Transaction t = { nextTxnId++, accNo, "WITHDRAW", amount, newBal };
    transactionList.push_back(t);

    cout << " [OK] Withdrawal successful! Updated Balance: Rs. " << fixed << setprecision(2) << newBal << "\n";
}

// Search Customer by Account Number (Binary Search)
void searchByAccountNo() {
    cout << "\n--- SEARCH CUSTOMER (BINARY SEARCH O(log n)) ---\n";
    int accNo;
    cout << " Enter Account Number: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx != -1) {
        cout << "\n [FOUND] Customer Details:\n";
        cout << " Account No:   " << customerList[idx].accountNo << "\n";
        cout << " Name:         " << customerList[idx].name << "\n";
        cout << " Phone:        " << customerList[idx].phone << "\n";
        cout << " Account Type: " << customerList[idx].accountType << "\n";
        cout << " Balance:      Rs. " << fixed << setprecision(2) << customerList[idx].balance << "\n";
    } else {
        cout << " [!] Account number " << accNo << " not found.\n";
    }
}

// Search Customer by Phone Number (Linear Search)
void searchByPhone() {
    cout << "\n--- SEARCH CUSTOMER (LINEAR SEARCH O(n)) ---\n";
    string phone;
    cout << " Enter 10-digit Phone Number: ";
    cin >> phone;

    int idx = linearSearchByPhone(customerList, phone);
    if (idx != -1) {
        cout << "\n [FOUND] Customer Details:\n";
        cout << " Account No:   " << customerList[idx].accountNo << "\n";
        cout << " Name:         " << customerList[idx].name << "\n";
        cout << " Phone:        " << customerList[idx].phone << "\n";
        cout << " Balance:      Rs. " << fixed << setprecision(2) << customerList[idx].balance << "\n";
    } else {
        cout << " [!] No customer found with phone: " << phone << "\n";
    }
}

// Sort Customers using Insertion Sort
void sortCustomers() {
    cout << "\n--- SORT CUSTOMERS BY BALANCE (INSERTION SORT O(n^2)) ---\n";
    cout << " 1. Ascending Order (Lowest to Highest Balance)\n";
    cout << " 2. Descending Order (Highest to Lowest Balance)\n";
    cout << " Enter choice (1 or 2): ";
    int choice;
    cin >> choice;

    bool asc = (choice == 1);
    insertionSortByBalance(customerList, asc);

    cout << " [OK] Customer records successfully sorted by balance.\n";
    displayAllCustomers();
}

// View Transaction History
void viewTransactionHistory() {
    cout << "\n--- AUDIT TRANSACTION HISTORY ---\n";
    int accNo;
    cout << " Enter Account Number: ";
    cin >> accNo;

    cout << "\n-------------------------------------------------------------------\n";
    cout << left 
         << setw(10) << "Txn ID"
         << setw(12) << "Acc No"
         << setw(12) << "Type"
         << setw(16) << "Amount (Rs.)"
         << setw(18) << "Balance After (Rs.)" << "\n";
    cout << "-------------------------------------------------------------------\n";

    int foundCount = 0;
    for (int i = 0; i < transactionList.size(); i++) {
        if (transactionList[i].accountNo == accNo) {
            foundCount++;
            cout << left 
                 << setw(10) << transactionList[i].txnId
                 << setw(12) << transactionList[i].accountNo
                 << setw(12) << transactionList[i].type
                 << right << setw(14) << fixed << setprecision(2) << transactionList[i].amount << "  "
                 << right << setw(16) << fixed << setprecision(2) << transactionList[i].balanceAfter << "\n";
        }
    }
    cout << "-------------------------------------------------------------------\n";
    if (foundCount == 0) {
        cout << " No transaction history found for Account #" << accNo << "\n";
    }
}

// ====================================================================================================
// SECTION 9: MAIN PROGRAM & INTERACTIVE MENU (MODULE I & II)
// ====================================================================================================

void printMenu() {
    cout << "\n===============================================================\n";
    cout << "      CS207 BANK MANAGEMENT SYSTEM - MAIN CONSOLE MENU         \n";
    cout << "===============================================================\n";
    cout << "  1. Add New Customer Account                                  \n";
    cout << "  2. Display All Customer Accounts                             \n";
    cout << "  3. Deposit Money                                             \n";
    cout << "  4. Withdraw Money                                            \n";
    cout << "  5. Search Customer by Account No (Binary Search O(log n))    \n";
    cout << "  6. Search Customer by Phone Number (Linear Search O(n))      \n";
    cout << "  7. Sort Customers by Balance (Insertion Sort O(n^2))         \n";
    cout << "  8. View Transaction Audit History                            \n";
    cout << "  9. Demo Module IV: 2D Array & Matrix Operations              \n";
    cout << " 10. Demo Module V: String Manipulation & Frequency Analysis   \n";
    cout << " 11. Demo Module VIII: Stack ADT & Parenthesis Checking        \n";
    cout << " 12. Demo Module IX: Queue ADT (Linear & Circular Queues)      \n";
    cout << " 13. Demo Module X: STL Containers (vector, deque, set, map)   \n";
    cout << " 14. Exit                                                      \n";
    cout << "===============================================================\n";
    cout << " Enter your choice (1-14): ";
}

int main() {
    seedInitialData(); // Load initial customer demo records into memory

    cout << "===============================================================\n";
    cout << "     WELCOME TO CS207 DATA STRUCTURES BANK MANAGEMENT SYSTEM   \n";
    cout << "===============================================================\n";

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            string ignoreStr;
            cin >> ignoreStr;
            cout << " [ERROR] Invalid input! Please enter a number between 1 and 14.\n";
            continue;
        }

        switch (choice) {
            case 1:  addNewCustomer(); break;
            case 2:  displayAllCustomers(); break;
            case 3:  depositFunds(); break;
            case 4:  withdrawFunds(); break;
            case 5:  searchByAccountNo(); break;
            case 6:  searchByPhone(); break;
            case 7:  sortCustomers(); break;
            case 8:  viewTransactionHistory(); break;
            case 9:  demoMatrixOperations(); break;
            case 10: demoStringOperations(); break;
            case 11: demoStack(); break;
            case 12: demoQueue(); break;
            case 13: demoSTLContainers(); break;
            case 14:
                cout << "\n Thank you for using Bank Management System. Goodbye!\n";
                return 0;
            default:
                cout << " [ERROR] Invalid option selected. Please choose between 1 and 14.\n";
                break;
        }
    }
    return 0;
}
