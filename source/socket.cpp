#include "api.h"

using namespace L5RP;

SocketManager::SocketManager(Plugin* plugin)
    : PluginAttachment(plugin)
{
    this->getPlugin()->getConsole()->trace("Socket manager thread is starting...");

    Config::getInstance().setDefaultValueAsLong("socket", "port", 22007);

    auto port = static_cast<uint16_t>(
        Config::getInstance().readValueAsLong("socket", "port")
    );

    this->listener = new SocketListener(
        this,
        port
    );

    new SocketCommand(
        this,
        [](std::vector<ArgumentValue> const& e) -> std::string
        {
            return "Hello World!";
        },
        u8"hello",
        {},
        {}
    );

    this->getPlugin()->getConsole()->trace("Socket manager thread has been started on port " + std::to_string(port) + "!");
}

SocketManager::~SocketManager()
{

    delete this->listener;

    for(auto pair : this->commands)
        delete pair.second;

}

SocketCommand* SocketManager::getCommand(std::string const& command) const
{
    try {
        return this->commands.at(command);
    } catch(std::out_of_range const&) {
        return nullptr;
    }
}

void SocketManager::update()
{

    this->listener->update();

}

SocketListener::SocketListener(SocketManager* manager, uint16_t port)
{
    this->manager = manager;
    this->port = port;

    this->_threadShouldRun = true;
    this->_thread = std::thread(&SocketListener::threadFunction, this);

}

SocketListener::~SocketListener()
{
    this->_threadShouldRun = false;

    this->aliveSocketLock.lock();

    for(auto socket : this->aliveSockets)
        delete socket;

    this->aliveSocketLock.unlock();

    this->destroy();
    this->_thread.join();
}

void SocketListener::threadFunction()
{

    try {
        this->initialize();

        while(this->_threadShouldRun) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            auto socket = this->waitForConnection();

            this->aliveSocketLock.lock();
            this->aliveSockets.push_back(socket);
            this->aliveSocketLock.unlock();
        }

    }

    catch(Exception const& e) {
        std::cout << "Exception happened inside Socket Listener thread. Thread execution will be stopped!" << std::endl;
        std::cout << "Error: " << e.getMessage() << std::endl;
    }
}

uint16_t SocketListener::getPort() const
{
    return this->port;
}

SocketManager* SocketListener::getManager() const
{
    return this->manager;
}

void SocketListener::update()
{

    if(this->aliveSocketLock.try_lock()) {

        for(auto socket : this->aliveSockets)
            socket->update();

        this->aliveSocketLock.unlock();
    }

}

void SocketListener::removeSocket(Socket* socket) {
    this->aliveSockets.erase(
        std::remove(
            this->aliveSockets.begin(),
            this->aliveSockets.end(),
            socket
        ),
        this->aliveSockets.end()
    );
}

#ifdef _WIN32

void SocketListener::initialize() {

    auto result = WSAStartup(
            MAKEWORD(2, 2),
            &wsaData
    );

    if(result != 0)
        throw Exception("Failed to initialize WinSocket2 library! Error: " + std::to_string(result));

    addrinfo socketInfo = {};
    socketInfo.ai_family = AF_INET; //ipv4
    socketInfo.ai_socktype = SOCK_STREAM;
    socketInfo.ai_protocol = IPPROTO_TCP;
    socketInfo.ai_flags = AI_PASSIVE;

    result = getaddrinfo(
          nullptr,
          std::to_string(this->port).c_str(),
          &socketInfo,
          &this->socketInfo
    );
    if(result != 0)
        throw Exception("Failed to initialize socket adress! Error: " + std::to_string(result));

    this->socketHandle = socket(
            this->socketInfo->ai_family,
            this->socketInfo->ai_socktype,
            this->socketInfo->ai_protocol
    );

    if(this->socketHandle == INVALID_SOCKET)
        throw Exception("Failed to create socket! Error: " + WSAGetLastError());

    result = bind(
            this->socketHandle,
            this->socketInfo->ai_addr,
            static_cast<int>(
                    this->socketInfo->ai_addrlen
            )
    );
    if(result == SOCKET_ERROR)
        throw Exception("Failed to bind socket to a port! Check if port is not already in use! Error: " + WSAGetLastError());


}

