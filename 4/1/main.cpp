#include <iostream>
#include <sstream>
#include <unordered_set>
#include "htblv1.h"

void Solution(std::istream &iss) {
    std::string s;
    Hashtable htbl;

    size_t counter = 0;

    char command;
    while (iss >> command >> s) {
        counter++;
        switch (command) {
            case '+': {
                if (!htbl.Find(s)) {
                    htbl.Insert(s);
                    std::cout << "OK"
                              << "\n";
                } else {
                    std::cout << "FAIL"
                              << "\n";
                }
                break;
            }
            case '-': {
                if (htbl.Erase(s)) {
                    std::cout << "OK"
                              << "\n";
                } else {
                    std::cout << "FAIL"
                              << "\n";
                }
                break;
            }
            case '?': {
                if (htbl.Find(s)) {
                    std::cout << "OK"
                              << "\n";
                } else {
                    std::cout << "FAIL"
                              << "\n";
                }
                break;
            }
        }
    }
}

int main() {
    Solution(std::cin);

    return 0;
}