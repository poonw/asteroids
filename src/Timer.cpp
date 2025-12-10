#include "Timer.h"

Timer::Timer(std::shared_ptr<RaylibInterface> raylibPtr,
             double                           duration,
             bool                             repeat,
             bool                             autostart,
             Operation                        callBack)
{
    m_raylibPtr = raylibPtr;
    m_duration  = duration;
    m_startTime = 0;
    m_active    = false;
    m_repeat    = repeat;
    m_callBack  = callBack;

    if (autostart)
    {
        activate();
    }
}

void Timer::update(void)
{
    if (m_active)
    {
        if ((m_raylibPtr->getTime() - m_startTime) >= m_duration)
        {
            if ((m_callBack != nullptr) && (m_startTime > 0))
            {
                m_callBack();
            }
            deactivate();
        }
    }
}

void Timer::activate(void)
{
    m_active    = true;
    m_startTime = m_raylibPtr->getTime();
}

void Timer::deactivate(void)
{
    m_active    = false;
    m_startTime = 0;
    if (m_repeat)
    {
        activate();
    }
}
