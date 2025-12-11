#include "Player.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"

Player::Player(std::shared_ptr<RaylibInterface> raylibPtr,
               std::filesystem::path            resourcePath)
    : m_raylibPtr(raylibPtr)
{
    assert(m_raylibPtr->isWindowReady());
    m_texture = m_raylibPtr->loadTexture((resourcePath / "spaceship.png").string());
    m_maxXPos = (float)(WINDOW_WIDTH - m_texture.width);
    m_maxYPos = (float)(WINDOW_HEIGHT - m_texture.height);

    m_position.x = (m_maxXPos / 2);
    m_position.y = (m_maxYPos - 10);
}

void Player::update(void)
{
    m_direction.x = int(m_raylibPtr->isKeyDown(KEY_RIGHT)) - int(m_raylibPtr->isKeyDown(KEY_LEFT));
    m_direction.y = int(m_raylibPtr->isKeyDown(KEY_DOWN)) - int(m_raylibPtr->isKeyDown(KEY_UP));

    float dt      = m_raylibPtr->getFrameTime();
    m_position.x += m_direction.x * PLAYER_SPEED * dt;
    m_position.y += m_direction.y * PLAYER_SPEED * dt;

    m_position.x = std::clamp(m_position.x, (float)0, m_maxXPos);
    m_position.y = std::clamp(m_position.y, (float)0, m_maxYPos);
}

void Player::draw(void)
{
    m_raylibPtr->drawTextureV(m_texture, m_position, WHITE);
}
