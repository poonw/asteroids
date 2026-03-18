#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include <functional>
#include <memory>
#include "RaylibInterface.h"
#include "Sprite.h"

class SpriteFactory
{
public:
    enum SpriteType
    {
        PLAYER,
        EXPLOSION,
        RED_LASER,
        YELLOW_LASER,
        METEOR,
        OPPONENT,
        STAR,
        POWERUP,
        UNDEFINED
    };

    virtual ~SpriteFactory(void) = default;

    virtual std::shared_ptr<Sprite> getSprite(SpriteType                                       type,
                                              std::shared_ptr<RaylibInterface>                 raylibPtr,
                                              const Sprite::SpriteAttr_t&                      attr,
                                              std::function<void(const Sprite::SpriteAttr_t&)> shootLaser = nullptr);
};

#endif // SPRITEFACTORY_H
