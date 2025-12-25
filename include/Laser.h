#ifndef LASER_H
#define LASER_H

#include "RaylibInterface.h"
#include "Sprite.h"

class Laser : public Sprite
{
public:
    Laser(std::shared_ptr<RaylibInterface> raylibPtr,
          Vector2                          position);
    ~Laser(void) = default;

    void      update(void) override;
    void      draw(void) override;
    Vector2   getCenter(void) override;
    float     getRadius(void) override;
    Rectangle getRect(void) override;
    void      setTextures(std::vector<Texture2D> textures) override;

private:
    void move(void);

    Vector2 m_direction = {0, 0};
    float   m_speed     = 0;
};

#endif // LASER_H
