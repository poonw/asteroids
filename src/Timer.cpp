#include "Timer.h"
#include "raylib.h"

Timer::Timer(double duration, bool repeat, bool autostart, Operation callBack)
{
    m_duration = duration;
    m_startTime = 0;
    m_active = false;
    m_repeat  = repeat;
    m_callBack = callBack;

    if (autostart)
    {
        activate();
    }
}

void Timer::update(void)
{
    if (m_active)
    {
        if ((GetTime() - m_startTime) >= m_duration)
        {
            if (( m_callBack != nullptr) && (m_startTime > 0))
            {
                m_callBack();
            }
            deactivate();
        }
    }
}

void Timer::activate(void)
{
    m_active = true;
    m_startTime = GetTime();
}

void Timer::deactivate(void)
{
    m_active = false;
    m_startTime = 0;
    if (m_repeat)
    {
        activate();
    }
}
