#include "Opponent.h"
#include <cassert>
#include <random>
#include "GameSettings.h"

Opponent::Opponent(std::shared_ptr<RaylibInterface>          raylibPtr,
                   std::function<void(Sprite::SpriteAttr_t)> shootLaser)
{
    assert(raylibPtr->isWindowReady());
    assert(shootLaser);
    m_raylibPtr     = raylibPtr;
    m_shootLaser    = shootLaser;
    m_direction     = {0, 1};
    m_speed         = OPPONENT_SPEED;
    m_laserInterval = 3000;

    // Random number generator
    std::random_device rd;
    std::mt19937       gen(rd()); // Mersenne Twister engine

    std::uniform_int_distribution<> distX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<> distY(-150, -50);

    m_position = {(float)distX(gen), (float)distY(gen)};
}

void Opponent::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;

    m_intervalCounter++;

    if (m_intervalCounter >= m_laserInterval)
    {
        Sprite::SpriteAttr_t laserAttr;
        laserAttr.m_position.x = m_position.x - (m_textures[0].width * 0.66);
        laserAttr.m_position.y = m_position.y - (m_textures[0].height * 0.66);

        laserAttr.m_direction = {0, 1};
        laserAttr.m_rotation  = 0;
        m_shootLaser(laserAttr);

        laserAttr.m_direction = {-0.5, 1};
        laserAttr.m_rotation  = 22.5;
        m_shootLaser(laserAttr);

        laserAttr.m_direction = {0.5, 1};
        laserAttr.m_rotation  = 337.5;
        m_shootLaser(laserAttr);

        m_intervalCounter = 0;
    }
}

void Opponent::update(void)
{
    assert(m_textures.size() == 1);
    move();

    if ((m_position.y - m_textures[0].height) > WINDOW_HEIGHT)
    {
        m_discard = true;
    }
}

void Opponent::draw(void)
{
    assert(m_textures.size() == 1);
    m_raylibPtr->drawTextureEx(m_textures[0], m_position, 180, 1, RED);
}

Vector2 Opponent::getCenter(void)
{
    assert(m_textures.size() == 1);
    return (Vector2((m_position.x - ((float)(m_textures[0].width) * 0.4)),
                    (m_position.y - ((float)(m_textures[0].height) * 0.4))));
}

float Opponent::getRadius(void)
{
    assert(m_textures.size() == 1);
    return m_radius;
}

Rectangle Opponent::getRect(void)
{
    assert(false);
    return (Rectangle(0, 0, 0, 0));
}

void Opponent::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
    m_radius   = (float)(std::min(m_textures[0].width, m_textures[0].height)) / 2;
}
