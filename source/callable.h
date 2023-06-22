#pragma once

namespace L5RP {

    //Eech really bad soliution, but i cant think of anything better atm.
    union _ArgumentValue {
        bool _bool;
        int _int;
        uint32_t _uint32_t;
        float _float;
        uint8_t _uint8_t;
        Player* _player;
        Vehicle* _vehicle;
        void* _ptr;
    };

    struct ArgumentValue {
        _ArgumentValue _union;
        std::string _string;
    };

    class ArgumentParseException
            : public Exception {

    public:

        explicit ArgumentParseException(std::string const& msg);

    };

    class ArgumentOutOfLimits
            : public ArgumentParseException {

    public:

        explicit ArgumentOutOfLimits(std::string const& msg);

    };

    class IArgument {

    public:

        enum InputType {
            NONE,
            WORD,
            DUMP
        };

    private:

        InputType inputType;

    public:

        explicit IArgument(
                InputType inputType
        );

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str) = 0;

        InputType getInputType() const;

    };

    class IntArgument
            : public IArgument {

    public:

        IntArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

        static int parse(std::string const& str);
    };

    class UnsignedInt32Argument
        : public IArgument {

    public:

        UnsignedInt32Argument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

        static uint32_t parse(std::string const& str);
    };

    /**
     * Basically IntArgument but has min max values.
     */
    class LimitedIntArgument
            : public IArgument {

        int min;
        int max;

    public:

        /**
         * min - Smallest allowed variable.
         * max - Biggest allowed variable.
         */
        LimitedIntArgument(int min, int max);

        virtual ArgumentValue parseValue(std::string const& str);

        static int parse(std::string const& str, int min, int max);

        void setMin(int min);
        void setMax(int max);

        int getMin() const;
        int getMax() const;
    };

    /**
     * Same as LimitedIntArgument, but instead of throwing exception 
     * returns specified default value
     */
    class LimitedDefaultIntArgument
            : public IArgument {

        int min;
        int max;
        int def;

    public:

        LimitedDefaultIntArgument(int min, int max, int def);

        virtual ArgumentValue parseValue(std::string const& str);

        static int parse(std::string const& str, int min, int max, int def);

        int getDefault() const;

    };

    /**
     * Basically is a uint8_t argument.
     */
    class ByteArgument
        : public IArgument
    {

    public:

        ByteArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

        static uint8_t parse(std::string const& str);

    };

    class FloatArgument
            : public IArgument {

    public:

        FloatArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

        static float parse(std::string const& str);

    };

    /**
     * Basically FloatArgument but has min max values.
     */
    class LimitedFloatArgument
            : public IArgument
    {
        float min;
        float max;

    public:

        /**
         * min - Smallest allowed variable.
         * max - Biggest allowed variable.
         */
        LimitedFloatArgument(float min, float max);

        virtual ArgumentValue parseValue(std::string const& str);

        static float parse(std::string const& str, float min, float max);

        float getMin() const;
        float getMax() const;

    };

    /**
     * Same as LimitedFloatArgument, but instead of throwing exception 
     * returns specified default value
     */
    class LimitedDefaultFloatArgument
            : public IArgument
    {

        float min;
        float max;
        float def;

    public:

        LimitedDefaultFloatArgument(float min, float max, float _defaut);

        virtual ArgumentValue parseValue(std::string const& str);

        static float parse(std::string const& str, float min, float max, float def);

        float getDefault() const;

    };

    class StringArgument
            : public IArgument {

    public:

        StringArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

    };

    /**
     * This will allow strings with spaces inside of it.
     * Only should be last mandatory or optional argument.
     * However, there's no validation for that.
     */
    class StringDumpArgument
            : public IArgument {

    public:

        StringDumpArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

    };

    /**
     * Either true, or false.
     */
    class BoolArgument
            : public IArgument {

    public:

        BoolArgument();

        /**
         * @throws ArgumentParseException
         */
        virtual ArgumentValue parseValue(std::string const& str);

        static bool parse(std::string const& str);

    };

    /**
     * Finds a player.
     * 
     * Accepts id or name
     */
    class PlayerArgument
            : public IArgument {

        PlayerManager* playerManager;

    public:

        explicit PlayerArgument(PlayerManager* playerManager);

        virtual ArgumentValue parseValue(std::string const& str);

        static Player* parse(std::string const& str, PlayerManager const* manager);
    };

    /**
     * Finds a Vehicle.
     * 
     * Accepts id.
     */
    class VehicleArgument
            : public IArgument {
        VehicleManager* vehicleManager;

    public:

        explicit VehicleArgument(VehicleManager* vehicleManager);

        virtual ArgumentValue parseValue(std::string const& str);

    };

    class CallableArgumentCountException
            : public ChatException {

    public:

        explicit CallableArgumentCountException(
                std::initializer_list<std::string> const& exception
        );
        explicit CallableArgumentCountException(
                std::vector<std::string> const& exception
        );

    };

    template <typename T>
    class Callable {

        std::vector<IArgument*> mandatoryArgs;
        std::vector<IArgument*> optionalArgs;

    protected:

        /**
         * You can override this if you want to generate custom exceptions. 
         */
        virtual void throwArgsCountException();

    public:

        explicit Callable(
                std::vector<IArgument*> const& mandatoryArgs,
                std::vector<IArgument*> const& optionalArgs
        );
        virtual ~Callable();

        /**
         * Only throws CallableArgumentCountException,
         * if mandatory arguments are not provided.
         * 
         * @throws CallableArgumentCountException
         * @throws ArgumentParseException 
         */
        virtual T onExecute(std::string const& str);
        virtual T onExecute(std::vector<std::string> const& args);
        virtual T onExecute(std::vector<ArgumentValue> const& args) = 0;

        std::vector<IArgument*> const& getMandatoryArgs() const;
        std::vector<IArgument*> const& getOptionalArgs() const;

    };

}

