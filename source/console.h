#pragma once

#define CONSOLE_LOG "console"
#define FILE_LOG "file"

namespace L5RP
{



    class Console
        : public PluginAttachment
    {

    public:

        class Command
            : public Callable<void>
        {

        public:

            typedef PlayerCommand::Argument Argument;

        private:

            Console* console;
            std::string desc;
            std::vector<std::string> names;
            std::vector<Argument*> mandatoryArgs;
            std::vector<Argument*> optionalArgs;
            std::function<void(std::vector<ArgumentValue> const&)> fn;

        protected:

            void throwArgsCountException() override;

        public:
            Command(
                Console* console,
                std::function<void(std::vector<ArgumentValue> const&)> const& fn,
                std::initializer_list<std::string> const& names,
                std::initializer_list<Argument*> const& mandatoryArgs,
                std::initializer_list<Argument*> const& optionalArgs,
                std::string const& description
            );
            virtual ~Command();

            void onExecute(std::string const& args) override;
            void onExecute(std::vector<std::string> const& args) override;
            void onExecute(std::vector<ArgumentValue> const& args) override;

            Console* getConsole() const;
            std::string const& getDescription() const;
        };

    private:

        friend class Plugin;
        friend class Command;

        static Console* instance;

        //Threader
        std::thread _thread;
        std::atomic<bool> _threadShouldRun;

        std::mutex _outputLock;
        std::queue<std::string> _output;

        void update();
        void threadFunc();

        //Callable
        std::unordered_map<std::string, Command*> commands;

        void registerCommand(Command* command, std::string const& name);

        void initializeLogger();
        void disposeLogger();

    public:

        explicit Console(Plugin* plugin);
        ~Console();

        static Console* getInstance()
        {
            return Console::instance;
        }

        template <typename T>
        inline Console const* trace(T const& log) const {
            spdlog::get(CONSOLE_LOG)->trace(log);
            spdlog::get(FILE_LOG)->trace(log);

            return this;
        }

        template <typename T>
        inline Console const* debug(T const& log) const {
            spdlog::get(CONSOLE_LOG)->debug(log);
            spdlog::get(FILE_LOG)->debug(log);

            return this;
        }

        template <typename T>
        inline Console const* info(T const& log) const {
            spdlog::get(CONSOLE_LOG)->info(log);
            spdlog::get(FILE_LOG)->info(log);

            return this;
        }

        template <typename T>
        inline Console const* warn(T const& log) const {
            spdlog::get(CONSOLE_LOG)->warn(log);
            spdlog::get(FILE_LOG)->warn(log);

            return this;
        }

        template <typename T>
        inline Console const* error(T const& log) const {
            spdlog::get(CONSOLE_LOG)->error(log);
            spdlog::get(FILE_LOG)->error(log);

            return this;
        }

        template <typename T>
        inline Console const* critical(T const& log) const {
            spdlog::get(CONSOLE_LOG)->critical(log);
            spdlog::get(FILE_LOG)->critical(log);

            return this;
        }
    };

}