#include "Player.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"

Player::Player(std::shared_ptr<RaylibInterface> raylibPtr,
               std::filesystem::path            resourcePath,
               std::function<void(Vector2)>     shootLaser)
{
    assert(raylibPtr->isWindowReady());
    assert(shootLaser);
    m_raylibPtr = raylibPtr;
    m_texture   = m_raylibPtr->loadTexture((resourcePath / "spaceship.png").string());

    m_shootLaser = shootLaser;
    m_maxXPos    = (float)(WINDOW_WIDTH - m_texture.width);
    m_maxYPos    = (float)(WINDOW_HEIGHT - m_texture.height);

    m_position.x = (m_maxXPos / 2);
    m_position.y = (m_maxYPos - 10);

    m_speed  = PLAYER_SPEED;
    m_radius = (float)(std::min(m_texture.width, m_texture.height)) / 2;
}

void Player::input(void)
{
    m_direction.x = int(m_raylibPtr->isKeyDown(KEY_RIGHT)) - int(m_raylibPtr->isKeyDown(KEY_LEFT));
    m_direction.y = int(m_raylibPtr->isKeyDown(KEY_DOWN)) - int(m_raylibPtr->isKeyDown(KEY_UP));

    if (m_raylibPtr->isKeyPressed(KEY_SPACE))
    {
        Vector2 laserPos;
        laserPos.x = m_position.x + (m_texture.width / 2);
        laserPos.y = m_position.y - (m_texture.height / 2);
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
    input();
    move();
}

void Player::draw(void)
{
    m_raylibPtr->drawTextureV(m_texture, m_position, WHITE);
}

Vector2 Player::getCenter(void)
{
    return (Vector2((m_position.x + ((float)(m_texture.width) / 2)),
                    (m_position.y + ((float)(m_texture.height) / 2))));
}

float Player::getRadius(void)
{
    return m_radius;
}
