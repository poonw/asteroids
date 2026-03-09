#include "Star.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Star::Star(std::shared_ptr<RaylibInterface> raylibPtr)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;

    // Random number generator
    std::random_device rd;
    std::mt19937       gen(rd()); // Mersenne Twister engine

    std::uniform_int_distribution<>       distX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<>       distY(0, WINDOW_HEIGHT);
    std::uniform_real_distribution<float> distScale(0.5, 1.6);

    m_position.x = distX(gen);
    m_position.y = distY(gen);
    m_scale      = distScale(gen);
}

void Star::update(void)
{
    assert(m_textures.size() == 1);

    float dt      = m_raylibPtr->getFrameTime();
    m_position.y += STAR_SPEED * dt;
    if (m_position.y > WINDOW_HEIGHT)
    {
        m_position.y -= WINDOW_HEIGHT;
    }
}

void Star::draw(void)
{
    assert(m_textures.size() == 1);
    m_raylibPtr->drawTextureEx(m_textures[0], m_position, 0, m_scale, WHITE);
}

void Star::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
}
