#pragma once

namespace L5RP {

    class CurlManager;

    class CurlTask {

        friend class CurlManager;

        std::function<void(std::string const& result)> fn;
        std::string url;
        std::string postData;
        std::string result;

        void execute();
        void executeResult();

    public:

        CurlTask(
                CurlManager* manager,
                std::function<void(std::string const& result)> const& fn,
                std::string const& url,
                std::string const& postData
        );
        virtual ~CurlTask() = default;

    };

    class CurlManager
        : public PluginAttachment
    {

        friend class CurlTask;
        friend class Plugin;

        std::thread _thread;
        std::atomic<bool> _threadShouldRun;

        std::mutex _inputLock;
        std::mutex _outputLock;
        std::queue<CurlTask*> _preinputTasks;
        std::queue<CurlTask*> _inputTasks;
        std::queue<CurlTask*> _outputTasks;

        void execute(CurlTask* curlCall);
        void threadFuncion();
        void update();

    public:

        explicit CurlManager(Plugin* plugin);
        ~CurlManager();

    };

}