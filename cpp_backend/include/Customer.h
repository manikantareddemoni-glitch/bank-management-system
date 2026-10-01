#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

/**
 * @brief Plain structure holding customer account data.
 * Module VI: struct (Syllabus whitelisted, no member functions, no constructors)
 * Money rule: Stored in paise as long long (1 Rupee = 100 Paise).
 */
struct Customer {
    int accountNo;
    string name;
    string phone;
    string email;
    string accountType;   // "SAVINGS" or "CURRENT"
    long long balancePaise; // Stored in paise to prevent float rounding errors
    string createdAt;
};

/**
 * @brief Plain structure holding transaction audit log data.
 */
struct Transaction {
    int txnId;
    int accountNo;
    string type;            // "DEPOSIT", "WITHDRAW", "OPENING"
    long long amountPaise;   // Transaction amount in paise
    long long balanceAfterPaise; // Account balance after transaction in paise
    string time;
};

#endif // CUSTOMER_H
