#include "bank.h"

// ============================================================================
//   STUDENT 3: SORTING ALGORITHMS & 2D MATRIX OPERATIONS
// ============================================================================
// Key Topics Covered:
// 1. Insertion Sort Algorithm (Sorting customer records by balance using for & while loops)
// 2. 2D Arrays / Matrices (2x2 Matrix arithmetic)
// 3. Nested For Loops:
//    - Matrix Addition
//    - Matrix Subtraction
//    - Matrix Multiplication
//    - Matrix Transpose (swapping rows and columns)
// ============================================================================

/**
 * Function: Insertion Sort by Balance
 * Explanation:
 * - Iterates through array from index 1 to n-1 using a for loop.
 * - Stores current element in `key`.
 * - Uses a while loop to shift elements that are greater (or smaller) to the right.
 * - Places `key` at its correct position (j + 1).
 */
void insertionSortByBalance(vector<Customer>& arr, bool ascending) {
    int n = (int)arr.size();
    for (int i = 1; i < n; i++) {
        Customer key = arr[i];
        int j = i - 1;

        if (ascending) {
            // Ascending Order (Lowest to Highest): Shift larger elements right
            while (j >= 0 && arr[j].balance > key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        } else {
            // Descending Order (Highest to Lowest): Shift smaller elements right
            while (j >= 0 && arr[j].balance < key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        }
        arr[j + 1] = key;
    }
}

/**
 * Function: Sort Customers Menu Option
 * Explanation: Asks user for sorting preference (Ascending/Descending) and sorts the list.
 */
void sortCustomers() {
    cout << "\n--- [STUDENT 3] SORT CUSTOMERS BY BALANCE (INSERTION SORT) ---\n";
    cout << "1. Ascending Order (Lowest to Highest Balance)\n";
    cout << "2. Descending Order (Highest to Lowest Balance)\n";
    cout << "Enter your choice (1 or 2): ";
    int choice;
    cin >> choice;

    bool asc = (choice == 1);
    insertionSortByBalance(customerList, asc);

    cout << "\n[SUCCESS] Customer list sorted successfully!\n";
    displayAllCustomers();
}

/**
 * Function: Helper to print a 2x2 Matrix
 * Explanation: Uses two nested for loops to print rows and columns with tabs.
 */
void print2DMatrix(string title, int mat[2][2]) {
    cout << "\n" << title << ":\n";
    for (int i = 0; i < 2; i++) {
        cout << "  ";
        for (int j = 0; j < 2; j++) {
            cout << mat[i][j] << "\t";
        }
        cout << "\n";
    }
}

/**
 * Function: 2D Matrix Operations Demonstration
 * Explanation:
 * Demonstrates basic 2D array matrix mathematics:
 * 1. Matrix Addition:       Sum[i][j]   = A[i][j] + B[i][j]
 * 2. Matrix Subtraction:    Diff[i][j]  = A[i][j] - B[i][j]
 * 3. Matrix Multiplication: Mult[i][j] += A[i][k] * B[k][j]
 * 4. Matrix Transpose:      Trans[j][i] = A[i][j]
 */
void demoMatrixOperations() {
    cout << "\n===============================================================\n";
    cout << "       [STUDENT 3] 2D ARRAY & MATRIX OPERATIONS DEMO           \n";
    cout << "===============================================================\n";

    // Two sample 2x2 matrices representing branch cash flows
    int A[2][2] = {
        {1000, 2000},
        {1500, 2500}
    };
    int B[2][2] = {
        {500,  1200},
        {800,  1400}
    };

    print2DMatrix("Matrix A (Branch 1 Cash Flow)", A);
    print2DMatrix("Matrix B (Branch 2 Cash Flow)", B);

    // 1. Matrix Addition (Sum = A + B)
    int Sum[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Sum[i][j] = A[i][j] + B[i][j];
        }
    }
    print2DMatrix("1. Matrix Addition (A + B)", Sum);

    // 2. Matrix Subtraction (Diff = A - B)
    int Diff[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Diff[i][j] = A[i][j] - B[i][j];
        }
    }
    print2DMatrix("2. Matrix Subtraction (A - B)", Diff);

    // 3. Matrix Multiplication (Mult = A * B)
    int Mult[2][2] = {
        {0, 0},
        {0, 0}
    };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Mult[i][j] = Mult[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
    print2DMatrix("3. Matrix Multiplication (A * B)", Mult);

    // 4. Matrix Transpose (Swap rows and columns)
    int Trans[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Trans[j][i] = A[i][j];
        }
    }
    print2DMatrix("4. Transpose of Matrix A", Trans);
}
