#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

typedef void (*Operation)(void);

class Timer
{
public:
    Timer(double duration, bool repeat = false, bool autostart = false, Operation callBack = nullptr);
    ~Timer(void)= default;

    void update(void);

private:
    void activate(void);
    void deactivate(void);

    double m_duration = 0;
    double m_startTime = 0;
    bool m_active = false;
    bool m_repeat = false;
    Operation m_callBack = nullptr;
};

#endif // TIMER_H