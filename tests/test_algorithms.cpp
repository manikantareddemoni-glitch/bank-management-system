#include "Customer.h"
#include "Bank.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

int g_testsPassed = 0;
int g_testsFailed = 0;

#define TEST_CASE(name) \
    cout << "[TEST] " << name << " ... ";

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        cout << "FAILED at line " << __LINE__ << "\n"; \
        g_testsFailed++; \
        return; \
    }

#define ASSERT_EQ(actual, expected) \
    if ((actual) != (expected)) { \
        cout << "FAILED at line " << __LINE__ << ": expected " << (expected) << " but got " << (actual) << "\n"; \
        g_testsFailed++; \
        return; \
    }

#define TEST_END \
    cout << "PASSED\n"; \
    g_testsPassed++;

void testLinearSearch() {
    TEST_CASE("Linear Search by Phone Number");
    vector<Customer> list;
    Customer c1 = {1001, "Alice",   "9876543210", "alice@ex.com", "SAVINGS", 100000, "2026-01-01"};
    Customer c2 = {1002, "Bob",     "9823456789", "bob@ex.com",   "CURRENT", 250000, "2026-01-01"};
    Customer c3 = {1003, "Charlie", "9712345678", "charlie@ex.com","SAVINGS", 50000, "2026-01-01"};
    list.push_back(c1);
    list.push_back(c2);
    list.push_back(c3);

    Customer found;
    ASSERT_TRUE(linearSearchByPhone(list, "9823456789", found));
    ASSERT_EQ(found.accountNo, 1002);
    ASSERT_EQ(found.name, string("Bob"));

    ASSERT_TRUE(!linearSearchByPhone(list, "0000000000", found));
    TEST_END;
}

void testBinarySearch() {
    TEST_CASE("Binary Search by Account Number");
    vector<Customer> sortedList;
    Customer c1 = {1001, "Alice",   "9876543210", "a@e.com", "SAVINGS", 100000, "2026-01-01"};
    Customer c2 = {1002, "Bob",     "9823456789", "b@e.com", "CURRENT", 250000, "2026-01-01"};
    Customer c3 = {1005, "Charlie", "9712345678", "c@e.com", "SAVINGS",  50000, "2026-01-01"};
    Customer c4 = {1010, "David",   "9601234567", "d@e.com", "CURRENT", 300000, "2026-01-01"};
    sortedList.push_back(c1);
    sortedList.push_back(c2);
    sortedList.push_back(c3);
    sortedList.push_back(c4);

    Customer found;
    ASSERT_TRUE(binarySearchByAccountNo(sortedList, 1005, found));
    ASSERT_EQ(found.name, string("Charlie"));

    ASSERT_TRUE(binarySearchByAccountNo(sortedList, 1001, found));
    ASSERT_EQ(found.name, string("Alice"));

    ASSERT_TRUE(binarySearchByAccountNo(sortedList, 1010, found));
    ASSERT_EQ(found.name, string("David"));

    ASSERT_TRUE(!binarySearchByAccountNo(sortedList, 9999, found));
    TEST_END;
}

void testSortAscendingDescending() {
    TEST_CASE("Insertion Sort - Ascending & Descending");
    vector<Customer> list;
    Customer c1 = {1001, "Alice", "111", "", "SAVINGS", 50000, ""};
    Customer c2 = {1002, "Bob",   "222", "", "SAVINGS", 150000, ""};
    Customer c3 = {1003, "Carol", "333", "", "SAVINGS", 20000, ""};
    Customer c4 = {1004, "David", "444", "", "SAVINGS", 80000, ""};
    list.push_back(c1);
    list.push_back(c2);
    list.push_back(c3);
    list.push_back(c4);

    vector<Customer> ascList = list;
    insertionSortByBalance(ascList, true);
    ASSERT_EQ(ascList[0].balancePaise, 20000LL);
    ASSERT_EQ(ascList[1].balancePaise, 50000LL);
    ASSERT_EQ(ascList[2].balancePaise, 80000LL);
    ASSERT_EQ(ascList[3].balancePaise, 150000LL);

    vector<Customer> descList = list;
    insertionSortByBalance(descList, false);
    ASSERT_EQ(descList[0].balancePaise, 150000LL);
    ASSERT_EQ(descList[1].balancePaise, 80000LL);
    ASSERT_EQ(descList[2].balancePaise, 50000LL);
    ASSERT_EQ(descList[3].balancePaise, 20000LL);
    TEST_END;
}

