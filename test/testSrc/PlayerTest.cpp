#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Player.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
using ::testing::EndsWith;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

void shootLaser(Vector2 vec)
{
}

namespace PlayerTest
{
class PlayerTest : public ::testing::Test
{
public:
    std::shared_ptr<Player>      m_Player     = nullptr;
    std::shared_ptr<RaylibMock>  m_raylibMock = nullptr;
    std::function<void(Vector2)> f_shootLaser = shootLaser;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));
        EXPECT_CALL((*m_raylibMock), loadTexture(EndsWith("spaceship.png"))).Times(Exactly(1));
        m_Player = std::make_shared<Player>(m_raylibMock, "", f_shootLaser);
        ASSERT_TRUE(m_Player != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(PlayerTest, updateWithoutSpaceKeyPressed)
{
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_RIGHT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_LEFT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_DOWN)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_UP)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyPressed(KEY_SPACE)).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getFrameTime()).Times(Exactly(1));
    m_Player->update();
}

TEST_F(PlayerTest, updateWithSpaceKeyPressed)
{
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_RIGHT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_LEFT)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_DOWN)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyDown(KEY_UP)).Times(Exactly(1));
    EXPECT_CALL((*m_raylibMock), isKeyPressed(KEY_SPACE)).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), getFrameTime()).Times(Exactly(1));
    m_Player->update();
}

TEST_F(PlayerTest, draw)
{
    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Player->draw();
}

} // namespace PlayerTest
