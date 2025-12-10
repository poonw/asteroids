#include "Star.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Star::Star(std::shared_ptr<RaylibInterface> raylibPtr,
           std::filesystem::path            resourcePath)
    : m_raylibPtr(raylibPtr)
{
    assert(m_raylibPtr->isWindowReady());
    m_texture = m_raylibPtr->loadTexture((resourcePath / "star.png").string());

    // Random number generator
    std::random_device rd;
    std::mt19937       gen(rd()); // Mersenne Twister engine

    std::uniform_int_distribution<>       distX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<>       distY(0, WINDOW_WIDTH);
    std::uniform_real_distribution<float> distScale(0.5, 1.6);

    m_position.x = distX(gen);
    m_position.y = distY(gen);
    m_scale      = distScale(gen);
}

void Star::update(void)
{
}

void Star::draw(void)
{
    m_raylibPtr->drawTextureEx(m_texture, m_position, 0, m_scale, WHITE);
}
