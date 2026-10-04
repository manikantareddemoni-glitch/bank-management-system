#include "bank.h"

using namespace std;

// ====================================================================================================
// ====================================================================================================
//   STUDENT 5: QUEUE ADT, STL CONTAINERS & DRIVER CONTROLLER (MODULES I, II, IX, X)
// ====================================================================================================
// ====================================================================================================
// Responsibilities:
// 1. Linear Queue ADT (FIFO: Enqueue, Dequeue, isQueueEmpty) for teller counter simulation
// 2. Circular Queue ADT with Modulo arithmetic wraparound logic
// 3. STL Demonstration (Module X):
//    - std::vector & std::pair (Data grouping)
//    - std::deque (Double-ended queue priority push/pop)
//    - std::set (Unique collection of registered phone numbers)
//    - std::map (O(log n) Key-Value lookups by Account Number)
// 4. Main Menu User Interface & Input Validation Loop (main controller driver)
// ====================================================================================================

/**
 * Student 5 Functions: Linear Queue (FIFO)
 */
void initQueue(LinearQueue& q) {
    q.front = 0;
    q.rear = -1;
}

bool isQueueEmpty(const LinearQueue& q) {
    return (q.rear < q.front);
}

void enqueue(LinearQueue& q, int tokenNo) {
    if (q.rear == MAX_QUEUE - 1) {
        cout << " [ERROR] Linear Queue is Full!\n";
        return;
    }
    q.arr[++q.rear] = tokenNo;
}

int dequeue(LinearQueue& q) {
    if (isQueueEmpty(q)) {
        cout << " [ERROR] Linear Queue is Empty!\n";
        return -1;
    }
    return q.arr[q.front++];
}

/**
 * Student 5 Functions: Circular Queue with Modulo Wraparound
 */
void initCircularQueue(CircularQueue& cq) {
    cq.front = 0;
    cq.rear = -1;
    cq.count = 0;
}

void circularEnqueue(CircularQueue& cq, int tokenNo) {
    if (cq.count == MAX_QUEUE) {
        cout << " [ERROR] Circular Queue is Full!\n";
        return;
    }
    cq.rear = (cq.rear + 1) % MAX_QUEUE; // Wrap around using modulo %
    cq.arr[cq.rear] = tokenNo;
    cq.count++;
}

int circularDequeue(CircularQueue& cq) {
    if (cq.count == 0) {
        cout << " [ERROR] Circular Queue is Empty!\n";
        return -1;
    }
    int val = cq.arr[cq.front];
    cq.front = (cq.front + 1) % MAX_QUEUE; // Wrap around using modulo %
    cq.count--;
    return val;
}

/**
 * Student 5 Function: Queue ADT Demo
 */
void demoQueue() {
    cout << "\n=======================================================\n";
    cout << "  [STUDENT 5] MODULE IX: QUEUE ADT DEMONSTRATION       \n";
    cout << "=======================================================\n";

    cout << " 1. Linear Queue for Bank Teller Counter (FIFO):\n";
    LinearQueue lq;
    initQueue(lq);
    enqueue(lq, 101);
    enqueue(lq, 102);
    enqueue(lq, 103);
    cout << "    Enqueued Token #101, #102, #103\n";
    cout << "    Servicing Customers:\n";
    while (!isQueueEmpty(lq)) {
        cout << "    Serving Token #" << dequeue(lq) << "\n";
    }

    cout << "\n 2. Circular Queue with Modulo Wraparound:\n";
    CircularQueue cq;
    initCircularQueue(cq);
    circularEnqueue(cq, 201);
    circularEnqueue(cq, 202);
    cout << "    Enqueued Token #201, #202 in Circular Queue\n";
    cout << "    Dequeued Token #" << circularDequeue(cq) << "\n";
}

/**
 * Student 5 Function: STL Containers Demonstration (Module X)
 */
