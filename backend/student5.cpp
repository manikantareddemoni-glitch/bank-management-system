#include "bank.h"

// ============================================================================
//   STUDENT 5: QUEUE DATA STRUCTURES & MAIN CONTROLLER MENU
// ============================================================================
// Key Topics Covered:
// 1. Linear Queue ADT (FIFO - First In First Out: enqueue, dequeue, isEmpty)
// 2. Circular Queue ADT (Using modulo % for circular wraparound)
// 3. Simple Array / List Traversal using basic for loops
// 4. Main Menu Loop (while loop, switch-case, and user input validation)
// ============================================================================

// ============================================================================
// LINEAR QUEUE IMPLEMENTATION (FIFO - First In, First Out)
// ============================================================================

/**
 * Function: Initialize Linear Queue
 */
void initQueue(LinearQueue& q) {
    q.front = 0;
    q.rear = -1;
}

/**
 * Function: Check if Linear Queue is Empty
 */
bool isQueueEmpty(const LinearQueue& q) {
    if (q.rear < q.front) {
        return true;
    }
    return false;
}

/**
 * Function: Insert Token into Linear Queue (Enqueue)
 */
void enqueue(LinearQueue& q, int tokenNo) {
    if (q.rear == MAX_QUEUE - 1) {
        cout << "\n[ERROR] Queue is Full (Overflow)!\n";
        return;
    }
    q.rear = q.rear + 1;
    q.arr[q.rear] = tokenNo;
}

/**
 * Function: Remove Token from Linear Queue (Dequeue)
 */
int dequeue(LinearQueue& q) {
    if (isQueueEmpty(q)) {
        cout << "\n[ERROR] Queue is Empty (Underflow)!\n";
        return -1;
    }
    int val = q.arr[q.front];
    q.front = q.front + 1;
    return val;
}

// ============================================================================
// CIRCULAR QUEUE IMPLEMENTATION (Modulo Arithmetic Wraparound)
// ============================================================================

/**
 * Function: Initialize Circular Queue
 */
void initCircularQueue(CircularQueue& cq) {
    cq.front = 0;
    cq.rear = -1;
    cq.count = 0;
}

/**
 * Function: Insert Token into Circular Queue
 */
void circularEnqueue(CircularQueue& cq, int tokenNo) {
    if (cq.count == MAX_QUEUE) {
        cout << "\n[ERROR] Circular Queue is Full!\n";
        return;
    }
    // Modulo arithmetic to wrap around to index 0 when reaching end
    cq.rear = (cq.rear + 1) % MAX_QUEUE;
    cq.arr[cq.rear] = tokenNo;
    cq.count = cq.count + 1;
}

/**
 * Function: Remove Token from Circular Queue
 */
int circularDequeue(CircularQueue& cq) {
    if (cq.count == 0) {
        cout << "\n[ERROR] Circular Queue is Empty!\n";
        return -1;
    }
    int val = cq.arr[cq.front];
    cq.front = (cq.front + 1) % MAX_QUEUE;
    cq.count = cq.count - 1;
    return val;
}

/**
 * Function: Queue Data Structure Demo
 */
void demoQueue() {
    cout << "\n===============================================================\n";
    cout << "       [STUDENT 5] QUEUE DATA STRUCTURE (FIFO) DEMO            \n";
    cout << "===============================================================\n";

    cout << "1. Linear Queue for Bank Token System (FIFO):\n";
    LinearQueue lq;
    initQueue(lq);
    enqueue(lq, 101);
    enqueue(lq, 102);
    enqueue(lq, 103);
    cout << "   Enqueued Tokens: 101, 102, 103\n";

    cout << "\n   Serving Tokens in FIFO Order:\n";
    while (!isQueueEmpty(lq)) {
        cout << "   Serving Token #" << dequeue(lq) << "\n";
    }

    cout << "\n2. Circular Queue (Tokens with Modulo Wraparound):\n";
    CircularQueue cq;
    initCircularQueue(cq);
    circularEnqueue(cq, 201);
    circularEnqueue(cq, 202);
    cout << "   Enqueued Tokens in Circular Queue: 201, 202\n";
    cout << "   Dequeued Token #" << circularDequeue(cq) << "\n";
}

