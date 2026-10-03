

#include <algorithm>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   PART 1 / MEMBER 1: ACCOUNT MANAGEMENT & DATA STRUCTURES (MODULES I, II, VI)
// ====================================================================================================
// ====================================================================================================

/**
 * 1. Date Structure
 * Explanation: Stores calendar date (day, month, year).
 * Concept: Nested structure inside Customer struct.
 */
struct Date {
  int day;   // 1 - 31
  int month; // 1 - 12
  int year;  // e.g. 2026
};

/**
 * 2. Address Structure
 * Explanation: Stores customer residential address.
 * Concept: Nested structure inside Customer struct.
 */
struct Address {
  string city;    // City name
  string state;   // State name
  string pincode; // 6-digit PIN code
};

/**
 * 3. Customer Structure
 * Explanation: Main entity representing a customer bank account.
 * Concept: Composite structure combining primitive types (int, string, double)
 *          and custom nested structures (Date, Address).
 */
struct Customer {
  int accountNo;      // Unique account number (e.g. 1001, 1002)
  string name;        // Customer full name
  string phone;       // 10-digit registered phone number
  string accountType; // "SAVINGS" or "CURRENT"
  double balance;     // Account balance in Rupees
  Date openedDate;    // Nested Date struct
  Address address;    // Nested Address struct
};

/**
 * 4. Transaction Structure
 * Explanation: Stores history log of each deposit or withdrawal.
 */
struct Transaction {
  int txnId;           // Unique transaction ID
  int accountNo;       // Related customer account number
  string type;         // "OPENING", "DEPOSIT", "WITHDRAW"
  double amount;       // Transaction amount in Rupees
  double balanceAfter; // Updated balance after transaction
};

// Global in-memory storage
vector<Customer> customerList;
vector<Transaction> transactionList;
int nextAccountNo = 1007; // Auto-increment counter for new account numbers
int nextTxnId = 7;        // Auto-increment counter for new transactions

// Forward declaration of Member 2's linear search function needed by Member 1
int linearSearchByPhone(const vector<Customer> &arr, string targetPhone);

/**
 * Member 1 Function: Seed Initial Demo Data
 * Explanation: Pre-loads 6 demo customer records into memory.
 */
