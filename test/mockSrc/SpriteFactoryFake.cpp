#include "SpriteFactoryFake.h"
#include "CircSpriteMock.h"
#include "PlayerMock.h"
#include "RectSpriteMock.h"
#include "SpriteMock.h"

SpriteFactoryFake::SpriteFactoryFake(void)
{
}

SpriteFactoryFake::~SpriteFactoryFake(void)
{
}

std::shared_ptr<Sprite> SpriteFactoryFake::getSprite(SpriteType                                       type,
                                                     std::shared_ptr<RaylibInterface>                 raylibPtr,
                                                     const Sprite::SpriteAttr_t&                      attr,
                                                     std::function<void(const Sprite::SpriteAttr_t&)> shootLaser)
{
    std::shared_ptr<Sprite> ret = nullptr;

    switch (type)
    {
        case PLAYER:
            m_playerMock = std::make_shared<NiceMock<PlayerMock>>();
            ret          = m_playerMock;
            break;

        case EXPLOSION:
            ret = std::make_shared<NiceMock<SpriteMock>>();
            m_explosionMocksList.push_back(std::dynamic_pointer_cast<NiceMock<SpriteMock>>(ret));
            break;

        case RED_LASER:
            ret = std::make_shared<NiceMock<RectSpriteMock>>();
            m_playerLaserMocksList.push_back(std::dynamic_pointer_cast<NiceMock<RectSpriteMock>>(ret));
            break;

        case YELLOW_LASER:
            ret = std::make_shared<NiceMock<RectSpriteMock>>();
            m_opponentLaserMocksList.push_back(std::dynamic_pointer_cast<NiceMock<RectSpriteMock>>(ret));
            break;

        case METEOR:
            ret = std::make_shared<NiceMock<CircSpriteMock>>();
            m_meteorMocksList.push_back(std::dynamic_pointer_cast<NiceMock<CircSpriteMock>>(ret));
            break;

        case OPPONENT:
            ret = std::make_shared<NiceMock<CircSpriteMock>>();
            m_opponentMocksList.push_back(std::dynamic_pointer_cast<NiceMock<CircSpriteMock>>(ret));
            break;

        case STAR:
            ret = std::make_shared<NiceMock<SpriteMock>>();
            m_starMocksList.push_back(std::dynamic_pointer_cast<NiceMock<SpriteMock>>(ret));
            break;

        case POWERUP:
            ret = std::make_shared<NiceMock<CircSpriteMock>>();
            m_dispersionMocksList.push_back(std::dynamic_pointer_cast<NiceMock<CircSpriteMock>>(ret));
            break;

        case UNDEFINED:
        default:
            break;
    }

    return ret;
}
