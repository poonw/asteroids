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
    m_speed     = LASER_SPEED;
    m_radius    = (float)(std::min(m_texture.width, m_texture.height)) / 2;
}

void Laser::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;
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

Vector2 Laser::getCenter(void)
{
    return (Vector2((m_position.x + ((float)(m_texture.width) / 2)),
                    (m_position.y + ((float)(m_texture.height) / 2))));
}

float Laser::getRadius(void)
{
    return m_radius;
}
