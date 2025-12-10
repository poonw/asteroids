#ifndef GAME_H
#define GAME_H

#include <array>
#include <memory>
#include "GameSettings.h"

class RaylibInterface;
class Sprite;

class Game
{
public:
    Game(std::shared_ptr<RaylibInterface> raylibPtr);
    ~Game(void);

    void run(void);
    void setPlayer(std::shared_ptr<Sprite> player);
    void setStarsList(std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS>& starsList);

private:
    void drawStars(void);

    std::shared_ptr<RaylibInterface>                     m_raylibPtr = nullptr;
    std::shared_ptr<Sprite>                              m_player    = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starsList;
};

#endif // GAME_H
