#include "Powerup.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Powerup::Powerup(std::shared_ptr<RaylibInterface> raylibPtr)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;

    // Random number generator
    std::random_device rd;
    std::mt19937       gen(rd()); // Mersenne Twister engine

    std::uniform_int_distribution<>       distX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<>       distY(-150, -50);
    std::uniform_int_distribution<>       distSpeed(350, 450);
    std::uniform_real_distribution<float> distDirection(-0.5, 0.5);

    m_position  = {(float)distX(gen), 0};
    m_speed     = distSpeed(gen);
    m_direction = {distDirection(gen), 1};
}

void Powerup::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;
}

void Powerup::update(void)
{
    assert(m_textures.size() == 1);
    move();

    if ((m_position.y - m_textures[0].height) > WINDOW_HEIGHT)
    {
        m_discard = true;
    }
}

void Powerup::draw(void)
{
    assert(m_textures.size() == 1);
    m_raylibPtr->drawTextureV(m_textures[0], m_position, WHITE);
}

Vector2 Powerup::getCenter(void)
{
    assert(m_textures.size() == 1);
    return m_position;
}

float Powerup::getRadius(void)
{
    assert(m_textures.size() == 1);
    return m_radius;
}

Rectangle Powerup::getRect(void)
{
    assert(false);
    return (Rectangle(0, 0, 0, 0));
}

void Powerup::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
    m_radius   = (float)(std::min(m_textures[0].width, m_textures[0].height)) / 2;
}
