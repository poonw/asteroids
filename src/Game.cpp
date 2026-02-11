#include "Game.h"
#include <algorithm>
#include <cassert>
#include <format>
#include "PlayerInterface.h"
#include "SpriteFactory.h"
#include "Timer.h"

Game::Game(std::shared_ptr<RaylibInterface> raylibPtr, std::shared_ptr<SpriteFactory> factoryPtr)
{
    assert(raylibPtr != nullptr);
    assert(factoryPtr != nullptr);
    m_raylibPtr = raylibPtr;
    m_factory   = factoryPtr;

    ////// raylib init //////
    m_state = WELCOME;
    m_raylibPtr->initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_gameName);
    m_raylibPtr->initAudioDevice();
    loadResources();

    Vector2 textsize;

    ////// welcome page //////
    Vector2 welcomeMenuSize = {280, 60};
    m_titlePosition.x       = ((WINDOW_WIDTH - ((m_gameName.length() / 2) * GAME_TITLE_FONTSIZE)) / 3);
    m_titlePosition.y       = ((WINDOW_HEIGHT / 2) - 200);

    m_startButton.m_selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) - 30), welcomeMenuSize.x, welcomeMenuSize.y};
    m_startButton.m_position          = {m_startButton.m_selectArea.x + 10, m_startButton.m_selectArea.y + 10};
    m_startButton.m_backgroundColor   = BLANK;
    m_startButton.m_textSize          = MENU_ITEM_FONTSIZE;
    m_startButton.m_displayText       = "Start";
    m_startButton.m_nextState         = PLAYING;
    m_startButton.m_selectSoundPlayed = false;

    m_settingsButton.m_selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) + 50), welcomeMenuSize.x, welcomeMenuSize.y};
    m_settingsButton.m_position          = {m_settingsButton.m_selectArea.x + 10, m_settingsButton.m_selectArea.y + 10};
    m_settingsButton.m_backgroundColor   = BLANK;
    m_settingsButton.m_textSize          = MENU_ITEM_FONTSIZE;
    m_settingsButton.m_displayText       = "Settings";
    m_settingsButton.m_nextState         = SETTINGS;
    m_settingsButton.m_selectSoundPlayed = false;

    m_quitButton.m_selectArea        = {m_titlePosition.x + 20, ((WINDOW_HEIGHT / 2) + 130), welcomeMenuSize.x, welcomeMenuSize.y};
    m_quitButton.m_position          = {m_quitButton.m_selectArea.x + 10, m_quitButton.m_selectArea.y + 10};
    m_quitButton.m_backgroundColor   = BLANK;
    m_quitButton.m_textSize          = MENU_ITEM_FONTSIZE;
    m_quitButton.m_displayText       = "Quit";
    m_quitButton.m_nextState         = EXIT_GAME;
    m_quitButton.m_selectSoundPlayed = false;

    ////// settings page //////
    float settingsMargin     = 100;
    m_settingsPageBackground = {settingsMargin, settingsMargin, (WINDOW_WIDTH - (2 * settingsMargin)), (WINDOW_HEIGHT - (2 * settingsMargin))};

    m_backButton.m_selectArea        = {(WINDOW_WIDTH - 300), (WINDOW_HEIGHT - 200), 150, 80};
    m_backButton.m_backgroundColor   = BLANK;
    m_backButton.m_textSize          = MENU_ITEM_FONTSIZE + 10;
    m_backButton.m_displayText       = "Back";
    m_backButton.m_nextState         = WELCOME;
    m_backButton.m_selectSoundPlayed = false;

    textsize                  = m_raylibPtr->measureTextEx(m_fontType, m_backButton.m_displayText, m_backButton.m_textSize, 0);
    m_backButton.m_position.x = (m_backButton.m_selectArea.x + ((m_backButton.m_selectArea.width - textsize.x) / 2));
    m_backButton.m_position.y = (m_backButton.m_selectArea.y + ((m_backButton.m_selectArea.height - textsize.y) / 2));

    ////// game over page //////
    textsize                 = m_raylibPtr->measureTextEx(m_fontType, m_gameoverText, GAME_OVER_FONTSIZE, 0);
    m_gameoverTextPosition.x = (WINDOW_WIDTH - textsize.x) / 2;
    m_gameoverTextPosition.y = WINDOW_HEIGHT;
    m_gameoverTextMaxHeight  = (WINDOW_HEIGHT - textsize.y) / 2;

    Vector2  gameoverMenuSize     = {240, 120};
    float    verticalOffset       = 100;
    float    buttonMargin         = 10;
    uint32_t gameoverMenuFontSize = MENU_ITEM_FONTSIZE + 30;

    m_newgameButton.m_selectArea        = {((WINDOW_WIDTH / 2) - gameoverMenuSize.x - buttonMargin),
                                           ((WINDOW_HEIGHT / 2) + verticalOffset),
                                           gameoverMenuSize.x,
                                           gameoverMenuSize.y};
    m_newgameButton.m_backgroundColor   = BLANK;
    m_newgameButton.m_textSize          = gameoverMenuFontSize;
    m_newgameButton.m_displayText       = "Retry";
    m_newgameButton.m_nextState         = WELCOME;
    m_newgameButton.m_selectSoundPlayed = false;

    textsize                     = m_raylibPtr->measureTextEx(m_fontType, m_newgameButton.m_displayText, m_newgameButton.m_textSize, 0);
    m_newgameButton.m_position.x = (m_newgameButton.m_selectArea.x + ((m_newgameButton.m_selectArea.width - textsize.x) / 2));
    m_newgameButton.m_position.y = (m_newgameButton.m_selectArea.y + ((m_newgameButton.m_selectArea.height - textsize.y) / 2));

    m_gameoverQuitButton.m_selectArea        = {((WINDOW_WIDTH / 2) + buttonMargin),
                                                ((WINDOW_HEIGHT / 2) + verticalOffset),
                                                gameoverMenuSize.x,
                                                gameoverMenuSize.y};
    m_gameoverQuitButton.m_backgroundColor   = BLANK;
    m_gameoverQuitButton.m_textSize          = gameoverMenuFontSize;
    m_gameoverQuitButton.m_displayText       = "Quit";
    m_gameoverQuitButton.m_nextState         = EXIT_GAME;
    m_gameoverQuitButton.m_selectSoundPlayed = false;

    textsize                          = m_raylibPtr->measureTextEx(m_fontType, m_gameoverQuitButton.m_displayText, m_gameoverQuitButton.m_textSize, 0);
    m_gameoverQuitButton.m_position.x = (m_gameoverQuitButton.m_selectArea.x + ((m_gameoverQuitButton.m_selectArea.width - textsize.x) / 2));
    m_gameoverQuitButton.m_position.y = (m_gameoverQuitButton.m_selectArea.y + ((m_gameoverQuitButton.m_selectArea.height - textsize.y) / 2));

    ////// playing page //////
    m_meteorTimer = std::make_shared<Timer>(m_raylibPtr,
                                            METEOR_TIMER_DURATION,
                                            true,
                                            true,
                                            std::bind(&Game::createMeteor, this));

    m_rampdownTimer = std::make_shared<Timer>(m_raylibPtr,
                                              1,
                                              false,
                                              false,
                                              std::bind(&Game::gameoverReset, this));

    m_opponentTimer = std::make_shared<Timer>(m_raylibPtr,
                                              OPPONENT_TIMER_DURATION,
                                              true,
                                              true,
                                              std::bind(&Game::createOpponent, this));

    m_dispersionTimer = std::make_shared<Timer>(m_raylibPtr,
                                                DISPERSION_TIMER_DURATION,
                                                true,
                                                true,
                                                std::bind(&Game::createPowerupDispersion, this));

    Sprite::SpriteAttr_t attr;
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index] = m_factory->getSprite(SpriteFactory::STAR, m_raylibPtr, attr);
        m_starsList[index]->setTextures(m_texturesMap["star"]);
    }
}

