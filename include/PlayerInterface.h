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

    virtual bool isInvincible(void) const     = 0;
    virtual void setInvincible(void)          = 0;
    virtual void setDispersedlaser(void)      = 0;
    virtual void setLeftKey(KeyboardKey key)  = 0;
    virtual void setRightKey(KeyboardKey key) = 0;
    virtual void setUpKey(KeyboardKey key)    = 0;
    virtual void setDownKey(KeyboardKey key)  = 0;
    virtual void setShootKey(KeyboardKey key) = 0;

protected:
    Vector2     m_direction       = {0, 0};
    float       m_speed           = 0;
    bool        m_invincible      = false;
    bool        m_dispersedLaser  = false;
    KeyboardKey m_leftKeySetting  = KEY_LEFT;
    KeyboardKey m_rightKeySetting = KEY_RIGHT;
    KeyboardKey m_upKeySetting    = KEY_UP;
    KeyboardKey m_downKeySetting  = KEY_DOWN;
    KeyboardKey m_shootKeySetting = KEY_SPACE;
};

#endif // PLAYERINTERFACE_H
