#include "SpriteFactory.h"
#include <cassert>
#include "Explosion.h"
#include "Laser.h"
#include "Meteor.h"
#include "Opponent.h"
#include "Star.h"

SpriteFactory::SpriteFactory(void)
{
}

SpriteFactory::~SpriteFactory(void)
{
}

std::shared_ptr<Sprite> SpriteFactory::getSprite(SpriteType                                type,
                                                 std::shared_ptr<RaylibInterface>          raylibPtr,
                                                 Sprite::SpriteAttr_t                      attr,
                                                 std::function<void(Sprite::SpriteAttr_t)> shootLaser)
{
    std::shared_ptr<Sprite> ret = nullptr;

    switch (type)
    {
        case EXPLOSION:
            ret = std::make_shared<Explosion>(raylibPtr, attr.m_position, attr.m_scale);
            break;

        case RED_LASER:
            ret = std::make_shared<Laser>(raylibPtr,
                                          attr.m_position,
                                          attr.m_direction,
                                          attr.m_rotation,
                                          WHITE);
            break;

        case YELLOW_LASER:
            ret = std::make_shared<Laser>(raylibPtr,
                                          attr.m_position,
                                          attr.m_direction,
                                          attr.m_rotation,
                                          YELLOW);
            break;

        case METEOR:
            ret = std::make_shared<Meteor>(raylibPtr);
            break;

        case OPPONENT:
            ret = std::make_shared<Opponent>(raylibPtr, shootLaser);
            break;

        case STAR:
            ret = std::make_shared<Star>(raylibPtr);
            break;

        case UNDEFINED:
        default:
            assert(false);
            break;
    }

    return ret;
}
