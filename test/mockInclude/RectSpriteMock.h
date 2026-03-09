#ifndef RECTSPRITEMOCK_H
#define RECTSPRITEMOCK_H

#include "RectFeature.h"
#include "Sprite.h"
#include "gmock/gmock.h"

class RectSpriteMock : public Sprite, public RectFeature
{
public:
    RectSpriteMock(void);
    ~RectSpriteMock(void) override;

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(Rectangle, getRect, (), (override));
    MOCK_METHOD(void, setTextures, (std::vector<Texture2D> textures), (override));
};

#endif // RECTSPRITEMOCK_H
