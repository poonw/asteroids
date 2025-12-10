#ifndef RAYLIBINTERFACE_H
#define RAYLIBINTERFACE_H

#include "raylib.h"
#include <string>

class RaylibInterface
{
public:
    virtual ~RaylibInterface(void)                                                  = default;
    virtual double    getTime(void)                                                 = 0;
    virtual void      initWindow(int width, int height, std::string title)          = 0;
    virtual void      closeWindow(void)                                             = 0;
    virtual Texture2D loadTexture(std::string filename)                             = 0;
    virtual bool      windowShouldClose(void)                                       = 0;
    virtual float     getFrameTime(void)                                            = 0;
    virtual void      beginDrawing(void)                                            = 0;
    virtual void      clearBackground(Color color)                                  = 0;
    virtual void      endDrawing(void)                                              = 0;
    virtual void      drawTextureV(Texture2D texture, Vector2 position, Color tint) = 0;
    virtual bool      isKeyDown(int key)                                            = 0;
};

#endif // RAYLIBINTERFACE_H
