#ifndef RAYLIBMOCK_H
#define RAYLIBMOCK_H

#include "RaylibInterface.h"
#include "gmock/gmock.h"

class RaylibMock : public RaylibInterface
{
public:
    RaylibMock(void);
    virtual ~RaylibMock(void);

    MOCK_METHOD(double, getTime, (), (override));
    MOCK_METHOD(void, initWindow, (int width, int height, std::string title), (override));
    MOCK_METHOD(void, closeWindow, (), (override));
    MOCK_METHOD(Texture2D, loadTexture, (std::string filename), (override));
    MOCK_METHOD(bool, windowShouldClose, (), (override));
    MOCK_METHOD(float, getFrameTime, (), (override));
    MOCK_METHOD(void, beginDrawing, (), (override));
    MOCK_METHOD(void, clearBackground, (Color color), (override));
    MOCK_METHOD(void, endDrawing, (), (override));
    MOCK_METHOD(void, drawTextureV, (Texture2D texture, Vector2 position, Color tint), (override));
    MOCK_METHOD(bool, isKeyDown, (int key), (override));
};

#endif // RAYLIBMOCK_H