void demoSTLContainers() {
    cout << "\n=======================================================\n";
    cout << "  [STUDENT 5] MODULE X: STL CONTAINERS & ITERATORS DEMO\n";
    cout << "=======================================================\n";

    // 1. std::vector & std::pair
    cout << "\n 1. std::vector & std::pair (AccountNo, Balance):\n";
    vector<pair<int, double>> accountPairs;
    for (size_t i = 0; i < customerList.size(); i++) {
        accountPairs.push_back(make_pair(customerList[i].accountNo, customerList[i].balance));
    }
    for (size_t i = 0; i < accountPairs.size(); i++) {
        cout << "    Account #" << accountPairs[i].first << " -> Rs. " << accountPairs[i].second << "\n";
    }

    // 2. std::deque (Double ended queue)
    cout << "\n 2. std::deque (VIP Front Push & Regular Back Push):\n";
    deque<string> tellerLine;
    tellerLine.push_back("Regular Customer #1");
    tellerLine.push_back("Regular Customer #2");
    tellerLine.push_front("VIP Customer (Emergency Priority)");
    cout << "    Front of Deque: " << tellerLine.front() << "\n";
    cout << "    Back of Deque:  " << tellerLine.back() << "\n";

    // 3. std::set (Unique elements)
    cout << "\n 3. std::set (Unique registered phone numbers):\n";
    set<string> uniquePhones;
    for (size_t i = 0; i < customerList.size(); i++) {
        uniquePhones.insert(customerList[i].phone);
    }
    cout << "    Total unique phone numbers in database: " << uniquePhones.size() << "\n";

    // 4. std::map (Fast Key-Value Lookup)
    cout << "\n 4. std::map (AccountNo -> Customer Name Map):\n";
    map<int, string> nameMap;
    for (size_t i = 0; i < customerList.size(); i++) {
        nameMap[customerList[i].accountNo] = customerList[i].name;
    }
    map<int, string>::iterator it = nameMap.find(1001);
    if (it != nameMap.end()) {
        cout << "    Map found Account #1001: " << it->second << "\n";
    }
}

/**
 * Student 5 Function: Main Console Menu Display (Module I & II)
 */
void printMenu() {
    cout << "\n===============================================================\n";
    cout << "      CS207 BANK MANAGEMENT SYSTEM - MAIN CONSOLE MENU         \n";
    cout << "===============================================================\n";
    cout << "  [STUDENT 1 - Account Management]\n";
    cout << "    1. Add New Customer Account                                \n";
    cout << "    2. Display All Customer Accounts                           \n";
    cout << "\n  [STUDENT 2 - Transactions & Searches]\n";
    cout << "    3. Deposit Money                                           \n";
    cout << "    4. Withdraw Money                                          \n";
    cout << "    5. Search Customer by Account No (Binary Search O(log n))  \n";
    cout << "    6. Search Customer by Phone Number (Linear Search O(n))    \n";
    cout << "    7. View Transaction Audit History                          \n";
    cout << "\n  [STUDENT 3 - Sorting & Matrix Math]\n";
    cout << "    8. Sort Customers by Balance (Insertion Sort O(n^2))       \n";
    cout << "    9. Demo Module IV: 2D Array & Matrix Operations            \n";
    cout << "\n  [STUDENT 4 - Strings & Stack ADT]\n";
    cout << "   10. Demo Module V: String Manipulation & Frequency Analysis \n";
    cout << "   11. Demo Module VIII: Stack ADT & Parenthesis Checking      \n";
    cout << "\n  [STUDENT 5 - Queue ADT, STL & Driver Loop]\n";
    cout << "   12. Demo Module IX: Queue ADT (Linear & Circular Queues)    \n";
    cout << "   13. Demo Module X: STL Containers (vector, deque, set, map) \n";
    cout << "   14. Exit                                                    \n";
    cout << "===============================================================\n";
    cout << " Enter your choice (1-14): ";
}

/**
 * Student 5 Function: Main Application Entry Point (Module I & II)
 */
int main() {
    seedInitialData(); // Load initial customer demo records into memory (Student 1)

    cout << "===============================================================\n";
    cout << "     WELCOME TO CS207 DATA STRUCTURES BANK MANAGEMENT SYSTEM   \n";
    cout << "===============================================================\n";

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            string ignoreStr;
            cin >> ignoreStr;
            cout << " [ERROR] Invalid input! Please enter a number between 1 and 14.\n";
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
            case 13: demoSTLContainers();     break;
            case 14:
                cout << "\n Thank you for using Bank Management System. Goodbye!\n";
                return 0;
            default:
                cout << " [ERROR] Invalid option selected. Please choose between 1 and 14.\n";
                break;
        }
    }
    return 0;
}
