#ifndef PLAYER_H
#define PLAYER_H

#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"

class Timer;

class Player : public Sprite
{
public:
    Player(std::shared_ptr<RaylibInterface>          raylibPtr,
           std::function<void(Sprite::SpriteAttr_t)> shootLaser);
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
    void moveIntoWindow(void);
    void renderVisible(void);
    void renderPlayable(void);

    typedef enum STATE_s
    {
        PLAYABLE = 0,
        INVISIBLE,
        VISIBLE,
        INVINCIBLE
    } STATE_t;

    Vector2 m_direction = {0, 0};
    float   m_speed     = 0;
    float   m_radius    = 0;
    float   m_maxXPos   = 0.0;
    float   m_maxYPos   = 0.0;
    float   m_startXPos = 0.0;
    float   m_startYPos = 0.0;
    STATE_t m_state     = PLAYABLE;

    std::function<void(Sprite::SpriteAttr_t)> m_shootLaser;
    std::shared_ptr<Timer>                    m_invisibleTimer  = nullptr;
    std::shared_ptr<Timer>                    m_invincibleTimer = nullptr;
};

#endif // PLAYER_H
