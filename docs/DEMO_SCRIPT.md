# Live Demo Script & Viva Exam Preparation Guide
**Course**: CS207 Data Structures using C++ (Aurora Higher Education and Research Academy)

This guide provides a timed step-by-step live presentation plan for project evaluators, followed by 20 critical viva questions and answers.

---

## Part 1: Timed Live Presentation Script (5-7 Minutes)

### Screen Layout Setup
- **Left Side of Screen**: Terminal running compiled `bank_app`
- **Right Side of Screen**: Web browser opened to [Supabase Console](https://supabase.com/dashboard) $\rightarrow$ **Table Editor** (`customers` & `transactions` tables).

---

### Step-by-Step Demo Flow

| Time | Action in C++ Terminal | Action in Supabase Table Editor | Key Point for Evaluator |
| :--- | :--- | :--- | :--- |
| **0:00 - 0:45** | Run `./bank_app`. Show startup message: `"Connected to Supabase database successfully"`. Select **Option 2 (Display All Customers)**. | Show 6 pre-seeded rows in `customers` table matching terminal. | Demonstrates real cloud database connectivity via `libpq` C API. |
| **0:45 - 1:30** | Select **Option 1 (Add Customer)**.<br>Name: `Ramesh Kumar`<br>Phone: `9988776655`<br>Email: `ramesh@example.com`<br>Type: `SAVINGS`<br>Deposit: `1500.50` | Click **Refresh** in Supabase `customers` & `transactions` tables. | Account `1007` appears in `customers`. Matching `OPENING` row appears in `transactions`. |
| **1:30 - 2:15** | Select **Option 4 (Deposit Funds)**.<br>Account No: `1007`<br>Amount: `500.00` | Click **Refresh** in Supabase `customers`. | Balance updates from `1500.50` $\rightarrow$ `2000.50`. New `DEPOSIT` record inserted into `transactions`. |
| **2:15 - 3:00** | **[Edge Case 1: Overdraft]**<br>Select **Option 5 (Withdraw Funds)**.<br>Account No: `1007`<br>Amount: `5000.00` (Exceeds balance). | Observe `customers` table in Supabase. | Terminal displays `[ERROR] Insufficient balance`. Database balance remains completely unchanged! |
| **3:00 - 3:45** | **[Edge Case 2: Duplicate Phone]**<br>Select **Option 1 (Add Customer)**.<br>Enter Phone: `9988776655` (Belongs to Ramesh). | Observe `customers` table in Supabase. | Terminal catches SQLSTATE `23505` and prints `[ERROR] Phone number already registered`. No new DB row created. |
| **3:45 - 4:30** | Select **Option 3 (Search Customer)**.<br>Account No: `1001` | Point to `binarySearchByAccountNo` function in source code. | Demonstrates $O(\log n)$ iterative binary search on sorted vector. |
| **4:30 - 5:15** | Select **Option 8 (Sort Customers)**.<br>Choice: `2` (Descending). | Terminal displays sorted table by balance from highest to lowest. | Demonstrates hand-coded **Insertion Sort** ($O(n^2)$ stable in-place sort). |
| **5:15 - 6:00** | Select **Option 10 (Delete / Close Account)**.<br>Account No: `1007`. Confirm `y`. | Click **Refresh** in Supabase `customers` & `transactions`. | Row `1007` disappears from `customers`. All matching transactions in `transactions` deleted via `ON DELETE CASCADE`. |

---

## Part 2: 20 Likely Viva Questions & Answers

### 1. Why PostgreSQL and libpq instead of SQLite or MySQL?
**Answer**: PostgreSQL is an enterprise-grade ACID-compliant database. Supabase provides PostgreSQL hosted in the cloud. `libpq` is the official C client library for PostgreSQL, providing direct database connectivity without external ORMs.

### 2. Why use std::vector for in-memory storage?
**Answer**: `std::vector` stores elements in contiguous memory blocks, maximizing CPU cache hit rates. It supports $O(1)$ constant-time random index access (`v[mid]`), which is essential for binary search.

### 3. What is the difference between Binary Search and Linear Search?
**Answer**: Linear Search inspects elements sequentially ($O(n)$ time) and works on unsorted data. Binary Search repeatedly halves the search space ($O(\log n)$ time) but requires the data to be pre-sorted.

### 4. Why use database transactions (BEGIN ... COMMIT / ROLLBACK) for deposit and withdrawal?
**Answer**: Financial operations require **Atomicity** (the 'A' in ACID). Updating the customer's balance and logging the transaction record must both succeed together or both fail. If any step fails, sending `ROLLBACK` reverts changes, preventing database corruption.

### 5. Why should money NEVER be stored using float or double?
**Answer**: Floating-point numbers use IEEE 754 binary representation, which cannot represent base-10 decimals like `0.10` or `0.01` exactly. Over time, rounding errors accumulate (e.g. `0.1 + 0.2 = 0.30000000000000004`). We store money strictly as `long long` integer paise.

### 6. What does FOR UPDATE do in your deposit and withdraw SQL queries?
**Answer**: `SELECT ... FOR UPDATE` acquires an exclusive row-level lock on the customer record in PostgreSQL. This prevents race conditions if two transactions attempt to update the same account balance simultaneously.

### 7. What is SQL Injection, and how did you prevent it?
**Answer**: SQL injection occurs when untrusted user input is directly concatenated into SQL query strings. We completely prevented SQL injection by using **parameterized queries** (`PQexecParams`) where inputs are sent separately as bound parameters ($1, $2).

### 8. What is the time and space complexity of Insertion Sort?
**Answer**: Insertion Sort has a time complexity of $O(n^2)$ in worst/average cases and $O(n)$ in the best case (when already sorted). Auxiliary space complexity is $O(1)$ because it sorts in-place.

### 9. Is Insertion Sort stable? Why does stability matter?
**Answer**: Yes, Insertion Sort is stable. Stability means elements with equal key values preserve their relative original order. If two accounts have the exact same balance, their order in the list remains unchanged after sorting.

### 10. How does the application handle error codes instead of exceptions?
**Answer**: Following procedural rules, functions return integer error constants (`DB_OK = 0`, `DB_NOT_FOUND = 1`, `DB_INSUFFICIENT = 2`, `DB_DUPLICATE_PHONE = 3`, `DB_ERROR = 4`, `DB_SAVINGS_MIN_BREACH = 5`). The caller inspects the return integer and prints clear user messages.

### 11. What is the minimum balance rule for SAVINGS accounts in your system?
**Answer**: `SAVINGS` accounts require an opening deposit of at least ₹500.00 (`50000` paise). During withdrawal, the system verifies that the remaining balance will not fall below ₹500.00. `CURRENT` accounts have a ₹0.00 limit.

### 12. How does your binary search guarantee its precondition is met?
**Answer**: Binary search requires sorted data. Database records are loaded executing `SELECT ... FROM customers ORDER BY account_no ASC`, guaranteeing that the loaded vector is pre-sorted by `account_no`.

### 13. How is the database connection pointer encapsulated?
**Answer**: In `Database.cpp`, the handle is declared as `static PGconn* conn = NULL;`. Because it is static and scoped to `Database.cpp`, no other file sees `libpq` C types, keeping the rest of the application completely syllabus-compliant.

### 14. What happens when an account is deleted?
**Answer**: The database schema defines `account_no` in `transactions` as `REFERENCES customers(account_no) ON DELETE CASCADE`. Deleting a customer row automatically deletes all associated transaction history records in PostgreSQL.

### 15. How does your linear scan for highest balance handle ties?
**Answer**: First, a single pass identifies the maximum balance value (`maxBalance`). A second pass gathers **all** customer records matching `maxBalance` into a vector, ensuring tied top account holders are all returned.

### 16. Why did you use string character traversal for money parsing instead of std::stod?
**Answer**: `std::stod` uses floating-point conversion. Traversal character by character extracts the rupee and paise parts as integer digits, guaranteeing exact integer paise values without precision loss.

### 17. How do you validate phone numbers and email addresses?
**Answer**: Phone numbers are validated in `isValidPhone` by ensuring the string contains exactly 10 numeric digits. Emails are validated in `isValidEmail` by checking for non-empty user and domain parts separated by `@` and `.`.

### 18. Where are database credentials stored?
**Answer**: Credentials are read dynamically from environment variable `DB_CONNECTION_STRING` or line 1 of `config.txt`. `config.txt` is listed in `.gitignore` to prevent committing sensitive passwords to Git.

### 19. Why is the database reloaded into memory after mutating operations?
**Answer**: PostgreSQL is the single source of truth. Reloading the vector (`bankSync()`) ensures the in-memory array accurately reflects server-side state, identity column sequence increments, and trigger timestamps.

### 20. Why use procedural C++ structs instead of OOP classes for this project?
**Answer**: Structs without member functions satisfy the 2nd-year CS207 syllabus whitelist. They act as clean, lightweight data containers while keeping business logic and database access procedural and easy to explain in a viva exam.
