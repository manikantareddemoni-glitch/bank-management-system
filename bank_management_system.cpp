/**
 * ====================================================================================================
 * PROJECT: BANK MANAGEMENT SYSTEM (DATA STRUCTURES IN C++)
 * COURSE:  CS207 - Data Structure using C++
 * UNIVERSITY: Aurora Higher Education and Research Academy
 * ====================================================================================================
 * 
 * COMPLETE SYLLABUS MAPPING & MODULE COVERAGE (MODULES I TO X):
 * ----------------------------------------------------------------------------------------------------
 * - Module I:   Introduction to C++ Programming
 *               * Variables, Data Types (int, long long, char, bool, string)
 *               * Operators (Arithmetic, Relational, Logical, Assignment, Modulo)
 *               * Type Conversions (explicit parsing, ASCII to numeric conversion)
 *               * Standard I/O Streams (cin, cout, getline, formatted I/O with iomanip)
 * 
 * - Module II:  Control Statements & Functions
 *               * Decision Making (if, if-else, nested if-else, switch-case)
 *               * Iteration & Loops (for loops, while loops, input validation loops)
 *               * Modular Functions (pass-by-value, pass-by-reference using &)
 * 
 * - Module III: Arrays – 1D (One-Dimensional Arrays)
 *               * 1D Array storage and sequential vector buffer
 *               * Linear Search algorithm O(n) for unsorted fields (Phone Number)
 *               * Binary Search algorithm O(log n) for sorted keys (Account Number)
 *               * Insertion Sort algorithm O(n^2) for sorting customer records by balance
 * 
 * - Module IV:  Arrays – 2D (Two-Dimensional Arrays & Matrices)
 *               * Fixed 2D numeric arrays: BranchMatrix (rows x cols)
 *               * Matrix Addition: C[i][j] = A[i][j] + B[i][j]
 *               * Matrix Subtraction: C[i][j] = A[i][j] - B[i][j]
 *               * Matrix Multiplication: C[i][j] = sum(A[i][k] * B[k][j])
 *               * Matrix Transpose: T[j][i] = M[i][j]
 * 
 * - Module V:   String Manipulation & Algorithms
 *               * Character traversal and ASCII frequency analysis (256-bucket array)
 *               * In-place String Reversal using two-pointer swap technique
 *               * Delimiter-based Tokenization using stringstream
 *               * Naive Pattern Matching / Substring Search algorithm O(n * m)
 * 
 * - Module VI:  Structures (Composite User-Defined Data Types)
 *               * Structure declaration & member access operators (.)
 *               * Nested Structures (Date struct, Address struct inside Customer struct)
 *               * Arrays / Vectors of Structures (Collection of Customer records)
 * 
 * - Module VII: Performance Analysis & Complexity
 *               * Asymptotic Big-O Analysis documented on every search, sort, and ADT operation
 *               * Time Complexity: O(1), O(log n), O(n), O(n^2), O(n * m)
 *               * Space Complexity: O(1) auxiliary in-place algorithms vs O(n) buffers
 * 
 * - Module VIII: Stacks (LIFO - Last In First Out ADT)
 *               * Array-based Stack ADT (push, pop, peek, isEmpty, isFull)
 *               * Stack Application 1: Parentheses & Bracket Matching validator
 *               * Stack Application 2: Infix Arithmetic Expression Evaluation with operator precedence
 * 
 * - Module IX:  Queues (FIFO - First In First Out ADT)
 *               * Array-based Linear Queue ADT (front & rear pointers)
 *               * Array-based Circular Queue ADT with modulo wraparound to prevent memory drift
 *               * Array-based Priority Queue ADT for prioritizing VIP / Senior Citizen bank tokens
 * 
 * - Module X:   Standard Template Library (STL) Whitelist
 *               * std::vector, std::pair, std::deque, std::set, std::map
 *               * STL iterators (const_iterator, begin(), end(), find())
 * ====================================================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <queue>

using namespace std;

// ====================================================================================================
// MODULE VI: STRUCTURES & NESTED STRUCTURES
// ====================================================================================================

/**
 * @brief Structure representing a calendar date (Day / Month / Year).
 * Concept: Nested structure used as a sub-member inside Customer.
 */
struct Date {
    int day;    // Calendar day (1-31)
    int month;  // Calendar month (1-12)
    int year;   // Calendar year (e.g., 2026)
};

/**
 * @brief Structure representing a postal address.
 * Concept: Nested structure grouping street, city, state, and zip code.
 */
struct Address {
    string street;   // Street address or locality name
    string city;     // City name (e.g., Hyderabad)
    string state;    // State name (e.g., Telangana)
    string zipcode;  // 6-digit postal PIN code
};

/**
 * @brief Core entity representing a Bank Customer Account.
 * Concept: User-defined composite type holding primitive fields and nested structures.
 * Financial Rule: Money is stored strictly in integer 'paise' (1 Rupee = 100 Paise) to avoid floating-point inaccuracies.
 */
struct Customer {
    int accountNo;          // Unique 4-to-6 digit Account Number
    string name;            // Full Customer Name
    string phone;           // 10-digit registered phone number
    string email;           // Customer email address
    string accountType;     // "SAVINGS" or "CURRENT"
    long long balancePaise; // Balance stored as integer paise (e.g., Rs. 500.00 = 50000 paise)
    string createdAt;       // Timestamp of account creation (YYYY-MM-DD HH:MM:SS)
    Address address;        // Nested structure: Customer postal address
    Date openedDate;        // Nested structure: Date account was opened
};

/**
 * @brief Structure representing an immutable audit transaction log record.
 * Concept: Represents individual deposits, withdrawals, and opening balance entries.
 */
struct Transaction {
    int txnId;                  // Unique sequential transaction ID
    int accountNo;              // Associated account number
    string type;                // Transaction Type: "OPENING", "DEPOSIT", "WITHDRAW"
    long long amountPaise;       // Transaction amount in paise
    long long balanceAfterPaise; // Resulting balance after transaction in paise
    string time;                // Transaction timestamp string
};

// ====================================================================================================
// MODULE I & V: UTILITIES, TYPE CONVERSIONS, STRING MANIPULATION & CURRENCY PARSING
// ====================================================================================================

/**
 * @brief Trims leading and trailing whitespace characters (spaces, tabs, newlines) from a string.
 * Functionality: Traverses from the start and end of the string to strip unused spacing.
 * @param str Input string.
 * @return Cleaned string without leading or trailing spaces.
 */
