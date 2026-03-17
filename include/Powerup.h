#ifndef POWERUP_H
#define POWERUP_H

#include "RaylibInterface.h"
#include "CircFeature.h"
#include "Sprite.h"

class Powerup : public Sprite, public CircFeature
{
public:
    Powerup(std::shared_ptr<RaylibInterface> raylibPtr);
    ~Powerup(void) override = default;

    void    update(void) override;
    void    draw(void) override;
    Vector2 getCenter(void) const override;
    float   getRadius(void) const override;
    void    setTextures(const std::vector<Texture2D>& textures) override;

private:
    void move(void);

    Vector2 m_direction = {0, 0};
    float   m_speed     = 0;
    float   m_rotation  = 0;
};

#endif // POWERUP_H
