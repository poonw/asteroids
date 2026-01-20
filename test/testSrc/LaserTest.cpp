#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Laser.h"
#include <memory>
#include "GameSettings.h"
#include "RaylibMock.h"

using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace LaserTest
{
class LaserTest : public ::testing::Test
{
public:
    std::shared_ptr<Laser>      m_Laser      = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        Vector2 startPos  = {0, WINDOW_HEIGHT};
        Vector2 direction = {0, -1};
        float   rotation  = 180;
        Color   color     = WHITE;
        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        m_Laser = std::make_shared<Laser>(m_raylibMock, startPos, direction, rotation, color);
        ASSERT_TRUE(m_Laser != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(LaserTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Laser->update(), "Assertion failed");
}

TEST_F(LaserTest, update)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Laser->setTextures({fakeTexture});

    EXPECT_FALSE(m_Laser->m_discard);

    //first call, the laser is still within frame
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Laser->update();
    EXPECT_FALSE(m_Laser->m_discard);

    //second call, the laser is out of frame
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Laser->update();
    EXPECT_TRUE(m_Laser->m_discard);
}

TEST_F(LaserTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Laser->draw(), "Assertion failed");
}

TEST_F(LaserTest, draw)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    m_Laser->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), drawTextureEx(A<Texture2D>(), A<Vector2>(), 180, 1, FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Laser->draw();
}

TEST_F(LaserTest, getCenter_death)
{
    EXPECT_DEATH(m_Laser->getCenter(), "Assertion failed");
}

TEST_F(LaserTest, getRadius_death)
{
    EXPECT_DEATH(m_Laser->getRadius(), "Assertion failed");
}

TEST_F(LaserTest, getRectWithoutTextures_death)
{
    EXPECT_DEATH(m_Laser->getRect(), "Assertion failed");
}

TEST_F(LaserTest, getRect)
{
    Texture2D fakeTexture = {0, 5, 15, 0, 0};
    m_Laser->setTextures({fakeTexture});

    EXPECT_THAT(m_Laser->getRect(), FieldsAre(5, (WINDOW_HEIGHT + 15), 5, 15));
}

TEST_F(LaserTest, setTextures_death)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    EXPECT_DEATH(m_Laser->setTextures(fakeTextures), "Assertion failed");
}

} // namespace LaserTest
