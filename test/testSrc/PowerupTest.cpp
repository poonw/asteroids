#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Powerup.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace PowerupTest
{
class PowerupTest : public ::testing::Test
{
public:
    std::shared_ptr<Powerup>    m_Powerup    = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        m_Powerup = std::make_shared<Powerup>(m_raylibMock);
        ASSERT_TRUE(m_Powerup != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(PowerupTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Powerup->update(), "Assertion failed");
}

TEST_F(PowerupTest, update)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Powerup->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Powerup->update();
}

TEST_F(PowerupTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Powerup->draw(), "Assertion failed");
}

TEST_F(PowerupTest, draw)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Powerup->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Powerup->draw();
}

TEST_F(PowerupTest, getCenterWithoutTextures_death)
{
    EXPECT_DEATH(m_Powerup->getCenter(), "Assertion failed");
}

TEST_F(PowerupTest, getCenter)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Powerup->setTextures({fakeTexture});

    EXPECT_THAT(m_Powerup->getCenter(), A<Vector2>());
}

TEST_F(PowerupTest, getRadiusWithoutTextures_death)
{
    EXPECT_DEATH(m_Powerup->getRadius(), "Assertion failed");
}

TEST_F(PowerupTest, getRadius)
{
    Texture2D fakeTexture = {0, 4, 15, 0, 0};
    m_Powerup->setTextures({fakeTexture});

    EXPECT_EQ(m_Powerup->getRadius(), 2);
}

TEST_F(PowerupTest, getRect_death)
{
    EXPECT_DEATH(m_Powerup->getRect(), "Assertion failed");
}

TEST_F(PowerupTest, setTextures_death)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    EXPECT_DEATH(m_Powerup->setTextures(fakeTextures), "Assertion failed");
}

} // namespace PowerupTest