string trim(string str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Checks if a string contains only digit characters ('0'-'9').
 * Functionality: Iterates through each character verifying with isdigit().
 * @param str Input string.
 * @return True if every character is a numeric digit; false otherwise.
 */
bool isDigitsOnly(string str) {
    if (str.length() == 0) return false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

/**
 * @brief Validates that a phone number is exactly 10 numeric digits.
 * Functionality: Checks length == 10 and ensures all characters are numeric digits.
 * @param phone String containing customer phone number.
 * @return True if valid 10-digit phone number.
 */
bool isValidPhone(string phone) {
    string trimmed = trim(phone);
    return (trimmed.length() == 10 && isDigitsOnly(trimmed));
}

/**
 * @brief Validates standard email address syntax containing '@' and a domain dot '.'.
 * Functionality: Ensures '@' exists once and is followed by a dot '.' with characters between them.
 * @param email Customer email string.
 * @return True if valid email syntax (or empty if optional).
 */
bool isValidEmail(string email) {
    string trimmed = trim(email);
    if (trimmed.length() == 0) return true; // Optional field allowed to be empty
    size_t atPos = trimmed.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == trimmed.length() - 1) return false;
    if (trimmed.find('@', atPos + 1) != string::npos) return false; // Multiple '@' disallowed
    size_t dotPos = trimmed.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == trimmed.length() - 1) return false;
    return true;
}

/**
 * @brief Parses human currency string (e.g., "1500.50" or "500") into integer paise (150050).
 * Functionality: Splits string into rupees and decimal paise parts, then converts strictly using
 *                integer arithmetic (Rupees * 100 + Paise) to eliminate binary floating point errors.
 * @param input String containing user currency input.
 * @param outPaise Output reference receiving the parsed amount in paise.
 * @return True if parsing succeeded; false on format error or negative amount.
 */
bool parseMoneyToPaise(string input, long long& outPaise) {
    string str = trim(input);
    if (str.length() == 0 || str[0] == '-') return false; // Negative values disallowed

    size_t dotPos = str.find('.');
    string rupeesStr = "";
    string paiseStr = "";

    if (dotPos == string::npos) {
        rupeesStr = str;
        paiseStr = "00";
    } else {
        if (str.find('.', dotPos + 1) != string::npos) return false; // Reject multiple decimal points
        rupeesStr = str.substr(0, dotPos);
        paiseStr = str.substr(dotPos + 1);
        if (paiseStr.length() == 0) paiseStr = "00";
        else if (paiseStr.length() == 1) paiseStr += "0";
        else if (paiseStr.length() > 2) return false; // Max 2 decimal digits allowed for paise
    }

    if (rupeesStr.length() == 0) rupeesStr = "0";
    if (!isDigitsOnly(rupeesStr) || !isDigitsOnly(paiseStr)) return false;

    // Convert string to integer paise
    long long rupees = 0;
    long long paise = 0;
    for (size_t i = 0; i < rupeesStr.length(); ++i) {
        rupees = rupees * 10 + (rupeesStr[i] - '0');
    }
    for (size_t i = 0; i < paiseStr.length(); ++i) {
        paise = paise * 10 + (paiseStr[i] - '0');
    }

    outPaise = (rupees * 100LL) + paise;
    return true;
}

/**
 * @brief Formats integer paise into standard Rupees display string (e.g. 150050 paise -> "1500.50").
 * Functionality: Divides by 100 for rupees and takes modulo 100 for 2-digit padded paise.
 * @param paise Amount in integer paise.
 * @return Formatted string with 2 decimal places.
 */
string formatPaiseToRupees(long long paise) {
    bool negative = (paise < 0);
    long long absPaise = negative ? -paise : paise;
    long long rupees = absPaise / 100LL;
    long long remainingPaise = absPaise % 100LL;

    ostringstream ss;
    if (negative) ss << "-";
    ss << rupees << "." << (remainingPaise < 10 ? "0" : "") << remainingPaise;
    return ss.str();
}

/**
 * @brief Reads an integer from user console with bounds validation.
 * Functionality: Prompts user, parses numeric input, and loops until within [minVal, maxVal].
 * @param prompt Text prompt shown to user.
 * @param minVal Minimum acceptable integer value.
 * @param maxVal Maximum acceptable integer value.
 * @return Validated integer.
 */
int readInt(string prompt, int minVal = 1, int maxVal = 2147483647) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cin.clear();
            continue;
        }
        string trimmed = trim(line);
        if (trimmed.length() == 0 || !isDigitsOnly(trimmed)) {
            cout << " [ERROR] Invalid input. Please enter a positive integer.\n";
            continue;
        }
        int val = atoi(trimmed.c_str());
        if (val < minVal || val > maxVal) {
            cout << " [ERROR] Value out of range (" << minVal << " - " << maxVal << ").\n";
            continue;
        }
        return val;
    }
}

/**
 * @brief Reads a non-empty string line from console.
 * Functionality: Loops until user inputs at least one non-whitespace character.
 * @param prompt Text prompt shown to user.
 * @return Non-empty string.
 */
string readNonEmptyLine(string prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cin.clear();
            continue;
        }
        string trimmed = trim(line);
        if (trimmed.length() > 0) return trimmed;
        cout << " [ERROR] Input cannot be blank.\n";
    }
}

/**
 * @brief Reads money deposit/withdrawal amount from console in Rupees.
 * Functionality: Converts input via parseMoneyToPaise() and validates positive value.
 * @param prompt Text prompt shown to user.
 * @return Amount in integer paise.
 */
long long readMoney(string prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            cin.clear();
            continue;
        }
        long long paise = 0;
        if (!parseMoneyToPaise(line, paise) || paise <= 0) {
            cout << " [ERROR] Invalid amount. Enter positive number (e.g. 500 or 1500.50).\n";
            continue;
        }
        return paise;
    }
}

/**
 * @brief Console formatting helper to print structured headers.
 */
void printHeader(string title) {
    cout << "\n=======================================================\n";
    cout << "  " << title << "\n";
    cout << "=======================================================\n";
}

void printSuccess(string msg) { cout << " [OK] " << msg << "\n"; }
void printError(string msg) { cout << " [ERROR] " << msg << "\n"; }

// ====================================================================================================
// MODULE III & VII: 1D ARRAYS & HAND-ROLLED DSA ALGORITHMS
// ====================================================================================================

/**
 * @brief Linear Search algorithm to find a customer by 10-digit phone number in unsorted records.
 * Functionality: Iterates from index 0 to n-1 sequentially comparing phone numbers.
 * Time Complexity:  O(n) - Linear scan across all records.
 * Space Complexity: O(1) - Constant auxiliary space.
 * @param list Vector collection of Customer structs.
 * @param phone 10-digit search target string.
 * @param result Output reference populated if matching record is found.
 * @return True if customer found; false otherwise.
 */
