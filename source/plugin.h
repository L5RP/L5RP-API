#pragma once

namespace L5RP {

    class Plugin :
            public rage::IPlugin,
            public rage::IEventHandler,
            public rage::ITickHandler,
            public rage::IPlayerHandler {

        void unloadScripts();

    public:

        explicit Plugin(rage::IMultiplayer* rage);
        ~Plugin();

        //Event Handling.
        rage::ITickHandler* GetTickHandler() override;

        void Tick() override;
        void Unload() override;

        rage::IMultiplayer* getRageMP() const;
        PlayerManager* getPlayerManager() const;
        VehicleManager* getVehicleManager() const;
        CheckPointManager* getCheckPointManager() const;
        ScriptManager* getScriptManager() const;
        TimerManager* getTimerManager() const;
        CurlManager* getCurlManager() const;
        Console* getConsole() const;
        Clock* getClock() const;
        SocketManager* getSocketManager() const;
        BlipManager* getBlipManager() const;

        bool isInitialized() const;

    private:

        rage::IMultiplayer* _rage;

        PlayerManager* _playerManager;
        VehicleManager* _vehicleManager;
        CheckPointManager* _checkPointManager;
        ScriptManager* _scriptManager;
        TimerManager* _timerManager;
        CurlManager* _curlManager;
        Console* _console;
        Clock* _clock;
        SocketManager* _socketManager;
        BlipManager* _blipManager;

        bool initialized = false;
    };

    class PluginAttachment {
        Plugin* plugin;

    public:

        explicit PluginAttachment(Plugin* plugin);

        Plugin* getPlugin() const;
    };

}