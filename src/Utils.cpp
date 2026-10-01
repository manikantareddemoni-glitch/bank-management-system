#include "Utils.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
using namespace std;

string trim(string str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool isDigitsOnly(string str) {
    if (str.length() == 0) return false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool isValidPhone(string phone) {
    string trimmed = trim(phone);
    return (trimmed.length() == 10 && isDigitsOnly(trimmed));
}

bool isValidEmail(string email) {
    string trimmed = trim(email);
    if (trimmed.length() == 0) return true; // Email optional
    size_t atPos = trimmed.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == trimmed.length() - 1) {
        return false;
    }
    if (trimmed.find('@', atPos + 1) != string::npos) {
        return false;
    }
    size_t dotPos = trimmed.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == trimmed.length() - 1) {
        return false;
    }
    return true;
}

bool parseMoneyToPaise(string input, long long& outPaise) {
    string str = trim(input);
    if (str.length() == 0) return false;
    if (str[0] == '-') return false; // Reject negative money

    size_t dotPos = str.find('.');
    string rupeesStr = "";
    string paiseStr = "";

    if (dotPos == string::npos) {
        rupeesStr = str;
        paiseStr = "00";
    } else {
        if (str.find('.', dotPos + 1) != string::npos) return false;
        rupeesStr = str.substr(0, dotPos);
        paiseStr = str.substr(dotPos + 1);
        if (paiseStr.length() == 0) paiseStr = "00";
        else if (paiseStr.length() == 1) paiseStr += "0";
        else if (paiseStr.length() > 2) return false; // Reject 3+ decimal places
    }

    if (rupeesStr.length() == 0) rupeesStr = "0";

    if (!isDigitsOnly(rupeesStr) || !isDigitsOnly(paiseStr)) {
        return false;
    }

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

int readInt(string prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            if (cin.eof()) {
                cout << "\n[EOF encountered. Exiting application]\n";
                exit(0);
            }
            cin.clear();
            continue;
        }
        string trimmed = trim(line);
        if (trimmed.length() == 0 || !isDigitsOnly(trimmed)) {
            cout << " [ERROR] Invalid input. Please enter a valid integer.\n";
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

long long readMoney(string prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            if (cin.eof()) {
                cout << "\n[EOF encountered. Exiting application]\n";
                exit(0);
            }
            cin.clear();
            continue;
        }
        long long paise = 0;
        if (!parseMoneyToPaise(line, paise) || paise <= 0) {
            cout << " [ERROR] Invalid money amount. Enter a positive number (max 2 decimal places, e.g. 500.50).\n";
            continue;
        }
        return paise;
    }
}

string readNonEmptyLine(string prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) {
            if (cin.eof()) {
                cout << "\n[EOF encountered. Exiting application]\n";
                exit(0);
            }
            cin.clear();
            continue;
        }
        string trimmed = trim(line);
        if (trimmed.length() > 0) {
            return trimmed;
        }
        cout << " [ERROR] Input cannot be empty or blank.\n";
    }
}

bool readYesNo(string prompt) {
    while (true) {
        cout << prompt << " (y/n): ";
        string line;
        if (!getline(cin, line)) {
            if (cin.eof()) {
                cout << "\n[EOF encountered. Exiting application]\n";
                exit(0);
            }
            cin.clear();
            continue;
        }
        string trimmed = trim(line);
        if (trimmed == "y" || trimmed == "Y" || trimmed == "yes" || trimmed == "YES") {
            return true;
        }
        if (trimmed == "n" || trimmed == "N" || trimmed == "no" || trimmed == "NO") {
            return false;
        }
        cout << " [ERROR] Please type 'y' for yes or 'n' for no.\n";
    }
}

void printHeader(string title) {
    cout << "\n=======================================================\n";
    cout << "  " << title << "\n";
    cout << "=======================================================\n";
}

void printSuccess(string message) {
    cout << " [OK] " << message << "\n";
}

void printError(string message) {
    cout << " [ERROR] " << message << "\n";
}

void printDbDetail(string detail) {
    cout << " [DB detail] " << detail << "\n";
}
