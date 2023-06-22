#include "api.h"

using namespace L5RP;

TimePoint::TimePoint(Day day, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    this->day = day;
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
}

std::string TimePoint::ToString(Day day)
{
    switch(day) {
        case Day::MONDAY:
            return "MONDAY";
        case Day::TUESDAY:
            return "TUESDAY";
        case Day::WEDNESDAY:
            return "WEDNESDAY";
        case Day::THURSDAY:
            return "THURSDAY";
        case Day::FRIDAY:
            return "FRIDAY";
        case Day::SATURDAY:
            return "SATURDAY";
        case Day::SUNDAY:
            return "SUNDAY";
        default:
            return "UNKNOWN";
    }
}

bool TimePoint::equals(TimePoint const& timePoint) const
{
    return timePoint.getDay() == this->day
           && timePoint.getHours() == this->hours
           && timePoint.getMinutes() == this->minutes
           && timePoint.getSeconds() == this->seconds;
}

uint8_t TimePoint::getHours() const
{
    return this->hours;
}

uint8_t TimePoint::getMinutes() const
{
    return this->minutes;
}

uint8_t TimePoint::getSeconds() const
{
    return this->seconds;
}

TimePoint::Day TimePoint::getDay() const
{
    return this->day;
}

void TimedEvent::update(TimePoint const& time)
{
    if(this->time.equals(time))
        this->callback();
}

TimedEvent::TimedEvent(Clock* clock, TimePoint const& time, std::function<void()> const& callback)
    : time(time.getDay(), time.getHours(), time.getMinutes(), time.getSeconds())
// Not sure why ^^^^ is required. Making a copy of it just to be safe.
{
    this->clock = clock;
    this->callback = callback;

    this->clock->registerTimedEvent(this);
}

TimedEvent::~TimedEvent()
{
    this->clock->unregisterTimedEvent(this);
}

TimePoint const& TimedEvent::getTime() const
{
    return this->time;
}

Clock* TimedEvent::getClock() const
{
    return this->clock;
}

void Clock::registerTimedEvent(TimedEvent* event)
{
    this->events.push_back(event);
}

void Clock::unregisterTimedEvent(TimedEvent* event)
{
    auto it = std::find(this->events.begin(), this->events.end(), event);
    if(it != this->events.end())
        this->events.erase(it);
}

Clock::Clock(Plugin* plugin)
{
    this->plugin = plugin;

    Config::getInstance().setDefaultValueAsLong("server", "time-offset", 3);
    this->timeOffset = std::chrono::hours(
        Config::getInstance().readValueAsLong("server", "time-offset")
    );

    new Timer(
        this->getPlugin()->getTimerManager(),
        std::chrono::milliseconds(1000),
        [this]()
        {
            auto rtime = this->fetchTime();

            for(TimedEvent* event : this->events)
                event->update(rtime);

            rage::time_t time = {};
            time.a.second = rtime.getSeconds();
            time.a.minute = rtime.getMinutes();
            time.a.hour = rtime.getHours();

            if(this->realTime)
                this->getPlugin()->getRageMP()->GetWorld().SetTime(time);
        }
    );
}

Clock::~Clock() = default;

void Clock::setTimeOffset(std::chrono::hours timeOffset)
{
    this->timeOffset = timeOffset;
}

TimePoint Clock::fetchTime() const
{
    auto time = std::chrono::system_clock::now() + this->timeOffset;

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()) % 60;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time.time_since_epoch()) % 60;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(time.time_since_epoch()) % 24;
    auto days = std::chrono::duration_cast<std::chrono::hours>(time.time_since_epoch()) / 24;

    auto rhours = static_cast<uint8_t>(hours.count());
    auto rminutes = static_cast<uint8_t>(minutes.count());
    auto rseconds = static_cast<uint8_t>(seconds.count());
    TimePoint::Day rdays = (TimePoint::Day) ((days.count() + 3) % 7);

    return {
        rdays,
        rhours,
        rminutes,
        rseconds
    };
}

time_t Clock::getSecondsSinceEpoch() const {
    auto time = std::chrono::system_clock::now() + this->timeOffset;

    return static_cast<time_t>(
        std::chrono::duration_cast<std::chrono::seconds>(
            time.time_since_epoch()
        ).count()
    );
}

std::string Clock::convertSecondsSinceEpochToString(time_t seconds) const {

    char timestamp[64] = {0};
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", gmtime(&seconds));
    return std::string(timestamp);
}

Plugin* Clock::getPlugin() const
{
    return this->plugin;
}

std::chrono::hours Clock::getTimeOffset() const
{
    return this->timeOffset;
}

bool Clock::isRealTimeEnabled() const
{
    return this->realTime;
}

void Clock::enableRealTime(bool enabled)
{
    this->realTime = enabled;
}

std::vector<TimedEvent*> const& Clock::getTimedEvents() const
{
    return this->events;
}