#ifndef RAYLIBWRAPPER_H
#define RAYLIBWRAPPER_H

#include "RaylibInterface.h"

class RaylibWrapper : public RaylibInterface
{
public:
    RaylibWrapper(void);
    virtual ~RaylibWrapper(void);

    double    getTime(void) override;
    void      initWindow(int width, int height, std::string title) override;
    void      closeWindow(void) override;
    Texture2D loadTexture(std::string filename) override;
    void      unloadTexture(Texture2D texture) override;
    bool      windowShouldClose(void) override;
    float     getFrameTime(void) override;
    void      beginDrawing(void) override;
    void      clearBackground(Color color) override;
    void      endDrawing(void) override;
    void      drawTextureV(Texture2D texture, Vector2 position, Color tint) override;
    bool      isKeyDown(int key) override;
    bool      isWindowReady(void) override;
    void      drawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint) override;
    bool      isKeyPressed(int key) override;
    bool      checkCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2) override;
    bool      checkCollisionCircleRec(Vector2 center, float radius, Rectangle rec) override;
    void      drawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) override;
    Font      loadFontEx(std::string fileName, int fontSize, int* codepoints, int codepointCount) override;
    void      drawTextEx(Font font, std::string text, Vector2 position, float fontSize, float spacing, Color tint) override;
    void      unloadFont(Font font) override;
    void      initAudioDevice(void) override;
    void      closeAudioDevice(void) override;
    Sound     loadSound(std::string fileName) override;
    void      unloadSound(Sound sound) override;
    Music     loadMusicStream(std::string fileName) override;
    void      unloadMusicStream(Music music) override;
    void      updateMusicStream(Music music) override;
    void      playSound(Sound sound) override;
    void      playMusicStream(Music music) override;
    void      drawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) override;
    Vector2   getMousePosition(void) override;
    bool      checkCollisionPointRec(Vector2 point, Rectangle rec) override;
    bool      isMouseButtonPressed(int button) override;
};

#endif // RAYLIBWRAPPER_H
