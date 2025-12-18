#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Meteor.h"
#include <memory>
#include "GameSettings.h"
#include "RaylibMock.h"

using ::testing::A;
using ::testing::EndsWith;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace MeteorTest
{
class MeteorTest : public ::testing::Test
{
public:
    std::shared_ptr<Meteor>     m_Meteor     = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        Texture2D fakeTexture = {0, 0, 0, 0, 0};
        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));
        EXPECT_CALL((*m_raylibMock), loadTexture(EndsWith("meteor.png")))
            .Times(Exactly(1))
            .WillOnce(Return(fakeTexture));

        m_Meteor = std::make_shared<Meteor>(m_raylibMock, "");
        ASSERT_TRUE(m_Meteor != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(MeteorTest, update)
{
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Meteor->update();
}

TEST_F(MeteorTest, draw)
{
    EXPECT_CALL((*m_raylibMock), drawTextureEx(A<Texture2D>(),
                                               A<Vector2>(),
                                               A<float>(),
                                               1,
                                               FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Meteor->draw();
}

} // namespace MeteorTest
