#ifndef SPRITEMOCK_H
#define SPRITEMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class SpriteMock : public Sprite
{
public:
    SpriteMock(void);
    virtual ~SpriteMock(void);

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (override));
    MOCK_METHOD(float, getRadius, (), (override));
    MOCK_METHOD(Rectangle, getRect, (), (override));
    MOCK_METHOD(void, setTextures, (std::vector<Texture2D> textures), (override));
};

#endif // SPRITEMOCK_H
