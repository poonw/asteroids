#include "Player.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"
#include "Timer.h"

Player::Player(std::shared_ptr<RaylibInterface>                 raylibPtr,
               std::function<void(const Sprite::SpriteAttr_t&)> shootLaser)
{
    assert(raylibPtr->isWindowReady());
    assert(shootLaser);
    m_raylibPtr  = raylibPtr;
    m_shootLaser = shootLaser;
    m_speed      = PLAYER_SPEED;

    int monitor    = m_raylibPtr->getCurrentMonitor();
    m_windowWidth  = m_raylibPtr->getMonitorWidth(monitor);
    m_windowHeight = m_raylibPtr->getMonitorHeight(monitor);

    std::function<void(void)> renderWarmupCallback        = std::bind(&Player::renderWarmup, this);
    std::function<void(void)> renderVincibleCallback      = std::bind(&Player::renderPlayable, this);
    std::function<void(void)> resetDispersedlaserCallback = std::bind(&Player::resetDispersedlaser, this);
    std::function<void(void)> resetInvincibleCallback     = std::bind(&Player::resetInvincible, this);

    m_invisibleTimer      = std::make_shared<Timer>(m_raylibPtr, 1, false, false, renderWarmupCallback);
    m_warmupTimer         = std::make_shared<Timer>(m_raylibPtr, 4, false, false, renderVincibleCallback);
    m_dispersedLaserTimer = std::make_shared<Timer>(m_raylibPtr, 15, false, false, resetDispersedlaserCallback);
    m_invincibleTimer     = std::make_shared<Timer>(m_raylibPtr, 10, false, false, resetInvincibleCallback);

    m_state   = PLAYABLE;
    m_discard = true;
}

void Player::input(void)
{
    m_direction.x = int(m_raylibPtr->isKeyDown(m_rightKeySetting)) - int(m_raylibPtr->isKeyDown(m_leftKeySetting));
    m_direction.y = int(m_raylibPtr->isKeyDown(m_downKeySetting)) - int(m_raylibPtr->isKeyDown(m_upKeySetting));

    if (m_raylibPtr->isKeyPressed(m_shootKeySetting))
    {
        Sprite::SpriteAttr_t laserAttr;
        laserAttr.m_position.x = m_position.x + (m_textures[0].width / 2);
        laserAttr.m_position.y = m_position.y - (m_textures[0].height / 2);
        laserAttr.m_direction  = {0, -1};
        laserAttr.m_rotation   = 180;
        m_shootLaser(laserAttr);

        if (m_invincible)
        {
            m_invincibleTimer->update();
        }

        if (m_dispersedLaser)
        {
            m_dispersedLaserTimer->update();

            laserAttr.m_direction = {-0.25, -1};
            laserAttr.m_rotation  = 173.875;
            m_shootLaser(laserAttr);

            laserAttr.m_direction = {0.25, -1};
            laserAttr.m_rotation  = 186.125;
            m_shootLaser(laserAttr);
        }
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

    switch (m_state)
    {
        case PLAYABLE:
        {
            if (m_discard)
            {
                m_state          = INVISIBLE;
                m_invincible     = false;
                m_dispersedLaser = false;
                m_position.x     = m_startXPos;
                m_position.y     = m_windowHeight + 100;
                m_invisibleTimer->activate();
            }
            else
            {
                input();
                move();
            }
            break;
        }

        case INVISIBLE:
            m_invisibleTimer->update();
            break;

        case MOVE_IN:
            moveIntoWindow();
            break;

        case WARMUP:
        {
            input();
            move();
            m_warmupTimer->update();
            break;
        }

        default:
            assert(false);
            break;
    }
}

void Player::draw(void)
{
    assert(m_textures.size() == 1);

    switch (m_state)
    {
        case PLAYABLE:
            if (m_invincible)
            {
                m_raylibPtr->drawTextureV(m_textures[0], m_position, DARKGRAY);
            }
            else
            {
                m_raylibPtr->drawTextureV(m_textures[0], m_position, WHITE);
            }
            break;

        case INVISIBLE:
            break;

        case MOVE_IN:
        case WARMUP:
            m_raylibPtr->drawTextureV(m_textures[0], m_position, DARKGRAY);
            break;

        default:
            assert(false);
            break;
    }
}

Vector2 Player::getCenter(void) const
{
    assert(m_textures.size() == 1);
    return (Vector2((m_position.x + ((float)(m_textures[0].width) / 2)),
                    (m_position.y + ((float)(m_textures[0].height) / 2))));
}

float Player::getRadius(void) const
{
    assert(m_textures.size() == 1);
    return m_radius;
}

void Player::setTextures(const std::vector<Texture2D>& textures)
{
    assert(textures.size() == 1);
    m_textures  = textures;
    m_maxXPos   = (m_windowWidth - m_textures[0].width);
    m_maxYPos   = (m_windowHeight - m_textures[0].height);
    m_startXPos = (m_maxXPos / 2);
    m_startYPos = (m_maxYPos - 100);

    m_position.x = m_startXPos;
    m_position.y = m_startYPos;

    m_radius = (float)(std::min(m_textures[0].width, m_textures[0].height)) / 2;
}

bool Player::isInvincible(void) const
{
    return m_invincible;
}

void Player::setInvincible(void)
{
    if (m_state != INVISIBLE)
    {
        m_invincible = true;
        m_invincibleTimer->activate();
    }
}

void Player::setDispersedlaser(void)
{
    if (m_state != INVISIBLE)
    {
        m_dispersedLaser = true;
        m_dispersedLaserTimer->activate();
    }
}

void Player::setLeftKey(KeyboardKey key)
{
    m_leftKeySetting = key;
}

void Player::setRightKey(KeyboardKey key)
{
    m_rightKeySetting = key;
}

void Player::setUpKey(KeyboardKey key)
{
    m_upKeySetting = key;
}

void Player::setDownKey(KeyboardKey key)
{
    m_downKeySetting = key;
}

void Player::setShootKey(KeyboardKey key)
{
    m_shootKeySetting = key;
}

void Player::resetDispersedlaser(void)
{
    m_dispersedLaser = false;
}

void Player::resetInvincible(void)
{
    m_invincible = false;
}

void Player::moveIntoWindow(void)
{
    assert(m_state == MOVE_IN);
    float dt      = m_raylibPtr->getFrameTime();
    m_position.y += (-1) * m_speed * dt;

    if (m_position.y <= m_startYPos)
    {
        m_state = WARMUP;
    }
}

void Player::renderWarmup(void)
{
    assert(m_state == INVISIBLE);
    m_state = MOVE_IN;
    m_warmupTimer->activate();
}

void Player::renderPlayable(void)
{
    assert(m_state == WARMUP);
    m_state   = PLAYABLE;
    m_discard = false;
}
