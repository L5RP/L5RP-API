#include "api.h"

using namespace L5RP;

Console* Console::instance = nullptr;

void Console::threadFunc()
{

    while(this->_threadShouldRun) {

        //Waiting and reading input from std::cin.
        std::string input;
        std::getline(std::cin, input, '\n');

        //Pushing contents to main thread.
        this->_outputLock.lock();
        _output.push(input);
        this->_outputLock.unlock();

        if(input == "exit" || input == "stop")
            break;
    }

}

void Console::update()
{

    if(this->_outputLock.try_lock()) {
        std::vector<std::string> tasks;

        while(!this->_output.empty()) {
            tasks.push_back(this->_output.back());
            this->_output.pop();
        }

        this->_outputLock.unlock();

        for(auto const& msg : tasks) {

            //Initializing name and args
            std::string name;
            std::string args;

            size_t it = msg.find(' ');
            if(it == std::string::npos)
                name = msg;

            else {
                name = msg.substr(0, it);
                args = msg.substr(it + 1, msg.size());
            }

            //Executing something.
            try {

                this->commands.at(name)->onExecute(args);

            }

            catch(ChatException const& e) {
                for(auto const& _msg : e.getMessages())
                    std::cout << _msg << std::endl;

            }

            catch(Exception const& e) {
                std::cout << e.getMessage() << std::endl;
            }

            catch(std::out_of_range const&) {
                std::cout << u8"* Tokios komandos nėra!" << std::endl;
            }

        }
    }

}

void Console::registerCommand(Command* command, std::string const& name)
{
    this->commands[name] = command;
}

#define HELP_CMD_COUNT_PER_PAGE 10

Console::Console(Plugin* plugin)
    : PluginAttachment(plugin)
{
    Console::instance = this;

    this->initializeLogger();

    this->_threadShouldRun = true;

    this->_thread = std::thread(&Console::threadFunc, this);

    //Initializing some default commands.
    new Console::Command(
        this,
        [](std::vector<ArgumentValue> const& args)
        {
            for(uint32_t i = 0; i < 20; i++)
                std::cout << std::endl;
        },
        {
            u8"clear", u8"valyti"
        },
        {},
        {},
        u8"Išvalo consolės logą"
    );


    new Console::Command(
        this,
        [this](std::vector<ArgumentValue> const& args)
        {
            this->getPlugin()->Unload();
        },
        {
            u8"stop", u8"exit"
        },
        {},
        {},
        u8"Išjungia serverį"
    );

    new Console::Command(
        this,
        [this](std::vector<ArgumentValue> const& args)
        {
            //Lets fetch all commands.
            std::unordered_map<std::string, Command*> const& allCommands = this->commands;

            //Lets count page count.
            int pageCount = (int) std::ceil(allCommands.size() / HELP_CMD_COUNT_PER_PAGE) + 1;

            //Lets determine which page we want to render.
            int page = 1;
            if(!args.empty())
                page = args[0]._union._int;

            //Some validation.
            if(page < 1 || page > pageCount)
                std::cout << u8"Toks puslapis neegzistuoja!" << std::endl;

            else {

                //Some more counting.
                int begin = (page - 1) * HELP_CMD_COUNT_PER_PAGE;
                int end = page * HELP_CMD_COUNT_PER_PAGE;

                //Rendering
                std::cout << u8"Pagalbos meniu | Puslapis: " << page << u8"/" << pageCount << std::endl;

                auto it = allCommands.begin();

                if(begin != 0)
                    std::advance(it, begin);

                for(int i = begin; i < allCommands.size() && i < end; i++) {
                    std::cout << u8"* " << it->first << u8" - " << it->second->getDescription() << u8"!" << std::endl;

                    if(it != allCommands.end())
                        it++;

                }

            }
        },
        {
            u8"help", u8"pagalba", u8"komandos"
        },
        {},
        {
            new PlayerCommand::Argument(
                new IntArgument(),
                u8"puslapis",
                u8"Nurodo, kuris puslapis bus rodomas"
            )
        },
        u8"Rodo šį pagalbos meniu"
    );
}

