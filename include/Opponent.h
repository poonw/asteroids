#ifndef OPPONENT_H
#define OPPONENT_H

#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"
#include "CircFeature.h"

class Opponent : public Sprite, public CircFeature
{
public:
    Opponent(std::shared_ptr<RaylibInterface>                 raylibPtr,
             std::function<void(const Sprite::SpriteAttr_t&)> shootLaser);
    ~Opponent(void) override = default;

    void    update(void) override;
    void    draw(void) override;
    Vector2 getCenter(void) const override;
    float   getRadius(void) const override;
    void    setTextures(const std::vector<Texture2D>& textures) override;

private:
    void move(void);

    std::function<void(const Sprite::SpriteAttr_t&)> m_shootLaser;
    Vector2                                          m_direction       = {0, 0};
    float                                            m_speed           = 0;
    uint32_t                                         m_laserInterval   = 0;
    uint32_t                                         m_intervalCounter = 0;
};

#endif // OPPONENT_H
