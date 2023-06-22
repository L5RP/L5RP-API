#pragma once

namespace L5RP {

    class TimePoint {

    public:

        enum Day {
            MONDAY,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY,
            SUNDAY
        };

        static std::string ToString(Day day);

    private:

        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        Day day;

    public:

        TimePoint(Day day, uint8_t hours, uint8_t minutes, uint8_t seconds);

        bool equals(TimePoint const& timePoint) const;

        uint8_t getSeconds() const;
        uint8_t getMinutes() const;
        uint8_t getHours() const;
        Day getDay() const;
    };

    class TimedEvent {

        friend class Clock;

        Clock* clock;
        TimePoint time;
        std::function<void()> callback;

        void update(TimePoint const& time);

    public:

        TimedEvent(Clock* clock, TimePoint const& time, std::function<void()> const& callback);
        virtual ~TimedEvent();

        TimePoint const& getTime() const;
        Clock* getClock() const;

    };

    class Clock {

        friend class TimedEvent;

        Plugin* plugin;
        std::chrono::hours timeOffset;
        bool realTime = false;
        std::vector<TimedEvent*> events;

        void registerTimedEvent(TimedEvent* event);
        void unregisterTimedEvent(TimedEvent* event);

    public:

        explicit Clock(Plugin* plugin);
        ~Clock();

        TimePoint fetchTime() const;
        time_t getSecondsSinceEpoch() const;
        std::string convertSecondsSinceEpochToString(time_t seconds) const;

        Plugin* getPlugin() const;
        std::chrono::hours getTimeOffset() const;
        void setTimeOffset(std::chrono::hours timeOffset);
        bool isRealTimeEnabled() const;
        void enableRealTime(bool enabled);
        std::vector<TimedEvent*> const& getTimedEvents() const;
    };

}