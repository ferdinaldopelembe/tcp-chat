#include "message.hpp"

Message::Message(std::string message, User sender) {
    this->setMessage(message);
    this->setSender(sender);
}