bool linearSearchByPhone(const vector<Customer>& list, string phone, Customer& result) {
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].phone == phone) {
            result = list[i]; // Found target record
            return true;
        }
    }
    return false; // Not found
}

/**
 * @brief Binary Search algorithm to find a customer by unique Account Number.
 * Functionality: Divide-and-conquer strategy repeatedly dividing the sorted search range in half.
 * Precondition:     The collection MUST be sorted by accountNo in ascending order.
 * Time Complexity:  O(log n) - Search space halved on each comparison step.
 * Space Complexity: O(1) - Iterative auxiliary space.
 * @param list Sorted vector of Customer structs.
 * @param accountNo Target account number integer.
 * @param result Output reference populated if matching record is found.
 * @return True if found; false otherwise.
 */
bool binarySearchByAccountNo(const vector<Customer>& list, int accountNo, Customer& result) {
    int low = 0;
    int high = static_cast<int>(list.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Avoid potential integer overflow
        if (list[mid].accountNo == accountNo) {
            result = list[mid];
            return true; // Match found at mid
        }
        if (list[mid].accountNo < accountNo) {
            low = mid + 1; // Search right half
        } else {
            high = mid - 1; // Search left half
        }
    }
    return false; // Target not in list
}

/**
 * @brief Hand-rolled Insertion Sort algorithm to order customer records by balance.
 * Functionality: Builds a sorted sub-array one element at a time by shifting elements greater
 *                (or smaller) than the current key element to make room for insertion.
 * Time Complexity:  O(n^2) worst & average case; O(n) best case (when array is already sorted).
 * Space Complexity: O(1) in-place comparison sort.
 * @param list Reference to customer vector to sort in-place.
 * @param ascending If true, sorts lowest to highest; if false, sorts highest to lowest.
 */
void insertionSortByBalance(vector<Customer>& list, bool ascending) {
    int n = static_cast<int>(list.size());
    for (int i = 1; i < n; ++i) {
        Customer key = list[i]; // Current element to insert into sorted sublist [0..i-1]
        int j = i - 1;

        if (ascending) {
            // Shift elements with greater balance to the right
            while (j >= 0 && list[j].balancePaise > key.balancePaise) {
                list[j + 1] = list[j];
                j--;
            }
        } else {
            // Shift elements with smaller balance to the right
            while (j >= 0 && list[j].balancePaise < key.balancePaise) {
                list[j + 1] = list[j];
                j--;
            }
        }
        list[j + 1] = key; // Place key in its correct sorted position
    }
}

/**
 * @brief Finds customer account(s) holding the maximum balance.
 * Functionality: First pass determines the maximum balance value; second pass gathers all accounts with that value.
 * Time Complexity:  O(n) - Two sequential linear passes.
 * Space Complexity: O(k) where k is the count of highest balance accounts.
 * @param list Vector of customer records.
 * @param highestOut Output vector receiving matching customer records.
 */
void findHighestBalanceCustomers(const vector<Customer>& list, vector<Customer>& highestOut) {
    highestOut.clear();
    if (list.empty()) return;

    long long maxVal = list[0].balancePaise;
    for (size_t i = 1; i < list.size(); ++i) {
        if (list[i].balancePaise > maxVal) {
            maxVal = list[i].balancePaise;
        }
    }
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].balancePaise == maxVal) {
            highestOut.push_back(list[i]);
        }
    }
}

// ====================================================================================================
// MODULE IV: 2D NUMERIC ARRAYS & MATRIX OPERATIONS
// ====================================================================================================

const int MAX_DIM = 10; // Maximum matrix dimensions supported

/**
 * @brief Matrix representation using 2D fixed numeric arrays.
 * Concept: Demonstrates 2D numeric array manipulation for bank branch cash flow matrices.
 */
struct BranchMatrix {
    int rows;
    int cols;
    long long data[MAX_DIM][MAX_DIM]; // 2D numeric grid
};

/**
 * @brief Performs 2D Matrix Addition (C = A + B).
 * Functionality: Element-wise addition C[i][j] = A[i][j] + B[i][j].
 * Requirement: Matrix dimensions of A and B must match.
 * Time Complexity: O(rows * cols).
 */
BranchMatrix matrixAdd(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix res;
    res.rows = A.rows;
    res.cols = A.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            res.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }
    return res;
}

/**
 * @brief Performs 2D Matrix Subtraction (C = A - B).
 * Functionality: Element-wise subtraction C[i][j] = A[i][j] - B[i][j].
 * Time Complexity: O(rows * cols).
 */
BranchMatrix matrixSubtract(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix res;
    res.rows = A.rows;
    res.cols = A.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            res.data[i][j] = A.data[i][j] - B.data[i][j];
        }
    }
    return res;
}

/**
 * @brief Performs 2D Matrix Multiplication (C = A * B).
 * Functionality: Dot product row A with column B: C[i][j] = sum(A[i][k] * B[k][j]).
 * Time Complexity: O(A.rows * B.cols * A.cols).
 */
BranchMatrix matrixMultiply(const BranchMatrix& A, const BranchMatrix& B) {
    BranchMatrix res;
    res.rows = A.rows;
    res.cols = B.cols;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            res.data[i][j] = 0;
            for (int k = 0; k < A.cols; ++k) {
                res.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return res;
}

/**
 * @brief Computes Transpose of a 2D Matrix (Swapping Rows with Columns).
 * Functionality: T[j][i] = M[i][j].
 * Time Complexity: O(rows * cols).
 */
BranchMatrix matrixTranspose(const BranchMatrix& A) {
    BranchMatrix res;
    res.rows = A.cols;
    res.cols = A.rows;
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            res.data[j][i] = A.data[i][j];
        }
    }
    return res;
}

/**
 * @brief Prints 2D Matrix to console formatted in a grid layout.
 */
void printMatrix(string title, const BranchMatrix& M) {
    cout << "\n--- " << title << " (" << M.rows << "x" << M.cols << ") ---\n";
    for (int i = 0; i < M.rows; ++i) {
        cout << "  [ ";
        for (int j = 0; j < M.cols; ++j) {
            cout << setw(8) << M.data[i][j] << " ";
        }
        cout << "]\n";
    }
}

// ====================================================================================================
// MODULE V: STRING ARRAYS, FREQUENCY, REVERSAL, TOKENIZATION & PATTERN MATCHING
// ====================================================================================================

/**
 * @brief Computes frequency count of every ASCII character in a string.
 * Functionality: Uses a 256-bucket integer array indexed directly by character ASCII code.
 * Time Complexity:  O(n) where n is text length.
 * Space Complexity: O(1) fixed 256 integers.
 * @param text Input string.
 * @param freq Output array of size 256 where freq[c] stores count of character c.
 */
