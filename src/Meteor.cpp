#include "Meteor.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Meteor::Meteor(std::shared_ptr<RaylibInterface> raylibPtr,
               std::filesystem::path            resourcePath)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;
    m_texture   = m_raylibPtr->loadTexture((resourcePath / "meteor.png").string());

    // Random number generator
    std::random_device rd;
    std::mt19937       gen(rd()); // Mersenne Twister engine

    std::uniform_int_distribution<>       distX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<>       distY(-150, -50);
    std::uniform_int_distribution<>       distSpeed(300, 400);
    std::uniform_real_distribution<float> distDirection(-0.5, 0.5);

    m_position  = {(float)distX(gen), (float)distY(gen)};
    m_speed     = distSpeed(gen);
    m_direction = {distDirection(gen), 1};
    m_scale     = 1;
}

void Meteor::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;

    m_rotation += 50 * dt;
}

void Meteor::update(void)
{
    move();

    if ((m_position.y - m_texture.height) > WINDOW_HEIGHT)
    {
        m_discard = true;
    }
}

void Meteor::draw(void)
{
    m_raylibPtr->drawTextureEx(m_texture, m_position, m_rotation, m_scale, WHITE);
}