void testSortEdgeCasesAndStability() {
    TEST_CASE("Insertion Sort - Edge Cases & Stability");
    // Empty vector
    vector<Customer> emptyList;
    insertionSortByBalance(emptyList, true);
    ASSERT_EQ(static_cast<int>(emptyList.size()), 0);

    // Single element
    vector<Customer> singleList;
    Customer cAlone = {1001, "Alone", "111", "", "SAVINGS", 50000, ""};
    singleList.push_back(cAlone);
    insertionSortByBalance(singleList, true);
    ASSERT_EQ(singleList[0].accountNo, 1001);

    // Duplicate balances testing stability (order of equal keys preserved)
    vector<Customer> dupList;
    Customer d1 = {1001, "FirstDup",  "111", "", "SAVINGS", 50000, ""};
    Customer d2 = {1002, "SecondDup", "222", "", "SAVINGS", 50000, ""};
    Customer d3 = {1003, "ThirdDup",  "333", "", "SAVINGS", 50000, ""};
    dupList.push_back(d1);
    dupList.push_back(d2);
    dupList.push_back(d3);

    insertionSortByBalance(dupList, true);
    ASSERT_EQ(dupList[0].name, string("FirstDup"));
    ASSERT_EQ(dupList[1].name, string("SecondDup"));
    ASSERT_EQ(dupList[2].name, string("ThirdDup"));
    TEST_END;
}

void testHighestBalance() {
    TEST_CASE("Find Highest Balance (Handling Empty & Multi-Customer Ties)");
    vector<Customer> emptyList;
    vector<Customer> highestOut;
    findHighestBalanceCustomers(emptyList, highestOut);
    ASSERT_EQ(static_cast<int>(highestOut.size()), 0);

    vector<Customer> list;
    Customer c1 = {1001, "Aarav", "111", "", "SAVINGS", 10000, ""};
    Customer c2 = {1002, "Priya", "222", "", "SAVINGS", 50000, ""};
    Customer c3 = {1003, "Rohan", "333", "", "SAVINGS", 30000, ""};
    list.push_back(c1);
    list.push_back(c2);
    list.push_back(c3);

    findHighestBalanceCustomers(list, highestOut);
    ASSERT_EQ(static_cast<int>(highestOut.size()), 1);
    ASSERT_EQ(highestOut[0].accountNo, 1002);

    vector<Customer> tieList;
    Customer t1 = {1001, "Aarav", "111", "", "SAVINGS", 50000, ""};
    Customer t2 = {1002, "Priya", "222", "", "SAVINGS", 50000, ""};
    Customer t3 = {1003, "Rohan", "333", "", "SAVINGS", 20000, ""};
    tieList.push_back(t1);
    tieList.push_back(t2);
    tieList.push_back(t3);

    findHighestBalanceCustomers(tieList, highestOut);
    ASSERT_EQ(static_cast<int>(highestOut.size()), 2);
    ASSERT_EQ(highestOut[0].accountNo, 1001);
    ASSERT_EQ(highestOut[1].accountNo, 1002);
    TEST_END;
}

int main() {
    cout << "=======================================================\n";
    cout << "  RUNNING HAND-ROLLED DSA ALGORITHM UNIT TESTS\n";
    cout << "=======================================================\n";

    testLinearSearch();
    testBinarySearch();
    testSortAscendingDescending();
    testSortEdgeCasesAndStability();
    testHighestBalance();

    cout << "=======================================================\n";
    cout << "  RESULTS: " << g_testsPassed << " passed, " << g_testsFailed << " failed.\n";
    cout << "=======================================================\n";

    return (g_testsFailed == 0) ? 0 : 1;
}
