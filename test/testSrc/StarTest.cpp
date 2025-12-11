#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Star.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
using ::testing::EndsWith;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace StarTest
{
class StarTest : public ::testing::Test
{
public:
    std::shared_ptr<Star>       m_Star       = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));
        EXPECT_CALL((*m_raylibMock), loadTexture(EndsWith("star.png"))).Times(Exactly(1));
        m_Star = std::make_shared<Star>(m_raylibMock, "");
        ASSERT_TRUE(m_Star != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(StarTest, update)
{
    m_Star->update();
}

TEST_F(StarTest, draw)
{
    EXPECT_CALL((*m_raylibMock), drawTextureEx(A<Texture2D>(),
                                               A<Vector2>(),
                                               0,
                                               A<float>(),
                                               FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Star->draw();
}

} // namespace StarTest
