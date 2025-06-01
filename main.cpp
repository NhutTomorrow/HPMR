#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "EmailTrie.h"

void showSuggestions(const std::vector<std::string>& suggestions) {
    std::cout << "Suggestions:\n";
    for(size_t i = 0; i < suggestions.size(); ++i) {
        std::cout << i+1 << ". " << suggestions[i] << "\n";
    }
}
void loadEmailsFromFile(const std::string& filename, EmailTrie& trie) {
    std::ifstream infile(filename);
    std::string email;
    while (std::getline(infile, email)) {
        if (!email.empty()) {
            trie.insert(email);
        }
    }
    infile.close();
}
long long measureSearchTime(EmailTrie& trie, std::string& prefix) {
    auto start = std::chrono::high_resolution_clock::now();
    auto suggestions = trie.searchList(prefix);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
long long measureInsertTime(EmailTrie& trie, const std::string& email) {
    auto start = std::chrono::high_resolution_clock::now();
    trie.insert(email);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
bool isValidEmail(const std::string& email) {
            auto at = email.find('@');
            if (at == std::string::npos || at == 0 || at == email.size() - 1) return false;
            auto dot = email.find('.', at);
            if (dot == std::string::npos || dot == at + 1 || dot == email.size() - 1) return false;
            if (email.find(' ') != std::string::npos) return false;
            return true;
}
int main() {
    EmailTrie emailTrie;
    // Thêm các email mẫu
   loadEmailsFromFile("input.txt",emailTrie);
   int option;
    std::string input;
    while (true) {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Add email\n";
        std::cout << "2. Delete email\n";
        std::cout << "3. Suggest emails by prefix\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> option;
        std::cin.ignore(); // Remove leftover '\n'

        if (option == 1) {
            std::cout << "Ente4r email to add: ";
            std::getline(std::cin, input);
            if(isValidEmail(input)){
            long long time_us = measureInsertTime(emailTrie, input);
            std::cout << "Email added!\n";
            std::cout << "Insert time: " << time_us << " microseconds\n";}
            else {
                std:: cout << "Invalid email format!\n";
            }
        } else if (option == 2) {
            std::cout << "Enter email to delete: ";
            std::getline(std::cin, input);
            if (emailTrie.remove(input)) {
                std::cout << "Email deleted!\n";
            } else {
                std::cout << "Email not found.\n";
            }
        } else if (option == 3) {
            std::cout << "Enter prefix: ";
            std::getline(std::cin, input);
            long long time_us = measureSearchTime(emailTrie, input);
            auto suggestions = emailTrie.searchList(input);
            if (!suggestions.empty()) {
                showSuggestions(suggestions);
            } else {
                std::cout << "No suggestions found.\n";
            }
            std::cout << "Search time: " << time_us << " microseconds\n";

        } else if (option == 4) {
            break;
        } else {
            std::cout << "Invalid option!\n";
        }
    }
}