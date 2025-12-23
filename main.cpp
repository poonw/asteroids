#include <functional>
#include <memory>
#include "Explosion.h"
#include "Game.h"
#include "Laser.h"
#include "Logger.h"
#include "Meteor.h"
#include "Player.h"
#include "RaylibWrapper.h"
#include "Star.h"

std::shared_ptr<Sprite> createLaser(std::shared_ptr<RaylibInterface> raylibPtr,
                                    Vector2                          position)
{
    return (std::make_shared<Laser>(raylibPtr, position));
}

std::shared_ptr<Sprite> createMeteor(std::shared_ptr<RaylibInterface> raylibPtr)
{
    return (std::make_shared<Meteor>(raylibPtr));
}

std::shared_ptr<Sprite> explodeMeteor(std::shared_ptr<RaylibInterface> raylibPtr,
                                      Vector2                          position)
{
    return (std::make_shared<Explosion>(raylibPtr, position));
}

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr = std::make_shared<RaylibWrapper>();

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position)>
        createLaserWrapper = createLaser;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr)>
        createMeteorWrapper = createMeteor;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position)>
        explodeMeteorWrapper = explodeMeteor;

    std::shared_ptr<Game> m_game = std::make_shared<Game>(raylibPtr,
                                                          createLaserWrapper,
                                                          createMeteorWrapper,
                                                          explodeMeteorWrapper);

    std::function<void(Vector2 position)> shootLaserWrapper = std::bind(&Game::shootLaser, m_game, std::placeholders::_1);

    std::shared_ptr<Player> player = std::make_shared<Player>(raylibPtr, shootLaserWrapper);
    m_game->setPlayer(player);

    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> starsList;
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        starsList[n] = std::make_shared<Star>(raylibPtr);
    }
    m_game->setStarsList(starsList);

    m_game->run();

    return 0;
}