void seedInitialData() {
  customerList.clear();
  transactionList.clear();

  Customer c1 = {1001,
                 "Aarav Sharma",
                 "9876543210",
                 "SAVINGS",
                 25000.50,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500001"}};
  Customer c2 = {1002,
                 "Priya Patel",
                 "9823456789",
                 "SAVINGS",
                 1000.00,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500002"}};
  Customer c3 = {1003,
                 "Rohan Verma",
                 "9712345678",
                 "CURRENT",
                 75000.00,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500003"}};
  Customer c4 = {1004,
                 "Ananya Iyer",
                 "9601234567",
                 "SAVINGS",
                 650.75,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500034"}};
  Customer c5 = {1005,
                 "Vikram Malhotra",
                 "9543210987",
                 "CURRENT",
                 150000.00,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500033"}};
  Customer c6 = {1006,
                 "Sanya Gupta",
                 "9432109876",
                 "SAVINGS",
                 500.00,
                 {1, 1, 2026},
                 {"Hyderabad", "Telangana", "500081"}};

  customerList.push_back(c1);
  customerList.push_back(c2);
  customerList.push_back(c3);
  customerList.push_back(c4);
  customerList.push_back(c5);
  customerList.push_back(c6);

  for (int i = 0; i < customerList.size(); i++) {
    Transaction t = {i + 1, customerList[i].accountNo, "OPENING",
                     customerList[i].balance, customerList[i].balance};
    transactionList.push_back(t);
  }
}

/**
 * Member 1 Function: Display All Customers in Tabular Format
 * Explanation: Iterates through customerList and formats output using setw().
 */
void displayAllCustomers() {
  if (customerList.empty()) {
    cout << " [!] No customer records found.\n";
    return;
  }

  cout << "\n------------------------------------------------------------------"
          "--------------------\n";
  cout << left << setw(12) << "Acc No" << setw(20) << "Customer Name"
       << setw(15) << "Phone Number" << setw(12) << "Type" << setw(15)
       << "Balance (Rs.)" << setw(12) << "City" << "\n";
  cout << "--------------------------------------------------------------------"
          "------------------\n";

  for (int i = 0; i < customerList.size(); i++) {
    cout << left << setw(12) << customerList[i].accountNo << setw(20)
         << customerList[i].name << setw(15) << customerList[i].phone
         << setw(12) << customerList[i].accountType << right << setw(13)
         << fixed << setprecision(2) << customerList[i].balance << "  " << left
         << setw(12) << customerList[i].address.city << "\n";
  }
  cout << "--------------------------------------------------------------------"
          "------------------\n";
  cout << " Total records: " << customerList.size() << "\n";
}

/**
 * Member 1 Function: Add New Customer Account
 * Explanation: Takes inputs, validates minimum deposit (Rs. 500 for Savings),
 *              checks duplicate phone numbers, and adds to database.
 */
void addNewCustomer() {
  cout << "\n--- [MEMBER 1] ADD NEW CUSTOMER ACCOUNT ---\n";
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
    cout << " [ERROR] SAVINGS account requires a minimum initial deposit of "
            "Rs. 500.00!\n";
    return;
  }

  newCust.openedDate = {3, 10, 2026};
  newCust.address = {"Hyderabad", "Telangana", "500001"};

  // Insert into customer list
  customerList.push_back(newCust);

  // Keep sorted by account number for Binary Search
  sort(customerList.begin(), customerList.end(),
       [](const Customer &a, const Customer &b) {
         return a.accountNo < b.accountNo;
       });

  // Record Opening Transaction Log
  Transaction t = {nextTxnId++, newCust.accountNo, "OPENING", newCust.balance,
                   newCust.balance};
  transactionList.push_back(t);

  cout << " [OK] Account created successfully! Allocated Account Number: "
       << newCust.accountNo << "\n";
}

// ====================================================================================================
// ====================================================================================================
//   PART 2 / MEMBER 2: FINANCIAL TRANSACTIONS & SEARCH ALGORITHMS (MODULES II,
//   III, VII)
// ====================================================================================================
// ====================================================================================================

/**
 * Member 2 Function: Linear Search Algorithm (Unsorted search by phone number)
 * ----------------------------------------------------------------------------
 * Explanation:
 *   - Starts from index 0 and compares phone numbers sequentially.
 *   - Time Complexity:  O(n) [Module VII]
 *   - Space Complexity: O(1)
 */
int linearSearchByPhone(const vector<Customer> &arr, string targetPhone) {
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i].phone == targetPhone) {
      return i; // Found at index i
    }
  }
  return -1; // Not found
}

/**
 * Member 2 Function: Binary Search Algorithm (Sorted search by Account Number)
 * ----------------------------------------------------------------------------
 * Explanation:
 *   - Precondition: Array MUST be sorted by accountNo.
 *   - Divides search range in half each step using mid = (low + high) / 2.
 *   - Time Complexity:  O(log n) [Module VII]
 *   - Space Complexity: O(1)
 */
int binarySearchByAccountNo(const vector<Customer> &arr, int targetAccNo) {
  int low = 0;
  int high = arr.size() - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (arr[mid].accountNo == targetAccNo) {
      return mid; // Found
    } else if (arr[mid].accountNo < targetAccNo) {
      low = mid + 1; // Search right half
    } else {
      high = mid - 1; // Search left half
    }
  }
  return -1; // Not found
}

/**
 * Member 2 Function: Deposit Funds
 * Explanation: Finds account via Binary Search, increases balance, and creates
 * an audit transaction log.
 */
void depositFunds() {
  cout << "\n--- [MEMBER 2] DEPOSIT FUNDS ---\n";
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
  Transaction t = {nextTxnId++, accNo, "DEPOSIT", amount,
                   customerList[idx].balance};
  transactionList.push_back(t);

  cout << " [OK] Deposit successful! Updated Balance: Rs. " << fixed
       << setprecision(2) << customerList[idx].balance << "\n";
}

/**
 * Member 2 Function: Withdraw Funds
 * Explanation: Validates sufficient balance and minimum Rs. 500 limit for
 * SAVINGS accounts.
 */
