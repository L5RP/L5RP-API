#include "api.h"

using namespace L5RP;

L5RP::Plugin::Plugin(rage::IMultiplayer* rage)
{
    this->_rage = rage;
    rage->AddEventHandler(this);

    this->_timerManager = new TimerManager(this);
    this->_clock = new Clock(this);
    this->_console = new Console(this);
    this->_playerManager = new PlayerManager(this);
    this->_vehicleManager = new VehicleManager(this);
    this->_checkPointManager = new CheckPointManager(this);
    this->_blipManager = new BlipManager(this);
    this->_curlManager = new CurlManager(this);
    this->_socketManager = new SocketManager(this);
    this->_scriptManager = new ScriptManager(this);

    //After everything is initialized we want to save config.
    Config::getInstance().save();

    new Promise(
        this->getTimerManager(),
        std::chrono::seconds(5),
        [this]()
        {
            this->getConsole()->info("L5RP server has just been started!");
            this->initialized = true;

            this->_scriptManager->onServerFullyInitialized();
        }
    );
}

L5RP::Plugin::~Plugin()
{
    delete this->_curlManager;
    //delete this->_socketManager;
    delete this->_blipManager;
    delete this->_checkPointManager;
    delete this->_vehicleManager;
    delete this->_playerManager;
    delete this->_console;
    delete this->_clock;
    delete this->_timerManager;

    std::exit(0);
}

rage::ITickHandler* L5RP::Plugin::GetTickHandler()
{
    return this;
}

void L5RP::Plugin::Tick()
{

    this->_console->update();
    this->_curlManager->update();
    this->_timerManager->onTick();
    this->_socketManager->update();
    this->_scriptManager->onTick();

}

void L5RP::Plugin::unloadScripts() {

    this->_console->info("Unloading scripts...");
    this->_console->info("Server shutdown after 10 seconds!");

    delete this->_scriptManager;

    new Promise(
        this->getTimerManager(),
        std::chrono::seconds(10),
        [this]()
        {
            delete this;
        }
    );
}

void L5RP::Plugin::Unload()
{

    this->_console->info("Kicking all players...");
    this->_console->info("Scripts will be unloaded in 5 seconds!");

    this->getPlayerManager()->sendGlobalMessage(
        "Kicking players... Server shutdown after 15 seconds"
    );

    for(auto pair : this->getPlayerManager()->getPlayers())
        pair.second->kick("Server is going to shutdown!");

    new Promise(
        this->getTimerManager(),
        std::chrono::milliseconds(5000),
        [this]()
        {
            this->initialized = false;

            this->unloadScripts();
        }
    );

}

rage::IMultiplayer* L5RP::Plugin::getRageMP() const
{
    return this->_rage;
}

L5RP::PlayerManager* L5RP::Plugin::getPlayerManager() const
{
    return this->_playerManager;
}

L5RP::VehicleManager* L5RP::Plugin::getVehicleManager() const
{
    return this->_vehicleManager;
}

L5RP::ScriptManager* L5RP::Plugin::getScriptManager() const
{
    return this->_scriptManager;
}

L5RP::TimerManager* L5RP::Plugin::getTimerManager() const
{
    return this->_timerManager;
}

L5RP::CurlManager* L5RP::Plugin::getCurlManager() const
{
    return this->_curlManager;
}

CheckPointManager* Plugin::getCheckPointManager() const
{
    return this->_checkPointManager;
}

L5RP::Console* L5RP::Plugin::getConsole() const
{
    return this->_console;
}

Clock* Plugin::getClock() const
{
    return this->_clock;
}

SocketManager* Plugin::getSocketManager() const {
    return this->_socketManager;
}

BlipManager* Plugin::getBlipManager() const {
    return this->_blipManager;
}

bool Plugin::isInitialized() const
{
    return this->initialized;
}

L5RP::PluginAttachment::PluginAttachment(Plugin* plugin)
{
    this->plugin = plugin;
}

L5RP::Plugin* L5RP::PluginAttachment::getPlugin() const
{
    return this->plugin;
}