#pragma once

#define SOCKET_INPUT_MAX_LENGTH 1024

namespace L5RP
{

    class SocketManager
        : public PluginAttachment
    {
        friend class Plugin;
        friend class SocketCommand;

        std::unordered_map<std::string, SocketCommand*> commands;
        SocketListener* listener;

        void update();

    public:

        explicit SocketManager(Plugin* plugin);
        virtual ~SocketManager();

        SocketCommand* getCommand(std::string const& command) const;
    };

    class SocketRequest {
        friend class Socket;

        Socket* socket;
        std::string request = "";
        std::string answer = "";

        //Called from main thread.
        void process();

    public:

        SocketRequest(Socket* socket, std::string const& request);

        Socket* getSocket() const;
        bool isAnswered() const;
        std::string const& getRequest() const;
        std::string const& getAnswer() const;
    };

    class SocketCommand
        : public Callable<std::string>
    {

        SocketManager* manager;
        std::function<std::string(std::vector<ArgumentValue> const&)> fn;
        std::string name;

    public:

        SocketCommand(
            SocketManager* manager,
            std::function<std::string(std::vector<ArgumentValue> const&)> const& fn,
            std::string const& name,
            std::initializer_list<IArgument*> const& mandatoryArgs,
            std::initializer_list<IArgument*> const& optionalArgs
        );

        std::string onExecute(std::string const& args) override;
        std::string onExecute(std::vector<std::string> const& args) override;
        std::string onExecute(std::vector<ArgumentValue> const& args) override;

        SocketManager* getManager() const;
        std::string const& getName() const;

    };

    class Socket
    {
        friend class SocketListener;

        SocketListener* listener;
        std::string ip;
        std::mutex currRequestLock;
        SocketRequest* currRequest = nullptr;
        std::atomic<bool> alive = true;

#ifdef _WIN32

        SOCKET handle;

#else

        int handle;

#endif

        std::thread _thread;
        std::atomic<bool> _threadShouldRun = false;

        void threadFunction();
        void dispose();
        std::string waitForMessage();
        void sendMessage(std::string const& message);

        void update();

    public:

#ifdef _WIN32

        Socket(SocketListener* listener, SOCKET socketHandle, std::string const& ip);

        SOCKET getHandle() const;

#else

        Socket(SocketListener* listener, int handle, std::string const& ip);

        int getHandle() const;

#endif

        virtual ~Socket();

        /**
         * Starts the inner thread. And starts to process requests that the socket provides.
         */
        void begin();
        void end();

        /**
         * Tells if socket is fully initialized and is alive.
         */
        bool isWorking() const;
        std::string const& getIP() const;
        SocketListener* getListener() const;

    };

    class SocketListener
    {
        friend class SocketManager;
        friend class Socket;

#ifdef _WIN32

        WSADATA wsaData = {};
        addrinfo* socketInfo = nullptr;
        SOCKET socketHandle = INVALID_SOCKET;

#else

        //Not too sure if i need to keep those here.
        //However, if i will find proper way to
        //dispose all of this i think this will be useful.
        int socketFileDescriptorID;
        int opt = 1; // Not sure what this one means :/

#endif

        SocketManager* manager;
        uint16_t port;

        std::mutex aliveSocketLock;
        std::vector<Socket*> aliveSockets;

        std::thread _thread;
        std::atomic<bool> _threadShouldRun = false;

        void threadFunction();

        //Those will be called from inner thread.
        void initialize();
        Socket* waitForConnection(); // <--- Freezes the thread.
        void destroy();

        //Those will be called from main thread.
        void update();
        void removeSocket(Socket* socket);

    public:

        SocketListener(SocketManager* manager, uint16_t port);
        virtual ~SocketListener();

        uint16_t getPort() const;
        SocketManager* getManager() const;

    };

}