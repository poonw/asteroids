#include "Laser.h"
#include <cassert>
#include "GameSettings.h"

Laser::Laser(std::shared_ptr<RaylibInterface> raylibPtr,
             std::filesystem::path            resourcePath,
             Vector2                          position)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;
    m_texture   = m_raylibPtr->loadTexture((resourcePath / "laser.png").string());
    m_position  = position;
    m_direction = {0, -1};
}

void Laser::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * PLAYER_SPEED * dt;
    m_position.y += m_direction.y * PLAYER_SPEED * dt;
}

void Laser::update(void)
{
    move();

    if ((m_position.y + m_texture.height) < 0)
    {
        m_discard = true;
    }
}

void Laser::draw(void)
{
    m_raylibPtr->drawTextureV(m_texture, m_position, WHITE);
}
