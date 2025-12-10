#include <filesystem>
#include <memory>
#include "Game.h"
#include "Logger.h"
#include "Player.h"
#include "RaylibWrapper.h"
#include "Star.h"

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr = std::make_shared<RaylibWrapper>();

    std::filesystem::path resourcesPath = "resources";
    std::filesystem::path audioPath     = resourcesPath / "audio";
    std::filesystem::path fontPath      = resourcesPath / "font";
    std::filesystem::path imagesPath    = resourcesPath / "images";

    std::shared_ptr<Game> m_game = std::make_shared<Game>(raylibPtr);

    std::shared_ptr<Player> player = std::make_shared<Player>(raylibPtr, imagesPath);
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
