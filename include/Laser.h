#ifndef LASER_H
#define LASER_H

#include <filesystem>
#include "RaylibInterface.h"
#include "Sprite.h"

class Laser : public Sprite
{
public:
    Laser(std::shared_ptr<RaylibInterface> raylibPtr,
          std::filesystem::path            resourcePath,
          Vector2                          position);
    ~Laser(void) = default;

    void update(void) override;
    void draw(void) override;

private:
    void move(void);
};

#endif // LASER_H
