#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <memory>
#include "RaylibInterface.h"

class Timer
{
public:
    Timer(std::shared_ptr<RaylibInterface> raylibPtr,
          double                           duration,
          bool                             repeat,
          bool                             autostart,
          std::function<void(void)>        callBack);
    ~Timer(void) = default;

    void update(void);
    void activate(void);

private:
    void deactivate(void);

    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    double                           m_duration  = 0;
    double                           m_startTime = 0;
    bool                             m_active    = false;
    bool                             m_repeat    = false;
    std::function<void(void)>        m_callBack;
};

#endif // TIMER_H
