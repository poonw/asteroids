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

bool RaylibWrapper::isKeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool RaylibWrapper::checkCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
    return (CheckCollisionCircles(center1, radius1, center2, radius2));
}

bool RaylibWrapper::checkCollisionCircleRec(Vector2 center, float radius, Rectangle rec)
{
    return (CheckCollisionCircleRec(center, radius, rec));
}

void RaylibWrapper::drawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

Font RaylibWrapper::loadFontEx(std::string fileName, int fontSize, int* codepoints, int codepointCount)
{
    const char* filefullpath = fileName.c_str();
    return (LoadFontEx(filefullpath, fontSize, codepoints, codepointCount));
}

void RaylibWrapper::drawTextEx(Font font, std::string text, Vector2 position, float fontSize, float spacing, Color tint)
{
    const char* charText = text.c_str();
    DrawTextEx(font, charText, position, fontSize, spacing, tint);
}

void RaylibWrapper::initAudioDevice(void)
{
    InitAudioDevice();
}

void RaylibWrapper::closeAudioDevice(void)
{
    CloseAudioDevice();
}

Sound RaylibWrapper::loadSound(std::string fileName)
{
    return (LoadSound(fileName.c_str()));
}

Music RaylibWrapper::loadMusicStream(std::string fileName)
{
    return (LoadMusicStream(fileName.c_str()));
}

void RaylibWrapper::unloadMusicStream(Music music)
{
    UnloadMusicStream(music);
}

void RaylibWrapper::updateMusicStream(Music music)
{
    UpdateMusicStream(music);
}

void RaylibWrapper::playSound(Sound sound)
{
    PlaySound(sound);
}

void RaylibWrapper::playMusicStream(Music music)
{
    PlayMusicStream(music);
}
