#include "SpriteFactoryFake.h"
#include "SpriteMock.h"

SpriteFactoryFake::SpriteFactoryFake(void)
{
}

SpriteFactoryFake::~SpriteFactoryFake(void)
{
}

std::shared_ptr<Sprite> SpriteFactoryFake::getSprite(SpriteType                                type,
                                                     std::shared_ptr<RaylibInterface>          raylibPtr,
                                                     Sprite::SpriteAttr_t                      attr,
                                                     std::function<void(Sprite::SpriteAttr_t)> shootLaser)
{
    std::shared_ptr<NiceMock<SpriteMock>> ret = std::make_shared<NiceMock<SpriteMock>>();

    switch (type)
    {
        case EXPLOSION:
            m_explosionMocksList.push_back(ret);
            break;

        case RED_LASER:
            m_playerLaserMocksList.push_back(ret);
            break;

        case YELLOW_LASER:
            m_opponentLaserMocksList.push_back(ret);
            break;

        case METEOR:
            m_meteorMocksList.push_back(ret);
            break;

        case OPPONENT:
            m_opponentMocksList.push_back(ret);
            break;

        case STAR:
            m_starMocksList.push_back(ret);
            break;

        case POWERUP:
            m_dispersionMocksList.push_back(ret);
            break;

        case UNDEFINED:
        default:
            break;
    }

    return std::dynamic_pointer_cast<Sprite>(ret);
}
