#pragma once

namespace L5RP {

    /**
     * Using custom exception class just because default one doesn't support u16string messages.
     */
    class Exception {

        std::string message;

    public:

        Exception(std::string const& message);

        std::string const& getMessage() const;

    };

    class ChatException {

        std::vector<std::string> messages;

    public:

        ChatException(
                std::vector<std::string> const& messages
        );

        std::vector<std::string> const& getMessages() const;

    };

}