#ifndef PLAYER_H
#define PLAYER_H

#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"

class Player : public Sprite
{
public:
    Player(std::shared_ptr<RaylibInterface> raylibPtr,
           std::function<void(Vector2)>     shootLaser);
    ~Player(void) = default;

    void      update(void) override;
    void      draw(void) override;
    Vector2   getCenter(void) override;
    float     getRadius(void) override;
    Rectangle getRect(void) override;
    void      setTextures(std::vector<Texture2D> textures) override;

private:
    void input(void);
    void move(void);

    Vector2 m_direction = {0, 0};
    float   m_speed     = 0;
    float   m_radius    = 0;
    float   m_maxXPos   = 0.0;
    float   m_maxYPos   = 0.0;

    std::function<void(Vector2)> m_shootLaser;
};

#endif // PLAYER_H