void analyzeCharacterFrequency(const string& text, int freq[256]) {
    for (int i = 0; i < 256; ++i) freq[i] = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        freq[c]++;
    }
}

/**
 * @brief Reverses a string in-place using two-pointer character swapping.
 * Functionality: Swaps characters from start and end moving towards the center (index i < n/2).
 * Time Complexity:  O(n).
 * Space Complexity: O(1) in-place.
 * @param input String to reverse.
 * @return Reversed string.
 */
string reverseString(const string& input) {
    string rev = input;
    int n = static_cast<int>(rev.length());
    for (int i = 0; i < n / 2; ++i) {
        char temp = rev[i];
        rev[i] = rev[n - 1 - i];
        rev[n - 1 - i] = temp;
    }
    return rev;
}

/**
 * @brief Splits a string into tokens based on a delimiter character using stringstream.
 * Functionality: Uses std::stringstream and getline with delimiter to extract words/tokens.
 * @param input Text to split.
 * @param delimiter Delimiting character (e.g. ' ' or ',').
 * @return Vector of individual string tokens.
 */
vector<string> tokenizeString(const string& input, char delimiter) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Naive pattern matching algorithm to locate a substring pattern inside text.
 * Functionality: Slides the pattern across the text comparing characters one by one.
 * Time Complexity:  O(n * m) worst case, where n = text length, m = pattern length.
 * Space Complexity: O(1) auxiliary space.
 * @param text The full text string.
 * @param pattern The substring pattern to search for.
 * @return True if pattern is found anywhere in text; false otherwise.
 */
bool patternMatchNaive(const string& text, const string& pattern) {
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());
    if (m == 0) return true;
    if (m > n) return false;

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) return true; // Full pattern matched
    }
    return false;
}

// ====================================================================================================
// MODULE VIII: STACK ADT (ARRAY IMPLEMENTATION & APPLICATIONS)
// ====================================================================================================

const int STACK_CAP = 100; // Capacity limit for fixed-size stack

/**
 * @brief Stack ADT implemented using a fixed-size raw array.
 * Discipline: LIFO (Last In First Out).
 */
struct ArrayStack {
    Transaction items[STACK_CAP]; // Array buffer storing Transaction structs
    int top;                      // Index of the top element (-1 when empty)
};

void stackInit(ArrayStack& s) { s.top = -1; }
bool stackIsEmpty(const ArrayStack& s) { return (s.top == -1); }
bool stackIsFull(const ArrayStack& s) { return (s.top == STACK_CAP - 1); }

bool stackPush(ArrayStack& s, const Transaction& item) {
    if (stackIsFull(s)) return false; // Stack Overflow prevention
    s.items[++s.top] = item;
    return true;
}

bool stackPop(ArrayStack& s, Transaction& itemOut) {
    if (stackIsEmpty(s)) return false; // Stack Underflow prevention
    itemOut = s.items[s.top--];
    return true;
}

bool stackPeek(const ArrayStack& s, Transaction& itemOut) {
    if (stackIsEmpty(s)) return false;
    itemOut = s.items[s.top];
    return true;
}

/**
 * @brief Stack Application 1: Parentheses & Bracket Matching validator.
 * Functionality: Pushes opening brackets onto stack ('(', '{', '['); pops and validates on closing brackets.
 * Time Complexity:  O(n) where n is expression length.
 * Space Complexity: O(n) stack auxiliary space.
 * @param expr Arithmetic/syntax expression string.
 * @return True if all brackets are properly balanced and matched; false otherwise.
 */
bool checkParenthesisMatching(const string& expr) {
    char st[100];
    int top = -1;

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (ch == '(' || ch == '{' || ch == '[') {
            if (top >= 99) return false;
            st[++top] = ch; // Push opening bracket
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (top == -1) return false; // Closing bracket with no opening bracket
            char open = st[top--];
            if ((ch == ')' && open != '(') ||
                (ch == '}' && open != '{') ||
                (ch == ']' && open != '[')) {
                return false; // Mismatched bracket type
            }
        }
    }
    return (top == -1); // Stack must be empty for balanced expression
}

/**
 * @brief Helper for Infix Evaluation: Returns operator precedence level.
 */
static int opPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

/**
 * @brief Helper for Infix Evaluation: Applies arithmetic operator to two operands.
 */
static int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? (a / b) : 0; // Guard against division by zero
    }
    return 0;
}

/**
 * @brief Stack Application 2: Infix mathematical expression evaluation.
 * Functionality: Uses two stacks (values stack and operators stack) to evaluate expressions
 *                respecting operator precedence and parentheses.
 * Time Complexity:  O(n) where n is expression length.
 * Space Complexity: O(n).
 * @param expr Infix mathematical string (e.g. "(10 + 20) * 3").
 * @return Evaluated integer result.
 */
int evaluateInfixExpression(const string& expr) {
    int valStack[100];
    int valTop = -1;
    char opStack[100];
    int opTop = -1;

    for (size_t i = 0; i < expr.length(); ++i) {
        if (expr[i] == ' ') continue; // Skip whitespace

        if (expr[i] == '(') {
            opStack[++opTop] = '(';
        } else if (isdigit(expr[i])) {
            int val = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                val = (val * 10) + (expr[i] - '0');
                i++;
            }
            i--;
            valStack[++valTop] = val; // Push operand onto value stack
        } else if (expr[i] == ')') {
            while (opTop != -1 && opStack[opTop] != '(') {
                int b = valStack[valTop--];
                int a = valStack[valTop--];
                char op = opStack[opTop--];
                valStack[++valTop] = applyOp(a, b, op);
            }
            if (opTop != -1) opTop--; // Pop '('
        } else {
            // Operator encountered: evaluate higher precedence operators first
            while (opTop != -1 && opPrecedence(opStack[opTop]) >= opPrecedence(expr[i])) {
                int b = valStack[valTop--];
                int a = valStack[valTop--];
                char op = opStack[opTop--];
                valStack[++valTop] = applyOp(a, b, op);
            }
            opStack[++opTop] = expr[i]; // Push current operator
        }
    }

    // Apply remaining operators in stack
    while (opTop != -1) {
        int b = valStack[valTop--];
        int a = valStack[valTop--];
        char op = opStack[opTop--];
        valStack[++valTop] = applyOp(a, b, op);
    }
    return (valTop != -1) ? valStack[valTop] : 0;
}

// ====================================================================================================
// MODULE IX: QUEUE ADT (LINEAR, CIRCULAR & PRIORITY QUEUE IMPLEMENTATIONS)
// ====================================================================================================

