#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

// String helper utilities (character-by-character traversal)
string trim(string str);
bool isDigitsOnly(string str);
bool isValidPhone(string phone);
bool isValidEmail(string email);

// Currency handling utilities (traversing string character by character)
// Money is stored strictly as long long paise (1 Rupee = 100 Paise)
bool parseMoneyToPaise(string input, long long& outPaise);
string formatPaiseToRupees(long long paise);

// Console input reading helpers (re-prompts until valid input received)
int readInt(string prompt, int minVal = 1, int maxVal = 2147483647);
long long readMoney(string prompt);
string readNonEmptyLine(string prompt);
bool readYesNo(string prompt);

// Console terminal formatting helpers
void printHeader(string title);
void printSuccess(string message);
void printError(string message);
void printDbDetail(string detail);

#endif // UTILS_H
