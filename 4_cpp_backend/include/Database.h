#ifndef DATABASE_H
#define DATABASE_H

#include "Customer.h"
#include <vector>
#include <string>
using namespace std;

// Database Return Code Constants (Plain integers per syllabus whitelist, no enum class)
const int DB_OK = 0;
const int DB_NOT_FOUND = 1;
const int DB_INSUFFICIENT = 2;
const int DB_DUPLICATE_PHONE = 3;
const int DB_ERROR = 4;
const int DB_SAVINGS_MIN_BREACH = 5;

// Function prototypes for libpq PostgreSQL database operations
bool dbConnect(string connStr);
void dbDisconnect();

int dbLoadCustomers(vector<Customer>& customersOut);
int dbInsertCustomer(const Customer& cust, long long initialDepositPaise, int& generatedAccNo, string& dbDetailOut);
int dbDeposit(int accountNo, long long amountPaise, long long& newBalancePaiseOut, string& dbDetailOut);
int dbWithdraw(int accountNo, long long amountPaise, long long& newBalancePaiseOut, string& dbDetailOut);
int dbUpdateCustomer(int accountNo, const string& name, const string& phone, const string& email, string& dbDetailOut);
int dbDeleteCustomer(int accountNo, string& dbDetailOut);
int dbLoadTransactions(int accountNo, vector<Transaction>& txnsOut, string& dbDetailOut);

#endif // DATABASE_H
