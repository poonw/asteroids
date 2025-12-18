#ifndef PLAYER_H
#define PLAYER_H

#include <filesystem>
#include <functional>
#include "RaylibInterface.h"
#include "Sprite.h"

class Player : public Sprite
{
public:
    Player(std::shared_ptr<RaylibInterface> raylibPtr,
           std::filesystem::path            resourcePath,
           std::function<void(Vector2)>     shootLaser);
    ~Player(void) = default;

    void    update(void) override;
    void    draw(void) override;
    Vector2 getCenter(void) override;
    float   getRadius(void) override;

private:
    void input(void);
    void move(void);

    std::function<void(Vector2)> m_shootLaser;
    float                        m_maxXPos = 0.0;
    float                        m_maxYPos = 0.0;
};

#endif // PLAYER_H
