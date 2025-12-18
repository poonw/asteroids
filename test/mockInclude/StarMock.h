#ifndef STARMOCK_H
#define STARMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class StarMock : public Sprite
{
public:
    StarMock(void);
    virtual ~StarMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (override));
    MOCK_METHOD(float, getRadius, (), (override));
};

#endif // STARMOCK_H
