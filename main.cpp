#include <filesystem>
#include <functional>
#include <memory>
#include "Game.h"
#include "Laser.h"
#include "Logger.h"
#include "Meteor.h"
#include "Player.h"
#include "RaylibWrapper.h"
#include "Star.h"

std::shared_ptr<Sprite> createLaser(std::shared_ptr<RaylibInterface> raylibPtr,
                                    std::filesystem::path            resourcePath,
                                    Vector2                          position)
{
    return (std::make_shared<Laser>(raylibPtr, resourcePath, position));
}

std::shared_ptr<Sprite> createMeteor(std::shared_ptr<RaylibInterface> raylibPtr,
                                     std::filesystem::path            resourcePath)
{
    return (std::make_shared<Meteor>(raylibPtr, resourcePath));
}

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr = std::make_shared<RaylibWrapper>();

    std::filesystem::path resourcesPath = "resources";
    std::filesystem::path audioPath     = resourcesPath / "audio";
    std::filesystem::path fontPath      = resourcesPath / "font";
    std::filesystem::path imagesPath    = resourcesPath / "images";

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        std::filesystem::path            resourcePath,
        Vector2                          position)>
        createLaserWrapper = createLaser;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        std::filesystem::path            resourcePath)>
        createMeteorWrapper = createMeteor;

    std::shared_ptr<Game> m_game = std::make_shared<Game>(raylibPtr,
                                                          imagesPath,
                                                          createLaserWrapper,
                                                          createMeteorWrapper);

    std::function<void(Vector2 position)> shootLaserWrapper = std::bind(&Game::shootLaser, m_game, std::placeholders::_1);

    std::shared_ptr<Player> player = std::make_shared<Player>(raylibPtr, imagesPath, shootLaserWrapper);
    m_game->setPlayer(player);

    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> starsList;
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        starsList[n] = std::make_shared<Star>(raylibPtr, imagesPath);
    }
    m_game->setStarsList(starsList);

    m_game->run();

    return 0;
}
