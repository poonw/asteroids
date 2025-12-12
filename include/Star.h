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
};

#endif // STAR_H