Console::~Console()
{
    this->trace("Waiting for Console thread to join...");
    this->_threadShouldRun = false;
    this->_thread.join();
    this->trace("Console thread has joined!");

    this->info("Server has been shut down");

    this->disposeLogger();
}

void Console::initializeLogger()
{
    using namespace std::experimental;

    //Initializing console logger.
    spdlog::stdout_color_mt(CONSOLE_LOG);
    auto consoleLogger = spdlog::get(CONSOLE_LOG);
    consoleLogger->set_level(spdlog::level::info);

    //Initializing file logger.
    if(!filesystem::exists("logs"))
        filesystem::create_directory("logs");

    spdlog::basic_logger_mt(FILE_LOG, "logs/server.txt");

    auto fileLogger = spdlog::get(FILE_LOG);
    fileLogger->flush_on(spdlog::level::err);
    fileLogger->set_level(spdlog::level::trace);

    //Reporting that both loggers are initialized.
    this->trace("Logger has been initialized!");
}

void Console::disposeLogger()
{
    this->trace("Logger has been destroyed!");

    spdlog::get(CONSOLE_LOG)->flush();
    spdlog::get(FILE_LOG)->flush();

    spdlog::drop_all();
}

void Console::Command::throwArgsCountException()
{

    std::vector<std::string> msg;
    std::string msg1 = u8"* Naudojimas: /" + this->names.at(0);

    for(Command::Argument* arg : this->mandatoryArgs)
        msg1 += u8" <" + arg->getName() + u8">";

    for(Command::Argument* arg : this->optionalArgs)
        msg1 += u8" [" + arg->getName() + u8"]";

    msg.push_back(msg1);

    for(Command::Argument* arg : this->mandatoryArgs)
        msg.push_back(arg->getName() + u8" - " + arg->getDescription());

    for(Command::Argument* arg : this->optionalArgs)
        msg.push_back(arg->getName() + u8" - " + arg->getDescription());

    if(this->names.size() != 1) {

        std::string msg2 = u8"* Ši komanda turi dar vardų: ";

        for(size_t i = 1; i < this->names.size(); i++)
            msg2 += u8"/" + this->names[i] + u8" ";

        msg.push_back(msg2);

    }

    throw CallableArgumentCountException(msg);

}

Console::Command::Command(
    Console* console,
    std::function<void(std::vector<ArgumentValue> const&)> const& fn,
    std::initializer_list<std::string> const& names,
    std::initializer_list<Argument*> const& mandatoryArgs,
    std::initializer_list<Argument*> const& optionalArgs,
    std::string const& desc
)
    : Callable<void>(
    ARGUMENT_CONVERTER(mandatoryArgs),
    ARGUMENT_CONVERTER(optionalArgs)
)
{

    this->console = console;
    this->fn = fn;
    this->desc = desc;
    this->names = std::vector<std::string>(names);
    this->mandatoryArgs = std::vector<Argument*>(mandatoryArgs);
    this->optionalArgs = std::vector<Argument*>(optionalArgs);

    for(std::string const& name : names)
        this->getConsole()
            ->registerCommand(this, name);

}

Console::Command::~Command()
{
    for(Argument* arg : this->mandatoryArgs)
        delete arg;

    for(Argument* arg : this->optionalArgs)
        delete arg;
}

void L5RP::Console::Command::onExecute(std::string const& args)
{
    Callable<void>::onExecute(args);
}

void L5RP::Console::Command::onExecute(std::vector<std::string> const& args)
{
    Callable<void>::onExecute(args);
}

void L5RP::Console::Command::onExecute(std::vector<ArgumentValue> const& args)
{
    this->fn(args);
}

std::string const& L5RP::Console::Command::getDescription() const
{
    return this->desc;
}

Console* L5RP::Console::Command::getConsole() const
{
    return this->console;
};


