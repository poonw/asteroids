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
        EXPLOSION,
        LASER,
        METEOR,
        OPPONENT,
        PLAYER,
        STAR,
        UNDEFINED
    };

    SpriteFactory(void);
    ~SpriteFactory(void);

    virtual std::shared_ptr<Sprite> getSprite(SpriteType                                type,
                                              std::shared_ptr<RaylibInterface>          raylibPtr,
                                              Sprite::SpriteAttr_t                      attr,
                                              std::function<void(Sprite::SpriteAttr_t)> shootLaser = nullptr);
};

#endif // SPRITEFACTORY_H
