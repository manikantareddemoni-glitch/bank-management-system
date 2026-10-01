# Data Structures & Algorithms (DSA) Explanation Guide
**Course**: CS207 Data Structures using C++ (Aurora Higher Education and Research Academy)

This document maps the project to the official CS207 course syllabus modules (Modules I - X) and provides detailed viva explanations for all hand-rolled algorithms.

---

## Syllabus Module Mapping Table

| Syllabus Module | Whitelisted Syllabus Concept | Project Implementation File & Function | Description & Purpose |
| :--- | :--- | :--- | :--- |
| **Module I - II** | Variables, Control Statements (`if/else`, `switch`), Loops (`while`, `for`), Functions, Pass by Reference | [main.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/main.cpp) (`main()`, `printMenu()`) | Procedural control flow, menu loop dispatch using `switch`, and pass-by-reference parameter passing (`vector<Customer>&`). |
| **Module III** | 1D Arrays & Array Operations | [Bank.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Bank.cpp) (`insertionSortByBalance`) | Contiguous elements iteration and shifting elements during Insertion Sort. |
| **Module V** | `std::string` methods & `<cctype>` functions | [Utils.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Utils.cpp) (`trim`, `isDigitsOnly`, `parseMoneyToPaise`) | Character-by-character string traversal for phone/email validation and integer paise currency conversion. |
| **Module VI** | `struct` (Structures without methods) | [Customer.h](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/include/Customer.h) (`Customer`, `Transaction`) | Plain structure definitions holding customer details and transaction log fields without classes or methods. |
| **Module VII** | Time & Space Complexity Analysis | [Bank.h](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/include/Bank.h) (Header comments) | Asymptotic Big-O annotations ($O(1)$, $O(n)$, $O(\log n)$, $O(n^2)$) documented above every algorithm function. |
| **Module VIII - IX** | Stack & Queue concepts (Optional Bonus) | [DSA_EXPLANATION.md](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/DSA_EXPLANATION.md) | Arrays can implement a 5-element Undo Stack or Recent Transaction Queue. |
| **Module X** | STL `std::vector` | [Bank.h](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/include/Bank.h) (`vector<Customer>`) | Contiguous dynamic array serving as the in-memory cache synchronized with PostgreSQL. |

---

## Hand-Rolled Searching & Sorting Algorithms

### 1. Searching Algorithm 1: Linear Search

#### What it is
A sequential search algorithm that checks elements one by one from beginning to end until a matching record is found.

#### Where it is used in this project
- **File**: [Bank.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Bank.cpp#L13)
- **Function**: `linearSearchByPhone(const vector<Customer>& customers, string phone, Customer& resultCust)`

#### Complexity & Viva Explanation
- **Time Complexity**:
  - Worst Case: $O(n)$ (scans all $n$ items if target is at the end or absent).
  - Best Case: $O(1)$ (target found at index 0).
- **Space Complexity**: $O(1)$ auxiliary space.
- **Viva Answer**: *"We use Linear Search for phone lookup because phone numbers are unique but not stored in sorted order in the vector. A linear scan guarantees finding the customer in $O(n)$ time."*

---

### 2. Searching Algorithm 2: Iterative Binary Search

#### What it is
A Divide-and-Conquer search algorithm that repeatedly divides a sorted search interval in half.

#### Where it is used in this project
- **File**: [Bank.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Bank.cpp#L23)
- **Function**: `binarySearchByAccountNo(const vector<Customer>& customers, int accountNo, Customer& resultCust)`

#### Precondition & Complexity
- **Precondition**: Vector **MUST** be pre-sorted in ascending order by `accountNo`.
- **Precondition Guarantee**: Database records are loaded via SQL `SELECT ... FROM customers ORDER BY account_no ASC`.
- **Time Complexity**: $O(\log n)$ worst/average case.
- **Space Complexity**: $O(1)$ auxiliary space (iterative loop using `low`, `high`, `mid` pointers without stack recursion).
- **Viva Answer**: *"We use Iterative Binary Search for account lookup because the vector is pre-sorted by account_no from PostgreSQL. Binary Search reduces lookup time from $O(n)$ to $O(\log n)$, taking at most 20 comparisons for 1,000,000 customers."*

---

### 3. Sorting Algorithm: Insertion Sort (Stable)

#### What it is
An in-place comparison sort that builds the sorted array one element at a time by inserting each item into its correct position.

#### Where it is used in this project
- **File**: [Bank.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Bank.cpp#L41)
- **Function**: `insertionSortByBalance(vector<Customer>& customers, bool ascending)`

#### Complexity & Stability
- **Time Complexity**:
  - Worst/Average Case: $O(n^2)$ (elements in reverse order).
  - Best Case: $O(n)$ (elements already sorted).
- **Space Complexity**: $O(1)$ auxiliary in-place sorting.
- **Stability**: **STABLE**. Preserves the relative order of customers with equal balances.
- **Viva Answer**: *"Insertion Sort is simple, in-place ($O(1)$ space), and stable. Stability ensures that if two customers have identical balances (e.g. ₹500.00), their relative order is preserved."*

---

### 4. Maximum Scan: Highest Balance (Handling Ties)

#### What it is
A single-pass linear scanning algorithm that locates the maximum balance value and collects **all** tied accounts.

#### Where it is used in this project
- **File**: [Bank.cpp](file:///c:/Users/shekar/OneDrive/Desktop/C++%20PROJECT/src/Bank.cpp#L67)
- **Function**: `findHighestBalanceCustomers(const vector<Customer>& customers, vector<Customer>& highestOut)`

#### Complexity & Viva Explanation
- **Time Complexity**: $O(n)$ single pass scan.
- **Space Complexity**: $O(k)$ where $k$ is the number of tied accounts.
- **Viva Answer**: *"Standard `std::max_element` only returns the first max element. Our linear scan loops through the vector to find the max balance, then gathers all tied customer accounts into a vector."*
