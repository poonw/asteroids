#ifndef SPRITEMOCK_H
#define SPRITEMOCK_H

#include "Sprite.h"
#include "gmock/gmock.h"

class SpriteMock : public Sprite
{
public:
    SpriteMock(void);
    ~SpriteMock(void) override;

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, draw, (), (override));
    MOCK_METHOD(void, setTextures, (std::vector<Texture2D> textures), (override));
};

#endif // SPRITEMOCK_H
