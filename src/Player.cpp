#include "Player.h"
#include "GameSettings.h"

Player::Player(std::shared_ptr<RaylibInterface> raylibPtr,
               std::filesystem::path            resourcePath,
               Vector2                          position)
    : m_raylibPtr(raylibPtr),
      m_position(position)
{
    m_texture = m_raylibPtr->loadTexture((resourcePath / "spaceship.png").string());
    m_maxXPos = (WINDOW_WIDTH - m_texture.width);
    m_maxYPos = (WINDOW_HEIGHT - m_texture.height);
}

void Player::update(void)
{
    m_direction.x = int(m_raylibPtr->isKeyDown(KEY_RIGHT)) - int(m_raylibPtr->isKeyDown(KEY_LEFT));
    m_direction.y = int(m_raylibPtr->isKeyDown(KEY_DOWN)) - int(m_raylibPtr->isKeyDown(KEY_UP));

    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * PLAYER_SPEED * dt;
    m_position.y += m_direction.y * PLAYER_SPEED * dt;

    if (m_position.x > m_maxXPos)
    {
        m_position.x = m_maxXPos;
    }
    else if (m_position.x < 0)
    {
        m_position.x = 0;
    }

    if (m_position.y > m_maxYPos)
    {
        m_position.y = m_maxYPos;
    }
    else if (m_position.y < 0)
    {
        m_position.y = 0;
    }
}

void Player::draw(void)
{
    m_raylibPtr->drawTextureV(m_texture, m_position, WHITE);
}
