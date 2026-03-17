#include "Explosion.h"
#include <cassert>
#include "GameSettings.h"

Explosion::Explosion(std::shared_ptr<RaylibInterface> raylibPtr,
                     Vector2                          position,
                     float                            scale)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;
    m_position  = position;
    m_scale     = scale;
}

void Explosion::update(void)
{
    assert(m_textures.size() > 1);
    float dt  = m_raylibPtr->getFrameTime();
    m_index  += (uint32_t)(EXPLOSION_SPEED * dt);
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
    m_raylibPtr->drawTextureEx(m_textures[m_index], m_position, 0, m_scale, WHITE);
}

void Explosion::setTextures(const std::vector<Texture2D>& textures)
{
    assert(textures.size() > 1);
    m_textures   = textures;
    m_position.x = m_position.x - (m_textures[0].width / 2) * m_scale;
    m_position.y = m_position.y - (m_textures[0].height / 2) * m_scale;
}