Socket* SocketListener::waitForConnection() {

    if(listen(this->socketHandle, SOMAXCONN) == SOCKET_ERROR)
        throw Exception("Failed to listen for a new connection!");

    SOCKET client = INVALID_SOCKET;
    sockaddr_in address = {};
    int size = sizeof(address);

    client = accept(
            this->socketHandle,
            (sockaddr*)&address,
            &size
    );

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ip, INET_ADDRSTRLEN);

    return new Socket(this, client, std::string(
            ip
    ));

}

void SocketListener::destroy() {
    if(this->socketHandle != INVALID_SOCKET)
        closesocket(this->socketHandle);

    if(this->socketInfo != nullptr)
        freeaddrinfo(this->socketInfo);

    WSACleanup();
}

void Socket::dispose() {
    closesocket(
            this->handle
    );
}

void Socket::sendMessage(std::string const& message)
{
    auto result = send(
            this->handle,
            message.c_str(),
            static_cast<int>(message.size()),
            0
    );

    if(result == SOCKET_ERROR)
        this->alive = false;
}

std::string Socket::waitForMessage() {
    char input[SOCKET_INPUT_MAX_LENGTH];

    auto result = recv(
            this->handle,
            input,
            SOCKET_INPUT_MAX_LENGTH,
            0
    );

    if(result == 0)
        this->alive = false;

    return std::string(input);
}

Socket::Socket(SocketListener* listener, SOCKET handle, std::string const& ip)
{
    this->listener = listener;
    this->handle = handle;
    this->ip = ip;
}

SOCKET Socket::getHandle() const
{
    return this->handle;
}

#else

void SocketListener::initialize()
{
    auto type =
        static_cast<sa_family_t>(
            AF_INET
        );

    //Registering server socket acceptor
    this->socketFileDescriptorID = socket(
        type,
        SOCK_STREAM,
        0
    );

    if(this->socketFileDescriptorID == 0)
        throw Exception(u8"Failed to register socket file descriptor");

    //Attaching server socket acceptor to port.
    auto result = setsockopt(
        this->socketFileDescriptorID,
        SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT,
        &this->opt,
        sizeof(this->opt)
    );

    if(result == -1)
        throw Exception(u8"Failed to register some utility that would free port after job is finished");

    sockaddr_in address = {};
    address.sin_family = type;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->getPort());

    result = bind(
        this->socketFileDescriptorID,
        (struct sockaddr*) &address,
        sizeof(address)
    );

    if(result < 0)
        throw Exception("Failed to attach socket to port. Probably port is already in use!");

    //Starts listening and put a cap into waiting connecctions.
    listen(
        this->socketFileDescriptorID,
        5
    );
}

Socket* SocketListener::waitForConnection()
{

    sockaddr_in socketAddress = {};
    socklen_t len = sizeof(socketAddress);

    auto socket = accept(
        this->socketFileDescriptorID,
        (sockaddr*) &socketAddress,
        &len
    );

    if(socket < 0)
        throw Exception("Failed to accept new connection request.");

    std::string ip = inet_ntoa(
        socketAddress.sin_addr
    );

    return new Socket(
        this,
        socket,
        ip
    );

}

void SocketListener::destroy()
{

    close(this->socketFileDescriptorID);

}

Socket::Socket(SocketListener* listener, int handle, std::string const& ip)
{
    this->listener = listener;
    this->handle = handle;
    this->ip = ip;
}

int Socket::getHandle() const
{
    return this->handle;
}

void Socket::dispose()
{
    close(
        this->handle
    );
}

std::string Socket::waitForMessage()
{
    char buffer[SOCKET_INPUT_MAX_LENGTH];
    bzero(buffer, SOCKET_INPUT_MAX_LENGTH);

    auto result = read(
        this->handle,
        buffer,
        SOCKET_INPUT_MAX_LENGTH
    );

    if(result == 0)
        this->alive = false;

    return std::string(buffer);
}

void Socket::sendMessage(std::string const& message)
{

    auto result = write(
        this->handle,
        message.c_str(),
        message.size()
    );

    if(result == -1)
        this->alive = false;

}

#endif

Socket::~Socket()
{

    if(this->_threadShouldRun)
        this->end();

    this->getListener()->removeSocket(this);

}

