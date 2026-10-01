#include "Database.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <libpq-fe.h>
using namespace std;

// Encapsulated static connection pointer (Hidden inside Database.cpp per architecture)
static PGconn* conn = NULL;

static string getConnectionString() {
    const char* envConn = getenv("DB_CONNECTION_STRING");
    if (envConn != NULL && string(envConn).length() > 0) {
        return string(envConn);
    }

    ifstream cfgFile("config.txt");
    if (cfgFile.is_open()) {
        string line;
        while (getline(cfgFile, line)) {
            line = trim(line);
            if (line.length() > 0 && line[0] != '#') {
                cfgFile.close();
                return line;
            }
        }
        cfgFile.close();
    }
    return "";
}

static void diagnoseConnectionError(const char* errMsg) {
    cout << "\n=======================================================\n";
    cout << " [ERROR] Supabase Database Connection Failed!\n";
    cout << "=======================================================\n";
    cout << " Technical detail: " << (errMsg ? errMsg : "Unknown error") << "\n\n";
    cout << " Diagnosis & Troubleshooting Checklist:\n";
    cout << "  1. Connection String Missing: Ensure DB_CONNECTION_STRING is set\n";
    cout << "     or 'config.txt' contains a valid connection URI.\n";
    cout << "  2. Wrong Password: Verify database password in Supabase settings.\n";
    cout << "  3. Pooler vs Direct: Supabase IPv4 requires the Session Pooler\n";
    cout << "     string (port 5432, user format 'postgres.<project-ref>').\n";
    cout << "  4. Network: Check internet connection or firewall rules.\n";
    cout << "  5. SSL Mode: Ensure 'sslmode=require' is appended to the URI.\n";
    cout << "=======================================================\n\n";
}

bool dbConnect(string customConnStr) {
    string connStr = customConnStr;
    if (connStr.length() == 0) {
        connStr = getConnectionString();
    }

    if (connStr.length() == 0) {
        cout << " [ERROR] No database connection string found!\n";
        cout << " Set environment variable DB_CONNECTION_STRING or create 'config.txt'.\n";
        return false;
    }

    conn = PQconnectdb(connStr.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        diagnoseConnectionError(PQerrorMessage(conn));
        PQfinish(conn);
        conn = NULL;
        return false;
    }

    PGresult* res = PQexec(conn, "SELECT 1");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        diagnoseConnectionError(PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        conn = NULL;
        return false;
    }

    PQclear(res);
    printSuccess("Connected to Supabase database successfully");
    return true;
}

void dbDisconnect() {
    if (conn != NULL) {
        PQfinish(conn);
        conn = NULL;
    }
}

int dbLoadCustomers(vector<Customer>& customersOut) {
    customersOut.clear();
    if (conn == NULL) return DB_ERROR;

    PGresult* res = PQexec(conn, 
        "SELECT account_no, name, phone, email, account_type, balance, "
        "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI:SS') as created_at "
        "FROM customers ORDER BY account_no ASC"
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return DB_ERROR;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        Customer c;
        c.accountNo = atoi(PQgetvalue(res, i, 0));
        c.name = PQgetvalue(res, i, 1);
        c.phone = PQgetvalue(res, i, 2);
        c.email = PQgetvalue(res, i, 3);
        c.accountType = PQgetvalue(res, i, 4);
        string balStr = PQgetvalue(res, i, 5);
        parseMoneyToPaise(balStr, c.balancePaise);
        c.createdAt = PQgetvalue(res, i, 6);
        customersOut.push_back(c);
    }

    PQclear(res);
    return DB_OK;
}

