#ifndef PLAYERMOCK_H
#define PLAYERMOCK_H

#include <filesystem>
#include "Sprite.h"
#include "gmock/gmock.h"

class PlayerMock : public Sprite
{
public:
    PlayerMock(void);
    virtual ~PlayerMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
};

#endif // PLAYERMOCK_H
