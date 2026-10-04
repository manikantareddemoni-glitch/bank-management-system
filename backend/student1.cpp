#include "bank.h"

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   STUDENT 1: ACCOUNT MANAGEMENT & DATA STRUCTURES (MODULES I, II, VI)
// ====================================================================================================
// ====================================================================================================
// Responsibilities:
// 1. Definition and initialization of global repository (customerList, transactionList)
// 2. Struct nesting (Date and Address inside Customer entity)
// 3. Pre-loading benchmark demo accounts (seedInitialData)
// 4. Formatted tabular customer reporting using <iomanip> (displayAllCustomers)
// 5. Account onboarding with input validation and duplicate detection (addNewCustomer)
// ====================================================================================================

// Global In-Memory Database Storage
vector<Customer> customerList;
vector<Transaction> transactionList;
int nextAccountNo = 1007; // Auto-increment counter for new account numbers
int nextTxnId = 7;        // Auto-increment counter for new transactions

/**
 * Student 1 Function: Seed Initial Demo Data
 * Explanation: Pre-loads 6 demo customer records into memory.
 */
void seedInitialData() {
    customerList.clear();
    transactionList.clear();

    Customer c1 = {1001, "Aarav Sharma", "9876543210", "SAVINGS", 25000.50, {1, 1, 2026}, {"Hyderabad", "Telangana", "500001"}};
    Customer c2 = {1002, "Priya Patel", "9823456789", "SAVINGS", 1000.00, {1, 1, 2026}, {"Hyderabad", "Telangana", "500002"}};
    Customer c3 = {1003, "Rohan Verma", "9712345678", "CURRENT", 75000.00, {1, 1, 2026}, {"Hyderabad", "Telangana", "500003"}};
    Customer c4 = {1004, "Ananya Iyer", "9601234567", "SAVINGS", 650.75, {1, 1, 2026}, {"Hyderabad", "Telangana", "500034"}};
    Customer c5 = {1005, "Vikram Malhotra", "9543210987", "CURRENT", 150000.00, {1, 1, 2026}, {"Hyderabad", "Telangana", "500033"}};
    Customer c6 = {1006, "Sanya Gupta", "9432109876", "SAVINGS", 500.00, {1, 1, 2026}, {"Hyderabad", "Telangana", "500081"}};

    customerList.push_back(c1);
    customerList.push_back(c2);
    customerList.push_back(c3);
    customerList.push_back(c4);
    customerList.push_back(c5);
    customerList.push_back(c6);

    for (size_t i = 0; i < customerList.size(); i++) {
        Transaction t = { static_cast<int>(i + 1), customerList[i].accountNo, "OPENING", customerList[i].balance, customerList[i].balance };
        transactionList.push_back(t);
    }
}

/**
 * Student 1 Function: Display All Customers in Tabular Format
 * Explanation: Iterates through customerList and formats output using setw() and setprecision().
 */
void displayAllCustomers() {
    if (customerList.empty()) {
        cout << " [!] No customer records found.\n";
        return;
    }

    cout << "\n--------------------------------------------------------------------------------------\n";
    cout << left << setw(12) << "Acc No" 
         << setw(20) << "Customer Name" 
         << setw(15) << "Phone Number" 
         << setw(12) << "Type" 
         << setw(15) << "Balance (Rs.)" 
         << setw(12) << "City" << "\n";
    cout << "--------------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < customerList.size(); i++) {
        cout << left << setw(12) << customerList[i].accountNo
             << setw(20) << customerList[i].name
             << setw(15) << customerList[i].phone
             << setw(12) << customerList[i].accountType
             << right << setw(13) << fixed << setprecision(2) << customerList[i].balance << "  "
             << left << setw(12) << customerList[i].address.city << "\n";
    }
    cout << "--------------------------------------------------------------------------------------\n";
    cout << " Total records: " << customerList.size() << "\n";
}

/**
 * Student 1 Function: Add New Customer Account
 * Explanation: Takes customer inputs, validates minimum deposit (Rs. 500 for Savings),
 *              checks duplicate phone numbers via Student 2's linearSearchByPhone, and saves record.
 */
void addNewCustomer() {
    cout << "\n--- [STUDENT 1] ADD NEW CUSTOMER ACCOUNT ---\n";
    Customer newCust;
    newCust.accountNo = nextAccountNo++;

    cout << " Enter Customer Name: ";
    cin.ignore();
    getline(cin, newCust.name);

    cout << " Enter 10-digit Phone Number: ";
    cin >> newCust.phone;

    // Check duplicate phone via Student 2's Linear Search
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

    newCust.openedDate = {3, 10, 2026};
    newCust.address = {"Hyderabad", "Telangana", "500001"};

    // Insert into customer list
    customerList.push_back(newCust);

    // Keep sorted by account number for Student 2's Binary Search
    sort(customerList.begin(), customerList.end(), [](const Customer& a, const Customer& b) {
        return a.accountNo < b.accountNo;
    });

    // Record Opening Transaction Log
    Transaction t = { nextTxnId++, newCust.accountNo, "OPENING", newCust.balance, newCust.balance };
    transactionList.push_back(t);

    cout << " [OK] Account created successfully! Allocated Account Number: " << newCust.accountNo << "\n";
}
