#ifndef STAR_H
#define STAR_H

#include <filesystem>
#include "RaylibInterface.h"
#include "Sprite.h"

class Star : public Sprite
{
public:
    Star(std::shared_ptr<RaylibInterface> raylibPtr,
         std::filesystem::path            resourcePath);
    ~Star(void) = default;

    void update(void) override;
    void draw(void) override;

private:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    Texture2D                        m_texture;
    Vector2                          m_position = {0, 0};
    float                            m_scale    = 0;
};

#endif // STAR_H
