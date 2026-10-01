#include "Database.h"
#include "Bank.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

void displayCustomersTable(const vector<Customer>& customers) {
    if (customers.empty()) {
        printError("No customer accounts found in system.");
        return;
    }

    cout << "\n" << string(96, '-') << "\n";
    cout << left 
         << setw(12) << "Account No"
         << setw(22) << "Name"
         << setw(15) << "Phone"
         << setw(10) << "Type"
         << setw(16) << "Balance (Rs.)"
         << setw(20) << "Created At" << "\n";
    cout << string(96, '-') << "\n";

    for (size_t i = 0; i < customers.size(); ++i) {
        const Customer& c = customers[i];
        cout << left 
             << setw(12) << c.accountNo
             << setw(22) << (c.name.length() > 20 ? c.name.substr(0, 17) + "..." : c.name)
             << setw(15) << c.phone
             << setw(10) << c.accountType
             << right << setw(14) << formatPaiseToRupees(c.balancePaise) << "  "
             << left << setw(20) << c.createdAt << "\n";
    }
    cout << string(96, '-') << "\n";
    cout << " Total records: " << customers.size() << "\n";
}

void handleAddCustomer(vector<Customer>& customers) {
    printHeader("ADD NEW CUSTOMER ACCOUNT");
    string name = readNonEmptyLine(" Enter Full Name: ");

    string phone;
    while (true) {
        phone = readNonEmptyLine(" Enter 10-digit Phone Number: ");
        if (isValidPhone(phone)) break;
        printError("Invalid phone number! Must be exactly 10 digits.");
    }

    string email;
    while (true) {
        cout << " Enter Email Address (optional, press Enter to skip): ";
        string line;
        getline(cin, line);
        email = trim(line);
        if (email.length() == 0 || isValidEmail(email)) break;
        printError("Invalid email format! Example: user@domain.com");
    }

    string type;
    while (true) {
        cout << " Select Account Type (1 = SAVINGS, 2 = CURRENT): ";
        int choice = readInt("", 1, 2);
        if (choice == 1) { type = "SAVINGS"; break; }
        if (choice == 2) { type = "CURRENT"; break; }
    }

    if (type == "SAVINGS") {
        cout << " Note: SAVINGS account requires an initial deposit of at least Rs. 500.00\n";
    }
    long long openingPaise = readMoney(" Enter Opening Deposit Amount (Rs.): ");

    int generatedAccNo = 0;
    string dbDetail = "";
    int code = bankCreateAccount(customers, name, phone, email, type, openingPaise, generatedAccNo, dbDetail);

    if (code == DB_OK) {
        printSuccess("Account created successfully! Allocated Account Number: " + to_string(generatedAccNo));
    } else if (code == DB_DUPLICATE_PHONE) {
        printError("Phone number already registered.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    } else if (code == DB_SAVINGS_MIN_BREACH) {
        printError("Creation failed: SAVINGS account requires minimum opening deposit of Rs. 500.00.");
    } else {
        printError("Database insertion error.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    }
}

void handleDisplayAllCustomers(const vector<Customer>& customers) {
    printHeader("ALL CUSTOMER ACCOUNTS");
    displayCustomersTable(customers);
}

void handleSearchCustomer(const vector<Customer>& customers) {
    printHeader("SEARCH CUSTOMER BY ACCOUNT NUMBER (BINARY SEARCH)");
    int accNo = readInt(" Enter Account Number to Search: ", 1000, 999999);

    Customer cust;
    if (binarySearchByAccountNo(customers, accNo, cust)) {
        printSuccess("Customer Account Found!");
        vector<Customer> singleVec;
        singleVec.push_back(cust);
        displayCustomersTable(singleVec);
    } else {
        printError("Account number " + to_string(accNo) + " not found.");
    }
}

void handleDeposit(vector<Customer>& customers) {
    printHeader("DEPOSIT FUNDS");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);
    long long amtPaise = readMoney(" Enter Amount to Deposit (Rs.): ");

    string dbDetail = "";
    int code = bankDeposit(customers, accNo, amtPaise, dbDetail);
    if (code == DB_OK) {
        Customer cust;
        if (binarySearchByAccountNo(customers, accNo, cust)) {
            printSuccess("Deposit successful! Updated Balance: Rs. " + formatPaiseToRupees(cust.balancePaise));
        } else {
            printSuccess("Deposit successful!");
        }
    } else if (code == DB_NOT_FOUND) {
        printError("Account number " + to_string(accNo) + " not found.");
    } else {
        printError("Deposit operation failed.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    }
}

void handleWithdraw(vector<Customer>& customers) {
    printHeader("WITHDRAW FUNDS");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);
    long long amtPaise = readMoney(" Enter Amount to Withdraw (Rs.): ");

    string dbDetail = "";
    int code = bankWithdraw(customers, accNo, amtPaise, dbDetail);
    if (code == DB_OK) {
        Customer cust;
        if (binarySearchByAccountNo(customers, accNo, cust)) {
            printSuccess("Withdrawal successful! Updated Balance: Rs. " + formatPaiseToRupees(cust.balancePaise));
        } else {
            printSuccess("Withdrawal successful!");
        }
    } else if (code == DB_NOT_FOUND) {
        printError("Account number " + to_string(accNo) + " not found.");
    } else if (code == DB_INSUFFICIENT) {
        printError("Withdrawal rejected: Insufficient balance.");
    } else if (code == DB_SAVINGS_MIN_BREACH) {
        printError("Withdrawal rejected: SAVINGS account balance cannot drop below Rs. 500.00.");
    } else {
        printError("Withdrawal operation failed.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    }
}

void handleDisplayBalance(const vector<Customer>& customers) {
    printHeader("CHECK ACCOUNT BALANCE");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);

    Customer cust;
    if (binarySearchByAccountNo(customers, accNo, cust)) {
        cout << "\n Account Number : " << cust.accountNo << "\n";
        cout << " Account Holder : " << cust.name << "\n";
        cout << " Account Type   : " << cust.accountType << "\n";
        cout << " Current Balance: Rs. " << formatPaiseToRupees(cust.balancePaise) << "\n";
    } else {
        printError("Account number " + to_string(accNo) + " not found.");
    }
}

void handleHighestBalance(const vector<Customer>& customers) {
    printHeader("HIGHEST BALANCE CUSTOMER(S)");
    vector<Customer> highest;
    findHighestBalanceCustomers(customers, highest);
    if (highest.empty()) {
        printError("No customer accounts available.");
        return;
    }
    if (highest.size() > 1) {
        cout << " [INFO] Tied for highest balance! Found " << highest.size() << " accounts:\n";
    }
    displayCustomersTable(highest);
}

void handleSortByBalance(vector<Customer>& customers) {
    printHeader("SORT CUSTOMERS BY BALANCE (INSERTION SORT)");
    cout << " 1. Ascending Order (Lowest to Highest)\n";
    cout << " 2. Descending Order (Highest to Lowest)\n";
    int choice = readInt(" Select sorting order (1-2): ", 1, 2);

    bool asc = (choice == 1);
    insertionSortByBalance(customers, asc);
    printSuccess(asc ? "Sorted customers in ASCENDING balance order." : "Sorted customers in DESCENDING balance order.");
    displayCustomersTable(customers);
}

void handleUpdateCustomer(vector<Customer>& customers) {
    printHeader("UPDATE CUSTOMER DETAILS");
    int accNo = readInt(" Enter Account Number to Update: ", 1000, 999999);

    Customer current;
    if (!binarySearchByAccountNo(customers, accNo, current)) {
        printError("Account number " + to_string(accNo) + " not found.");
        return;
    }

    cout << " Current Name: " << current.name << "\n";
    string name = readNonEmptyLine(" Enter New Name (or same to keep): ");

    cout << " Current Phone: " << current.phone << "\n";
    string phone;
    while (true) {
        phone = readNonEmptyLine(" Enter New 10-digit Phone: ");
        if (isValidPhone(phone)) break;
        printError("Invalid phone number! Must be 10 digits.");
    }

    cout << " Current Email: " << current.email << "\n";
    string email;
    while (true) {
        cout << " Enter New Email (optional, press Enter to keep blank): ";
        string line;
        getline(cin, line);
        email = trim(line);
        if (email.length() == 0 || isValidEmail(email)) break;
        printError("Invalid email format!");
    }

    string dbDetail = "";
    int code = bankUpdateCustomer(customers, accNo, name, phone, email, dbDetail);
    if (code == DB_OK) {
        printSuccess("Customer details updated successfully!");
    } else if (code == DB_DUPLICATE_PHONE) {
        printError("Phone number already registered to another account.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    } else {
        printError("Update operation failed.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    }
}

void handleDeleteCustomer(vector<Customer>& customers) {
    printHeader("DELETE / CLOSE ACCOUNT");
    int accNo = readInt(" Enter Account Number to Close: ", 1000, 999999);

    Customer cust;
    if (!binarySearchByAccountNo(customers, accNo, cust)) {
        printError("Account number " + to_string(accNo) + " not found.");
        return;
    }

    cout << " Account Holder: " << cust.name << " | Balance: Rs. " << formatPaiseToRupees(cust.balancePaise) << "\n";
    if (!readYesNo(" Are you sure you want to permanently delete this account?")) {
        cout << " [INFO] Account deletion cancelled.\n";
        return;
    }

    string dbDetail = "";
    int code = bankDeleteAccount(customers, accNo, dbDetail);
    if (code == DB_OK) {
        printSuccess("Account " + to_string(accNo) + " deleted successfully!");
    } else {
        printError("Account deletion failed.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
    }
}

void handleViewTransactions() {
    printHeader("TRANSACTION HISTORY");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);

    vector<Transaction> txns;
    string dbDetail = "";
    int code = bankGetTransactions(accNo, txns, dbDetail);

    if (code == DB_NOT_FOUND) {
        printError("Account number " + to_string(accNo) + " does not exist.");
        return;
    } else if (code != DB_OK) {
        printError("Failed to load transaction history.");
        if (dbDetail.length() > 0) printDbDetail(dbDetail);
        return;
    }

    if (txns.empty()) {
        cout << " [INFO] No transactions found for account " << accNo << ".\n";
        return;
    }

    cout << "\n" << string(85, '-') << "\n";
    cout << left 
         << setw(10) << "Txn ID"
         << setw(14) << "Account No"
         << setw(12) << "Type"
         << setw(16) << "Amount (Rs.)"
         << setw(20) << "Balance After (Rs.)"
         << setw(20) << "Txn Time" << "\n";
    cout << string(85, '-') << "\n";

    for (size_t i = 0; i < txns.size(); ++i) {
        const Transaction& t = txns[i];
        cout << left 
             << setw(10) << t.txnId
             << setw(14) << t.accountNo
             << setw(12) << t.type
             << right << setw(14) << formatPaiseToRupees(t.amountPaise) << "  "
             << right << setw(18) << formatPaiseToRupees(t.balanceAfterPaise) << "  "
             << left << setw(20) << t.time << "\n";
    }
    cout << string(85, '-') << "\n";
    cout << " Total transactions: " << txns.size() << "\n";
}

void printMenu() {
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|          BANK MANAGEMENT SYSTEM (SUPABASE DB)         |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << "|  1. Add Customer Account                              |\n";
    cout << "|  2. Display All Customers                             |\n";
    cout << "|  3. Search Customer by Account No (Binary Search)     |\n";
    cout << "|  4. Deposit Funds                                     |\n";
    cout << "|  5. Withdraw Funds                                    |\n";
    cout << "|  6. Display Account Balance                           |\n";
    cout << "|  7. Find Customer(s) with Highest Balance             |\n";
    cout << "|  8. Sort Customers by Balance (Insertion Sort)        |\n";
    cout << "|  9. Update Customer Details                           |\n";
    cout << "| 10. Delete / Close Account                            |\n";
    cout << "| 11. View Transaction History                          |\n";
    cout << "| 12. Exit                                              |\n";
    cout << "+-------------------------------------------------------+\n";
}

int main() {
    if (!dbConnect("")) {
        return 1;
    }

    vector<Customer> customers;
    bankSync(customers);

    while (true) {
        printMenu();
        int choice = readInt(" Select an option (1-12): ", 1, 12);
        switch (choice) {
            case 1:  handleAddCustomer(customers); break;
            case 2:  handleDisplayAllCustomers(customers); break;
            case 3:  handleSearchCustomer(customers); break;
            case 4:  handleDeposit(customers); break;
            case 5:  handleWithdraw(customers); break;
            case 6:  handleDisplayBalance(customers); break;
            case 7:  handleHighestBalance(customers); break;
            case 8:  handleSortByBalance(customers); break;
            case 9:  handleUpdateCustomer(customers); break;
            case 10: handleDeleteCustomer(customers); break;
            case 11: handleViewTransactions(); break;
            case 12:
                dbDisconnect();
                cout << "\n Thank you for using Bank Management System. Goodbye!\n";
                return 0;
            default:
                printError("Invalid menu choice.");
                break;
        }
    }
    return 0;
}
