#include "Game.h"
#include "GameSettings.h"
#include "RaylibInterface.h"
#include "Sprite.h"

Game::Game(std::shared_ptr<RaylibInterface> raylibPtr,
           std::shared_ptr<Sprite>          player)
    : m_raylibPtr(raylibPtr),
      m_player(player)
{
    m_raylibPtr->initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
}

Game::~Game(void)
{
    m_raylibPtr->closeWindow();
}

void Game::loop(void)
{
    while (!m_raylibPtr->windowShouldClose())
    {
        m_player->update();

        m_raylibPtr->beginDrawing();

        m_raylibPtr->clearBackground(BLACK);
        m_player->draw();

        m_raylibPtr->endDrawing();
    }
}