void withdrawFunds() {
  cout << "\n--- [MEMBER 2] WITHDRAW FUNDS ---\n";
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
    cout << " [ERROR] Insufficient balance! Current Balance: Rs. "
         << customerList[idx].balance << "\n";
    return;
  }

  double newBal = customerList[idx].balance - amount;
  if (customerList[idx].accountType == "SAVINGS" && newBal < 500.0) {
    cout << " [ERROR] Withdrawal rejected: SAVINGS balance cannot fall below "
            "Rs. 500.00!\n";
    return;
  }

  customerList[idx].balance = newBal;

  // Log transaction
  Transaction t = {nextTxnId++, accNo, "WITHDRAW", amount, newBal};
  transactionList.push_back(t);

  cout << " [OK] Withdrawal successful! Updated Balance: Rs. " << fixed
       << setprecision(2) << newBal << "\n";
}

/**
 * Member 2 Function: Search by Account Number UI
 */
void searchByAccountNo() {
  cout
      << "\n--- [MEMBER 2] SEARCH BY ACCOUNT NO (BINARY SEARCH O(log n)) ---\n";
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
    cout << " Balance:      Rs. " << fixed << setprecision(2)
         << customerList[idx].balance << "\n";
  } else {
    cout << " [!] Account number " << accNo << " not found.\n";
  }
}

/**
 * Member 2 Function: Search by Phone Number UI
 */
void searchByPhone() {
  cout << "\n--- [MEMBER 2] SEARCH BY PHONE NO (LINEAR SEARCH O(n)) ---\n";
  string phone;
  cout << " Enter 10-digit Phone Number: ";
  cin >> phone;

  int idx = linearSearchByPhone(customerList, phone);
  if (idx != -1) {
    cout << "\n [FOUND] Customer Details:\n";
    cout << " Account No:   " << customerList[idx].accountNo << "\n";
    cout << " Name:         " << customerList[idx].name << "\n";
    cout << " Phone:        " << customerList[idx].phone << "\n";
    cout << " Balance:      Rs. " << fixed << setprecision(2)
         << customerList[idx].balance << "\n";
  } else {
    cout << " [!] No customer found with phone: " << phone << "\n";
  }
}

/**
 * Member 2 Function: View Transaction Audit History
 */
void viewTransactionHistory() {
  cout << "\n--- [MEMBER 2] AUDIT TRANSACTION HISTORY ---\n";
  int accNo;
  cout << " Enter Account Number: ";
  cin >> accNo;

  cout << "\n------------------------------------------------------------------"
          "-\n";
  cout << left << setw(10) << "Txn ID" << setw(12) << "Acc No" << setw(12)
       << "Type" << setw(16) << "Amount (Rs.)" << setw(18)
       << "Balance After (Rs.)" << "\n";
  cout << "-------------------------------------------------------------------"
          "\n";

  int foundCount = 0;
  for (int i = 0; i < transactionList.size(); i++) {
    if (transactionList[i].accountNo == accNo) {
      foundCount++;
      cout << left << setw(10) << transactionList[i].txnId << setw(12)
           << transactionList[i].accountNo << setw(12)
           << transactionList[i].type << right << setw(14) << fixed
           << setprecision(2) << transactionList[i].amount << "  " << right
           << setw(16) << fixed << setprecision(2)
           << transactionList[i].balanceAfter << "\n";
    }
  }
  cout << "-------------------------------------------------------------------"
          "\n";
  if (foundCount == 0) {
    cout << " No transaction history found for Account #" << accNo << "\n";
  }
}

// ====================================================================================================
// ====================================================================================================
//   PART 3 / MEMBER 3: SORTING ALGORITHMS & 2D MATRIX OPERATIONS (MODULES III,
//   IV, VII)
// ====================================================================================================
// ====================================================================================================

/**
 * Member 3 Function: Insertion Sort Algorithm (Sorting by Balance)
 * ----------------------------------------------------------------
 * Explanation:
 *   - Inserts elements one-by-one into their sorted position by shifting larger
 * elements.
 *   - Time Complexity:  O(n^2) worst case, O(n) best case [Module VII]
 *   - Space Complexity: O(1) in-place sort
 */
void insertionSortByBalance(vector<Customer> &arr, bool ascending) {
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    Customer key = arr[i];
    int j = i - 1;

    if (ascending) {
      // Shift elements with larger balance to the right
      while (j >= 0 && arr[j].balance > key.balance) {
        arr[j + 1] = arr[j];
        j = j - 1;
      }
    } else {
      // Shift elements with smaller balance to the right (Descending)
      while (j >= 0 && arr[j].balance < key.balance) {
        arr[j + 1] = arr[j];
        j = j - 1;
      }
    }
    arr[j + 1] = key;
  }
}

