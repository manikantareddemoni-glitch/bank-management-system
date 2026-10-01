#ifndef BANK_H
#define BANK_H

#include "Customer.h"
#include "Database.h"
#include <vector>
#include <string>
using namespace std;

// Synchronize in-memory customer vector with database single source of truth
bool bankSync(vector<Customer>& customers);

// =========================================================================
// HAND-ROLLED DSA ALGORITHMS (Written by hand per syllabus rules)
// =========================================================================

/**
 * @brief Linear search for customer by phone number.
 * Purpose: Locates customer record in an unsorted vector.
 * Precondition: None.
 * Time Complexity: O(n) worst/average case.
 * Space Complexity: O(1) auxiliary space.
 */
bool linearSearchByPhone(const vector<Customer>& customers, string phone, Customer& resultCust);

/**
 * @brief Iterative binary search for customer by account number.
 * Purpose: Fast logarithmic lookup by account number.
 * Precondition: Vector MUST be sorted by accountNo in ascending order.
 * Time Complexity: O(log n) worst/average case.
 * Space Complexity: O(1) auxiliary space.
 */
bool binarySearchByAccountNo(const vector<Customer>& customers, int accountNo, Customer& resultCust);

/**
 * @brief Hand-coded Insertion Sort to order customers by balance.
 * Purpose: Sorts vector by balancePaise in ascending or descending order.
 * Precondition: Vector populated.
 * Time Complexity: O(n^2) worst/average case, O(n) best case.
 * Space Complexity: O(1) auxiliary in-place sorting.
 * Stability: Stable.
 */
void insertionSortByBalance(vector<Customer>& customers, bool ascending);

/**
 * @brief Linear scan to locate customer(s) with maximum balance.
 * Purpose: Identifies highest balance account(s), handling multi-customer ties.
 * Precondition: None.
 * Time Complexity: O(n) single pass scan.
 * Space Complexity: O(k) for tied account records.
 */
void findHighestBalanceCustomers(const vector<Customer>& customers, vector<Customer>& highestOut);

// =========================================================================
// PROCEDURAL BANK BUSINESS OPERATIONS
// =========================================================================
int bankCreateAccount(vector<Customer>& customers, string name, string phone, string email, string type, long long openingPaise, int& generatedAccNo, string& dbDetailOut);
int bankDeposit(vector<Customer>& customers, int accountNo, long long amountPaise, string& dbDetailOut);
int bankWithdraw(vector<Customer>& customers, int accountNo, long long amountPaise, string& dbDetailOut);
int bankUpdateCustomer(vector<Customer>& customers, int accountNo, string name, string phone, string email, string& dbDetailOut);
int bankDeleteAccount(vector<Customer>& customers, int accountNo, string& dbDetailOut);
int bankGetTransactions(int accountNo, vector<Transaction>& txnsOut, string& dbDetailOut);

#endif // BANK_H
