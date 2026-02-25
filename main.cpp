#include <functional>
#include <memory>
#include "Game.h"
#include "Logger.h"
#include "Player.h"
#include "RaylibWrapper.h"
#include "SpriteFactory.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wenum-compare"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma GCC diagnostic pop

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr  = std::make_shared<RaylibWrapper>();
    std::shared_ptr<SpriteFactory> factoryPtr = std::make_shared<SpriteFactory>();

    std::shared_ptr<Game> game = std::make_shared<Game>(raylibPtr, factoryPtr);

    std::shared_ptr<Player> player = std::make_shared<Player>(raylibPtr, std::bind(&Game::playerShootLaser, game, std::placeholders::_1));
    game->setPlayer(player);

    game->run();

    return 0;
}
