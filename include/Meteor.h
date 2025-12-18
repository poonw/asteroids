#ifndef METEOR_H
#define METEOR_H

#include <filesystem>
#include "RaylibInterface.h"
#include "Sprite.h"

class Meteor : public Sprite
{
public:
    Meteor(std::shared_ptr<RaylibInterface> raylibPtr,
           std::filesystem::path            resourcePath);
    ~Meteor(void) = default;

    void update(void) override;
    void draw(void) override;

private:
    void move(void);
};

#endif // METEOR_H
