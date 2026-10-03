#include "bank.h"

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   STUDENT 2: FINANCIAL TRANSACTIONS & SEARCH ALGORITHMS (MODULES II, III, VII)
// ====================================================================================================
// ====================================================================================================
// Responsibilities:
// 1. Linear Search Implementation (O(n) unsorted lookups by phone number)
// 2. Binary Search Implementation (O(log n) sorted divide-and-conquer lookups by account number)
// 3. Deposit transaction processing with ledger journaling
// 4. Withdrawal transaction processing with SAVINGS minimum balance constraint enforcement
// 5. Transaction Audit Ledger display for compliance
// ====================================================================================================

/**
 * Student 2 Function: Linear Search Algorithm (Unsorted search by phone number)
 * ----------------------------------------------------------------------------
 * Explanation:
 *   - Starts from index 0 and compares phone numbers sequentially.
 *   - Time Complexity:  O(n) [Module VII]
 *   - Space Complexity: O(1)
 */
int linearSearchByPhone(const vector<Customer>& arr, string targetPhone) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i].phone == targetPhone) {
            return static_cast<int>(i); // Found at index i
        }
    }
    return -1; // Not found
}

/**
 * Student 2 Function: Binary Search Algorithm (Sorted search by Account Number)
 * ----------------------------------------------------------------------------
 * Explanation:
 *   - Precondition: Array MUST be sorted by accountNo.
 *   - Divides search range in half each step using mid = low + (high - low) / 2.
 *   - Time Complexity:  O(log n) [Module VII]
 *   - Space Complexity: O(1)
 */
int binarySearchByAccountNo(const vector<Customer>& arr, int targetAccNo) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

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
 * Student 2 Function: Deposit Funds
 * Explanation: Finds account via Binary Search, increases balance, and creates an audit transaction log.
 */
void depositFunds() {
    cout << "\n--- [STUDENT 2] DEPOSIT FUNDS ---\n";
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

/**
 * Student 2 Function: Withdraw Funds
 * Explanation: Validates sufficient balance and minimum Rs. 500 limit for SAVINGS accounts.
 */
void withdrawFunds() {
    cout << "\n--- [STUDENT 2] WITHDRAW FUNDS ---\n";
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
        cout << " [ERROR] Withdrawal rejected: SAVINGS balance cannot fall below Rs. 500.00!\n";
        return;
    }

    customerList[idx].balance = newBal;

    // Log transaction
    Transaction t = { nextTxnId++, accNo, "WITHDRAW", amount, newBal };
    transactionList.push_back(t);

    cout << " [OK] Withdrawal successful! Updated Balance: Rs. " << fixed << setprecision(2) << newBal << "\n";
}

/**
 * Student 2 Function: Search by Account Number UI
 */
void searchByAccountNo() {
    cout << "\n--- [STUDENT 2] SEARCH BY ACCOUNT NO (BINARY SEARCH O(log n)) ---\n";
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

/**
 * Student 2 Function: Search by Phone Number UI
 */
void searchByPhone() {
    cout << "\n--- [STUDENT 2] SEARCH BY PHONE NO (LINEAR SEARCH O(n)) ---\n";
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

/**
 * Student 2 Function: View Transaction Audit History
 */
void viewTransactionHistory() {
    cout << "\n--- [STUDENT 2] AUDIT TRANSACTION HISTORY ---\n";
    int accNo;
    cout << " Enter Account Number: ";
    cin >> accNo;

    cout << "\n--------------------------------------------------------------------\n";
    cout << left << setw(10) << "Txn ID" 
         << setw(12) << "Acc No" 
         << setw(12) << "Type" 
         << setw(16) << "Amount (Rs.)" 
         << setw(18) << "Balance After (Rs.)" << "\n";
    cout << "--------------------------------------------------------------------\n";

    int foundCount = 0;
    for (size_t i = 0; i < transactionList.size(); i++) {
        if (transactionList[i].accountNo == accNo) {
            foundCount++;
            cout << left << setw(10) << transactionList[i].txnId
                 << setw(12) << transactionList[i].accountNo
                 << setw(12) << transactionList[i].type
                 << right << setw(14) << fixed << setprecision(2) << transactionList[i].amount << "  "
                 << right << setw(16) << fixed << setprecision(2) << transactionList[i].balanceAfter << "\n";
        }
    }
    cout << "--------------------------------------------------------------------\n";
    if (foundCount == 0) {
        cout << " No transaction history found for Account #" << accNo << "\n";
    }
}
