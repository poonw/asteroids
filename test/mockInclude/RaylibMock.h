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
    MOCK_METHOD(bool, isWindowReady, (), (override));
    MOCK_METHOD(void, drawTextureEx, (Texture2D texture, Vector2 position, float rotation, float scale, Color tint), (override));
    MOCK_METHOD(bool, isKeyPressed, (int key), (override));
    MOCK_METHOD(bool, checkCollisionCircles, (Vector2 center1, float radius1, Vector2 center2, float radius2), (override));
    MOCK_METHOD(bool, checkCollisionCircleRec, (Vector2 center, float radius, Rectangle rec), (override));
    MOCK_METHOD(void, drawTexturePro, (Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint), (override));
    MOCK_METHOD(Font, loadFontEx, (std::string fileName, int fontSize, int* codepoints, int codepointCount), (override));
    MOCK_METHOD(void, drawTextEx, (Font font, std::string text, Vector2 position, float fontSize, float spacing, Color tint), (override));
};

#endif // RAYLIBMOCK_H