void Socket::begin()
{

    if(!this->_threadShouldRun) {
        this->_threadShouldRun = true;
        this->_thread = std::thread(&Socket::threadFunction, this);
    } else
        std::cout << "[WARN] Socket::begin was called twice!" << std::endl;
}

void Socket::end()
{

    if(this->_threadShouldRun) {
        std::cout << "Waiting for socket thread to stop...." << std::endl;

        this->_threadShouldRun = false;
        this->_thread.join();

        std::cout << "Socket thread was stopeed!" << std::endl;
    } else
        std::cout << "[WARN] Socket::end was called twice!" << std::endl;

}

#define SOCKET_THREAD_STATE_INPUT 0
#define SOCKET_THREAD_STATE_OUTPUT 1

void Socket::threadFunction()
{

    try {
        uint8_t threadState = SOCKET_THREAD_STATE_INPUT;

        while(this->_threadShouldRun) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if(threadState == SOCKET_THREAD_STATE_INPUT) {
                auto msg = this->waitForMessage();

                if(!this->alive)
                    break;

                this->currRequestLock.lock();
                this->currRequest = new SocketRequest(
                    this,
                    msg
                );
                this->currRequestLock.unlock();

                threadState = SOCKET_THREAD_STATE_OUTPUT;
            } else {
                this->currRequestLock.lock();

                if(this->currRequest->isAnswered()) {
                    this->sendMessage(
                        this->currRequest->getAnswer()
                    );
                    this->currRequest = nullptr;

                    threadState = SOCKET_THREAD_STATE_INPUT;
                }

                this->currRequestLock.unlock();
            }
        }

    }

    catch(Exception const& e) {
        std::cout << "Exception happened inside Socket thread. Thread execution will be stopped!" << std::endl;
        std::cout << "Error: " << e.getMessage() << std::endl;
    }

    this->dispose();

}

bool Socket::isWorking() const
{
    return this->_threadShouldRun;
}

std::string const& Socket::getIP() const
{
    return this->ip;
}

SocketListener* Socket::getListener() const
{
    return this->listener;
}

void Socket::update()
{
    if(!this->isWorking())
        this->begin();

    else if(!this->alive)
        delete this;

    else if(this->currRequestLock.try_lock()) {
        if(this->currRequest != nullptr)
            this->currRequest->process();

        this->currRequestLock.unlock();
    }
}

SocketRequest::SocketRequest(Socket* socket, std::string const& request)
{
    this->socket = socket;
    this->request = request;
}

std::string const& SocketRequest::getAnswer() const
{
    return this->answer;
}

std::string const& SocketRequest::getRequest() const
{
    return this->request;
}

bool SocketRequest::isAnswered() const
{
    return !this->answer.empty();
}

Socket* SocketRequest::getSocket() const
{
    return this->socket;
}

void SocketRequest::process()
{
    std::string cmdName;
    std::string cmdArgs;

    size_t it = this->request.find(' ');
    if(it == std::string::npos)
        cmdName = this->request;

    else {
        cmdName = this->request.substr(0, it);
        cmdArgs = this->request.substr(it + 1, this->request.size());
    }

    auto command = this->getSocket()->getListener()->getManager()->getCommand(cmdName);

    if(command != nullptr)
        this->answer = command->onExecute(cmdArgs);

    else
        this->answer = "Error: Requested Socket Command was not found!";
}

SocketCommand::SocketCommand(
    SocketManager* manager,
    std::function<std::string(std::vector<ArgumentValue> const&)> const& fn,
    std::string const& name,
    std::initializer_list<IArgument*> const& mandatoryArgs,
    std::initializer_list<IArgument*> const& optionalArgs
)
    : Callable<std::string>(
    mandatoryArgs,
    optionalArgs
)
{
    this->manager = manager;
    this->fn = fn;
    this->name = name;

    this->manager->commands[this->name] = this;
}

std::string SocketCommand::onExecute(std::string const& args)
{
    return Callable<std::string>::onExecute(args);
}

std::string SocketCommand::onExecute(std::vector<std::string> const& args)
{
    return Callable<std::string>::onExecute(args);
}

std::string SocketCommand::onExecute(std::vector<ArgumentValue> const& args)
{

    return this->fn(args);

}

SocketManager* SocketCommand::getManager() const
{
    return this->manager;
}

std::string const& SocketCommand::getName() const
{
    return this->name;
}