#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Timer.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::Mock;

namespace TimerTest
{
class TimerTest : public ::testing::Test
{
public:
    std::shared_ptr<Timer>      m_Timer      = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(TimerTest, autostartTrue)
{
    EXPECT_CALL((*m_raylibMock), getTime()).Times(1);

    m_Timer = std::make_shared<Timer>(m_raylibMock, 0, false, true, nullptr);
    ASSERT_TRUE(m_Timer != nullptr);
}

TEST_F(TimerTest, autostartFalse)
{
    EXPECT_CALL((*m_raylibMock), getTime()).Times(0);

    m_Timer = std::make_shared<Timer>(m_raylibMock, 0, false, false, nullptr);
    ASSERT_TRUE(m_Timer != nullptr);
}

TEST_F(TimerTest, autostartFalseThenActivate)
{
    EXPECT_CALL((*m_raylibMock), getTime()).Times(1);

    m_Timer = std::make_shared<Timer>(m_raylibMock, 0, false, false, nullptr);
    ASSERT_TRUE(m_Timer != nullptr);

    m_Timer->activate();
}

TEST_F(TimerTest, update)
{
    EXPECT_CALL((*m_raylibMock), getTime()).Times(2);

    m_Timer = std::make_shared<Timer>(m_raylibMock, 0, false, true, nullptr);
    ASSERT_TRUE(m_Timer != nullptr);

    m_Timer->update();
}

} // namespace TimerTest
