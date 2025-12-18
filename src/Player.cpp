#include "Player.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"

Player::Player(std::shared_ptr<RaylibInterface> raylibPtr,
               std::function<void(Vector2)>     shootLaser)
{
    assert(raylibPtr->isWindowReady());
    assert(shootLaser);
    m_raylibPtr  = raylibPtr;
    m_shootLaser = shootLaser;
    m_speed      = PLAYER_SPEED;
}

void Player::input(void)
{
    m_direction.x = int(m_raylibPtr->isKeyDown(KEY_RIGHT)) - int(m_raylibPtr->isKeyDown(KEY_LEFT));
    m_direction.y = int(m_raylibPtr->isKeyDown(KEY_DOWN)) - int(m_raylibPtr->isKeyDown(KEY_UP));

    if (m_raylibPtr->isKeyPressed(KEY_SPACE))
    {
        Vector2 laserPos;
        laserPos.x = m_position.x + (m_textures[0].width / 2);
        laserPos.y = m_position.y - (m_textures[0].height / 2);
        m_shootLaser(laserPos);
    }
}

void Player::move(void)
{
    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * m_speed * dt;
    m_position.y += m_direction.y * m_speed * dt;
    m_position.x  = std::clamp(m_position.x, (float)0, m_maxXPos);
    m_position.y  = std::clamp(m_position.y, (float)0, m_maxYPos);
}

void Player::update(void)
{
    assert(m_textures.size() == 1);
    input();
    move();
}

void Player::draw(void)
{
    assert(m_textures.size() == 1);
    m_raylibPtr->drawTextureV(m_textures[0], m_position, WHITE);
}

Vector2 Player::getCenter(void)
{
    assert(m_textures.size() == 1);
    return (Vector2((m_position.x + ((float)(m_textures[0].width) / 2)),
                    (m_position.y + ((float)(m_textures[0].height) / 2))));
}

float Player::getRadius(void)
{
    assert(m_textures.size() == 1);
    return m_radius;
}

Rectangle Player::getRect(void)
{
    assert(false);
    return (Rectangle(0, 0, 0, 0));
}

void Player::setTextures(std::vector<Texture2D> textures)
{
    assert(textures.size() == 1);
    m_textures = textures;
    m_maxXPos  = (float)(WINDOW_WIDTH - m_textures[0].width);
    m_maxYPos  = (float)(WINDOW_HEIGHT - m_textures[0].height);

    m_position.x = (m_maxXPos / 2);
    m_position.y = (m_maxYPos - 10);

    m_radius = (float)(std::min(m_textures[0].width, m_textures[0].height)) / 2;
}
