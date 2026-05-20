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
#include "Sprite.h"

class PlayerInterface;
class SpriteFactory;
class Timer;

class Game
{
public:
    typedef enum STATE_e
    {
        WELCOME = 0,
        PLAYING,
        SETTINGS,
        HIGHSCORES,
        GAME_OVER,
        EXIT_GAME
    } STATE_t;

    Game(std::shared_ptr<RaylibInterface> raylibPtr, std::shared_ptr<SpriteFactory> factoryPtr);
    ~Game(void);
    Game(const Game& game)            = delete;
    Game& operator=(const Game& game) = delete;
    Game(Game&& game)                 = delete;
    Game& operator=(Game&& game)      = delete;

    void run(void);
    void playerShootLaser(const Sprite::SpriteAttr_t& attr);
    void opponentShootLaser(const Sprite::SpriteAttr_t& attr);
    void createMeteor(void);
    void createOpponent(void);
    void createPowerupDispersion(void);
    void createPowerupInvincibility(void);
    void createPowerupExtralife(void);
#ifdef DEBUG_
    void setState(STATE_t state);
#endif

private:
    typedef struct GameButton_s
    {
        Vector2                   m_position;
        Rectangle                 m_selectArea;
        Color                     m_backgroundColor;
        uint32_t                  m_textSize;
        std::string               m_displayText;
        STATE_t                   m_nextState;
        bool                      m_selectSoundPlayed;
        std::function<void(void)> m_callBack = nullptr;
    } GameButton_t;

    void loadResources(void);
    void unloadResources(void);
    void updatePlayingPage(void);
    void drawPlayingPage(void);
    void drawStars(void);
    void drawSprites(void);
    void discardSprites(void);
    void discardAllSprites(void);
    void checkCollisions(void);
    void drawStats(void);
    void checkButtonUpdate(GameButton_t& button);
    void drawButton(const GameButton_t& button);
    void drawSettingsText(void);
    void gameoverReset(void);
    void resetSettingsToDefault(void);
    void setSettings(void);
    void refreshPlayingPage(void);
    void refreshWelcomePage(void);
    void refreshSettingsPage(void);
    void refreshHighscoresPage(void);
    void refreshGameOverPage(void);

    const std::string           m_gameName     = "Asteroids";
    const std::filesystem::path m_resourcePath = "resources";
    STATE_t                     m_state        = EXIT_GAME;

    float m_windowWidth  = 0;
    float m_windowHeight = 0;

    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    std::shared_ptr<SpriteFactory>   m_factory   = nullptr;

    std::unordered_map<std::string, std::vector<Texture2D>> m_texturesMap;

    Font  m_fontType;
    Sound m_explosionSound;
    Sound m_laserSound;
    Sound m_selectSound;
    Sound m_dispersionSound;
    Sound m_invincibilitySound;
    Sound m_extralifeSound;
    Music m_backGroundMusic;

    // welcome page
    Vector2      m_titlePosition;
    GameButton_t m_startButton;
    GameButton_t m_settingsButton;
    GameButton_t m_highscoresButton;
    GameButton_t m_quitButton;

    // settings page
    std::unordered_map<int, KeyboardKey> m_leftKeyMap = {
        {0, KEY_LEFT},
        {1, KEY_A   }
    };

    std::unordered_map<int, KeyboardKey> m_rightKeyMap = {
        {0, KEY_RIGHT},
        {1, KEY_D    }
    };

    std::unordered_map<int, KeyboardKey> m_upKeyMap = {
        {0, KEY_UP},
        {1, KEY_W }
    };

    std::unordered_map<int, KeyboardKey> m_downKeyMap = {
        {0, KEY_DOWN},
        {1, KEY_S   }
    };

    std::unordered_map<int, KeyboardKey> m_shootKeyMap = {
        {0, KEY_SPACE},
        {1, KEY_ENTER}
    };

    GameButton_t      m_resetButton;
    GameButton_t      m_okButton;
    Rectangle         m_settingsPageBackground;
    int               m_leftControlSetting  = 0;
    int               m_rightControlSetting = 0;
    int               m_upControlSetting    = 0;
    int               m_downControlSetting  = 0;
    int               m_shootControlSetting = 0;
    bool              m_dropDown1Active     = false;
    bool              m_dropDown2Active     = false;
    bool              m_dropDown3Active     = false;
    bool              m_dropDown4Active     = false;
    bool              m_dropDown5Active     = false;
    const std::string m_descRow1Text        = "Move left";
    const std::string m_descRow2Text        = "Move right";
    const std::string m_descRow3Text        = "Move up";
    const std::string m_descRow4Text        = "Move down";
    const std::string m_descRow5Text        = "Shoot laser";
    const std::string m_dropDown1Text       = "Left Arrow;A";
    const std::string m_dropDown2Text       = "Right Arrow;D";
    const std::string m_dropDown3Text       = "Up Arrow;W";
    const std::string m_dropDown4Text       = "Down Arrow;S";
    const std::string m_dropDown5Text       = "Space Bar;Enter";

    // highscores page
    GameButton_t      m_backButton;
    Rectangle         m_highscoresPageBackground;

    // game over page
    GameButton_t      m_newgameButton;
    GameButton_t      m_gameoverQuitButton;
    const std::string m_gameoverText = "Game Over";
    Vector2           m_gameoverTextPosition;
    float             m_gameoverTextMaxHeight = 0;

    // playing page
    uint32_t                                             m_score              = 0;
    uint32_t                                             m_lives              = MAX_LIVES;
    std::shared_ptr<PlayerInterface>                     m_player             = nullptr;
    std::shared_ptr<Timer>                               m_meteorTimer        = nullptr;
    std::shared_ptr<Timer>                               m_rampdownTimer      = nullptr;
    std::shared_ptr<Timer>                               m_opponentTimer      = nullptr;
    std::shared_ptr<Timer>                               m_dispersionTimer    = nullptr;
    std::shared_ptr<Timer>                               m_invincibilityTimer = nullptr;
    std::shared_ptr<Timer>                               m_extralifeTimer     = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starsList;
    std::vector<std::shared_ptr<Sprite>>                 m_playerLasersList;
    std::vector<std::shared_ptr<Sprite>>                 m_meteorsList;
    std::vector<std::shared_ptr<Sprite>>                 m_explosionsList;
    std::vector<std::shared_ptr<Sprite>>                 m_opponentsList;
    std::vector<std::shared_ptr<Sprite>>                 m_opponentLasersList;
    std::vector<std::shared_ptr<Sprite>>                 m_dispersionsList;
    std::vector<std::shared_ptr<Sprite>>                 m_invincibilitiesList;
    std::vector<std::shared_ptr<Sprite>>                 m_extralifesList;
};

#endif // GAME_H
