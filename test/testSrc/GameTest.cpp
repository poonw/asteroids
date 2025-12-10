#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include "GameSettings.h"
#include <memory>
#include "PlayerMock.h"
#include "RaylibMock.h"

using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace GameTest
{
class GameTest : public ::testing::Test
{
public:
    std::shared_ptr<Game>       m_Game       = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;
    std::shared_ptr<PlayerMock> m_playerMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        m_playerMock = std::make_shared<PlayerMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);
        ASSERT_TRUE(m_playerMock != nullptr);

        EXPECT_CALL((*m_raylibMock), initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game")).Times(Exactly(1));
        EXPECT_CALL((*m_raylibMock), closeWindow()).Times(Exactly(1));
        m_Game = std::make_shared<Game>(m_raylibMock, m_playerMock);
        ASSERT_TRUE(m_Game != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(GameTest, loop)
{
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), update());
    EXPECT_CALL((*m_raylibMock), beginDrawing());
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255)));
    EXPECT_CALL((*m_playerMock), draw());
    EXPECT_CALL((*m_raylibMock), endDrawing());

    m_Game->loop();
}

} // namespace GameTest
