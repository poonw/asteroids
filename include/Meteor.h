#ifndef METEOR_H
#define METEOR_H

#include "RaylibInterface.h"
#include "CircFeature.h"
#include "Sprite.h"

class Meteor : public Sprite, public CircFeature
{
public:
    Meteor(std::shared_ptr<RaylibInterface> raylibPtr);
    ~Meteor(void) override = default;

    void    update(void) override;
    void    draw(void) override;
    Vector2 getCenter(void) override;
    float   getRadius(void) override;
    void    setTextures(std::vector<Texture2D> textures) override;

private:
    void move(void);

    Vector2   m_direction = {0, 0};
    float     m_speed     = 0;
    float     m_rotation  = 0;
    Rectangle m_rect      = {0, 0, 0, 0};
    Vector2   m_origin    = {0, 0};
};

#endif // METEOR_H
