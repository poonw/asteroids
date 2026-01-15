#ifndef GAME_H
#define GAME_H

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "GameSettings.h"
#include "RaylibInterface.h"

class Sprite;
class Timer;

class Game
{
public:
    typedef enum STATE_e
    {
        WELCOME = 0,
        PLAYING,
        SETTINGS,
        GAME_OVER,
        EXIT_GAME
    } STATE_t;

    Game(std::shared_ptr<RaylibInterface> raylibPtr,
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
             explodeMeteorWrapper);
    ~Game(void);

    void run(void);
    void setPlayer(std::shared_ptr<Sprite> player);
    void setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList);
    void shootLaser(Vector2 position);
    void createMeteor(void);
#ifdef DEBUG_
    void setState(STATE_t state);
#endif

private:
    typedef struct GameButton_s
    {
        Vector2     m_position;
        Rectangle   m_selectArea;
        Color       m_backgroundColor;
        uint32_t    m_textSize;
        std::string m_displayText;
        STATE_t     m_nextState;
        bool        m_selectSoundPlayed;
    } GameButton_t;

    void loadResources(void);
    void unloadResources(void);
    void update(void);
    void drawPlayingPage(void);
    void drawStars(void);
    void drawSprites(void);
    void discardSprites(void);
    void checkCollisions(void);
    void drawStats(void);
    void checkButtonUpdate(GameButton_t& button);
    void drawButton(GameButton_t button);
    void drawSettingsText(void);
    void gameoverReset(void);
    void refreshPlayingPage(void);
    void refreshWelcomePage(void);
    void refreshSettingsPage(void);
    void refreshGameOverPage(void);

    std::string                      m_gameName     = "Asteroids";
    STATE_t                          m_state        = EXIT_GAME;
    const std::filesystem::path      m_resourcePath = "resources";
    std::shared_ptr<RaylibInterface> m_raylibPtr    = nullptr;

    std::unordered_map<std::string, std::vector<Texture2D>> m_texturesMap;

    Font  m_fontType;
    Sound m_explosionSound;
    Sound m_laserSound;
    Sound m_selectSound;
    Music m_backGroundMusic;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position)>
        m_createLaser;
    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr)>
        m_createMeteor;
    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position,
        float                            scale)>
        m_explodeMeteor;

    // welcome page
    Vector2      m_titlePosition;
    GameButton_t m_startButton;
    GameButton_t m_settingsButton;
    GameButton_t m_quitButton;

    // settings page
    GameButton_t m_backButton;
    Rectangle    m_settingsPageBackground;

    // game over page
    GameButton_t m_newgameButton;
    GameButton_t m_gameoverQuitButton;
    std::string  m_gameoverText = "Game Over";
    Vector2      m_gameoverTextPosition;
    float        m_gameoverTextMaxHeight = 0;

    // playing page
    uint32_t                                             m_score         = 0;
    uint32_t                                             m_lives         = MAX_LIVES;
    std::shared_ptr<Timer>                               m_meteorTimer   = nullptr;
    std::shared_ptr<Timer>                               m_rampdownTimer = nullptr;
    std::shared_ptr<Sprite>                              m_player        = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starsList;
    std::vector<std::shared_ptr<Sprite>>                 m_lasersList;
    std::vector<std::shared_ptr<Sprite>>                 m_meteorsList;
    std::vector<std::shared_ptr<Sprite>>                 m_explosionsList;
};

#endif // GAME_H
