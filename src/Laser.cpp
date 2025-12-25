#include "Laser.h"
#include <cassert>
#include "GameSettings.h"

Laser::Laser(std::shared_ptr<RaylibInterface> raylibPtr,
             Vector2                          position)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;
    m_position  = position;
    m_direction = {0, -1};
    m_speed     = LASER_SPEED;
}

void Laser::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;
}

void Laser::update(void)
{
    assert(m_textures.size() == 1);
    move();

    if ((m_position.y + m_textures[0].height) < 0)
    {
        m_discard = true;
    }
}

void Laser::draw(void)
{
    assert(m_textures.size() == 1);
    m_raylibPtr->drawTextureV(m_textures[0], m_position, WHITE);
}

Vector2 Laser::getCenter(void)
{
    assert(false);
    return (Vector2(0, 0));
}

float Laser::getRadius(void)
{
    assert(false);
    return 0;
}

Rectangle Laser::getRect(void)
{
    assert(m_textures.size() == 1);
    return (Rectangle(m_position.x, m_position.y, m_textures[0].width, m_textures[0].height));
}

void Laser::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
}
