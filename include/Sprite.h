#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include "RaylibInterface.h"

class Sprite
{
public:
    Sprite(void);
    virtual ~Sprite(void);

    virtual void update(void) = 0;
    virtual void draw(void)   = 0;

    bool m_discard = false;

protected:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    Texture2D                        m_texture   = {0, 0, 0, 0, 0};
    Vector2                          m_position  = {0, 0};
    Vector2                          m_direction = {0, 0};
    float                            m_scale     = 0;
    float                            m_speed     = 0;
    float                            m_rotation  = 0;

private:
};

#endif // SPRITE_H
