#include <iostream>
#include "auth.hpp"

// Configure your app credentials
YoussefAuth::api auth(
    "YourAppName",
    "YourOwnerId",
    "YourAppSecret",
    "1.0"
);

int main() {
    SetConsoleTitleA("Youssef Auth â€¢ C++ Example");

    std::cout << "==================================================" << std::endl;
    std::cout << "          YOUSSEF AUTH â€¢ C++ CLIENT (x64)         " << std::endl;
    std::cout << "==================================================" << std::endl;

    std::cout << "\n[+] Connecting to Youssef Auth server..." << std::endl;
    if (!auth.init()) {
        std::cout << "[-] Failed to initialize session! Check credentials." << std::endl;
        system("pause");
        return 1;
    }

    std::cout << "[+] Connected successfully!" << std::endl;
    std::cout << "\n1. Login with Username & Password" << std::endl;
    std::cout << "2. Activate with License Key" << std::endl;
    std::cout << "\nChoice: ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::string u, p;
        std::cout << "Username: ";
        std::cin >> u;
        std::cout << "Password: ";
        std::cin >> p;

        if (auth.login(u, p)) {
            std::cout << "\n[+] Login Successful! Welcome " << u << std::endl;
        } else {
            std::cout << "\n[-] Login Failed! Invalid credentials or HWID mismatch." << std::endl;
        }
    } else if (choice == 2) {
        std::string k;
        std::cout << "License Key: ";
        std::cin >> k;

        if (auth.license(k)) {
            std::cout << "\n[+] License Key Validated Successfully!" << std::endl;
        } else {
            std::cout << "\n[-] Invalid or expired license!" << std::endl;
        }
    }

    system("pause");
    return 0;
}