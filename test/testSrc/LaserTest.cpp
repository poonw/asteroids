#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Laser.h"
#include <memory>
#include "GameSettings.h"
#include "RaylibMock.h"

using ::testing::A;
using ::testing::EndsWith;
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

        Vector2   startPos    = {0, WINDOW_HEIGHT};
        Texture2D fakeTexture = {0, 0, 0, 0, 0};
        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));
        EXPECT_CALL((*m_raylibMock), loadTexture(EndsWith("laser.png")))
            .Times(Exactly(1))
            .WillOnce(Return(fakeTexture));

        m_Laser = std::make_shared<Laser>(m_raylibMock, "", startPos);
        ASSERT_TRUE(m_Laser != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(LaserTest, update)
{
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

TEST_F(LaserTest, draw)
{
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Laser->draw();
}

} // namespace LaserTest
