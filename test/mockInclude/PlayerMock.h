#ifndef PLAYERMOCK_H
#define PLAYERMOCK_H

#include "PlayerInterface.h"
#include "gmock/gmock.h"

class PlayerMock : public PlayerInterface
{
public:
    PlayerMock(void);
    ~PlayerMock(void) override;

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (const, override));
    MOCK_METHOD(float, getRadius, (), (const, override));
    MOCK_METHOD(void, setTextures, (const std::vector<Texture2D>& textures), (override));
    MOCK_METHOD(void, setInvincible, (), (override));
    MOCK_METHOD(void, setDispersedlaser, (), (override));
    MOCK_METHOD(void, setLeftKey, (KeyboardKey key), (override));
    MOCK_METHOD(void, setRightKey, (KeyboardKey key), (override));
    MOCK_METHOD(void, setUpKey, (KeyboardKey key), (override));
    MOCK_METHOD(void, setDownKey, (KeyboardKey key), (override));
    MOCK_METHOD(void, setShootKey, (KeyboardKey key), (override));
};

#endif // PLAYERMOCK_H
