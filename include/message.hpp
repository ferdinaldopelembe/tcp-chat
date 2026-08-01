#include <SFML/Network.hpp>
#include "user.hpp"

#pragma once

class Message {
    
    private:
        std::string message;
        User sender;

    public:
        Message() = default;
        Message(std::string, User);

        // Getters
        std::string getMessage() { return this->message; }
        User getSender() { return this->sender; }

        // Setters
        void setMessage(std::string message) { this->message = message; }
        void setSender(User sender) { this->sender = sender; }
};