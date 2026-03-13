#ifndef SPRITEFACTORYFAKE_H
#define SPRITEFACTORYFAKE_H

#include "SpriteFactory.h"
#include "gmock/gmock.h"

using ::testing::NiceMock;

class CircSpriteMock;
class RectSpriteMock;
class SpriteMock;

class SpriteFactoryFake : public SpriteFactory
{
public:
    SpriteFactoryFake(void);
    ~SpriteFactoryFake(void) override;

    std::shared_ptr<Sprite> getSprite(SpriteType                                type,
                                      std::shared_ptr<RaylibInterface>          raylibPtr,
                                      Sprite::SpriteAttr_t                      attr,
                                      std::function<void(Sprite::SpriteAttr_t)> shootLaser = nullptr);

    std::vector<std::shared_ptr<NiceMock<SpriteMock>>>     m_starMocksList;
    std::vector<std::shared_ptr<NiceMock<SpriteMock>>>     m_explosionMocksList;
    std::vector<std::shared_ptr<NiceMock<CircSpriteMock>>> m_meteorMocksList;
    std::vector<std::shared_ptr<NiceMock<CircSpriteMock>>> m_opponentMocksList;
    std::vector<std::shared_ptr<NiceMock<CircSpriteMock>>> m_dispersionMocksList;
    std::vector<std::shared_ptr<NiceMock<RectSpriteMock>>> m_playerLaserMocksList;
    std::vector<std::shared_ptr<NiceMock<RectSpriteMock>>> m_opponentLaserMocksList;
};

#endif // SPRITEFACTORYFAKE_H
