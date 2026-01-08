#include "Game.h"
#include <algorithm>
#include <cassert>
#include <format>
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
               Vector2                          position,
               float                            scale)>
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

    m_titlePosition.x = ((WINDOW_WIDTH - ((m_gameName.length() / 2) * GAME_TITLE_SIZE)) / 3);
    m_titlePosition.y = ((WINDOW_HEIGHT / 2) - 200);

    m_startButton.selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) - 30), 280, 60};
    m_startButton.position          = {m_startButton.selectArea.x + 10, m_startButton.selectArea.y + 10};
    m_startButton.backgroundColor   = BLANK;
    m_startButton.displayText       = "Start";
    m_startButton.m_nextState       = PLAYING;
    m_startButton.selectSoundPlayed = false;

    m_settingsButton.selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) + 50), 280, 60};
    m_settingsButton.position          = {m_settingsButton.selectArea.x + 10, m_settingsButton.selectArea.y + 10};
    m_settingsButton.backgroundColor   = BLANK;
    m_settingsButton.displayText       = "Settings";
    m_settingsButton.m_nextState       = SETTINGS;
    m_settingsButton.selectSoundPlayed = false;

    m_quitButton.selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) + 130), 280, 60};
    m_quitButton.position          = {m_quitButton.selectArea.x + 10, m_quitButton.selectArea.y + 10};
    m_quitButton.backgroundColor   = BLANK;
    m_quitButton.displayText       = "Quit";
    m_quitButton.m_nextState       = EXIT_GAME;
    m_quitButton.selectSoundPlayed = false;

    m_backButton.selectArea        = {(WINDOW_WIDTH - 300), (WINDOW_HEIGHT - 200), 110, 60};
    m_backButton.position          = {m_backButton.selectArea.x + 10, m_backButton.selectArea.y + 10};
    m_backButton.backgroundColor   = BLANK;
    m_backButton.displayText       = "Back";
    m_backButton.m_nextState       = WELCOME;
    m_backButton.selectSoundPlayed = false;

    m_settingsPageBackground = {100, 100, (WINDOW_WIDTH - (2 * 100)), (WINDOW_HEIGHT - (2 * 100))};

    m_meteorTimer = std::make_shared<Timer>(m_raylibPtr,
                                            METEOR_TIMER_DURATION,
                                            true,
                                            true,
                                            std::bind(&Game::createMeteor, this));

    m_raylibPtr->initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_gameName);
    m_raylibPtr->initAudioDevice();
    loadResources();
}

Game::~Game(void)
{
    unloadResources();
    m_raylibPtr->closeAudioDevice();
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
        switch (m_state)
        {
            case WELCOME:
                refreshWelcomePage();
                break;

            case SETTINGS:
                refreshSettingsPage();
                break;

            case PLAYING:
                refreshPlayingPage();
                break;

            case GAME_OVER:
                break;

            case EXIT_GAME:
                m_raylibPtr->closeWindow();
                break;

            default:
                assert(false);
                break;
        }
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

#ifdef DEBUG_
void Game::setState(STATE_t state)
{
    m_state = state;
}
#endif

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

    m_fontType        = m_raylibPtr->loadFontEx((fontPath / "Stormfaze.otf").string(), GAME_TITLE_SIZE, NULL, 0);
    m_explosionSound  = m_raylibPtr->loadSound((audioPath / "explosion.wav").string());
    m_laserSound      = m_raylibPtr->loadSound((audioPath / "laser.wav").string());
    m_selectSound     = m_raylibPtr->loadSound((audioPath / "select.mp3").string());
    m_backGroundMusic = m_raylibPtr->loadMusicStream((audioPath / "music.wav").string());

    m_raylibPtr->playMusicStream(m_backGroundMusic);
}

