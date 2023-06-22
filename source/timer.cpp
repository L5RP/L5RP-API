#include "api.h"

L5RP::DeltaTimeCounter::DeltaTimeCounter()
{

    this->reset();

}

std::chrono::milliseconds L5RP::DeltaTimeCounter::calcDeltaTime()
{

    //Get current time
    auto _currTime = std::chrono::system_clock::now();

    std::chrono::milliseconds timeElapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>
            (_currTime - _lastTime);

    return timeElapsed;

}

void L5RP::DeltaTimeCounter::reset()
{

    this->_lastTime = std::chrono::system_clock::now();

}

L5RP::TimerManager::TimerManager(Plugin* plugin)
    : PluginAttachment(plugin) {}

L5RP::TimerManager::~TimerManager()
{

    this->_timers.clear();

}

void L5RP::TimerManager::onTick()
{

    for(Timer* timer : this->_timers)
        timer->onTick();

}

void L5RP::TimerManager::registerTimer(Timer* timer)
{

    this->_timers.push_back(timer);

}

void L5RP::TimerManager::unregisterTimer(Timer* timer)
{

    auto it = std::find(this->_timers.begin(), this->_timers.end(), timer);
    if(it != this->_timers.end())
        this->_timers.erase(it);

}

L5RP::Timer::Timer(
    TimerManager* manager,
    std::chrono::milliseconds duration,
    std::function<void()> const& function
)
    : DeltaTimeCounter()
{

    this->manager = manager;
    this->duration = duration;
    this->function = function;

    this->manager->registerTimer(this);

}

L5RP::Timer::~Timer()
{

    this->manager->unregisterTimer(this);

}

void L5RP::Timer::onTick()
{

    std::chrono::milliseconds elapsedTime = this->calcDeltaTime();

    if(elapsedTime >= this->duration) {

        this->reset();

        this->function();

    }

}

L5RP::TimerManager* L5RP::Timer::getManager() const
{
    return this->manager;
}

std::chrono::milliseconds L5RP::Timer::getDuration() const
{
    return this->duration;
}

L5RP::Promise::Promise(
    TimerManager* manager,
    std::chrono::milliseconds duration,
    std::function<void()> const& function
)
    : Timer(
    manager,
    duration,
    [this, function]()
    {
        function();

        delete this;
    }
) {}
