#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerInterface.h"
#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"

class Timer;

class Player : public PlayerInterface
{
public:
    Player(std::shared_ptr<RaylibInterface>          raylibPtr,
           std::function<void(Sprite::SpriteAttr_t)> shootLaser);
    ~Player(void) = default;

    void    update(void) override;
    void    draw(void) override;
    Vector2 getCenter(void) override;
    float   getRadius(void) override;
    void    setTextures(std::vector<Texture2D> textures) override;
    void    setInvincible(void) override;
    void    setDispersedlaser(void) override;

private:
    void input(void);
    void move(void);
    void moveIntoWindow(void);
    void renderWarmup(void);
    void renderPlayable(void);

    typedef enum STATE_s
    {
        PLAYABLE = 0,
        INVISIBLE,
        MOVE_IN,
        WARMUP,
        INVINCIBLE
    } STATE_t;

    float   m_maxXPos   = 0.0;
    float   m_maxYPos   = 0.0;
    float   m_startXPos = 0.0;
    float   m_startYPos = 0.0;
    STATE_t m_state     = PLAYABLE;

    std::function<void(Sprite::SpriteAttr_t)> m_shootLaser;
    std::shared_ptr<Timer>                    m_invisibleTimer  = nullptr;
    std::shared_ptr<Timer>                    m_warmupTimer     = nullptr;
    std::shared_ptr<Timer>                    m_invincibleTimer = nullptr;
};

#endif // PLAYER_H