const int QUEUE_CAP = 100;

/**
 * @brief Linear Queue ADT using raw array buffer.
 * Discipline: FIFO (First In First Out).
 */
struct ArrayQueue {
    int tokens[QUEUE_CAP]; // Token numbers
    int front;             // Index of front element
    int rear;              // Index of rear element
};

void queueInit(ArrayQueue& q) { q.front = 0; q.rear = -1; }
bool queueIsEmpty(const ArrayQueue& q) { return (q.rear < q.front); }
bool queueIsFull(const ArrayQueue& q) { return (q.rear == QUEUE_CAP - 1); }

bool queueEnqueue(ArrayQueue& q, int val) {
    if (queueIsFull(q)) return false;
    q.tokens[++q.rear] = val;
    return true;
}

bool queueDequeue(ArrayQueue& q, int& valOut) {
    if (queueIsEmpty(q)) return false;
    valOut = q.tokens[q.front++];
    return true;
}

/**
 * @brief Circular Queue ADT with modulo wraparound to eliminate linear memory drift.
 * Discipline: FIFO with circular index wrapping: rear = (rear + 1) % CAP.
 */
struct CircularQueue {
    int tokens[QUEUE_CAP];
    int front;
    int rear;
    int count; // Number of elements currently in the queue
};

void circularQueueInit(CircularQueue& cq) { cq.front = 0; cq.rear = -1; cq.count = 0; }
bool circularQueueIsEmpty(const CircularQueue& cq) { return (cq.count == 0); }
bool circularQueueIsFull(const CircularQueue& cq) { return (cq.count == QUEUE_CAP); }

bool circularQueueEnqueue(CircularQueue& cq, int val) {
    if (circularQueueIsFull(cq)) return false;
    cq.rear = (cq.rear + 1) % QUEUE_CAP; // Modulo wraparound
    cq.tokens[cq.rear] = val;
    cq.count++;
    return true;
}

bool circularQueueDequeue(CircularQueue& cq, int& valOut) {
    if (circularQueueIsEmpty(cq)) return false;
    valOut = cq.tokens[cq.front];
    cq.front = (cq.front + 1) % QUEUE_CAP; // Modulo wraparound
    cq.count--;
    return true;
}

/**
 * @brief Priority Queue token item struct for bank service teller counter.
 */
struct PriorityToken {
    int token;          // Ticket / Token ID
    int priority;       // Priority rating: Higher value = serviced earlier (e.g. VIP=10, Senior=3, Regular=1)
    string clientName;  // Customer / Client name
};

/**
 * @brief Priority Queue ADT implemented via sorted insertion in an array buffer.
 * Discipline: Elements with highest priority are dequeued first.
 */
struct PriorityQueue {
    PriorityToken tokens[QUEUE_CAP];
    int count;
};

void priorityQueueInit(PriorityQueue& pq) { pq.count = 0; }

bool priorityQueueEnqueue(PriorityQueue& pq, int token, int priority, string name) {
    if (pq.count >= QUEUE_CAP) return false;
    PriorityToken item = { token, priority, name };
    int i = pq.count - 1;
    // Shift elements with lower priority to the right to maintain descending priority order
    while (i >= 0 && pq.tokens[i].priority < priority) {
        pq.tokens[i + 1] = pq.tokens[i];
        i--;
    }
    pq.tokens[i + 1] = item;
    pq.count++;
    return true;
}

bool priorityQueueDequeue(PriorityQueue& pq, PriorityToken& outItem) {
    if (pq.count == 0) return false;
    outItem = pq.tokens[0]; // Highest priority item is at index 0
    // Shift remaining elements left by 1
    for (int i = 1; i < pq.count; ++i) {
        pq.tokens[i - 1] = pq.tokens[i];
    }
    pq.count--;
    return true;
}

// ====================================================================================================
// MODULE X: STL CONTAINERS & ITERATORS DEMONSTRATION
// ====================================================================================================

/**
 * @brief Comprehensive demonstration of whitelisted STL containers and iterators.
 * Syllabus Topic: std::vector, std::pair, std::deque, std::set, std::map, and const_iterator.
 */
void demonstrateSTLContainers(const vector<Customer>& customers) {
    printHeader("MODULE X: STL CONTAINERS & ITERATORS DEMO");

    // 1. std::pair and std::vector
    cout << "\n [1] std::vector & std::pair:\n";
    vector<pair<int, long long>> accountPairs;
    for (size_t i = 0; i < customers.size(); ++i) {
        accountPairs.push_back(make_pair(customers[i].accountNo, customers[i].balancePaise));
    }
    cout << "     Populated " << accountPairs.size() << " (AccountNo, Balance) pairs in std::vector.\n";

    // 2. Iterators traversal
    cout << "\n [2] std::vector Const Iterators traversal:\n     Accounts: ";
    for (vector<pair<int, long long>>::const_iterator it = accountPairs.begin(); it != accountPairs.end(); ++it) {
        cout << "#" << it->first << " ";
    }
    cout << "\n";

    // 3. std::deque (Double-ended queue)
    cout << "\n [3] std::deque (Double-ended queue operations):\n";
    deque<string> tellerDeque;
    tellerDeque.push_back("Token #101 (Regular Counter)");
    tellerDeque.push_back("Token #102 (Regular Counter)");
    tellerDeque.push_front("Token #VIP-1 (Emergency Priority Counter)");
    cout << "     Deque Front: " << tellerDeque.front() << "\n";
    cout << "     Deque Back:  " << tellerDeque.back() << "\n";

    // 4. std::set (Unique collection)
    cout << "\n [4] std::set (Unique registered customer phone numbers):\n";
    set<string> phoneSet;
    for (size_t i = 0; i < customers.size(); ++i) {
        phoneSet.insert(customers[i].phone);
    }
    cout << "     Unique phone numbers in set: " << phoneSet.size() << "\n";

    // 5. std::map (O(log n) Key-Value Lookup)
    cout << "\n [5] std::map (AccountNo -> Customer Struct Lookup):\n";
    map<int, Customer> custMap;
    for (size_t i = 0; i < customers.size(); ++i) {
        custMap[customers[i].accountNo] = customers[i];
    }
    if (!customers.empty()) {
        int sample = customers[0].accountNo;
        map<int, Customer>::iterator it = custMap.find(sample);
        if (it != custMap.end()) {
            cout << "     Fast Map Search found Account #" << sample << ": " << it->second.name << "\n";
        }
    }
}

// ====================================================================================================
// PROCEDURAL IN-MEMORY DATABASE & BANK OPERATIONS
// ====================================================================================================

