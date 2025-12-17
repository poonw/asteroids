#ifndef GAME_H
#define GAME_H

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>
#include "GameSettings.h"
#include "RaylibInterface.h"

class Sprite;
class Timer;

class Game
{
public:
    Game(std::shared_ptr<RaylibInterface> raylibPtr,
         std::filesystem::path            resourcePath,
         std::function<std::shared_ptr<Sprite>(
             std::shared_ptr<RaylibInterface> raylibPtr,
             std::filesystem::path            resourcePath,
             Vector2                          position)>
             createLaserWrapper,
         std::function<std::shared_ptr<Sprite>(
             std::shared_ptr<RaylibInterface> raylibPtr,
             std::filesystem::path            resourcePath)>
             createMeteorWrapper);
    ~Game(void);

    void run(void);
    void setPlayer(std::shared_ptr<Sprite> player);
    void setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList);
    void shootLaser(Vector2 position);

private:
    void drawStars(void);
    void discardSprites(void);
    void createMeteor(void);

    std::shared_ptr<RaylibInterface>                     m_raylibPtr = nullptr;
    std::filesystem::path                                m_resourcePath;
    std::shared_ptr<Sprite>                              m_player = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starsList;
    std::vector<std::shared_ptr<Sprite>>                 m_lasersList;
    std::vector<std::shared_ptr<Sprite>>                 m_meteorsList;
    std::shared_ptr<Timer>                               m_meteorTimer = nullptr;

    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        std::filesystem::path            resourcePath,
        Vector2                          position)>
        m_createLaser;
    std::function<std::shared_ptr<Sprite>(
        std::shared_ptr<RaylibInterface> raylibPtr,
        std::filesystem::path            resourcePath)>
        m_createMeteor;
};

#endif // GAME_H
