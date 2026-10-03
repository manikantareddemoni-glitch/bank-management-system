#include "bank.h"

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   STUDENT 3: SORTING ALGORITHMS & 2D MATRIX OPERATIONS (MODULES III, IV, VII)
// ====================================================================================================
// ====================================================================================================
// Responsibilities:
// 1. Insertion Sort Algorithm implementation for sorting customers by balance (Ascending & Descending)
// 2. Sorting time complexity analysis (O(n^2) worst/average case, O(n) best case)
// 3. 2D Array Matrix Operations (Module IV):
//    - Matrix Addition
//    - Matrix Subtraction
//    - Matrix Multiplication
//    - Matrix Transposition
// ====================================================================================================

/**
 * Student 3 Function: Insertion Sort Algorithm (Sorting by Balance)
 * ----------------------------------------------------------------
 * Explanation:
 *   - Inserts elements one-by-one into their sorted position by shifting adjacent elements.
 *   - Time Complexity:  O(n^2) worst/average case, O(n) best case [Module VII]
 *   - Space Complexity: O(1) in-place sort
 */
void insertionSortByBalance(vector<Customer>& arr, bool ascending) {
    int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; i++) {
        Customer key = arr[i];
        int j = i - 1;

        if (ascending) {
            // Shift elements with larger balance to the right
            while (j >= 0 && arr[j].balance > key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        } else {
            // Shift elements with smaller balance to the right (Descending)
            while (j >= 0 && arr[j].balance < key.balance) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        }
        arr[j + 1] = key;
    }
}

/**
 * Student 3 Function: Sort Customers Menu Handler
 */
void sortCustomers() {
    cout << "\n--- [STUDENT 3] SORT CUSTOMERS BY BALANCE (INSERTION SORT O(n^2)) ---\n";
    cout << " 1. Ascending Order (Lowest to Highest Balance)\n";
    cout << " 2. Descending Order (Highest to Lowest Balance)\n";
    cout << " Enter choice (1 or 2): ";
    int choice;
    cin >> choice;

    bool asc = (choice == 1);
    insertionSortByBalance(customerList, asc);

    cout << " [OK] Customer records successfully sorted.\n";
    displayAllCustomers();
}

/**
 * Student 3 Function: Helper to print 2x2 Matrix
 */
void print2DMatrix(string title, int mat[2][2]) {
    cout << "\n--- " << title << " (2x2 Matrix) ---\n";
    for (int i = 0; i < 2; i++) {
        cout << "  [ ";
        for (int j = 0; j < 2; j++) {
            cout << setw(6) << mat[i][j] << " ";
        }
        cout << "]\n";
    }
}

/**
 * Student 3 Function: 2D Numeric Array Matrix Operations (Module IV)
 * Explanation:
 *   1. Matrix Addition: C[i][j] = A[i][j] + B[i][j]
 *   2. Matrix Subtraction: Diff[i][j] = A[i][j] - B[i][j]
 *   3. Matrix Multiplication: Mult[i][j] = Sum(A[i][k] * B[k][j])
 *   4. Matrix Transpose: Trans[j][i] = A[i][j] (Swap rows & columns)
 */
void demoMatrixOperations() {
    cout << "\n=======================================================\n";
    cout << "  [STUDENT 3] MODULE IV: 2D ARRAY & MATRIX DEMONSTRATION \n";
    cout << "=======================================================\n";

    int A[2][2] = {{1000, 2000}, {1500, 2500}};
    int B[2][2] = {{500,  1200}, {800,  1400}};

    print2DMatrix("Branch A Cash Flow", A);
    print2DMatrix("Branch B Cash Flow", B);

    // 1. Matrix Addition
    int Sum[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Sum[i][j] = A[i][j] + B[i][j];
        }
    }
    print2DMatrix("1. Matrix Addition (A + B)", Sum);

    // 2. Matrix Subtraction
    int Diff[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Diff[i][j] = A[i][j] - B[i][j];
        }
    }
    print2DMatrix("2. Matrix Subtraction (A - B)", Diff);

    // 3. Matrix Multiplication
    int Mult[2][2] = {{0, 0}, {0, 0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Mult[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    print2DMatrix("3. Matrix Multiplication (A * B)", Mult);

    // 4. Matrix Transpose
    int Trans[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Trans[j][i] = A[i][j];
        }
    }
    print2DMatrix("4. Matrix Transpose of A (Rows <-> Columns)", Trans);
}
