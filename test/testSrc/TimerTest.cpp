#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Timer.h"
#include <memory>

using ::testing::Mock;

namespace TimerTest
{
class TimerTest : public ::testing::Test
{
public:
    std::shared_ptr<Timer> m_Timer;

    void SetUp(void)
    {
        m_Timer = std::make_shared<Timer>(0, false, false, nullptr);

        ASSERT_TRUE(m_Timer != nullptr);
    }
};

TEST_F(TimerTest, test1)
{
    EXPECT_TRUE(true);
}

TEST_F(TimerTest, test2)
{
    EXPECT_FALSE(false);
}
} // namespace TimerTest
