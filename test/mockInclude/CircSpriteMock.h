#ifndef CIRCSPRITEMOCK_H
#define CIRCSPRITEMOCK_H

#include "CircFeature.h"
#include "Sprite.h"
#include "gmock/gmock.h"

class CircSpriteMock : public Sprite, public CircFeature
{
public:
    CircSpriteMock(void);
    ~CircSpriteMock(void) override;

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Vector2, getCenter, (), (const, override));
    MOCK_METHOD(float, getRadius, (), (const, override));
    MOCK_METHOD(void, setTextures, (const std::vector<Texture2D>& textures), (override));
};

#endif // CIRCSPRITEMOCK_H