/**
 * Member 3 Function: Sort Customers Menu Handler
 */
void sortCustomers() {
  cout << "\n--- [MEMBER 3] SORT CUSTOMERS BY BALANCE (INSERTION SORT O(n^2)) "
          "---\n";
  cout << " 1. Ascending Order (Lowest to Highest Balance)\n";
  cout << " 2. Descending Order (Highest to Lowest Balance)\n";
  cout << " Enter choice (1 or 2): ";
  int choice;
  cin >> choice;

  bool asc = (choice == 1);
  insertionSortByBalance(customerList, asc);

  cout << " [OK] Customer records successfully sorted.\n";
  displayAllCustomers();
}

/**
 * Member 3 Function: Helper to print 2x2 Matrix
 */
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

/**
 * Member 3 Function: 2D Numeric Array Matrix Operations (Module IV)
 * Explanation:
 *   1. Matrix Addition: C[i][j] = A[i][j] + B[i][j]
 *   2. Matrix Subtraction: Diff[i][j] = A[i][j] - B[i][j]
 *   3. Matrix Multiplication: Mult[i][j] = Sum(A[i][k] * B[k][j])
 *   4. Matrix Transpose: Trans[j][i] = A[i][j] (Swap rows & columns)
 */
void demoMatrixOperations() {
  cout << "\n=======================================================\n";
  cout << "  [MEMBER 3] MODULE IV: 2D ARRAY & MATRIX DEMONSTRATION \n";
  cout << "=======================================================\n";

  int A[2][2] = {{1000, 2000}, {1500, 2500}};
  int B[2][2] = {{500, 1200}, {800, 1400}};

  print2DMatrix("Branch A Cash Flow", A);
  print2DMatrix("Branch B Cash Flow", B);

  // 1. Matrix Addition
  int Sum[2][2];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      Sum[i][j] = A[i][j] + B[i][j];
    }
  }
  print2DMatrix("1. Matrix Addition (A + B)", Sum);

  // 2. Matrix Subtraction
  int Diff[2][2];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      Diff[i][j] = A[i][j] - B[i][j];
    }
  }
  print2DMatrix("2. Matrix Subtraction (A - B)", Diff);

  // 3. Matrix Multiplication
  int Mult[2][2] = {{0, 0}, {0, 0}};
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        Mult[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  print2DMatrix("3. Matrix Multiplication (A * B)", Mult);

  // 4. Matrix Transpose
  int Trans[2][2];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      Trans[j][i] = A[i][j];
    }
  }
  print2DMatrix("4. Matrix Transpose of A (Rows <-> Columns)", Trans);
}

// ====================================================================================================
// ====================================================================================================
//   PART 4 / MEMBER 4: STRING ALGORITHMS & STACK ADT (MODULES V, VIII)
// ====================================================================================================
// ====================================================================================================

/**
 * Member 4 Function: In-place String Reversal
 * Explanation: Swaps str[i] with str[n - 1 - i] moving inwards.
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
 * Member 4 Function: Character Frequency Counter
 * Explanation: Uses 256-bucket ASCII array to count each character.
 */
