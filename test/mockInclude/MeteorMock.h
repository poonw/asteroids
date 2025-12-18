#ifndef METEORMOCK_H
#define METEORMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class MeteorMock : public Sprite
{
public:
    MeteorMock(void);
    virtual ~MeteorMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (override));
    MOCK_METHOD(float, getRadius, (), (override));
};

#endif // METEORMOCK_H
