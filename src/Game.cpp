#include "Game.h"
#include <cassert>
#include "GameSettings.h"
#include "RaylibInterface.h"
#include "Sprite.h"

Game::Game(std::shared_ptr<RaylibInterface> raylibPtr)
    : m_raylibPtr(raylibPtr)
{
    m_raylibPtr->initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        m_starsList[n] = nullptr;
    }
}

Game::~Game(void)
{
    m_raylibPtr->closeWindow();
}

void Game::run(void)
{
    assert(m_player != nullptr);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        assert(m_starsList[n] != nullptr);
    }

    while (!m_raylibPtr->windowShouldClose())
    {
        m_player->update();

        m_raylibPtr->beginDrawing();

        m_raylibPtr->clearBackground(BLACK);
        drawStars();
        m_player->draw();

        m_raylibPtr->endDrawing();
    }
}

void Game::setPlayer(std::shared_ptr<Sprite> player)
{
    m_player = player;
}

void Game::setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList)
{
    m_starsList = starsList;
}

void Game::drawStars(void)
{
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        m_starsList[n]->draw();
    }
}
