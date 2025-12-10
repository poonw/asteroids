#include "RaylibWrapper.h"

RaylibWrapper::RaylibWrapper(void)
{
}

RaylibWrapper::~RaylibWrapper(void)
{
}

double RaylibWrapper::getTime(void)
{
    return GetTime();
}

void RaylibWrapper::initWindow(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
}

void RaylibWrapper::closeWindow(void)
{
    CloseWindow();
}

Texture2D RaylibWrapper::loadTexture(std::string filename)
{
    const char* filefullpath = filename.c_str();
    return LoadTexture(filefullpath);
}

bool RaylibWrapper::windowShouldClose(void)
{
    return WindowShouldClose();
}

float RaylibWrapper::getFrameTime(void)
{
    return GetFrameTime();
}

void RaylibWrapper::beginDrawing(void)
{
    BeginDrawing();
}

void RaylibWrapper::clearBackground(Color color)
{
    ClearBackground(color);
}

void RaylibWrapper::endDrawing(void)
{
    EndDrawing();
}

void RaylibWrapper::drawTextureV(Texture2D texture, Vector2 position, Color tint)
{
    DrawTextureV(texture, position, tint);
}

bool RaylibWrapper::isKeyDown(int key)
{
    return IsKeyDown(key);
}

bool RaylibWrapper::isWindowReady(void)
{
    return IsWindowReady();
}

void RaylibWrapper::drawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
{
    DrawTextureEx(texture, position, rotation, scale, tint);
}