int dbInsertCustomer(const Customer& cust, long long initialDepositPaise, int& generatedAccNo, string& dbDetailOut) {
    (void)initialDepositPaise;
    generatedAccNo = 0;
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    PGresult* res = PQexec(conn, "BEGIN");
    PQclear(res);

    string balStr = formatPaiseToRupees(cust.balancePaise);
    const char* paramValues[5];
    paramValues[0] = cust.name.c_str();
    paramValues[1] = cust.phone.c_str();
    paramValues[2] = cust.email.c_str();
    paramValues[3] = cust.accountType.c_str();
    paramValues[4] = balStr.c_str();

    res = PQexecParams(conn,
        "INSERT INTO customers (name, phone, email, account_type, balance) "
        "VALUES ($1, $2, $3, $4, $5) RETURNING account_no",
        5, NULL, paramValues, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        const char* sqlState = PQresultErrorField(res, PG_DIAG_SQLSTATE);
        dbDetailOut = PQerrorMessage(conn);
        bool isDup = (sqlState != NULL && string(sqlState) == "23505");
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return isDup ? DB_DUPLICATE_PHONE : DB_ERROR;
    }

    generatedAccNo = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    string accNoStr = to_string(generatedAccNo);
    const char* txnParams[3];
    txnParams[0] = accNoStr.c_str();
    txnParams[1] = balStr.c_str();
    txnParams[2] = balStr.c_str();

    res = PQexecParams(conn,
        "INSERT INTO transactions (account_no, txn_type, amount, balance_after) "
        "VALUES ($1, 'OPENING', $2, $3)",
        3, NULL, txnParams, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_ERROR;
    }

    PQclear(res);
    res = PQexec(conn, "COMMIT");
    PQclear(res);
    return DB_OK;
}

int dbDeposit(int accountNo, long long amountPaise, long long& newBalancePaiseOut, string& dbDetailOut) {
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    PGresult* res = PQexec(conn, "BEGIN");
    PQclear(res);

    string accNoStr = to_string(accountNo);
    const char* p1[1] = { accNoStr.c_str() };

    // Select row for update
    res = PQexecParams(conn,
        "SELECT balance FROM customers WHERE account_no = $1 FOR UPDATE",
        1, NULL, p1, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        if (PQntuples(res) == 0) dbDetailOut = "Account number not found.";
        else dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_NOT_FOUND;
    }

    string curBalStr = PQgetvalue(res, 0, 0);
    PQclear(res);

    long long currentBal = 0;
    parseMoneyToPaise(curBalStr, currentBal);
    newBalancePaiseOut = currentBal + amountPaise;

    string newBalStr = formatPaiseToRupees(newBalancePaiseOut);
    string amtStr = formatPaiseToRupees(amountPaise);

    const char* pUpd[2] = { newBalStr.c_str(), accNoStr.c_str() };
    res = PQexecParams(conn,
        "UPDATE customers SET balance = $1 WHERE account_no = $2",
        2, NULL, pUpd, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_ERROR;
    }
    PQclear(res);

    const char* pTxn[3] = { accNoStr.c_str(), amtStr.c_str(), newBalStr.c_str() };
    res = PQexecParams(conn,
        "INSERT INTO transactions (account_no, txn_type, amount, balance_after) "
        "VALUES ($1, 'DEPOSIT', $2, $3)",
        3, NULL, pTxn, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_ERROR;
    }
    PQclear(res);

    res = PQexec(conn, "COMMIT");
    PQclear(res);
    return DB_OK;
}

int dbWithdraw(int accountNo, long long amountPaise, long long& newBalancePaiseOut, string& dbDetailOut) {
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    PGresult* res = PQexec(conn, "BEGIN");
    PQclear(res);

    string accNoStr = to_string(accountNo);
    const char* p1[1] = { accNoStr.c_str() };

    res = PQexecParams(conn,
        "SELECT account_type, balance FROM customers WHERE account_no = $1 FOR UPDATE",
        1, NULL, p1, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        dbDetailOut = "Account number not found.";
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_NOT_FOUND;
    }

    string typeStr = PQgetvalue(res, 0, 0);
    string curBalStr = PQgetvalue(res, 0, 1);
    PQclear(res);

    long long currentBal = 0;
    parseMoneyToPaise(curBalStr, currentBal);

    if (amountPaise > currentBal) {
        dbDetailOut = "Requested amount exceeds current account balance.";
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_INSUFFICIENT;
    }

    long long tempNewBal = currentBal - amountPaise;
    if (typeStr == "SAVINGS" && tempNewBal < 50000LL) {
        dbDetailOut = "SAVINGS account balance cannot drop below Rs. 500.00.";
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_SAVINGS_MIN_BREACH;
    }

    newBalancePaiseOut = tempNewBal;
    string newBalStr = formatPaiseToRupees(newBalancePaiseOut);
    string amtStr = formatPaiseToRupees(amountPaise);

    const char* pUpd[2] = { newBalStr.c_str(), accNoStr.c_str() };
    res = PQexecParams(conn,
        "UPDATE customers SET balance = $1 WHERE account_no = $2",
        2, NULL, pUpd, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_ERROR;
    }
    PQclear(res);

    const char* pTxn[3] = { accNoStr.c_str(), amtStr.c_str(), newBalStr.c_str() };
    res = PQexecParams(conn,
        "INSERT INTO transactions (account_no, txn_type, amount, balance_after) "
        "VALUES ($1, 'WITHDRAW', $2, $3)",
        3, NULL, pTxn, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        res = PQexec(conn, "ROLLBACK");
        PQclear(res);
        return DB_ERROR;
    }
    PQclear(res);

    res = PQexec(conn, "COMMIT");
    PQclear(res);
    return DB_OK;
}