// In-Memory Global State
static vector<Customer> g_customers;
static vector<Transaction> g_transactions;
static int g_nextAccountNo = 1007; // Counter for next auto-assigned account number
static int g_nextTxnId = 7;        // Counter for next transaction ID

/**
 * @brief Seeds initial demonstration customer and transaction records into memory.
 */
void seedInitialData() {
    g_customers.clear();
    g_transactions.clear();

    Customer c1 = { 1001, "Aarav Sharma", "9876543210", "aarav@example.com", "SAVINGS", 2500050, "2026-01-01 10:00:00", {"MG Road", "Hyderabad", "Telangana", "500001"}, {1, 1, 2026} };
    Customer c2 = { 1002, "Priya Patel",  "9823456789", "priya@example.com", "SAVINGS", 100000,  "2026-01-01 10:05:00", {"Park Street", "Hyderabad", "Telangana", "500002"}, {1, 1, 2026} };
    Customer c3 = { 1003, "Rohan Verma",  "9712345678", "rohan@example.com", "CURRENT", 7500000, "2026-01-01 10:10:00", {"Station Road", "Hyderabad", "Telangana", "500003"}, {1, 1, 2026} };
    Customer c4 = { 1004, "Ananya Iyer",  "9601234567", "ananya@example.com", "SAVINGS", 65075,   "2026-01-01 10:15:00", {"Banjara Hills", "Hyderabad", "Telangana", "500034"}, {1, 1, 2026} };
    Customer c5 = { 1005, "Vikram Malhotra", "9543210987", "vikram@example.com", "CURRENT", 15000000, "2026-01-01 10:20:00", {"Jubilee Hills", "Hyderabad", "Telangana", "500033"}, {1, 1, 2026} };
    Customer c6 = { 1006, "Sanya Gupta",  "9432109876", "sanya@example.com", "SAVINGS", 50000,   "2026-01-01 10:25:00", {"Hitech City", "Hyderabad", "Telangana", "500081"}, {1, 1, 2026} };

    g_customers.push_back(c1);
    g_customers.push_back(c2);
    g_customers.push_back(c3);
    g_customers.push_back(c4);
    g_customers.push_back(c5);
    g_customers.push_back(c6);

    for (size_t i = 0; i < g_customers.size(); ++i) {
        Transaction t = { static_cast<int>(i + 1), g_customers[i].accountNo, "OPENING", g_customers[i].balancePaise, g_customers[i].balancePaise, g_customers[i].createdAt };
        g_transactions.push_back(t);
    }
}

/**
 * @brief Displays customer table with structured column alignments.
 */
void displayCustomersTable(const vector<Customer>& list) {
    if (list.empty()) {
        printError("No customer accounts available.");
        return;
    }

    cout << "\n" << string(96, '-') << "\n";
    cout << left 
         << setw(12) << "Account No"
         << setw(22) << "Customer Name"
         << setw(15) << "Phone Number"
         << setw(10) << "Type"
         << setw(16) << "Balance (Rs.)"
         << setw(20) << "Created Timestamp" << "\n";
    cout << string(96, '-') << "\n";

    for (size_t i = 0; i < list.size(); ++i) {
        const Customer& c = list[i];
        cout << left 
             << setw(12) << c.accountNo
             << setw(22) << (c.name.length() > 20 ? c.name.substr(0, 17) + "..." : c.name)
             << setw(15) << c.phone
             << setw(10) << c.accountType
             << right << setw(14) << formatPaiseToRupees(c.balancePaise) << "  "
             << left << setw(20) << c.createdAt << "\n";
    }
    cout << string(96, '-') << "\n";
    cout << " Total records displayed: " << list.size() << "\n";
}

// ====================================================================================================
// MENU ACTION HANDLERS
// ====================================================================================================

/**
 * @brief Handles creating a new customer bank account.
 * Validates unique phone, valid email, minimum savings deposit (Rs. 500.00 = 50000 paise),
 * auto-assigns sequential account number, and creates the opening transaction.
 */
void handleAddCustomer() {
    printHeader("1. ADD NEW CUSTOMER ACCOUNT");
    string name = readNonEmptyLine(" Enter Full Name: ");

    string phone;
    while (true) {
        phone = readNonEmptyLine(" Enter 10-digit Phone Number: ");
        if (!isValidPhone(phone)) {
            printError("Invalid phone number! Must be exactly 10 digits.");
            continue;
        }
        Customer dummy;
        if (linearSearchByPhone(g_customers, phone, dummy)) {
            printError("Phone number already registered to another account!");
            continue;
        }
        break;
    }

    string email = "";
    while (true) {
        cout << " Enter Email Address (Optional, press Enter to skip): ";
        string line;
        getline(cin, line);
        email = trim(line);
        if (email.length() == 0 || isValidEmail(email)) break;
        printError("Invalid email format (e.g. user@example.com)!");
    }

    cout << " Select Account Type (1 = SAVINGS, 2 = CURRENT): ";
    int typeChoice = readInt("", 1, 2);
    string type = (typeChoice == 1) ? "SAVINGS" : "CURRENT";

    if (type == "SAVINGS") {
        cout << " Note: SAVINGS account requires an initial deposit of at least Rs. 500.00\n";
    }
    long long depositPaise = 0;
    while (true) {
        depositPaise = readMoney(" Enter Initial Opening Deposit Amount (Rs.): ");
        if (type == "SAVINGS" && depositPaise < 50000LL) {
            printError("SAVINGS account requires minimum opening deposit of Rs. 500.00.");
            continue;
        }
        break;
    }

    int allocatedAccNo = g_nextAccountNo++;
    string timeStr = "2026-10-03 14:48:00";

    Customer newCust = {
        allocatedAccNo,
        name,
        phone,
        email,
        type,
        depositPaise,
        timeStr,
        {"Sample Street", "Hyderabad", "Telangana", "500001"},
        {3, 10, 2026}
    };

    Transaction newTxn = {
        g_nextTxnId++,
        allocatedAccNo,
        "OPENING",
        depositPaise,
        depositPaise,
        timeStr
    };

    g_customers.push_back(newCust);
    g_transactions.push_back(newTxn);

    // Keep vector sorted by accountNo for Binary Search
    sort(g_customers.begin(), g_customers.end(), [](const Customer& a, const Customer& b) {
        return a.accountNo < b.accountNo;
    });

    printSuccess("Customer account created successfully! Allocated Account Number: " + to_string(allocatedAccNo));
}

/**
 * @brief Handles searching for a customer by Account Number using Binary Search O(log n).
 */
