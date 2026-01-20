#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Player.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace PlayerTest
{
class PlayerTest : public ::testing::Test
{
public:
    std::shared_ptr<Player>     m_Player     = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void shootLaser(Sprite::SpriteAttr_t attr)
    {
    }

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        std::function<void(Sprite::SpriteAttr_t)> f_shootLaser = std::bind(&PlayerTest::shootLaser, this, std::placeholders::_1);
        m_Player                                               = std::make_shared<Player>(m_raylibMock, f_shootLaser);
        ASSERT_TRUE(m_Player != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(PlayerTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Player->update(), "Assertion failed");
}

TEST_F(PlayerTest, updatePlayableWithoutSpaceKeyPressed)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Player->setTextures({fakeTexture});
    m_Player->m_discard = false;

    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_RIGHT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_LEFT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_DOWN)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_UP)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyPressed(KEY_SPACE)).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getFrameTime()).Times(Exactly(1));

    m_Player->update();
}

TEST_F(PlayerTest, updatePlayableWithSpaceKeyPressed)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Player->setTextures({fakeTexture});
    m_Player->m_discard = false;

    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_RIGHT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_LEFT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_DOWN)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_UP)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyPressed(KEY_SPACE)).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), getFrameTime()).Times(Exactly(1));

    m_Player->update();
}

TEST_F(PlayerTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Player->draw(), "Assertion failed");
}

TEST_F(PlayerTest, draw)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Player->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));

    m_Player->draw();
}

TEST_F(PlayerTest, getCenterWithoutTextures_death)
{
    EXPECT_DEATH(m_Player->getCenter(), "Assertion failed");
}

TEST_F(PlayerTest, getRadiusWithoutTextures_death)
{
    EXPECT_DEATH(m_Player->getRadius(), "Assertion failed");
}

TEST_F(PlayerTest, getCenter_getRadius)
{
    Texture2D fakeTexture = {0, 6, 10, 0, 0};
    m_Player->setTextures({fakeTexture});

    EXPECT_EQ(m_Player->getRadius(), 3);
    EXPECT_THAT(m_Player->getCenter(), FieldsAre(800, 795));
}

TEST_F(PlayerTest, getRect_death)
{
    EXPECT_DEATH(m_Player->getRect(), "Assertion failed");
}

TEST_F(PlayerTest, setTextures_death)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    EXPECT_DEATH(m_Player->setTextures(fakeTextures), "Assertion failed");
}

TEST_F(PlayerTest, stateMachine)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Player->setTextures({fakeTexture});

    EXPECT_TRUE(m_Player->m_discard);

    // PLAYABLE --> INVISIBLE
    EXPECT_CALL((*m_raylibMock), getTime()).WillOnce(Return(1));
    m_Player->update();
    m_Player->draw();
    EXPECT_TRUE(m_Player->m_discard);

    // INVISIBLE --> VISIBLE
    EXPECT_CALL((*m_raylibMock), getTime())
        .WillOnce(Return(2))
        .WillOnce(Return(2));
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(80, 80, 80, 255)))
        .Times(Exactly(1));
    m_Player->update();
    m_Player->draw();
    EXPECT_TRUE(m_Player->m_discard);

    // VISIBLE
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(0.1));
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(80, 80, 80, 255)))
        .Times(Exactly(1));
    m_Player->update();
    m_Player->draw();
    EXPECT_TRUE(m_Player->m_discard);

    // VISIBLE --> INVINCIBLE
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(80, 80, 80, 255)))
        .Times(Exactly(1));
    m_Player->update();
    m_Player->draw();
    EXPECT_TRUE(m_Player->m_discard);

    // INVINCIBLE --> PLAYABLE
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_RIGHT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_LEFT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_DOWN)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_UP)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyPressed(KEY_SPACE)).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    EXPECT_CALL((*m_raylibMock), getTime()).WillOnce(Return(6));
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Player->update();
    m_Player->draw();
    EXPECT_FALSE(m_Player->m_discard);
}

} // namespace PlayerTest
