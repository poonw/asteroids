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
    bool      windowShouldClose(void) override;
    float     getFrameTime(void) override;
    void      beginDrawing(void) override;
    void      clearBackground(Color color) override;
    void      endDrawing(void) override;
    void      drawTextureV(Texture2D texture, Vector2 position, Color tint) override;
    bool      isKeyDown(int key) override;
};

#endif // RAYLIBWRAPPER_H