void Game::unloadResources(void)
{
    m_raylibPtr->unloadMusicStream(m_backGroundMusic);
    m_raylibPtr->unloadSound(m_selectSound);
    m_raylibPtr->unloadSound(m_laserSound);
    m_raylibPtr->unloadSound(m_explosionSound);
    m_raylibPtr->unloadFont(m_fontType);

    for (uint32_t index = 0; index < m_texturesMap["explosion"].size(); index++)
    {
        m_raylibPtr->unloadTexture(m_texturesMap["explosion"][index]);
    }

    m_raylibPtr->unloadTexture(m_texturesMap["meteor"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["laser"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["star"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["player"][0]);
}

void Game::update(void)
{
    m_meteorTimer->update();
    m_player->update();
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->update();
    }
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

void Game::drawPlayingPage(void)
{
    m_raylibPtr->beginDrawing();

    m_raylibPtr->clearBackground(BLACK);
    drawStars();
    m_player->draw();
    drawSprites();
    drawStats();

    m_raylibPtr->endDrawing();
}

void Game::drawStars(void)
{
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        m_starsList[n]->draw();
    }
}

void Game::drawSprites(void)
{
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

                std::shared_ptr<Sprite> explosion = m_explodeMeteor(m_raylibPtr, m_meteorsList[imeteor]->getCenter(), 2);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);

                m_score++;
            }
        }
    }

    if (!(m_player->m_discard))
    {
        for (uint32_t index = 0; index < m_meteorsList.size(); index++)
        {
            if (m_raylibPtr->checkCollisionCircles(m_player->getCenter(),
                                                   m_player->getRadius(),
                                                   m_meteorsList[index]->getCenter(),
                                                   m_meteorsList[index]->getRadius()))
            {
                m_lives--;
                if (m_lives == 0)
                {
                    m_state = EXIT_GAME;
                }
                else
                {
                    m_meteorsList[index]->m_discard = true;
                    m_player->m_discard             = true;

                    std::shared_ptr<Sprite> explosion = m_explodeMeteor(m_raylibPtr, m_player->getCenter(), 3);
                    explosion->setTextures(m_texturesMap["explosion"]);
                    m_explosionsList.push_back(explosion);
                    m_raylibPtr->playSound(m_explosionSound);
                }
            }
        }
    }
}

void Game::drawStats(void)
{
    m_raylibPtr->drawTextEx(m_fontType,
                            "lives: " + std::format("{:>5}", std::to_string(m_lives)),
                            Vector2((WINDOW_WIDTH - 150), 30),
                            STAT_SIZE,
                            0,
                            WHITE);

    m_raylibPtr->drawTextEx(m_fontType,
                            "score: " + std::format("{:>4}", std::to_string(m_score)),
                            Vector2((WINDOW_WIDTH - 150), (30 + STAT_SIZE)),
                            STAT_SIZE,
                            0,
                            WHITE);
}

void Game::checkButtonUpdate(GameButton_t& button)
{
    Vector2 mousePosition = m_raylibPtr->getMousePosition();

    if (m_raylibPtr->checkCollisionPointRec(mousePosition, button.selectArea))
    {
        button.backgroundColor = DARKGRAY;

        if (m_raylibPtr->isMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            m_state = button.m_nextState;
        }

        if (!(button.selectSoundPlayed))
        {
            m_raylibPtr->playSound(m_selectSound);
            button.selectSoundPlayed = true;
        }
    }
    else
    {
        button.backgroundColor = BLANK;

        if (button.selectSoundPlayed)
        {
            button.selectSoundPlayed = false;
        }
    }
}

void Game::drawButton(GameButton_t button)
{
    m_raylibPtr->drawRectangleRounded(button.selectArea, 0.2, 0, button.backgroundColor);
    m_raylibPtr->drawTextEx(m_fontType, button.displayText, button.position, MENU_ITEM_SIZE, 0, LIGHTGRAY);
}

void Game::drawSettingsText(void)
{
    std::string row1col1 = "Spacebar";
    std::string row1col2 = "- shoot laser";
    std::string row2col1 = "Arrow keys";
    std::string row2col2 = "- move the spaceship";

    m_raylibPtr->drawTextEx(m_fontType, row1col1, Vector2(150, 180), MENU_ITEM_SIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row1col2, Vector2(480, 180), MENU_ITEM_SIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row2col1, Vector2(150, 240), MENU_ITEM_SIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row2col2, Vector2(480, 240), MENU_ITEM_SIZE + 10, 0, RAYWHITE);
}

void Game::refreshPlayingPage(void)
{
    discardSprites();
    update();
    drawPlayingPage();
    checkCollisions();
}

void Game::refreshWelcomePage(void)
{
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->update();
    }

    checkButtonUpdate(m_startButton);
    checkButtonUpdate(m_settingsButton);
    checkButtonUpdate(m_quitButton);

    m_raylibPtr->updateMusicStream(m_backGroundMusic);

    m_raylibPtr->beginDrawing();

    m_raylibPtr->clearBackground(BLACK);
    drawStars();
    m_raylibPtr->drawTextEx(m_fontType, m_gameName, m_titlePosition, GAME_TITLE_SIZE, 0, GOLD);
    drawButton(m_startButton);
    drawButton(m_settingsButton);
    drawButton(m_quitButton);

    m_raylibPtr->endDrawing();
}

void Game::refreshSettingsPage(void)
{
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->update();
    }

    checkButtonUpdate(m_backButton);
    m_raylibPtr->updateMusicStream(m_backGroundMusic);

    m_raylibPtr->beginDrawing();

    m_raylibPtr->clearBackground(BLACK);
    drawStars();
    m_raylibPtr->drawRectangleRounded(m_settingsPageBackground, 0.05, 0, {30, 30, 30, 200});
    drawSettingsText();
    drawButton(m_backButton);
    m_raylibPtr->endDrawing();
}
