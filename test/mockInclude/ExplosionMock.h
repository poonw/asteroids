#ifndef EXPLOSIONMOCK_H
#define EXPLOSIONMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class ExplosionMock : public Sprite
{
public:
    ExplosionMock(void);
    virtual ~ExplosionMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (override));
    MOCK_METHOD(float, getRadius, (), (override));
    MOCK_METHOD(Rectangle, getRect, (), (override));
    MOCK_METHOD(void, setTextures, (std::vector<Texture2D> textures), (override));
};

#endif // EXPLOSIONMOCK_H
