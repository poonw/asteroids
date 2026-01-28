#include <memory>
#include "Game.h"
#include "Logger.h"
#include "RaylibWrapper.h"
#include "SpriteFactory.h"

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");

    std::shared_ptr<RaylibWrapper> raylibPtr  = std::make_shared<RaylibWrapper>();
    std::shared_ptr<SpriteFactory> factoryPtr = std::make_shared<SpriteFactory>();

    std::shared_ptr<Game> m_game = std::make_shared<Game>(raylibPtr, factoryPtr);

    m_game->run();

    return 0;
}
