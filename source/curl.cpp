#include "api.h"

L5RP::CurlTask::CurlTask(
    CurlManager* manager,
    std::function<void(std::string const& result)> const& fn,
    std::string const& url,
    std::string const& postData
)
{
    this->url = url;
    this->postData = postData;
    this->fn = fn;

    manager->execute(this);
}

struct MemoryStruct
{
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
)
{
    size_t realsize = size * nmemb;
    auto mem = (MemoryStruct*) userp;

    mem->memory = (char*) realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == nullptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = '\0';

    return realsize;
}

void L5RP::CurlTask::execute()
{

    CURL* curl = curl_easy_init();

    MemoryStruct chunk = {};
    chunk.size = 0;
    chunk.memory = nullptr;

    curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, this->postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteMemoryCallback
    );

    curl_easy_perform(curl);

    this->result.assign(chunk.memory, chunk.size);
    free(chunk.memory);

    curl_easy_cleanup(curl);

}

void L5RP::CurlTask::executeResult()
{
    this->fn(this->result);
}

L5RP::CurlManager::CurlManager(Plugin* plugin)
    : PluginAttachment(plugin)
{
    this->getPlugin()->getConsole()->trace("Waiting for CURL thread to begin...");

    this->_threadShouldRun = true;
    this->_thread = std::thread(&CurlManager::threadFuncion, this);

    this->getPlugin()->getConsole()->trace("CURL thread has just started!");
}

L5RP::CurlManager::~CurlManager()
{
    this->getPlugin()->getConsole()->trace("Waiting for CURL thread to join...");

    //--- Handles input.
    //Only locks the lock and executed further code if lock is currently unlocked.
    this->_inputLock.lock();
    while(!this->_preinputTasks.empty()) {
        this->_inputTasks.push(this->_preinputTasks.back());
        this->_preinputTasks.pop();
    }
    this->_inputLock.unlock();


    this->_threadShouldRun = false;
    this->_thread.join();

    //--- Handles output.
    //Only locks the lock and executed further code if lock is currently unlocked.
    this->_outputLock.lock();
    std::vector<CurlTask*> tasks;

    while(!this->_outputTasks.empty()) {
        tasks.push_back(this->_outputTasks.back());
        this->_outputTasks.pop();
    }

    this->_outputLock.unlock();

    for(
        CurlTask* task : tasks
        ) {
        //task->executeResult();
        //delete task;
    }

    this->getPlugin()->getConsole()->trace("CURL thread has just joined!");
}

void L5RP::CurlManager::execute(CurlTask* curlCall)
{

    this->_preinputTasks.push(curlCall);

}

void L5RP::CurlManager::threadFuncion()
{
    curl_global_init(CURL_GLOBAL_ALL);

    while(this->_threadShouldRun) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        //Allocating space for inputs.
        std::vector<CurlTask*> tasks;
        std::vector<CurlTask*> results;

        //Reading inputs.
        this->_inputLock.lock();
        while(!this->_inputTasks.empty()) {

            tasks.push_back(this->_inputTasks.front());
            this->_inputTasks.pop();

        }
        this->_inputLock.unlock();

        //Executing inputs getting outputs.
        for(
            CurlTask* task : tasks
            ) {
            task->execute();

            results.push_back(task);
        }

        //Pushing outputs.
        this->_outputLock.lock();
        for(
            CurlTask* result : results
            )
            _outputTasks.push(result);
        this->_outputLock.unlock();
    }

    curl_global_cleanup();
}

void L5RP::CurlManager::update()
{
    //--- Handles input.
    //Only locks the lock and executed further code if lock is currently unlocked.
    if(this->_inputLock.try_lock()) {

        while(!this->_preinputTasks.empty()) {

            this->_inputTasks.push(this->_preinputTasks.front());
            this->_preinputTasks.pop();
        }

        this->_inputLock.unlock();
    }

    //--- Handles output.
    //Only locks the lock and executed further code if lock is currently unlocked.
    if(this->_outputLock.try_lock()) {
        std::vector<CurlTask*> tasks;

        while(!this->_outputTasks.empty()) {

            tasks.push_back(this->_outputTasks.front());
            this->_outputTasks.pop();
        }

        this->_outputLock.unlock();

        for(
            CurlTask* task : tasks
            ) {
            task->executeResult();

            delete task;
        }
    }
}
