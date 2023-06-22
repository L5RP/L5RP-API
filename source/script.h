#pragma once

namespace L5RP
{

    class Script;
    typedef uint32_t ScriptType;

    class ScriptManager
        : public PluginAttachment
    {
        friend class Plugin;

        std::map<ScriptType, Script*> _scripts;

        void initScripts();
        void freeScripts();

        void onTick();
        void onServerFullyInitialized();

    public:

        explicit ScriptManager(Plugin* plugin);
        ~ScriptManager();

        void registerScript(Script* script);

        Script* getRawScript(ScriptType type) const;

        template <typename T>
        T* getScript(ScriptType type) const { return (T*) this->getRawScript(type); }
    };

    class Script
    {
    public:

        Script(ScriptManager* manager, ScriptType scriptType);

        //This is called when Server is Fully Initialized.
        //(All Scripts loaded, connected to database).
        //You can only interact with other scripts after onServerInit
        virtual void onServerInit() {};
        virtual void onServerFullInit() {}; // <---- Is called when server accepts player connections. And all calls with database are finished.
        virtual void onServerExit() {}
        virtual void onServerTick() {}

        ScriptManager* getManager() const;
        ScriptType getType() const;

    private:

        ScriptManager* _manager;
        ScriptType _type;

    };

    // ------------------------------ Helper classes when developing scripts
    template <typename T>
    class ScriptAttachment
    {

        T* script;

    public:

        explicit ScriptAttachment(T* script)
        {
            this->script = script;
        }

        T* getScript() const
        {
            return this->script;
        }

    };

    template <typename T>
    class ScriptTimer
        : public ScriptAttachment<T>,
          public Timer
    {

    public:

        ScriptTimer(T* script, std::chrono::milliseconds time, std::function<void()> const& fn)
            : ScriptAttachment<T>(script),
              Timer(script->getManager()->getPlugin()->getTimerManager(), time, fn) {}

    };

    template <typename T>
    class ScriptPromise
        : public ScriptAttachment<T>,
          public Promise
    {

    public:

        ScriptPromise(T* script, std::chrono::milliseconds time, std::function<void()> const& fn)
            : ScriptAttachment<T>(script),
              Promise(script->getManager()->getPlugin()->getTimerManager(), time, fn) {}

    };
}