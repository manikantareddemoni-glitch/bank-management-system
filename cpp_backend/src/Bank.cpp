#include "Bank.h"
#include <iostream>
using namespace std;

bool bankSync(vector<Customer>& customers) {
    int code = dbLoadCustomers(customers);
    return (code == DB_OK);
}

// =========================================================================
// HAND-ROLLED DSA ALGORITHMS (Written by hand per syllabus requirements)
// =========================================================================

bool linearSearchByPhone(const vector<Customer>& customers, string phone, Customer& resultCust) {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].phone == phone) {
            resultCust = customers[i];
            return true;
        }
    }
    return false;
}

bool binarySearchByAccountNo(const vector<Customer>& customers, int accountNo, Customer& resultCust) {
    int low = 0;
    int high = static_cast<int>(customers.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (customers[mid].accountNo == accountNo) {
            resultCust = customers[mid];
            return true;
        }
        if (customers[mid].accountNo < accountNo) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return false;
}

void insertionSortByBalance(vector<Customer>& customers, bool ascending) {
    int n = static_cast<int>(customers.size());
    for (int i = 1; i < n; ++i) {
        Customer key = customers[i];
        int j = i - 1;

        if (ascending) {
            // Ascending insertion sort
            while (j >= 0 && customers[j].balancePaise > key.balancePaise) {
                customers[j + 1] = customers[j];
                j--;
            }
        } else {
            // Descending insertion sort
            while (j >= 0 && customers[j].balancePaise < key.balancePaise) {
                customers[j + 1] = customers[j];
                j--;
            }
        }
        customers[j + 1] = key;
    }
}

void findHighestBalanceCustomers(const vector<Customer>& customers, vector<Customer>& highestOut) {
    highestOut.clear();
    if (customers.empty()) return;

    long long maxBalance = customers[0].balancePaise;
    for (size_t i = 1; i < customers.size(); ++i) {
        if (customers[i].balancePaise > maxBalance) {
            maxBalance = customers[i].balancePaise;
        }
    }

    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].balancePaise == maxBalance) {
            highestOut.push_back(customers[i]);
        }
    }
}

// =========================================================================
// PROCEDURAL BANK BUSINESS OPERATIONS
// =========================================================================

int bankCreateAccount(vector<Customer>& customers, string name, string phone, string email, string type, long long openingPaise, int& generatedAccNo, string& dbDetailOut) {
    if (type == "SAVINGS" && openingPaise < 50000LL) {
        dbDetailOut = "SAVINGS account requires minimum opening deposit of Rs. 500.00.";
        return DB_SAVINGS_MIN_BREACH;
    }

    Customer c;
    c.name = name;
    c.phone = phone;
    c.email = email;
    c.accountType = type;
    c.balancePaise = openingPaise;

    int code = dbInsertCustomer(c, openingPaise, generatedAccNo, dbDetailOut);
    if (code == DB_OK) {
        bankSync(customers);
    }
    return code;
}

int bankDeposit(vector<Customer>& customers, int accountNo, long long amountPaise, string& dbDetailOut) {
    long long newBal = 0;
    int code = dbDeposit(accountNo, amountPaise, newBal, dbDetailOut);
    if (code == DB_OK) {
        bankSync(customers);
    }
    return code;
}

int bankWithdraw(vector<Customer>& customers, int accountNo, long long amountPaise, string& dbDetailOut) {
    long long newBal = 0;
    int code = dbWithdraw(accountNo, amountPaise, newBal, dbDetailOut);
    if (code == DB_OK) {
        bankSync(customers);
    }
    return code;
}

int bankUpdateCustomer(vector<Customer>& customers, int accountNo, string name, string phone, string email, string& dbDetailOut) {
    int code = dbUpdateCustomer(accountNo, name, phone, email, dbDetailOut);
    if (code == DB_OK) {
        bankSync(customers);
    }
    return code;
}

int bankDeleteAccount(vector<Customer>& customers, int accountNo, string& dbDetailOut) {
    int code = dbDeleteCustomer(accountNo, dbDetailOut);
    if (code == DB_OK) {
        bankSync(customers);
    }
    return code;
}

int bankGetTransactions(int accountNo, vector<Transaction>& txnsOut, string& dbDetailOut) {
    return dbLoadTransactions(accountNo, txnsOut, dbDetailOut);
}
