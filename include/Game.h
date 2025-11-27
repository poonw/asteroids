#ifndef GAME_H
#define GAME_H

#include <memory>

class RaylibInterface;
class Sprite;

class Game
{
public:
    Game(std::shared_ptr<RaylibInterface> raylibPtr,
         std::shared_ptr<Sprite>          player);
    ~Game(void);

    void loop(void);

private:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    std::shared_ptr<Sprite>          m_player    = nullptr;
};

#endif // GAME_H
