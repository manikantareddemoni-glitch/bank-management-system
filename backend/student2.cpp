#include "bank.h"

// ============================================================================
//   STUDENT 2: FINANCIAL TRANSACTIONS & SEARCH ALGORITHMS
// ============================================================================
// Key Topics Covered:
// 1. Linear Search Algorithm (O(n) - checking each element using a for loop)
// 2. Binary Search Algorithm (O(log n) - divide and conquer using a while loop)
// 3. Conditional Statements (if-else for deposit, withdrawal & minimum balance checks)
// 4. Updating Struct Data & Logging Transactions
// ============================================================================

/**
 * Function: Linear Search by Phone Number
 * Explanation:
 * - Starts from index 0 and goes to the end of the array using a for loop.
 * - Compares each customer's phone with targetPhone.
 * - Returns index if matched, otherwise returns -1.
 */
int linearSearchByPhone(const vector<Customer>& arr, string targetPhone) {
    int total = arr.size();
    for (int i = 0; i < total; i++) {
        if (arr[i].phone == targetPhone) {
            return i; // Found at index i
        }
    }
    return -1; // Not found
}

/**
 * Function: Binary Search by Account Number
 * Explanation:
 * - Requires array to be sorted by account number.
 * - Uses low, high and mid pointers in a while loop.
 * - If target is at mid, returns mid.
 * - If target is greater, search right half (low = mid + 1).
 * - If target is smaller, search left half (high = mid - 1).
 */
int binarySearchByAccountNo(const vector<Customer>& arr, int targetAccNo) {
    int low = 0;
    int high = (int)arr.size() - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

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
 * Function: Deposit Money
 * Explanation:
 * 1. Takes account number and searches using Binary Search.
 * 2. Adds deposit amount to balance.
 * 3. Records a new transaction in transactionList.
 */
void depositFunds() {
    cout << "\n--- [STUDENT 2] DEPOSIT FUNDS ---\n";
    int accNo;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx == -1) {
        cout << "\n[ERROR] Account Number " << accNo << " not found!\n";
        return;
    }

    cout << "Customer Name: " << customerList[idx].name << "\n";
    cout << "Current Balance: Rs. " << customerList[idx].balance << "\n";
    cout << "Enter Amount to Deposit (Rs.): ";
    cin >> amount;

    if (amount <= 0) {
        cout << "\n[ERROR] Deposit amount must be greater than 0!\n";
        return;
    }

    // Add amount to balance
    customerList[idx].balance = customerList[idx].balance + amount;

    // Record transaction
    Transaction t;
    t.txnId = nextTxnId;
    nextTxnId = nextTxnId + 1;
    t.accountNo = accNo;
    t.type = "DEPOSIT";
    t.amount = amount;
    t.balanceAfter = customerList[idx].balance;
    transactionList.push_back(t);

    cout << "\n[SUCCESS] Deposit successful!\n";
    cout << "Updated Balance: Rs. " << customerList[idx].balance << "\n";
}

/**
 * Function: Withdraw Money
 * Explanation:
 * 1. Takes account number and searches using Binary Search.
 * 2. Checks if balance is sufficient.
 * 3. Checks minimum balance limit (Rs. 500 for SAVINGS).
 * 4. Deducts amount and records transaction.
 */
void withdrawFunds() {
    cout << "\n--- [STUDENT 2] WITHDRAW FUNDS ---\n";
    int accNo;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx == -1) {
        cout << "\n[ERROR] Account Number " << accNo << " not found!\n";
        return;
    }

    cout << "Customer Name: " << customerList[idx].name << "\n";
    cout << "Current Balance: Rs. " << customerList[idx].balance << "\n";
    cout << "Enter Amount to Withdraw (Rs.): ";
    cin >> amount;

    if (amount <= 0) {
        cout << "\n[ERROR] Withdrawal amount must be greater than 0!\n";
        return;
    }

    // Check if account has enough money
    if (amount > customerList[idx].balance) {
        cout << "\n[ERROR] Insufficient balance! Available: Rs. " << customerList[idx].balance << "\n";
        return;
    }

    double newBalance = customerList[idx].balance - amount;

    // Minimum balance check for SAVINGS account
    if (customerList[idx].accountType == "SAVINGS" && newBalance < 500.0) {
        cout << "\n[ERROR] Withdrawal rejected: SAVINGS balance cannot fall below Rs. 500!\n";
        return;
    }

    // Deduct amount
    customerList[idx].balance = newBalance;

    // Record transaction
    Transaction t;
    t.txnId = nextTxnId;
    nextTxnId = nextTxnId + 1;
    t.accountNo = accNo;
    t.type = "WITHDRAW";
    t.amount = amount;
    t.balanceAfter = newBalance;
    transactionList.push_back(t);

    cout << "\n[SUCCESS] Withdrawal successful!\n";
    cout << "Remaining Balance: Rs. " << newBalance << "\n";
}

