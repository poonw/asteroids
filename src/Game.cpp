#include "Game.h"
#include <algorithm>
#include <cassert>
#include "GameSettings.h"
#include "Sprite.h"
#include "Timer.h"

Game::Game(std::shared_ptr<RaylibInterface> raylibPtr,
           std::function<std::shared_ptr<Sprite>(
               std::shared_ptr<RaylibInterface> raylibPtr,
               Vector2                          position)>
               createLaserWrapper,
           std::function<std::shared_ptr<Sprite>(
               std::shared_ptr<RaylibInterface> raylibPtr)>
               createMeteorWrapper,
           std::function<std::shared_ptr<Sprite>(
               std::shared_ptr<RaylibInterface> raylibPtr,
               Vector2                          position)>
               explodeMeteorWrapper)
{
    assert(raylibPtr != nullptr);
    assert(createLaserWrapper);
    assert(createMeteorWrapper);
    assert(explodeMeteorWrapper);
    m_raylibPtr     = raylibPtr;
    m_createLaser   = createLaserWrapper;
    m_createMeteor  = createMeteorWrapper;
    m_explodeMeteor = explodeMeteorWrapper;

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        m_starsList[n] = nullptr;
    }

    std::function<void(void)> createMeteorCallback = std::bind(&Game::createMeteor, this);

    m_raylibPtr->initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    m_meteorTimer = std::make_shared<Timer>(m_raylibPtr, METEOR_TIMER_DURATION, true, true, createMeteorCallback);
    m_raylibPtr->initAudioDevice();
    loadResources();
    m_raylibPtr->playMusicStream(m_backGroundMusic);
}

Game::~Game(void)
{
    m_raylibPtr->closeAudioDevice();
    m_raylibPtr->unloadMusicStream(m_backGroundMusic);
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
        update();
        draw();
        checkCollisions();
    }
}

void Game::setPlayer(std::shared_ptr<Sprite> player)
{
    m_player = player;
    m_player->setTextures(m_texturesMap["player"]);
}

void Game::setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList)
{
    m_starsList = starsList;
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->setTextures(m_texturesMap["star"]);
    }
}

void Game::shootLaser(Vector2 position)
{
    std::shared_ptr<Sprite> laser = m_createLaser(m_raylibPtr, position);
    laser->setTextures(m_texturesMap["laser"]);
    m_lasersList.push_back(laser);
    m_raylibPtr->playSound(m_laserSound);
}

void Game::createMeteor(void)
{
    std::shared_ptr<Sprite> meteor = m_createMeteor(m_raylibPtr);
    meteor->setTextures(m_texturesMap["meteor"]);
    m_meteorsList.push_back(meteor);
}

void Game::update(void)
{
    m_meteorTimer->update();
    m_player->update();
    for (uint32_t index = 0; index < m_lasersList.size(); index++)
    {
        m_lasersList[index]->update();
    }
    for (uint32_t index = 0; index < m_meteorsList.size(); index++)
    {
        m_meteorsList[index]->update();
    }
    for (uint32_t index = 0; index < m_explosionsList.size(); index++)
    {
        m_explosionsList[index]->update();
    }
    m_raylibPtr->updateMusicStream(m_backGroundMusic);
}

void Game::draw(void)
{
    m_raylibPtr->beginDrawing();

    m_raylibPtr->clearBackground(BLACK);
    drawStars();
    drawScore();
    m_player->draw();
    for (uint32_t index = 0; index < m_lasersList.size(); index++)
    {
        m_lasersList[index]->draw();
    }
    for (uint32_t index = 0; index < m_meteorsList.size(); index++)
    {
        m_meteorsList[index]->draw();
    }
    for (uint32_t index = 0; index < m_explosionsList.size(); index++)
    {
        m_explosionsList[index]->draw();
    }

    m_raylibPtr->endDrawing();
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
    for (auto it = m_meteorsList.begin(); it != m_meteorsList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_meteorsList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = m_explosionsList.begin(); it != m_explosionsList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_explosionsList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::checkCollisions(void)
{
    for (uint32_t ilaser = 0; ilaser < m_lasersList.size(); ilaser++)
    {
        for (uint32_t imeteor = 0; imeteor < m_meteorsList.size(); imeteor++)
        {
            if (m_raylibPtr->checkCollisionCircleRec(m_meteorsList[imeteor]->getCenter(),
                                                     m_meteorsList[imeteor]->getRadius(),
                                                     m_lasersList[ilaser]->getRect()))
            {
                m_lasersList[ilaser]->m_discard   = true;
                m_meteorsList[imeteor]->m_discard = true;

                std::shared_ptr<Sprite> explosion = m_explodeMeteor(m_raylibPtr, m_meteorsList[imeteor]->getCenter());
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);

                m_score++;
            }
        }
    }

    for (uint32_t index = 0; index < m_meteorsList.size(); index++)
    {
        if (m_raylibPtr->checkCollisionCircles(m_player->getCenter(),
                                               m_player->getRadius(),
                                               m_meteorsList[index]->getCenter(),
                                               m_meteorsList[index]->getRadius()))
        {
            m_raylibPtr->closeWindow();
        }
    }
}

void Game::drawScore(void)
{
    m_raylibPtr->drawTextEx(m_fontType, std::to_string(m_score), Vector2(50, 50), FONT_SIZE, 0, WHITE);
}

void Game::loadResources(void)
{
    std::filesystem::path audioPath  = m_resourcePath / "audio";
    std::filesystem::path fontPath   = m_resourcePath / "font";
    std::filesystem::path imagesPath = m_resourcePath / "images";

    m_texturesMap["player"] = {m_raylibPtr->loadTexture((imagesPath / "spaceship.png").string())};
    m_texturesMap["star"]   = {m_raylibPtr->loadTexture((imagesPath / "star.png").string())};
    m_texturesMap["laser"]  = {m_raylibPtr->loadTexture((imagesPath / "laser.png").string())};
    m_texturesMap["meteor"] = {m_raylibPtr->loadTexture((imagesPath / "meteor.png").string())};

    uint32_t               numberOfExplosionTextures = 28;
    std::vector<Texture2D> explosionTextures(numberOfExplosionTextures);
    for (uint32_t index = 0; index < numberOfExplosionTextures; index++)
    {
        explosionTextures[index] = m_raylibPtr->loadTexture((imagesPath / "explosion" / (std::to_string(index + 1) + ".png")).string());
    }
    m_texturesMap["explosion"] = explosionTextures;

    m_fontType        = m_raylibPtr->loadFontEx((fontPath / "Stormfaze.otf").string(), FONT_SIZE, NULL, 0);
    m_explosionSound  = m_raylibPtr->loadSound((audioPath / "explosion.wav").string());
    m_laserSound      = m_raylibPtr->loadSound((audioPath / "laser.wav").string());
    m_backGroundMusic = m_raylibPtr->loadMusicStream((audioPath / "music.wav").string());
}