int dbUpdateCustomer(int accountNo, const string& name, const string& phone, const string& email, string& dbDetailOut) {
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    string accNoStr = to_string(accountNo);
    const char* params[4] = { name.c_str(), phone.c_str(), email.c_str(), accNoStr.c_str() };

    PGresult* res = PQexecParams(conn,
        "UPDATE customers SET name = $1, phone = $2, email = $3 WHERE account_no = $4",
        4, NULL, params, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        const char* sqlState = PQresultErrorField(res, PG_DIAG_SQLSTATE);
        dbDetailOut = PQerrorMessage(conn);
        bool isDup = (sqlState != NULL && string(sqlState) == "23505");
        PQclear(res);
        return isDup ? DB_DUPLICATE_PHONE : DB_ERROR;
    }

    char* affected = PQcmdTuples(res);
    int count = atoi(affected);
    PQclear(res);

    return (count > 0) ? DB_OK : DB_NOT_FOUND;
}

int dbDeleteCustomer(int accountNo, string& dbDetailOut) {
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    string accNoStr = to_string(accountNo);
    const char* params[1] = { accNoStr.c_str() };

    PGresult* res = PQexecParams(conn,
        "DELETE FROM customers WHERE account_no = $1",
        1, NULL, params, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        return DB_ERROR;
    }

    char* affected = PQcmdTuples(res);
    int count = atoi(affected);
    PQclear(res);

    return (count > 0) ? DB_OK : DB_NOT_FOUND;
}

int dbLoadTransactions(int accountNo, vector<Transaction>& txnsOut, string& dbDetailOut) {
    txnsOut.clear();
    dbDetailOut = "";
    if (conn == NULL) return DB_ERROR;

    string accNoStr = to_string(accountNo);
    const char* params[1] = { accNoStr.c_str() };

    // Verify account exists
    PGresult* checkRes = PQexecParams(conn, "SELECT 1 FROM customers WHERE account_no = $1", 1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(checkRes) != PGRES_TUPLES_OK || PQntuples(checkRes) == 0) {
        PQclear(checkRes);
        return DB_NOT_FOUND;
    }
    PQclear(checkRes);

    PGresult* res = PQexecParams(conn,
        "SELECT txn_id, account_no, txn_type, amount, balance_after, "
        "TO_CHAR(txn_time, 'YYYY-MM-DD HH24:MI:SS') as txn_time "
        "FROM transactions WHERE account_no = $1 ORDER BY txn_id ASC",
        1, NULL, params, NULL, NULL, 0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        dbDetailOut = PQerrorMessage(conn);
        PQclear(res);
        return DB_ERROR;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        Transaction t;
        t.txnId = atoi(PQgetvalue(res, i, 0));
        t.accountNo = atoi(PQgetvalue(res, i, 1));
        t.type = PQgetvalue(res, i, 2);
        parseMoneyToPaise(PQgetvalue(res, i, 3), t.amountPaise);
        parseMoneyToPaise(PQgetvalue(res, i, 4), t.balanceAfterPaise);
        t.time = PQgetvalue(res, i, 5);
        txnsOut.push_back(t);
    }

    PQclear(res);
    return DB_OK;
}
