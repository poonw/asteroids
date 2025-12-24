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
             Vector2                          position)>
             explodeMeteorWrapper);
    ~Game(void);

    void run(void);
    void setPlayer(std::shared_ptr<Sprite> player);
    void setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList);
    void shootLaser(Vector2 position);
    void createMeteor(void);

private:
    void update(void);
    void draw(void);
    void drawStars(void);
    void discardSprites(void);
    void checkCollisions(void);
    void drawScore(void);
    void loadResources(void);

    const std::filesystem::path      m_resourcePath = "resources";
    std::shared_ptr<RaylibInterface> m_raylibPtr    = nullptr;
    std::shared_ptr<Timer>           m_meteorTimer  = nullptr;

    std::shared_ptr<Sprite>                              m_player = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starsList;
    std::vector<std::shared_ptr<Sprite>>                 m_lasersList;
    std::vector<std::shared_ptr<Sprite>>                 m_meteorsList;
    std::vector<std::shared_ptr<Sprite>>                 m_explosionsList;

    std::unordered_map<std::string, std::vector<Texture2D>> m_texturesMap;
    Font                                                    m_fontType;

    uint32_t m_score = 0;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position)>
        m_createLaser;
    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr)>
        m_createMeteor;
    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        Vector2                          position)>
        m_explodeMeteor;
};

#endif // GAME_H