/**
 * Function: Search Customer by Account Number
 * Explanation: Uses Binary Search (O(log n)) to find and display customer details.
 */
void searchByAccountNo() {
    cout << "\n--- [STUDENT 2] SEARCH BY ACCOUNT NUMBER (BINARY SEARCH) ---\n";
    int accNo;
    cout << "Enter Account Number to Search: ";
    cin >> accNo;

    int idx = binarySearchByAccountNo(customerList, accNo);
    if (idx != -1) {
        cout << "\n[FOUND] Customer Account Details:\n";
        cout << "  Account Number : " << customerList[idx].accountNo << "\n";
        cout << "  Customer Name  : " << customerList[idx].name << "\n";
        cout << "  Phone Number   : " << customerList[idx].phone << "\n";
        cout << "  Account Type   : " << customerList[idx].accountType << "\n";
        cout << "  Current Balance: Rs. " << customerList[idx].balance << "\n";
        cout << "  Branch / City  : " << customerList[idx].address.city << "\n";
    } else {
        cout << "\n[!] No account found with Account Number: " << accNo << "\n";
    }
}

/**
 * Function: Search Customer by Phone Number
 * Explanation: Uses Linear Search (O(n)) to find and display customer details.
 */
void searchByPhone() {
    cout << "\n--- [STUDENT 2] SEARCH BY PHONE NUMBER (LINEAR SEARCH) ---\n";
    string phone;
    cout << "Enter 10-digit Phone Number: ";
    cin >> phone;

    int idx = linearSearchByPhone(customerList, phone);
    if (idx != -1) {
        cout << "\n[FOUND] Customer Account Details:\n";
        cout << "  Account Number : " << customerList[idx].accountNo << "\n";
        cout << "  Customer Name  : " << customerList[idx].name << "\n";
        cout << "  Phone Number   : " << customerList[idx].phone << "\n";
        cout << "  Account Type   : " << customerList[idx].accountType << "\n";
        cout << "  Current Balance: Rs. " << customerList[idx].balance << "\n";
    } else {
        cout << "\n[!] No customer found with Phone Number: " << phone << "\n";
    }
}

/**
 * Function: View Transaction History
 * Explanation: Loops through all transactions and displays logs matching the given account.
 */
void viewTransactionHistory() {
    cout << "\n--- [STUDENT 2] VIEW TRANSACTION AUDIT HISTORY ---\n";
    int accNo;
    cout << "Enter Account Number: ";
    cin >> accNo;

    cout << "\n========================================================================\n";
    cout << "                      TRANSACTION LOGS FOR ACCOUNT #" << accNo << "\n";
    cout << "========================================================================\n";

    int count = 0;
    for (int i = 0; i < (int)transactionList.size(); i++) {
        if (transactionList[i].accountNo == accNo) {
            count = count + 1;
            cout << "Txn ID #" << transactionList[i].txnId
                 << " | Type: " << transactionList[i].type
                 << " | Amount: Rs. " << transactionList[i].amount
                 << " | Balance After: Rs. " << transactionList[i].balanceAfter << "\n";
        }
    }
    cout << "========================================================================\n";
    if (count == 0) {
        cout << "No transaction records found for this account.\n";
    } else {
        cout << "Total Transactions: " << count << "\n";
    }
}
