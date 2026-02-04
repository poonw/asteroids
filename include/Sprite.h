#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <vector>
#include "RaylibInterface.h"

class Sprite
{
public:
    typedef struct SpriteAttr_s
    {
        Vector2 m_position  = {0, 0};
        Vector2 m_direction = {0, 0};
        float   m_scale     = 0.0;
        float   m_rotation  = 0.0;
        Color   m_color     = WHITE;
    } SpriteAttr_t;

    Sprite(void) {};
    virtual ~Sprite(void) {};

    virtual void      update(void)                                 = 0;
    virtual void      draw(void)                                   = 0;
    virtual Vector2   getCenter(void)                              = 0;
    virtual float     getRadius(void)                              = 0;
    virtual Rectangle getRect(void)                                = 0;
    virtual void      setTextures(std::vector<Texture2D> textures) = 0;

    bool m_discard = false;

protected:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    Vector2                          m_position  = {0, 0};
    std::vector<Texture2D>           m_textures;
};

#endif // SPRITE_H
