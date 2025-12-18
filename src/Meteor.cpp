#include "Meteor.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Meteor::Meteor(std::shared_ptr<RaylibInterface> raylibPtr)
{
    assert(raylibPtr->isWindowReady());
    m_raylibPtr = raylibPtr;

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
    m_rotation   += 50 * dt;
}

void Meteor::update(void)
{
    assert(m_textures.size() == 1);
    move();

    if ((m_position.y - m_textures[0].height) > WINDOW_HEIGHT)
    {
        m_discard = true;
    }
}

void Meteor::draw(void)
{
    assert(m_textures.size() == 1);
    Rectangle targetRect = Rectangle(m_position.x, m_position.y, m_textures[0].width, m_textures[0].height);
    m_raylibPtr->drawTexturePro(m_textures[0], m_rect, targetRect, m_origin, m_rotation, WHITE);
}

Vector2 Meteor::getCenter(void)
{
    assert(m_textures.size() == 1);
    return m_position;
}

float Meteor::getRadius(void)
{
    assert(m_textures.size() == 1);
    return m_radius;
}

Rectangle Meteor::getRect(void)
{
    assert(false);
    return (Rectangle(0, 0, 0, 0));
}

void Meteor::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
    m_radius   = (float)(std::min(m_textures[0].width, m_textures[0].height)) / 2;
    m_rect     = Rectangle(0, 0, m_textures[0].width, m_textures[0].height);
    m_origin   = Vector2(m_textures[0].width / 2, m_textures[0].height / 2);
}
