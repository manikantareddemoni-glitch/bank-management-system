# Bank Management System (Group 1 Mini Project)
**Course**: CS207 Data Structures using C++ (Aurora Higher Education and Research Academy)

A procedural **C++17** mini project integrated directly with a **Supabase PostgreSQL** cloud database via the PostgreSQL C client library (**`libpq`**). This system demonstrates hand-rolled searching and sorting algorithms (Insertion Sort, Linear Search, Iterative Binary Search, Maximum Linear Scan) and integer paise currency handling, adhering strictly to 2nd-year B.Tech syllabus rules.

---

## Features Overview

1. **Add Customer Account**: Create `SAVINGS` or `CURRENT` accounts with mandatory minimum initial balance rules.
2. **Display All Customers**: Aligned ASCII tabular output showing account details and balances.
3. **Search Customer by Account Number**: $O(\log n)$ lookup using hand-rolled **Iterative Binary Search**.
4. **Deposit Funds**: Atomically deposit funds using PostgreSQL row locking (`FOR UPDATE`) and transaction audit logging.
5. **Withdraw Funds**: Withdraw funds with balance verification and strict enforcement of the ₹500.00 minimum balance for `SAVINGS` accounts.
6. **Display Balance**: Instant balance check for any account.
7. **Find Customer with Highest Balance**: Manual $O(n)$ linear scan identifying top balance account(s), correctly handling multi-customer ties.
8. **Sort Customers by Balance**: Hand-coded **Insertion Sort** offering user-selectable Ascending or Descending order.
9. **Update Customer Details**: Safely modify name, phone number, and email.
10. **Delete / Close Account**: Permanently remove account records with explicit confirmation and cascading transaction deletion.
11. **View Transaction History**: Comprehensive audit log showing all historical opening deposits, deposits, and withdrawals.
12. **Exit**: Gracefully close connection resources.

---

## Tech Stack & Architecture

- **Language**: Procedural C++17 (Whitelist: `struct`, `vector`, `string`, `cin`/`cout`, `switch`, loops, plain functions)
- **Database**: PostgreSQL (Hosted on Supabase)
- **Database Driver**: `libpq` (PostgreSQL C API `<libpq-fe.h>`)
- **Build System**: CMake (Minimum 3.15) OR Plain `g++` CLI command
- **Architecture Pattern**: Strict Layered Modular Architecture
  - `src/main.cpp` $\rightarrow$ Menu loop (`switch`) and user prompt dispatching.
  - `src/Bank.cpp` $\rightarrow$ Procedural business rules & hand-crafted DSA algorithms on `vector<Customer>`.
  - `src/Database.cpp` $\rightarrow$ The **ONLY** file containing `<libpq-fe.h>` and SQL statements. Database handle hidden as `static PGconn* conn = NULL;`.
  - `src/Utils.cpp` $\rightarrow$ The **ONLY** file reading raw console input (`cin`), input validation, and integer paise currency conversion.

---

## Data Model & Currency Safety

> [!IMPORTANT]
> **Floating-Point Money Rule**: IEEE 754 floating-point numbers (`float`, `double`) suffer from precision rounding errors. In this project, money is stored internally as **`long long` integer paise** ($1 \text{ Rupee} = 100 \text{ Paise}$). Conversions occur strictly at the I/O boundary by traversing strings character by character:
> - Input `"1234.50"` $\rightarrow$ Parsed to `123450` paise.
> - Output `123450` paise $\rightarrow$ Formatted to `"1234.50"`.
> - Inputs with 3+ decimal places are strictly rejected.

---

## Setup Instructions

### 1. Supabase Cloud Database Setup

1. Log in to [Supabase](https://supabase.com) and create a new project.
2. Navigate to the **SQL Editor** tab.
3. Open `sql/schema.sql` from this repository, paste its contents into the editor, and click **Run**.
4. Navigate to **Project Settings** $\rightarrow$ **Database** $\rightarrow$ **Connection String**.
5. Select **URI** under **Session Pooler** (Port `5432`, IPv4 compatible).
6. Copy the connection string. It will look like:
   ```text
   postgresql://postgres.<project-ref>:[YOUR-PASSWORD]@aws-0-[region].pooler.supabase.com:5432/postgres?sslmode=require
   ```
   *(Replace `[YOUR-PASSWORD]` with your actual database password).*

---

### 2. Configuration Setup

The application looks for the connection string in two places:
1. Environment variable `DB_CONNECTION_STRING`
2. Local `config.txt` file (Fallback)

To use `config.txt`:
1. Copy `config.txt.example` to `config.txt`:
   ```bash
   cp config.txt.example config.txt
   ```
2. Paste your valid Supabase Session Pooler connection string into line 1 of `config.txt`.

> [!CAUTION]
> `config.txt` contains sensitive credentials and is listed in `.gitignore`. Never commit `config.txt` to version control.

---

### 3. Prerequisites & Installation

#### Option A: MSYS2 / MinGW-w64 (Recommended)
```bash
pacman -S --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-postgresql
```

#### Option B: vcpkg
```cmd
vcpkg install libpq:x64-windows
```

---

## Build & Run Guide

### Method 1: Building with Plain `g++` (CLI)

```bash
# Compile main application
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Database.cpp src/Bank.cpp src/Utils.cpp -lpq -o bank_app

# Compile DB-free unit tests
g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_algorithms.cpp src/Bank.cpp src/Utils.cpp src/Database.cpp -lpq -o test_algorithms

# Run unit tests
./test_algorithms

# Run main application
./bank_app
```

### Method 2: Building with CMake

```bash
# Generate build files
cmake -B build

# Compile project
cmake --build build

# Run main application
./build/bank_app
```

---

## Minimum Balance Rules Summary

- **SAVINGS Account**:
  - Minimum Opening Balance: **₹500.00** (`50000` paise)
  - Minimum Maintained Balance: **₹500.00** (`50000` paise)
- **CURRENT Account**:
  - Minimum Opening Balance: **₹0.00** (`0` paise)
  - Minimum Maintained Balance: **₹0.00** (`0` paise)
