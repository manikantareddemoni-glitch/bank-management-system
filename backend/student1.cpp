#include "bank.h"

// ============================================================================
//   STUDENT 1: CUSTOMER ACCOUNT MANAGEMENT & BASIC DATA STRUCTURES
// ============================================================================
// Key Topics Covered:
// 1. Structures (Customer struct with nested Date and Address structs)
// 2. Arrays / Vectors (Storing customer records)
// 3. For Loops (Iterating over customer records to display them)
// 4. Input / Output with cin and cout
// 5. Basic Bubble Sort with simple for loops (Sorting by Account Number)
// ============================================================================

// Global storage for bank customers and transactions
vector<Customer> customerList;
vector<Transaction> transactionList;
int nextAccountNo = 1007; // Counter for next customer account number
int nextTxnId = 7;        // Counter for next transaction ID

/**
 * Function: Sort Accounts By Account Number
 * Explanation: Uses basic Bubble Sort with two simple for loops.
 * Keeps records sorted so Student 2's Binary Search works correctly.
 */
void sortAccountsByNumber() {
    int n = customerList.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (customerList[j].accountNo > customerList[j + 1].accountNo) {
                Customer temp = customerList[j];
                customerList[j] = customerList[j + 1];
                customerList[j + 1] = temp;
            }
        }
    }
}

/**
 * Function: Seed Initial Demo Data
 * Explanation: Adds 6 initial sample customers so you can test the program right away.
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

    // Create initial opening transaction logs using a simple for loop
    for (int i = 0; i < (int)customerList.size(); i++) {
        Transaction t;
        t.txnId = i + 1;
        t.accountNo = customerList[i].accountNo;
        t.type = "OPENING";
        t.amount = customerList[i].balance;
        t.balanceAfter = customerList[i].balance;
        transactionList.push_back(t);
    }
}

/**
 * Function: Display All Customers
 * Explanation: Uses a simple for loop to print all customer details.
 * (No complex formatting functions used - plain standard cout).
 */
void displayAllCustomers() {
    int total = customerList.size();
    if (total == 0) {
        cout << "\n[!] No customer records found.\n";
        return;
    }

    cout << "\n========================================================================\n";
    cout << "                    ALL CUSTOMER ACCOUNTS LIST                          \n";
    cout << "========================================================================\n";

    // Simple for loop to print each customer
    for (int i = 0; i < total; i++) {
        cout << "Record #" << (i + 1) << ":\n";
        cout << "  Account Number : " << customerList[i].accountNo << "\n";
        cout << "  Customer Name  : " << customerList[i].name << "\n";
        cout << "  Phone Number   : " << customerList[i].phone << "\n";
        cout << "  Account Type   : " << customerList[i].accountType << "\n";
        cout << "  Current Balance: Rs. " << customerList[i].balance << "\n";
        cout << "  Branch / City  : " << customerList[i].address.city << "\n";
        cout << "------------------------------------------------------------------------\n";
    }

    cout << "Total Records: " << total << "\n";
}

/**
 * Function: Add New Customer Account
 * Explanation:
 * 1. Takes user input using cin and getline
 * 2. Checks if phone already exists using Student 2's Linear Search
 * 3. Validates minimum deposit (Rs. 500 for Savings)
 * 4. Adds customer to list and sorts list by account number
 */
void addNewCustomer() {
    cout << "\n--- [STUDENT 1] ADD NEW CUSTOMER ACCOUNT ---\n";
    
    Customer newCust;
    newCust.accountNo = nextAccountNo;
    nextAccountNo = nextAccountNo + 1;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, newCust.name);

    cout << "Enter 10-digit Phone Number: ";
    cin >> newCust.phone;

    // Check duplicate phone number using linear search
    int existingIndex = linearSearchByPhone(customerList, newCust.phone);
    if (existingIndex != -1) {
        cout << "\n[ERROR] Phone number already registered with another account!\n";
        return;
    }

    cout << "Select Account Type (1 for SAVINGS, 2 for CURRENT): ";
    int typeChoice;
    cin >> typeChoice;
    if (typeChoice == 1) {
        newCust.accountType = "SAVINGS";
    } else {
        newCust.accountType = "CURRENT";
    }

    cout << "Enter Initial Deposit Amount (Rs.): ";
    cin >> newCust.balance;

    // Minimum balance check
    if (newCust.accountType == "SAVINGS" && newCust.balance < 500.0) {
        cout << "\n[ERROR] SAVINGS account requires a minimum initial deposit of Rs. 500!\n";
        return;
    }

    // Default registration date and city
    newCust.openedDate.day = 4;
    newCust.openedDate.month = 10;
    newCust.openedDate.year = 2026;
    newCust.address.city = "Hyderabad";
    newCust.address.state = "Telangana";
    newCust.address.pincode = "500001";

    // Add customer to list
    customerList.push_back(newCust);

    // Keep sorted by account number using our simple bubble sort
    sortAccountsByNumber();

    // Create an opening transaction record
    Transaction t;
    t.txnId = nextTxnId;
    nextTxnId = nextTxnId + 1;
    t.accountNo = newCust.accountNo;
    t.type = "OPENING";
    t.amount = newCust.balance;
    t.balanceAfter = newCust.balance;
    transactionList.push_back(t);

    cout << "\n[SUCCESS] Account created successfully!\n";
    cout << "Assigned Account Number: " << newCust.accountNo << "\n";
}
