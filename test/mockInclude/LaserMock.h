#ifndef LASERMOCK_H
#define LASERMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class LaserMock : public Sprite
{
public:
    LaserMock(void);
    virtual ~LaserMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
};

#endif // LASERMOCK_H
