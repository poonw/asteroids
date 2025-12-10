#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <memory>
#include "RaylibInterface.h"

typedef void (*Operation)(void);

class Timer
{
public:
    Timer(std::shared_ptr<RaylibInterface> raylibPtr,
          double                           duration,
          bool                             repeat    = false,
          bool                             autostart = false,
          Operation                        callBack  = nullptr);
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
    Operation                        m_callBack  = nullptr;
};

#endif // TIMER_H
