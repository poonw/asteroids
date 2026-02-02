#ifndef PLAYERMOCK_H
#define PLAYERMOCK_H

#include "PlayerInterface.h"
#include "gmock/gmock.h"

class PlayerMock : public PlayerInterface
{
public:
    PlayerMock(void);
    virtual ~PlayerMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (override));
    MOCK_METHOD(float, getRadius, (), (override));
    MOCK_METHOD(void, setTextures, (std::vector<Texture2D> textures), (override));
    MOCK_METHOD(void, setInvincible, (), (override));
    MOCK_METHOD(void, setDispersedlaser, (), (override));
};

#endif // PLAYERMOCK_H