Game::~Game(void)
{
    unloadResources();
    m_raylibPtr->closeAudioDevice();
}

void Game::run(void)
{
    assert(m_player != nullptr);
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
                refreshGameOverPage();
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

void Game::setPlayer(std::shared_ptr<PlayerInterface> player)
{
    m_player = player;
    m_player->setTextures(m_texturesMap["player"]);
}

void Game::playerShootLaser(Sprite::SpriteAttr_t attr)
{
    assert(m_state == PLAYING);
    std::shared_ptr<Sprite> laserM = m_factory->getSprite(SpriteFactory::RED_LASER, m_raylibPtr, attr);
    laserM->setTextures(m_texturesMap["laser"]);
    m_playerLasersList.push_back(laserM);

    m_raylibPtr->playSound(m_laserSound);
}

void Game::opponentShootLaser(Sprite::SpriteAttr_t attr)
{
    assert(m_state == PLAYING);
    std::shared_ptr<Sprite> laserM = m_factory->getSprite(SpriteFactory::YELLOW_LASER, m_raylibPtr, attr);
    laserM->setTextures(m_texturesMap["laser"]);
    m_opponentLasersList.push_back(laserM);
}

void Game::createMeteor(void)
{
    assert(m_state == PLAYING);
    Sprite::SpriteAttr_t    attr;
    std::shared_ptr<Sprite> meteor = m_factory->getSprite(SpriteFactory::METEOR, m_raylibPtr, attr);
    meteor->setTextures(m_texturesMap["meteor"]);
    m_meteorsList.push_back(meteor);
}

void Game::createOpponent(void)
{
    assert(m_state == PLAYING);
    Sprite::SpriteAttr_t    attr;
    std::shared_ptr<Sprite> opponent = m_factory->getSprite(SpriteFactory::OPPONENT,
                                                            m_raylibPtr,
                                                            attr,
                                                            std::bind(&Game::opponentShootLaser, this, std::placeholders::_1));
    opponent->setTextures(m_texturesMap["player"]);
    m_opponentsList.push_back(opponent);
}

void Game::createPowerupDispersion(void)
{
    assert(m_state == PLAYING);
    Sprite::SpriteAttr_t    attr;
    std::shared_ptr<Sprite> powerup = m_factory->getSprite(SpriteFactory::POWERUP, m_raylibPtr, attr);
    powerup->setTextures(m_texturesMap["dispersion"]);
    m_dispersionsList.push_back(powerup);
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

    m_texturesMap["player"]        = {m_raylibPtr->loadTexture((imagesPath / "spaceship.png").string())};
    m_texturesMap["star"]          = {m_raylibPtr->loadTexture((imagesPath / "star.png").string())};
    m_texturesMap["laser"]         = {m_raylibPtr->loadTexture((imagesPath / "laser.png").string())};
    m_texturesMap["meteor"]        = {m_raylibPtr->loadTexture((imagesPath / "meteor.png").string())};
    m_texturesMap["dispersion"]    = {m_raylibPtr->loadTexture((imagesPath / "dispersion.png").string())};
    m_texturesMap["invincibility"] = {m_raylibPtr->loadTexture((imagesPath / "invincibility.png").string())};

    uint32_t               numberOfExplosionTextures = 28;
    std::vector<Texture2D> explosionTextures(numberOfExplosionTextures);
    for (uint32_t index = 0; index < numberOfExplosionTextures; index++)
    {
        explosionTextures[index] = m_raylibPtr->loadTexture((imagesPath / "explosion" / (std::to_string(index + 1) + ".png")).string());
    }
    m_texturesMap["explosion"] = explosionTextures;

    m_fontType           = m_raylibPtr->loadFontEx((fontPath / "Stormfaze.otf").string(), GAME_OVER_FONTSIZE, NULL, 0);
    m_explosionSound     = m_raylibPtr->loadSound((audioPath / "explosion.wav").string());
    m_laserSound         = m_raylibPtr->loadSound((audioPath / "laser.wav").string());
    m_selectSound        = m_raylibPtr->loadSound((audioPath / "select.mp3").string());
    m_dispersionSound    = m_raylibPtr->loadSound((audioPath / "dispersion.mp3").string());
    m_invincibilitySound = m_raylibPtr->loadSound((audioPath / "invincibility.mp3").string());
    m_extralifeSound     = m_raylibPtr->loadSound((audioPath / "extralife.mp3").string());
    m_backGroundMusic    = m_raylibPtr->loadMusicStream((audioPath / "music.wav").string());

    m_raylibPtr->playMusicStream(m_backGroundMusic);
}

void Game::unloadResources(void)
{
    m_raylibPtr->unloadMusicStream(m_backGroundMusic);
    m_raylibPtr->unloadSound(m_dispersionSound);
    m_raylibPtr->unloadSound(m_invincibilitySound);
    m_raylibPtr->unloadSound(m_extralifeSound);
    m_raylibPtr->unloadSound(m_selectSound);
    m_raylibPtr->unloadSound(m_laserSound);
    m_raylibPtr->unloadSound(m_explosionSound);
    m_raylibPtr->unloadFont(m_fontType);

    for (uint32_t index = 0; index < m_texturesMap["explosion"].size(); index++)
    {
        m_raylibPtr->unloadTexture(m_texturesMap["explosion"][index]);
    }

    m_raylibPtr->unloadTexture(m_texturesMap["invincibility"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["dispersion"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["meteor"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["laser"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["star"][0]);
    m_raylibPtr->unloadTexture(m_texturesMap["player"][0]);
}

void Game::updatePlayingPage(void)
{
    m_meteorTimer->update();
    m_dispersionTimer->update();
    m_opponentTimer->update();
    m_rampdownTimer->update();
    m_player->update();
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->update();
    }
    for (uint32_t index = 0; index < m_playerLasersList.size(); index++)
    {
        m_playerLasersList[index]->update();
    }
    for (uint32_t index = 0; index < m_meteorsList.size(); index++)
    {
        m_meteorsList[index]->update();
    }
    for (uint32_t index = 0; index < m_explosionsList.size(); index++)
    {
        m_explosionsList[index]->update();
    }
    for (uint32_t index = 0; index < m_opponentsList.size(); index++)
    {
        m_opponentsList[index]->update();
    }
    for (uint32_t index = 0; index < m_opponentLasersList.size(); index++)
    {
        m_opponentLasersList[index]->update();
    }
    for (uint32_t index = 0; index < m_dispersionsList.size(); index++)
    {
        m_dispersionsList[index]->update();
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
    for (uint32_t index = 0; index < m_playerLasersList.size(); index++)
    {
        m_playerLasersList[index]->draw();
    }
    for (uint32_t index = 0; index < m_meteorsList.size(); index++)
    {
        m_meteorsList[index]->draw();
    }
    for (uint32_t index = 0; index < m_explosionsList.size(); index++)
    {
        m_explosionsList[index]->draw();
    }
    for (uint32_t index = 0; index < m_opponentsList.size(); index++)
    {
        m_opponentsList[index]->draw();
    }
    for (uint32_t index = 0; index < m_opponentLasersList.size(); index++)
    {
        m_opponentLasersList[index]->draw();
    }
    for (uint32_t index = 0; index < m_dispersionsList.size(); index++)
    {
        m_dispersionsList[index]->draw();
    }
}

void Game::discardSprites(void)
{
    for (auto it = m_playerLasersList.begin(); it != m_playerLasersList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_playerLasersList.erase(it);
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
    for (auto it = m_opponentsList.begin(); it != m_opponentsList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_opponentsList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = m_opponentLasersList.begin(); it != m_opponentLasersList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_opponentLasersList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = m_dispersionsList.begin(); it != m_dispersionsList.end();)
    {
        if ((*(*it)).m_discard)
        {
            it = m_dispersionsList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::discardAllSprites(void)
{
    for (auto it = m_playerLasersList.begin(); it != m_playerLasersList.end();)
    {
        it = m_playerLasersList.erase(it);
    }
    for (auto it = m_meteorsList.begin(); it != m_meteorsList.end();)
    {
        it = m_meteorsList.erase(it);
    }
    for (auto it = m_explosionsList.begin(); it != m_explosionsList.end();)
    {
        it = m_explosionsList.erase(it);
    }
    for (auto it = m_opponentsList.begin(); it != m_opponentsList.end();)
    {
        it = m_opponentsList.erase(it);
    }
    for (auto it = m_opponentLasersList.begin(); it != m_opponentLasersList.end();)
    {
        it = m_opponentLasersList.erase(it);
    }
    for (auto it = m_dispersionsList.begin(); it != m_dispersionsList.end();)
    {
        it = m_dispersionsList.erase(it);
    }
}

void Game::checkCollisions(void)
{
    for (uint32_t ilaser = 0; ilaser < m_playerLasersList.size(); ilaser++)
    {
        for (uint32_t imeteor = 0; imeteor < m_meteorsList.size(); imeteor++)
        {
            if (m_raylibPtr->checkCollisionCircleRec(m_meteorsList[imeteor]->getCenter(),
                                                     m_meteorsList[imeteor]->getRadius(),
                                                     m_playerLasersList[ilaser]->getRect()))
            {
                m_playerLasersList[ilaser]->m_discard = true;
                m_meteorsList[imeteor]->m_discard     = true;

                Sprite::SpriteAttr_t attr;
                attr.m_position                   = m_meteorsList[imeteor]->getCenter();
                attr.m_scale                      = 2;
                std::shared_ptr<Sprite> explosion = m_factory->getSprite(SpriteFactory::EXPLOSION, m_raylibPtr, attr);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);

                m_score++;
            }
        }

        for (uint32_t ioppo = 0; ioppo < m_opponentsList.size(); ioppo++)
        {
            if (m_raylibPtr->checkCollisionCircleRec(m_opponentsList[ioppo]->getCenter(),
                                                     m_opponentsList[ioppo]->getRadius(),
                                                     m_playerLasersList[ilaser]->getRect()))
            {
                m_playerLasersList[ilaser]->m_discard = true;
                m_opponentsList[ioppo]->m_discard     = true;

                Sprite::SpriteAttr_t attr;
                attr.m_position                   = m_opponentsList[ioppo]->getCenter();
                attr.m_scale                      = 3;
                std::shared_ptr<Sprite> explosion = m_factory->getSprite(SpriteFactory::EXPLOSION, m_raylibPtr, attr);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);

                m_score += 10;
            }
        }
    }

    for (uint32_t index = 0; index < m_dispersionsList.size(); index++)
    {
        if (m_raylibPtr->checkCollisionCircles(m_player->getCenter(),
                                               m_player->getRadius(),
                                               m_dispersionsList[index]->getCenter(),
                                               m_dispersionsList[index]->getRadius()))
        {
            m_dispersionsList[index]->m_discard = true;
            m_player->setDispersedlaser();
            m_raylibPtr->playSound(m_dispersionSound);
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
                    m_rampdownTimer->activate();
                }
                m_meteorsList[index]->m_discard = true;
                m_player->m_discard             = true;

                Sprite::SpriteAttr_t attr;
                attr.m_position                   = m_player->getCenter();
                attr.m_scale                      = 3;
                std::shared_ptr<Sprite> explosion = m_factory->getSprite(SpriteFactory::EXPLOSION, m_raylibPtr, attr);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);
            }
        }

        for (uint32_t ilaser = 0; ilaser < m_opponentLasersList.size(); ilaser++)
        {
            if (m_raylibPtr->checkCollisionCircleRec(m_player->getCenter(),
                                                     m_player->getRadius(),
                                                     m_opponentLasersList[ilaser]->getRect()))
            {
                m_lives--;
                if (m_lives == 0)
                {
                    m_rampdownTimer->activate();
                }
                m_opponentLasersList[ilaser]->m_discard = true;
                m_player->m_discard                     = true;

                Sprite::SpriteAttr_t attr;
                attr.m_position                   = m_player->getCenter();
                attr.m_scale                      = 3;
                std::shared_ptr<Sprite> explosion = m_factory->getSprite(SpriteFactory::EXPLOSION, m_raylibPtr, attr);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);
            }
        }

        for (uint32_t index = 0; index < m_opponentsList.size(); index++)
        {
            if (m_raylibPtr->checkCollisionCircles(m_player->getCenter(),
                                                   m_player->getRadius(),
                                                   m_opponentsList[index]->getCenter(),
                                                   m_opponentsList[index]->getRadius()))
            {
                m_lives--;
                if (m_lives == 0)
                {
                    m_rampdownTimer->activate();
                }
                m_opponentsList[index]->m_discard = true;
                m_player->m_discard               = true;

                Sprite::SpriteAttr_t attr;
                attr.m_position                   = m_player->getCenter();
                attr.m_scale                      = 3;
                std::shared_ptr<Sprite> explosion = m_factory->getSprite(SpriteFactory::EXPLOSION, m_raylibPtr, attr);
                explosion->setTextures(m_texturesMap["explosion"]);
                m_explosionsList.push_back(explosion);
                m_raylibPtr->playSound(m_explosionSound);
            }
        }
    }
}

void Game::drawStats(void)
{
    m_raylibPtr->drawTextEx(m_fontType,
                            "lives: " + std::format("{:>5}", std::to_string(m_lives)),
                            Vector2((WINDOW_WIDTH - 150), 30),
                            STAT_FONTSIZE,
                            0,
                            WHITE);

    m_raylibPtr->drawTextEx(m_fontType,
                            "score: " + std::format("{:>4}", std::to_string(m_score)),
                            Vector2((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                            STAT_FONTSIZE,
                            0,
                            WHITE);
}

void Game::checkButtonUpdate(GameButton_t& button)
{
    Vector2 mousePosition = m_raylibPtr->getMousePosition();

    if (m_raylibPtr->checkCollisionPointRec(mousePosition, button.m_selectArea))
    {
        button.m_backgroundColor = DARKGRAY;

        if (m_raylibPtr->isMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            m_state = button.m_nextState;
        }

        if (!(button.m_selectSoundPlayed))
        {
            m_raylibPtr->playSound(m_selectSound);
            button.m_selectSoundPlayed = true;
        }
    }
    else
    {
        button.m_backgroundColor = BLANK;

        if (button.m_selectSoundPlayed)
        {
            button.m_selectSoundPlayed = false;
        }
    }
}

void Game::drawButton(GameButton_t button)
{
    m_raylibPtr->drawRectangleRounded(button.m_selectArea, 0.2, 0, button.m_backgroundColor);
    m_raylibPtr->drawTextEx(m_fontType, button.m_displayText, button.m_position, button.m_textSize, 0, LIGHTGRAY);
}

void Game::drawSettingsText(void)
{
    std::string row1col1 = "Spacebar";
    std::string row1col2 = "- shoot laser";
    std::string row2col1 = "Arrow keys";
    std::string row2col2 = "- move the spaceship";

    m_raylibPtr->drawTextEx(m_fontType, row1col1, Vector2(150, 180), MENU_ITEM_FONTSIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row1col2, Vector2(480, 180), MENU_ITEM_FONTSIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row2col1, Vector2(150, 240), MENU_ITEM_FONTSIZE + 10, 0, RAYWHITE);
    m_raylibPtr->drawTextEx(m_fontType, row2col2, Vector2(480, 240), MENU_ITEM_FONTSIZE + 10, 0, RAYWHITE);
}

void Game::gameoverReset(void)
{
    discardAllSprites();
    m_gameoverTextPosition.y = WINDOW_HEIGHT;
    m_lives                  = MAX_LIVES;
    m_score                  = 0;
    m_state                  = GAME_OVER;
}

void Game::refreshPlayingPage(void)
{
    discardSprites();
    updatePlayingPage();
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
    m_raylibPtr->drawTextEx(m_fontType, m_gameName, m_titlePosition, GAME_TITLE_FONTSIZE, 0, GOLD);
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

void Game::refreshGameOverPage(void)
{
    for (uint32_t index = 0; index < NUMBER_OF_STARS; index++)
    {
        m_starsList[index]->update();
    }

    if (m_gameoverTextPosition.y > m_gameoverTextMaxHeight)
    {
        m_gameoverTextPosition.y += (-1) * 100 * m_raylibPtr->getFrameTime();
    }
    else
    {
        m_gameoverTextPosition.y = m_gameoverTextMaxHeight;
        checkButtonUpdate(m_newgameButton);
        checkButtonUpdate(m_gameoverQuitButton);
    }

    m_raylibPtr->updateMusicStream(m_backGroundMusic);

    m_raylibPtr->beginDrawing();

    m_raylibPtr->clearBackground(BLACK);
    drawStars();
    m_raylibPtr->drawTextEx(m_fontType, m_gameoverText, m_gameoverTextPosition, GAME_OVER_FONTSIZE, 0, RED);
    if (m_gameoverTextPosition.y == m_gameoverTextMaxHeight)
    {
        drawButton(m_newgameButton);
        drawButton(m_gameoverQuitButton);
    }
    m_raylibPtr->endDrawing();
}
