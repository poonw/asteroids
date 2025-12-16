#include "Game.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"
#include "Sprite.h"

Game::Game(std::shared_ptr<RaylibInterface> raylibPtr,
           std::filesystem::path            resourcePath,
           std::function<std::shared_ptr<Sprite>(
               std::shared_ptr<RaylibInterface> raylibPtr,
               std::filesystem::path            resourcePath,
               Vector2                          position)>
               createLaserWrapper)
    : m_raylibPtr(raylibPtr),
      m_resourcePath(resourcePath)
{
    assert(createLaserWrapper);
    m_createLaser = createLaserWrapper;
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
        discardSprites();

        m_player->update();
        for (uint32_t index = 0; index < m_lasersList.size(); index++)
        {
            m_lasersList[index]->update();
        }

        m_raylibPtr->beginDrawing();

        m_raylibPtr->clearBackground(BLACK);
        drawStars();
        m_player->draw();
        for (uint32_t index = 0; index < m_lasersList.size(); index++)
        {
            m_lasersList[index]->draw();
        }

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

void Game::shootLaser(Vector2 position)
{
    m_lasersList.push_back(m_createLaser(m_raylibPtr, m_resourcePath, position));
}

void Game::drawStars(void)
{
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        m_starsList[n]->draw();
    }
}

void Game::discardSprites(void)
{
    for (auto it = m_lasersList.begin(); it != m_lasersList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_lasersList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