void countCharacterFrequency(string str) {
  int count[256] = {0};

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
 * Member 4 Function: Naive Pattern Search (Substring Matching)
 */
bool searchPattern(string text, string pattern) {
  int n = text.length();
  int m = pattern.length();
  if (m > n)
    return false;

  for (int i = 0; i <= n - m; i++) {
    int j = 0;
    while (j < m && text[i + j] == pattern[j]) {
      j++;
    }
    if (j == m)
      return true;
  }
  return false;
}

/**
 * Member 4 Function: String Manipulation Demo
 */
void demoStringOperations() {
  cout << "\n=======================================================\n";
  cout << "  [MEMBER 4] MODULE V: STRING MANIPULATION DEMO        \n";
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
 * Member 4 Structure: Array-based Stack ADT (LIFO)
 */
const int MAX_STACK = 50;

struct SimpleStack {
  int arr[MAX_STACK];
  int top;
};

void initStack(SimpleStack &s) { s.top = -1; }
bool isStackEmpty(const SimpleStack &s) { return (s.top == -1); }
bool isStackFull(const SimpleStack &s) { return (s.top == MAX_STACK - 1); }

void push(SimpleStack &s, int val) {
  if (isStackFull(s)) {
    cout << " [ERROR] Stack Overflow!\n";
    return;
  }
  s.arr[++s.top] = val;
}

int pop(SimpleStack &s) {
  if (isStackEmpty(s)) {
    cout << " [ERROR] Stack Underflow!\n";
    return -1;
  }
  return s.arr[s.top--];
}

/**
 * Member 4 Function: Stack Application - Parenthesis Matching
 */
bool isBalancedParentheses(string expr) {
  SimpleStack s;
  initStack(s);

  for (int i = 0; i < expr.length(); i++) {
    if (expr[i] == '(') {
      push(s, 1);
    } else if (expr[i] == ')') {
      if (isStackEmpty(s))
        return false;
      pop(s);
    }
  }
  return isStackEmpty(s);
}

/**
 * Member 4 Function: Stack ADT Demo
 */
void demoStack() {
  cout << "\n=======================================================\n";
  cout << "  [MEMBER 4] MODULE VIII: STACK ADT DEMONSTRATION      \n";
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
  cout << "    Expression \"" << expr1
       << "\": " << (isBalancedParentheses(expr1) ? "BALANCED" : "UNBALANCED")
       << "\n";
  cout << "    Expression \"" << expr2
       << "\": " << (isBalancedParentheses(expr2) ? "BALANCED" : "UNBALANCED")
       << "\n";
}

// ====================================================================================================
// ====================================================================================================
//   PART 5 / MEMBER 5: QUEUE ADT, STL CONTAINERS & MAIN PROGRAM MENU (MODULES
//   I, II, IX, X)
// ====================================================================================================
// ====================================================================================================

/**
 * Member 5 Structure: Linear Queue (FIFO)
 */
const int MAX_QUEUE = 50;

struct LinearQueue {
  int arr[MAX_QUEUE];
  int front;
  int rear;
};

void initQueue(LinearQueue &q) {
  q.front = 0;
  q.rear = -1;
}
bool isQueueEmpty(const LinearQueue &q) { return (q.rear < q.front); }

void enqueue(LinearQueue &q, int tokenNo) {
  if (q.rear == MAX_QUEUE - 1) {
    cout << " [ERROR] Linear Queue is Full!\n";
    return;
  }
  q.arr[++q.rear] = tokenNo;
}

int dequeue(LinearQueue &q) {
  if (isQueueEmpty(q)) {
    cout << " [ERROR] Linear Queue is Empty!\n";
    return -1;
  }
  return q.arr[q.front++];
}

/**
 * Member 5 Structure: Circular Queue with Modulo Wraparound
 */
struct CircularQueue {
  int arr[MAX_QUEUE];
  int front;
  int rear;
  int count;
};

void initCircularQueue(CircularQueue &cq) {
  cq.front = 0;
  cq.rear = -1;
  cq.count = 0;
}

void circularEnqueue(CircularQueue &cq, int tokenNo) {
  if (cq.count == MAX_QUEUE) {
    cout << " [ERROR] Circular Queue is Full!\n";
    return;
  }
  cq.rear = (cq.rear + 1) % MAX_QUEUE; // Wrap around using modulo %
  cq.arr[cq.rear] = tokenNo;
  cq.count++;
}

int circularDequeue(CircularQueue &cq) {
  if (cq.count == 0) {
    cout << " [ERROR] Circular Queue is Empty!\n";
    return -1;
  }
  int val = cq.arr[cq.front];
  cq.front = (cq.front + 1) % MAX_QUEUE; // Wrap around using modulo %
  cq.count--;
  return val;
}

/**
 * Member 5 Function: Queue ADT Demo
 */
void demoQueue() {
  cout << "\n=======================================================\n";
  cout << "  [MEMBER 5] MODULE IX: QUEUE ADT DEMONSTRATION        \n";
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

/**
 * Member 5 Function: STL Containers Demonstration (Module X)
 */
void demoSTLContainers() {
  cout << "\n=======================================================\n";
  cout << "  [MEMBER 5] MODULE X: STL CONTAINERS & ITERATORS DEMO \n";
  cout << "=======================================================\n";

  // 1. std::vector & std::pair
  cout << "\n 1. std::vector & std::pair (AccountNo, Balance):\n";
  vector<pair<int, double>> accountPairs;
  for (int i = 0; i < customerList.size(); i++) {
    accountPairs.push_back(
        make_pair(customerList[i].accountNo, customerList[i].balance));
  }
  for (int i = 0; i < accountPairs.size(); i++) {
    cout << "    Account #" << accountPairs[i].first << " -> Rs. "
         << accountPairs[i].second << "\n";
  }

  // 2. std::deque (Double ended queue)
  cout << "\n 2. std::deque (VIP Front Push & Regular Back Push):\n";
  deque<string> tellerLine;
  tellerLine.push_back("Regular Customer #1");
  tellerLine.push_back("Regular Customer #2");
  tellerLine.push_front("VIP Customer (Emergency Priority)");
  cout << "    Front of Deque: " << tellerLine.front() << "\n";
  cout << "    Back of Deque:  " << tellerLine.back() << "\n";

  // 3. std::set (Unique elements)
  cout << "\n 3. std::set (Unique registered phone numbers):\n";
  set<string> uniquePhones;
  for (int i = 0; i < customerList.size(); i++) {
    uniquePhones.insert(customerList[i].phone);
  }
  cout << "    Total unique phone numbers in database: " << uniquePhones.size()
       << "\n";

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

/**
 * Member 5 Function: Main Console Menu Display (Module I & II)
 */
void printMenu() {
  cout << "\n===============================================================\n";
  cout << "      CS207 BANK MANAGEMENT SYSTEM - MAIN CONSOLE MENU         \n";
  cout << "===============================================================\n";
  cout << "  [MEMBER 1 - Account Management]\n";
  cout << "    1. Add New Customer Account                                \n";
  cout << "    2. Display All Customer Accounts                           \n";
  cout << "\n  [MEMBER 2 - Transactions & Searches]\n";
  cout << "    3. Deposit Money                                           \n";
  cout << "    4. Withdraw Money                                          \n";
  cout << "    5. Search Customer by Account No (Binary Search O(log n))  \n";
  cout << "    6. Search Customer by Phone Number (Linear Search O(n))    \n";
  cout << "    7. View Transaction Audit History                          \n";
  cout << "\n  [MEMBER 3 - Sorting & Matrix Math]\n";
  cout << "    8. Sort Customers by Balance (Insertion Sort O(n^2))       \n";
  cout << "    9. Demo Module IV: 2D Array & Matrix Operations            \n";
  cout << "\n  [MEMBER 4 - Strings & Stack ADT]\n";
  cout << "   10. Demo Module V: String Manipulation & Frequency Analysis \n";
  cout << "   11. Demo Module VIII: Stack ADT & Parenthesis Checking      \n";
  cout << "\n  [MEMBER 5 - Queue ADT, STL & Driver Loop]\n";
  cout << "   12. Demo Module IX: Queue ADT (Linear & Circular Queues)    \n";
  cout << "   13. Demo Module X: STL Containers (vector, deque, set, map) \n";
  cout << "   14. Exit                                                    \n";
  cout << "===============================================================\n";
  cout << " Enter your choice (1-14): ";
}

/**
 * Member 5 Function: Main Application Entry Point (Module I & II)
 */
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
      cout << " [ERROR] Invalid input! Please enter a number between 1 and "
              "14.\n";
      continue;
    }

    switch (choice) {
    case 1:
      addNewCustomer();
      break;
    case 2:
      displayAllCustomers();
      break;
    case 3:
      depositFunds();
      break;
    case 4:
      withdrawFunds();
      break;
    case 5:
      searchByAccountNo();
      break;
    case 6:
      searchByPhone();
      break;
    case 7:
      viewTransactionHistory();
      break;
    case 8:
      sortCustomers();
      break;
    case 9:
      demoMatrixOperations();
      break;
    case 10:
      demoStringOperations();
      break;
    case 11:
      demoStack();
      break;
    case 12:
      demoQueue();
      break;
    case 13:
      demoSTLContainers();
      break;
    case 14:
      cout << "\n Thank you for using Bank Management System. Goodbye!\n";
      return 0;
    default:
      cout << " [ERROR] Invalid option selected. Please choose between 1 and "
              "14.\n";
      break;
    }
  }
  return 0;
}
