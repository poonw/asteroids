#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include "CircFeature.h"
#include "Sprite.h"
#include <vector>
#include "RaylibInterface.h"

class PlayerInterface : public Sprite, public CircFeature
{
public:
    virtual ~PlayerInterface(void) = default;

    virtual void setInvincible(void)     = 0;
    virtual void setDispersedlaser(void) = 0;

protected:
    Vector2 m_direction      = {0, 0};
    float   m_speed          = 0;
    bool    m_invincible     = false;
    bool    m_dispersedLaser = false;
};

#endif // PLAYERINTERFACE_H
