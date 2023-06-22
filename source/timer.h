#pragma once

namespace L5RP {

    class Timer;

    class DeltaTimeCounter {
        std::chrono::time_point<std::chrono::system_clock> _lastTime;

    public:

        DeltaTimeCounter();

        //Calculates how much time has passed since last check.
        std::chrono::milliseconds calcDeltaTime();

        //Resets counter
        void reset();
    };

    class TimerManager :
            public PluginAttachment {

        friend class Plugin;
        friend class Timer;

        std::vector<Timer*> _timers;

        void registerTimer(Timer* timer);
        void unregisterTimer(Timer* timer);

        void onTick();

    public:

        explicit TimerManager(Plugin* plugin);
        ~TimerManager();

    };

    class Timer :
            public DeltaTimeCounter {
        friend class TimerManager;

        TimerManager* manager;
        std::chrono::milliseconds duration;
        std::function<void()> function;

        void onTick();

    public:

        Timer(
                TimerManager* manager,
                std::chrono::milliseconds duration,
                std::function<void()> const& execute
        );
        virtual ~Timer();

        TimerManager* getManager() const;
        std::chrono::milliseconds getDuration() const;
    };

    class Promise :
            public Timer {

    public:

        Promise(
                TimerManager* manager,
                std::chrono::milliseconds duration,
                std::function<void()> const& execute
        );
        virtual ~Promise() = default;

    };
}