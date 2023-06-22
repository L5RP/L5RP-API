#include "api.h"

L5RP::ArgumentParseException::ArgumentParseException(std::string const& msg)
        : Exception(msg)
{}

L5RP::ArgumentOutOfLimits::ArgumentOutOfLimits(std::string const& msg)
        : ArgumentParseException(msg)
{}

L5RP::UnsignedInt32Argument::UnsignedInt32Argument()
    : IArgument(
    InputType::WORD
)
{

}

L5RP::ArgumentValue L5RP::UnsignedInt32Argument::parseValue(std::string const& str)
{

    ArgumentValue result = {};
    result._union = {};
    result._union._uint32_t = UnsignedInt32Argument::parse(str);
    return result;

}

uint32_t L5RP::UnsignedInt32Argument::parse(std::string const& str)
{
    try {
        return static_cast<uint32_t>(std::stoul(str));
    }

    catch (...) {
        throw ArgumentParseException(u8"@r" + str + u8" nėra sveikasis skaičius!");
    }
}

L5RP::IntArgument::IntArgument()
        : IArgument(
        InputType::WORD
)
{

}

L5RP::ArgumentValue L5RP::IntArgument::parseValue(std::string const& str)
{

    ArgumentValue result = {};
    result._union = {};
    result._union._int = IntArgument::parse(str);
    return result;

}

int L5RP::IntArgument::parse(std::string const& str)
{
    try {
        return std::stoi(str);
    }

    catch (...) {
        throw ArgumentParseException(u8"@r" + str + u8" nėra sveikasis skaičius!");
    }
}

L5RP::IArgument::IArgument(InputType inputType)
{
    this->inputType = inputType;
}

L5RP::IArgument::InputType L5RP::IArgument::getInputType() const
{
    return this->inputType;
}

L5RP::LimitedIntArgument::LimitedIntArgument(
        int min,
        int max
)
        : IArgument(
        InputType::WORD
)
{

    this->min = min;
    this->max = max;

}

L5RP::ArgumentValue L5RP::LimitedIntArgument::parseValue(
        std::string const& str
)
{

    ArgumentValue value = {};
    value._union._int = LimitedIntArgument::parse(
            str,
            this->min,
            this->max
    );
    return value;

}

int L5RP::LimitedIntArgument::parse(std::string const& str, int min, int max)
{

    int val = IntArgument::parse(str);

    if (val < min || val > max)
        throw ArgumentOutOfLimits(
                u8"@r" + str +
                u8" turėtų būti ne mažesnis, kaip " + std::to_string(min) +
                u8" ir ne didesnis, kaip " + std::to_string(max)
        );

    return val;

}

void L5RP::LimitedIntArgument::setMin(int min) {
    this->min = min;
}

void L5RP::LimitedIntArgument::setMax(int max) {
    this->max = max;
}

int L5RP::LimitedIntArgument::getMin() const
{
    return this->min;
}

int L5RP::LimitedIntArgument::getMax() const
{
    return this->max;
}

L5RP::LimitedDefaultIntArgument::LimitedDefaultIntArgument(int min, int max, int def)
        : IArgument(
        InputType::WORD
)
{
    this->min = min;
    this->max = max;
    this->def = def;
}

L5RP::ArgumentValue L5RP::LimitedDefaultIntArgument::parseValue(std::string const& str)
{

    ArgumentValue value = {};
    value._union._int = LimitedDefaultIntArgument::parse(
            str,
            this->min,
            this->max,
            this->def
    );
    return value;

}

int L5RP::LimitedDefaultIntArgument::parse(std::string const& str, int min, int max, int _default)
{

    try {

        return LimitedIntArgument::parse(str, min, max);

    } catch (ArgumentOutOfLimits const&) {

        return _default;

    }

}

int L5RP::LimitedDefaultIntArgument::getDefault() const
{
    return this->def;
}

L5RP::ArgumentValue L5RP::ByteArgument::parseValue(std::string const& str)
{
    ArgumentValue result;

    result._union._uint8_t = ByteArgument::parse(str);

    return result;
}

L5RP::ByteArgument::ByteArgument()
        : IArgument(
        InputType::WORD
)
{

}

uint8_t L5RP::ByteArgument::parse(std::string const& str)
{

    return static_cast<uint8_t>(
            LimitedIntArgument::parse(str, 0, 255)
    );

}

L5RP::FloatArgument::FloatArgument()
        : IArgument(
        InputType::WORD
)
{

}

L5RP::ArgumentValue L5RP::FloatArgument::parseValue(std::string const& str)
{

    ArgumentValue value = {};
    value._union._float = FloatArgument::parse(str);
    return value;

}

float L5RP::FloatArgument::parse(std::string const& str)
{

    try {
        return std::stof(str);
    }

    catch (...) {
        throw ArgumentParseException(u8"@r" + str + u8" nėra skaičius!");
    }

}

