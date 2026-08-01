#include <iostream>
#include "user.hpp"

User::User(std::string name) {
    this->setName(name);
}

User User::create() {
    std::string name;

    std::cout << "Enter your name> ";
    std::getline(std::cin, name);

    User createdUser(name);

    return createdUser;
}