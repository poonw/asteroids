#include "Explosion.h"
#include <cassert>
#include "GameSettings.h"

Explosion::Explosion(std::shared_ptr<RaylibInterface> raylibPtr,
                     Vector2                          position)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;
    m_position  = position;
}

void Explosion::update(void)
{
    assert(m_textures.size() > 1);
    float dt  = m_raylibPtr->getFrameTime();
    m_index  += (uint32_t)(1000 * dt);
    if (m_index >= m_textures.size())
    {
        m_index   = 0;
        m_discard = true;
    }
}

void Explosion::draw(void)
{
    assert(m_textures.size() > 1);
    assert(m_index < m_textures.size());
    m_raylibPtr->drawTextureV(m_textures[m_index], m_position, WHITE);
}

Vector2 Explosion::getCenter(void)
{
    assert(false);
    return {0, 0};
}

float Explosion::getRadius(void)
{
    assert(false);
    return 0;
}

Rectangle Explosion::getRect(void)
{
    assert(false);
    return (Rectangle(0, 0, 0, 0));
}

void Explosion::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() > 1);
    m_textures   = textures;
    m_position.x = m_position.x - (m_textures[0].width / 2);
    m_position.y = m_position.y - (m_textures[0].height / 2);
}
