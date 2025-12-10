#ifndef PLAYER_H
#define PLAYER_H

#include <filesystem>
#include "RaylibInterface.h"
#include "Sprite.h"

class Player : public Sprite
{
public:
    Player(std::shared_ptr<RaylibInterface> raylibPtr,
           std::filesystem::path            resourcePath);
    ~Player(void) = default;

    void update(void) override;
    void draw(void) override;

private:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    Texture2D                        m_texture;
    Vector2                          m_position  = {0, 0};
    Vector2                          m_direction = {0, 0};
    float                            m_maxXPos   = 0.0;
    float                            m_maxYPos   = 0.0;
};

#endif // PLAYER_H
