#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include "GameSettings.h"
#include <memory>
#include "PlayerMock.h"
#include "RaylibMock.h"
#include "StarMock.h"

using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace GameTest
{
class GameTest : public ::testing::Test
{
public:
    std::shared_ptr<Game>                                m_Game       = nullptr;
    std::shared_ptr<RaylibMock>                          m_raylibMock = nullptr;
    std::shared_ptr<PlayerMock>                          m_playerMock = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starMocksList;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        m_playerMock = std::make_shared<PlayerMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);
        ASSERT_TRUE(m_playerMock != nullptr);

        for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
        {
            m_starMocksList[n] = std::make_shared<StarMock>();
            ASSERT_TRUE(m_starMocksList[n] != nullptr);
        }

        EXPECT_CALL((*m_raylibMock), initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game")).Times(Exactly(1));
        EXPECT_CALL((*m_raylibMock), closeWindow()).Times(Exactly(1));
        m_Game = std::make_shared<Game>(m_raylibMock);
        ASSERT_TRUE(m_Game != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(GameTest, playerIsNotSet)
{
    m_Game->setStarsList(m_starMocksList);
    EXPECT_DEATH(m_Game->run(), "Assertion failed");
}

TEST_F(GameTest, starsAreNotSet)
{
    m_Game->setPlayer(m_playerMock);
    EXPECT_DEATH(m_Game->run(), "Assertion failed");
}

TEST_F(GameTest, loop)
{
    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), update());
    EXPECT_CALL((*m_raylibMock), beginDrawing());
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255)));
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), draw());
    }
    EXPECT_CALL((*m_playerMock), draw());
    EXPECT_CALL((*m_raylibMock), endDrawing());

    m_Game->run();
}

} // namespace GameTest
