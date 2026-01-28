#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Opponent.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace OpponentTest
{
class OpponentTest : public ::testing::Test
{
public:
    std::shared_ptr<Opponent>   m_Opponent   = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void shootLaser(Sprite::SpriteAttr_t attr)
    {
    }

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        std::function<void(Sprite::SpriteAttr_t)> f_shootLaser = std::bind(&OpponentTest::shootLaser, this, std::placeholders::_1);
        m_Opponent                                             = std::make_shared<Opponent>(m_raylibMock, f_shootLaser);
        ASSERT_TRUE(m_Opponent != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(OpponentTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Opponent->update(), "Assertion failed");
}

TEST_F(OpponentTest, update)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Opponent->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Opponent->update();
}

TEST_F(OpponentTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Opponent->draw(), "Assertion failed");
}

TEST_F(OpponentTest, draw)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Opponent->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), drawTextureEx(A<Texture2D>(),
                                               A<Vector2>(),
                                               180,
                                               1,
                                               FieldsAre(230, 41, 55, 255)))
        .Times(Exactly(1));
    m_Opponent->draw();
}

TEST_F(OpponentTest, getCenterWithoutTextures_death)
{
    EXPECT_DEATH(m_Opponent->getCenter(), "Assertion failed");
}

TEST_F(OpponentTest, getCenter)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Opponent->setTextures({fakeTexture});

    EXPECT_THAT(m_Opponent->getCenter(), A<Vector2>());
}

TEST_F(OpponentTest, getRadiusWithoutTextures_death)
{
    EXPECT_DEATH(m_Opponent->getRadius(), "Assertion failed");
}

TEST_F(OpponentTest, getRadius)
{
    Texture2D fakeTexture = {0, 4, 15, 0, 0};
    m_Opponent->setTextures({fakeTexture});

    EXPECT_EQ(m_Opponent->getRadius(), 2);
}

TEST_F(OpponentTest, getRect_death)
{
    EXPECT_DEATH(m_Opponent->getRect(), "Assertion failed");
}

TEST_F(OpponentTest, setTextures_death)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    EXPECT_DEATH(m_Opponent->setTextures(fakeTextures), "Assertion failed");
}

} // namespace OpponentTest
