#ifndef OPPONENT_H
#define OPPONENT_H

#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"

class Opponent : public Sprite
{
public:
    Opponent(std::shared_ptr<RaylibInterface>          raylibPtr,
             std::function<void(Sprite::SpriteAttr_t)> shootLaser);
    ~Opponent(void) = default;

    void      update(void) override;
    void      draw(void) override;
    Vector2   getCenter(void) override;
    float     getRadius(void) override;
    Rectangle getRect(void) override;
    void      setTextures(std::vector<Texture2D> textures) override;

private:
    void move(void);

    std::function<void(Sprite::SpriteAttr_t)> m_shootLaser;
    Vector2                                   m_direction       = {0, 0};
    float                                     m_speed           = 0;
    float                                     m_radius          = 0;
    uint32_t                                  m_laserInterval   = 0;
    uint32_t                                  m_intervalCounter = 0;
};

#endif // OPPONENT_H