L5RP::LimitedFloatArgument::LimitedFloatArgument(
        float min,
        float max
)
        : IArgument(
        InputType::WORD
)
{

    this->min = min;
    this->max = max;

}

L5RP::ArgumentValue L5RP::LimitedFloatArgument::parseValue(
        std::string const& str
)
{

    ArgumentValue value = {};
    value._union._float = LimitedFloatArgument::parse(
            str,
            this->min,
            this->max
    );
    return value;

}

float L5RP::LimitedFloatArgument::parse(std::string const& str, float min, float max)
{

    float val = FloatArgument::parse(str);

    if (val < min || val > max)
        throw ArgumentOutOfLimits(
                u8"@r" + str +
                u8" turėtų būti ne mažesnis, kaip " + std::to_string(min) +
                u8" ir ne didesnis, kaip " + std::to_string(max)
        );

    return val;

}

float L5RP::LimitedFloatArgument::getMin() const
{
    return this->min;
}

float L5RP::LimitedFloatArgument::getMax() const
{
    return this->max;
}

L5RP::LimitedDefaultFloatArgument::LimitedDefaultFloatArgument(
        float min,
        float max,
        float def
) : IArgument(
        InputType::WORD
)
{

    this->min = min;
    this->max = max;
    this->def = def;

}

L5RP::ArgumentValue L5RP::LimitedDefaultFloatArgument::parseValue(std::string const& str)
{

    ArgumentValue value = {};
    value._union._float = LimitedDefaultFloatArgument::parse(
            str,
            this->min,
            this->max,
            this->def
    );
    return value;

}

float L5RP::LimitedDefaultFloatArgument::parse(std::string const& str, float min, float max, float _default)
{

    try {

        return LimitedFloatArgument::parse(
                str,
                min,
                max
        );

    } catch (ArgumentOutOfLimits const&) {

        return _default;

    }

}

float L5RP::LimitedDefaultFloatArgument::getDefault() const
{
    return this->def;
}

L5RP::StringArgument::StringArgument()
        : IArgument(
        InputType::WORD
)
{

}

L5RP::ArgumentValue L5RP::StringArgument::parseValue(std::string const& str)
{
    ArgumentValue result = {};
    result._string = str;
    return result;
}

L5RP::StringDumpArgument::StringDumpArgument()
        : IArgument(
        InputType::DUMP
)
{

}

L5RP::ArgumentValue L5RP::StringDumpArgument::parseValue(std::string const& str)
{
    ArgumentValue result = {};
    result._string = str;
    return result;
}

L5RP::BoolArgument::BoolArgument()
        : IArgument(
        InputType::WORD
)
{

}

L5RP::ArgumentValue L5RP::BoolArgument::parseValue(std::string const& str)
{

    ArgumentValue result = {};
    result._union._bool = BoolArgument::parse(str);
    return result;

}

bool L5RP::BoolArgument::parse(std::string const& str)
{

    if (str == u8"true" || str == u8"1")
        return true;

    if (str == u8"false" || str == u8"0")
        return false;

    throw ArgumentParseException(u8"@r" + str + u8" gali būti tik true arba false!");

}

L5RP::PlayerArgument::PlayerArgument(
        PlayerManager* playerManager
)
        : IArgument(
        InputType::WORD
)
{
    this->playerManager = playerManager;
}

L5RP::ArgumentValue L5RP::PlayerArgument::parseValue(
        std::string const& str
)
{

    ArgumentValue val = {};
    val._union._player = PlayerArgument::parse(str, this->playerManager);
    return val;

}

L5RP::Player* L5RP::PlayerArgument::parse(std::string const& str, PlayerManager const* manager)
{

    Player* player = manager->getPlayer(str);

    if (player == nullptr)
        throw ArgumentParseException(u8"@r" u8"Tokio žaidėjo nėra!");

    return player;

}

L5RP::VehicleArgument::VehicleArgument(
        VehicleManager* vehicleManager
)
        : IArgument(
        InputType::WORD
)
{
    this->vehicleManager = vehicleManager;
}

L5RP::ArgumentValue L5RP::VehicleArgument::parseValue(
        std::string const& str
)
{

    Vehicle* vehicle = this->vehicleManager->getVehicle(str);

    if (vehicle == nullptr)
        throw ArgumentParseException(u8"@r" u8"Mašina nebuvo rasta!");

    ArgumentValue val = {};
    val._union._vehicle = vehicle;
    return val;

}

L5RP::CallableArgumentCountException::CallableArgumentCountException(
        std::initializer_list<std::string> const& msg
)
        : ChatException(std::vector<std::string>(msg))
{

}

L5RP::CallableArgumentCountException::CallableArgumentCountException(
        std::vector<std::string> const& msg
)
        : ChatException(msg)
{

}