template <typename T>
void L5RP::Callable<T>::throwArgsCountException()
{
    throw CallableArgumentCountException(
        {
            u8"Wrong number of arguments was passed to callable!"
        }
    );
}

template <typename T>
L5RP::Callable<T>::Callable(std::vector<IArgument*> const& mandatoryArgs, std::vector<IArgument*> const& optionalArgs)
{
    this->mandatoryArgs = mandatoryArgs;
    this->optionalArgs = optionalArgs;
}

template <typename T>
L5RP::Callable<T>::~Callable()
{

    for (IArgument* arg : this->mandatoryArgs)
        delete arg;
    this->mandatoryArgs.clear();

    for (IArgument* arg : this->optionalArgs)
        delete arg;
    this->optionalArgs.clear();

}

template <typename T>
T L5RP::Callable<T>::onExecute(std::string const& str)
{

    std::vector<std::string> res;

    std::stringstream args(str);

    std::string arg;
    while (args >> arg)
        res.push_back(arg);

    return this->onExecute(res);

}

template <typename T>
T L5RP::Callable<T>::onExecute(std::vector<std::string> const& args)
{

    //Lets count how many words we want to execute.
    size_t len = 0;
    for(auto argument : this->mandatoryArgs)
        if(argument->getInputType() != IArgument::InputType::NONE)
            len++;

    if (args.size() < len)
        this->throwArgsCountException();

    else {

        std::vector<ArgumentValue> result;

        size_t currArg = 0;
        for (size_t i = 0; i < args.size(); i++) {

            IArgument* currArgument = nullptr;

            while(true) {

                if (i < this->mandatoryArgs.size())
                    currArgument = this->mandatoryArgs.at(i);

                else if(i < this->mandatoryArgs.size() + this->optionalArgs.size())
                    currArgument = this->optionalArgs.at(i - this->mandatoryArgs.size());

                else
                    break;

                if(currArgument->getInputType() != IArgument::InputType::NONE)
                    break;

                result.push_back(currArgument->parseValue(""));
                currArg++;
            }

            if(currArgument == nullptr)
                break;

            //Some handling for StringDumpArgument.
            if (currArgument->getInputType() == IArgument::DUMP) {

                //If argument is of StringDumpArgument type.
                //We want to construct string with all remaining args.

                std::string constructedString = args[i];
                for (size_t j = i + 1; j < args.size(); j++)
                    constructedString += " " + args[j];

                result.push_back(currArgument->parseValue(constructedString));

                break;
            }

            //Then we can parse our string and add it to result.
            //NOTE: We dont want to handle any exceptions here.
            result.push_back(currArgument->parseValue(args.at(i)));
            currArg++;

        }

        return this->onExecute(result);

    }

    return T();
}

template <typename T>
std::vector<L5RP::IArgument*> const& L5RP::Callable<T>::getMandatoryArgs() const
{
    return this->mandatoryArgs;
}

template <typename T>
std::vector<L5RP::IArgument*> const& L5RP::Callable<T>::getOptionalArgs() const
{
    return this->optionalArgs;
}