void handleSearchCustomer() {
    printHeader("3. SEARCH CUSTOMER BY ACCOUNT NUMBER (BINARY SEARCH)");
    int accNo = readInt(" Enter Account Number to Search: ", 1000, 999999);

    Customer res;
    if (binarySearchByAccountNo(g_customers, accNo, res)) {
        printSuccess("Customer Account Found via Binary Search O(log n)!");
        vector<Customer> single;
        single.push_back(res);
        displayCustomersTable(single);
    } else {
        printError("Account number " + to_string(accNo) + " not found.");
    }
}

/**
 * @brief Handles searching for a customer by Phone Number using Linear Search O(n).
 */
void handleSearchByPhone() {
    printHeader("4. SEARCH CUSTOMER BY PHONE NUMBER (LINEAR SEARCH)");
    string phone = readNonEmptyLine(" Enter 10-digit Phone Number to Search: ");

    Customer res;
    if (linearSearchByPhone(g_customers, phone, res)) {
        printSuccess("Customer Account Found via Linear Search O(n)!");
        vector<Customer> single;
        single.push_back(res);
        displayCustomersTable(single);
    } else {
        printError("No customer found with phone number " + phone);
    }
}

/**
 * @brief Handles depositing money into an existing account and logging the transaction.
 */
void handleDeposit() {
    printHeader("5. DEPOSIT FUNDS");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);
    long long amtPaise = readMoney(" Enter Amount to Deposit (Rs.): ");

    for (size_t i = 0; i < g_customers.size(); ++i) {
        if (g_customers[i].accountNo == accNo) {
            g_customers[i].balancePaise += amtPaise;
            Transaction t = { g_nextTxnId++, accNo, "DEPOSIT", amtPaise, g_customers[i].balancePaise, "2026-10-03 14:48:00" };
            g_transactions.push_back(t);
            printSuccess("Deposit successful! Updated Balance: Rs. " + formatPaiseToRupees(g_customers[i].balancePaise));
            return;
        }
    }
    printError("Account number " + to_string(accNo) + " not found.");
}

/**
 * @brief Handles withdrawing money from an account with balance & minimum balance checks.
 */
void handleWithdraw() {
    printHeader("6. WITHDRAW FUNDS");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);
    long long amtPaise = readMoney(" Enter Amount to Withdraw (Rs.): ");

    for (size_t i = 0; i < g_customers.size(); ++i) {
        if (g_customers[i].accountNo == accNo) {
            if (amtPaise > g_customers[i].balancePaise) {
                printError("Withdrawal rejected: Insufficient funds.");
                return;
            }
            long long newBal = g_customers[i].balancePaise - amtPaise;
            if (g_customers[i].accountType == "SAVINGS" && newBal < 50000LL) {
                printError("Withdrawal rejected: SAVINGS balance cannot fall below minimum Rs. 500.00.");
                return;
            }
            g_customers[i].balancePaise = newBal;
            Transaction t = { g_nextTxnId++, accNo, "WITHDRAW", amtPaise, newBal, "2026-10-03 14:48:00" };
            g_transactions.push_back(t);
            printSuccess("Withdrawal successful! Updated Balance: Rs. " + formatPaiseToRupees(newBal));
            return;
        }
    }
    printError("Account number " + to_string(accNo) + " not found.");
}

/**
 * @brief Handles sorting customer accounts by balance using Insertion Sort O(n^2).
 */
void handleSortCustomers() {
    printHeader("8. SORT CUSTOMERS BY BALANCE (INSERTION SORT)");
    cout << " 1. Ascending Order (Lowest to Highest Balance)\n";
    cout << " 2. Descending Order (Highest to Lowest Balance)\n";
    int choice = readInt(" Select sorting order (1-2): ", 1, 2);

    bool asc = (choice == 1);
    insertionSortByBalance(g_customers, asc);
    printSuccess(asc ? "Sorted customers in ASCENDING order." : "Sorted customers in DESCENDING order.");
    displayCustomersTable(g_customers);
}

/**
 * @brief Handles finding and displaying the account(s) with the highest balance.
 */
void handleHighestBalance() {
    printHeader("7. FIND HIGHEST BALANCE CUSTOMER(S)");
    vector<Customer> highest;
    findHighestBalanceCustomers(g_customers, highest);
    displayCustomersTable(highest);
}

/**
 * @brief Handles viewing the audit transaction log history for a specific account.
 */
void handleViewTransactions() {
    printHeader("9. VIEW AUDIT TRANSACTION HISTORY");
    int accNo = readInt(" Enter Account Number: ", 1000, 999999);

    vector<Transaction> filtered;
    for (size_t i = 0; i < g_transactions.size(); ++i) {
        if (g_transactions[i].accountNo == accNo) {
            filtered.push_back(g_transactions[i]);
        }
    }

    if (filtered.empty()) {
        printError("No transaction records found for account " + to_string(accNo));
        return;
    }

    cout << "\n" << string(85, '-') << "\n";
    cout << left 
         << setw(10) << "Txn ID"
         << setw(14) << "Account No"
         << setw(12) << "Type"
         << setw(16) << "Amount (Rs.)"
         << setw(20) << "Balance After (Rs.)"
         << setw(20) << "Txn Timestamp" << "\n";
    cout << string(85, '-') << "\n";

    for (size_t i = 0; i < filtered.size(); ++i) {
        const Transaction& t = filtered[i];
        cout << left 
             << setw(10) << t.txnId
             << setw(14) << t.accountNo
             << setw(12) << t.type
             << right << setw(14) << formatPaiseToRupees(t.amountPaise) << "  "
             << right << setw(18) << formatPaiseToRupees(t.balanceAfterPaise) << "  "
             << left << setw(20) << t.time << "\n";
    }
    cout << string(85, '-') << "\n";
}

/**
 * @brief Demonstrates 2D array matrix operations (Addition, Subtraction, Multiplication, Transpose).
 */
void handle2DMatrixDemo() {
    printHeader("10. DEMO MODULE IV: 2D NUMERIC ARRAYS & MATRIX OPERATIONS");
    BranchMatrix Q1 = { 2, 2, {{10000, 20000}, {15000, 25000}} };
    BranchMatrix Q2 = { 2, 2, {{5000,  12000}, {8000,  14000}} };

    printMatrix("Quarter 1 Cash Flow Matrix", Q1);
    printMatrix("Quarter 2 Cash Flow Matrix", Q2);

    BranchMatrix Sum = matrixAdd(Q1, Q2);
    printMatrix("Combined Matrix Addition (Q1 + Q2)", Sum);

    BranchMatrix Trans = matrixTranspose(Sum);
    printMatrix("Transposed Matrix (Rows <-> Cols)", Trans);

    BranchMatrix Prod = matrixMultiply(Q1, Q2);
    printMatrix("Matrix Multiplication (Q1 * Q2)", Prod);
}

