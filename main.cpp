#include <filesystem>
#include <memory>
#include "Game.h"
#include "Logger.h"
#include "Player.h"
#include "RaylibWrapper.h"

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr = std::make_shared<RaylibWrapper>();

    std::filesystem::path resourcesPath = "resources";
    std::filesystem::path audioPath     = resourcesPath / "audio";
    std::filesystem::path fontPath      = resourcesPath / "font";
    std::filesystem::path imagesPath    = resourcesPath / "images";

    Vector2 startPosition = {0,0};
    std::shared_ptr<Player> player = std::make_shared<Player>(raylibPtr, imagesPath, startPosition);

    std::shared_ptr<Game> m_game = std::make_shared<Game>(raylibPtr, player);

    m_game->loop();

    return 0;
}