/**
 * Function: Basic Array / List Demonstration
 * Explanation: Uses simple for loops to demonstrate basic data traversal.
 */
void demoSimpleArrays() {
    cout << "\n===============================================================\n";
    cout << "       [STUDENT 5] BASIC ARRAY TRAVERSAL DEMONSTRATION         \n";
    cout << "===============================================================\n";

    cout << "Customer Account Numbers and Balances:\n";
    for (int i = 0; i < (int)customerList.size(); i++) {
        cout << "  [" << (i + 1) << "] Account #" << customerList[i].accountNo
             << " -> Balance: Rs. " << customerList[i].balance << "\n";
    }
}

/**
 * Function: Main Menu Display
 */
void printMenu() {
    cout << "\n===============================================================\n";
    cout << "              BANK MANAGEMENT SYSTEM - MAIN MENU               \n";
    cout << "===============================================================\n";
    cout << "  [STUDENT 1 - Account Management]\n";
    cout << "    1. Add New Customer Account\n";
    cout << "    2. Display All Customer Accounts\n";
    cout << "\n  [STUDENT 2 - Transactions & Searches]\n";
    cout << "    3. Deposit Money\n";
    cout << "    4. Withdraw Money\n";
    cout << "    5. Search Customer by Account Number (Binary Search)\n";
    cout << "    6. Search Customer by Phone Number (Linear Search)\n";
    cout << "    7. View Transaction Audit History\n";
    cout << "\n  [STUDENT 3 - Sorting & Matrix Operations]\n";
    cout << "    8. Sort Customers by Balance (Insertion Sort)\n";
    cout << "    9. 2D Matrix Operations Demo (Addition, Subtraction, Mult)\n";
    cout << "\n  [STUDENT 4 - Strings & Stack ADT]\n";
    cout << "   10. String Operations & Pattern Matching Demo\n";
    cout << "   11. Stack ADT & Parentheses Balance Check Demo\n";
    cout << "\n  [STUDENT 5 - Queue ADT & Arrays]\n";
    cout << "   12. Queue ADT Demo (Linear & Circular Queues)\n";
    cout << "   13. Basic Array Traversal Demo\n";
    cout << "   14. Exit\n";
    cout << "===============================================================\n";
    cout << "Enter your choice (1-14): ";
}

/**
 * Main Application Function
 */
int main() {
    // 1. Preload initial customer records
    seedInitialData();

    cout << "===============================================================\n";
    cout << "          WELCOME TO THE BANK MANAGEMENT SYSTEM                \n";
    cout << "===============================================================\n";

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            string junk;
            cin >> junk;
            cout << "\n[ERROR] Invalid input! Please enter a number between 1 and 14.\n";
            continue;
        }

        switch (choice) {
            case 1:  addNewCustomer();        break;
            case 2:  displayAllCustomers();   break;
            case 3:  depositFunds();          break;
            case 4:  withdrawFunds();         break;
            case 5:  searchByAccountNo();     break;
            case 6:  searchByPhone();         break;
            case 7:  viewTransactionHistory();break;
            case 8:  sortCustomers();         break;
            case 9:  demoMatrixOperations();  break;
            case 10: demoStringOperations();  break;
            case 11: demoStack();             break;
            case 12: demoQueue();             break;
            case 13: demoSimpleArrays();      break;
            case 14:
                cout << "\nThank you for using the Bank Management System. Goodbye!\n";
                return 0;
            default:
                cout << "\n[ERROR] Invalid option selected! Please choose 1-14.\n";
                break;
        }
    }
    return 0;
}