/**
 * @brief Demonstrates string reversal, character frequency analysis, tokenization, and pattern matching.
 */
void handleStringDemo() {
    printHeader("11. DEMO MODULE V: STRING MANIPULATION & FREQUENCY ANALYSIS");
    string text = readNonEmptyLine(" Enter string text to analyze: ");

    cout << "\n 1. String Reversal: " << reverseString(text) << "\n";

    int freq[256];
    analyzeCharacterFrequency(text, freq);
    cout << " 2. Character Frequency Map:\n    ";
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0 && isprint(i)) {
            cout << "'" << static_cast<char>(i) << "':" << freq[i] << " ";
        }
    }
    cout << "\n";

    cout << " 3. Tokenization by space delimiter:\n";
    vector<string> tokens = tokenizeString(text, ' ');
    for (size_t i = 0; i < tokens.size(); ++i) {
        cout << "    Token [" << i << "]: " << tokens[i] << "\n";
    }

    string pattern = readNonEmptyLine(" Enter pattern substring to match: ");
    if (patternMatchNaive(text, pattern)) {
        printSuccess("Pattern found in string via Naive Search O(n*m)!");
    } else {
        printError("Pattern NOT found in string.");
    }
}

/**
 * @brief Demonstrates Array Stack ADT, Parenthesis Matching, and Infix Math Evaluation.
 */
void handleStackDemo() {
    printHeader("12. DEMO MODULE VIII: STACK ADT & APPLICATIONS");
    cout << " 1. Parenthesis Matching Syntax Validator\n";
    cout << " 2. Infix Mathematical Expression Evaluation\n";
    int choice = readInt(" Select option (1-2): ", 1, 2);

    if (choice == 1) {
        string expr = readNonEmptyLine(" Enter expression with brackets (e.g. {[a+b]*(c-d)}): ");
        if (checkParenthesisMatching(expr)) printSuccess("Parentheses are BALANCED!");
        else printError("Parentheses are UNBALANCED!");
    } else {
        string expr = readNonEmptyLine(" Enter math expression (e.g. (10 + 20) * 3): ");
        int res = evaluateInfixExpression(expr);
        printSuccess("Evaluated Result = " + to_string(res));
    }
}

/**
 * @brief Demonstrates Linear Queue, Circular Queue, and Priority Queue ADTs.
 */
void handleQueueDemo() {
    printHeader("13. DEMO MODULE IX: QUEUE ADT (LINEAR, CIRCULAR, PRIORITY)");

    // Circular Queue
    CircularQueue cq;
    circularQueueInit(cq);
    circularQueueEnqueue(cq, 101);
    circularQueueEnqueue(cq, 102);
    circularQueueEnqueue(cq, 103);
    cout << "\n [Circular Queue] Enqueued tokens 101, 102, 103.\n";
    int tOut;
    if (circularQueueDequeue(cq, tOut)) {
        cout << " Serviced from Circular Queue: Token #" << tOut << "\n";
    }

    // Priority Queue
    PriorityQueue pq;
    priorityQueueInit(pq);
    priorityQueueEnqueue(pq, 501, 1, "Regular Customer");
    priorityQueueEnqueue(pq, 999, 10, "VIP High Net Worth");
    priorityQueueEnqueue(pq, 502, 3, "Senior Citizen");
    cout << "\n [Priority Queue] Servicing in priority order (Highest priority first):\n";
    PriorityToken pt;
    while (priorityQueueDequeue(pq, pt)) {
        cout << "  Serviced Token #" << pt.token << " (" << pt.clientName << ") [Priority Level: " << pt.priority << "]\n";
    }
}

// ====================================================================================================
// MAIN APPLICATION ENTRY POINT & MENU
// ====================================================================================================

/**
 * @brief Displays the 15-option interactive console menu covering all syllabus modules.
 */
void printMenu() {
    cout << "\n+-------------------------------------------------------------+\n";
    cout << "|         CS207 BANK MANAGEMENT SYSTEM (ALL MODULES)          |\n";
    cout << "+-------------------------------------------------------------+\n";
    cout << "|  1. Add Customer Account                                    |\n";
    cout << "|  2. Display All Customer Accounts                           |\n";
    cout << "|  3. Search Customer by Account No (Binary Search O(log n))  |\n";
    cout << "|  4. Search Customer by Phone (Linear Search O(n))           |\n";
    cout << "|  5. Deposit Funds                                           |\n";
    cout << "|  6. Withdraw Funds                                          |\n";
    cout << "|  7. Find Customer(s) with Highest Balance                   |\n";
    cout << "|  8. Sort Customers by Balance (Insertion Sort O(n^2))       |\n";
    cout << "|  9. View Audit Transaction History                          |\n";
    cout << "| 10. Demo Module IV: 2D Matrix Operations                    |\n";
    cout << "| 11. Demo Module V: String Manipulation & Frequency Analysis |\n";
    cout << "| 12. Demo Module VIII: Stack ADT & Infix Evaluation          |\n";
    cout << "| 13. Demo Module IX: Queue ADT (Linear, Circular, Priority)  |\n";
    cout << "| 14. Demo Module X: STL Containers & Iterators               |\n";
    cout << "| 15. Exit                                                    |\n";
    cout << "+-------------------------------------------------------------+\n";
}

/**
 * @brief Application entry point.
 * Initializes initial dataset and executes the main event loop.
 */
int main() {
    seedInitialData(); // Load initial customer accounts and opening balances

    cout << "===============================================================\n";
    cout << "  CS207: DATA STRUCTURE USING C++ - BANK MANAGEMENT SYSTEM    \n";
    cout << "===============================================================\n";

    while (true) {
        printMenu();
        int choice = readInt(" Select an option (1-15): ", 1, 15);
        switch (choice) {
            case 1:  handleAddCustomer(); break;
            case 2:  displayCustomersTable(g_customers); break;
            case 3:  handleSearchCustomer(); break;
            case 4:  handleSearchByPhone(); break;
            case 5:  handleDeposit(); break;
            case 6:  handleWithdraw(); break;
            case 7:  handleHighestBalance(); break;
            case 8:  handleSortCustomers(); break;
            case 9:  handleViewTransactions(); break;
            case 10: handle2DMatrixDemo(); break;
            case 11: handleStringDemo(); break;
            case 12: handleStackDemo(); break;
            case 13: handleQueueDemo(); break;
            case 14: demonstrateSTLContainers(g_customers); break;
            case 15:
                cout << "\n Thank you for using Bank Management System. Goodbye!\n";
                return 0;
            default:
                printError("Invalid choice.");
                break;
        }
    }
    return 0;
}
