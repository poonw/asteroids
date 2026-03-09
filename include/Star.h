#ifndef STAR_H
#define STAR_H

#include "RaylibInterface.h"
#include "Sprite.h"

class Star : public Sprite
{
public:
    Star(std::shared_ptr<RaylibInterface> raylibPtr);
    ~Star(void) override = default;

    void update(void) override;
    void draw(void) override;
    void setTextures(std::vector<Texture2D> textures) override;

private:
    float m_scale = 0;
};

#endif // STAR_H
