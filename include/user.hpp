#include <iostream>
#include <string>

#pragma once

class User {
    private:
        std::string name;

    public:
        User() = default;
        User(std::string);

        static User create();

        // Getters
        std::string getName() { return this->name; }

        // Setters
        void setName(std::string name) { this->name = name; }
};