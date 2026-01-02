#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "RaylibInterface.h"
#include "Sprite.h"

class Explosion : public Sprite
{
public:
    Explosion(std::shared_ptr<RaylibInterface> raylibPtr,
              Vector2                          position,
              float                            scale);
    ~Explosion(void) = default;

    void      update(void) override;
    void      draw(void) override;
    Vector2   getCenter(void) override;
    float     getRadius(void) override;
    Rectangle getRect(void) override;
    void      setTextures(std::vector<Texture2D> textures) override;

private:
    uint32_t m_index = 0;
    float    m_scale = 0;
};

#endif // EXPLOSION_H
