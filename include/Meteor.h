#ifndef METEOR_H
#define METEOR_H

#include "RaylibInterface.h"
#include "Sprite.h"

class Meteor : public Sprite
{
public:
    Meteor(std::shared_ptr<RaylibInterface> raylibPtr);
    ~Meteor(void) = default;

    void      update(void) override;
    void      draw(void) override;
    Vector2   getCenter(void) override;
    float     getRadius(void) override;
    Rectangle getRect(void) override;
    void      setTextures(std::vector<Texture2D> textures) override;

private:
    void move(void);

    Vector2   m_direction = {0, 0};
    float     m_scale     = 0;
    float     m_speed     = 0;
    float     m_rotation  = 0;
    float     m_radius    = 0;
    Rectangle m_rect      = {0, 0, 0, 0};
    Vector2   m_origin    = {0, 0};
};

#endif // METEOR_H
