#ifndef RAYLIBINTERFACE_H
#define RAYLIBINTERFACE_H

#include "raylib.h"
#include <string>

class RaylibInterface
{
public:
    virtual ~RaylibInterface(void)                                                                                                    = default;
    virtual double    getTime(void)                                                                                                   = 0;
    virtual void      initWindow(int width, int height, std::string title)                                                            = 0;
    virtual void      closeWindow(void)                                                                                               = 0;
    virtual Texture2D loadTexture(std::string filename)                                                                               = 0;
    virtual bool      windowShouldClose(void)                                                                                         = 0;
    virtual float     getFrameTime(void)                                                                                              = 0;
    virtual void      beginDrawing(void)                                                                                              = 0;
    virtual void      clearBackground(Color color)                                                                                    = 0;
    virtual void      endDrawing(void)                                                                                                = 0;
    virtual void      drawTextureV(Texture2D texture, Vector2 position, Color tint)                                                   = 0;
    virtual bool      isKeyDown(int key)                                                                                              = 0;
    virtual bool      isWindowReady(void)                                                                                             = 0;
    virtual void      drawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)                     = 0;
    virtual bool      isKeyPressed(int key)                                                                                           = 0;
    virtual bool      checkCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)                           = 0;
    virtual bool      checkCollisionCircleRec(Vector2 center, float radius, Rectangle rec)                                            = 0;
    virtual void      drawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) = 0;
    virtual Font      loadFontEx(std::string fileName, int fontSize, int* codepoints, int codepointCount)                             = 0;
    virtual void      drawTextEx(Font font, std::string text, Vector2 position, float fontSize, float spacing, Color tint)            = 0;
};

#endif // RAYLIBINTERFACE_H
