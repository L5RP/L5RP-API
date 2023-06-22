#include "api.h"

L5RP::Exception::Exception(std::string const& message)
{
    this->message = message;
}

std::string const& L5RP::Exception::getMessage() const
{
    return this->message;
}

L5RP::ChatException::ChatException(
        std::vector<std::string> const& messages
)
{
    this->messages = messages;
}

std::vector<std::string> const& L5RP::ChatException::getMessages() const
{
    return this->messages;
}
