#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Meteor.h"
#include <memory>
#include "RaylibMock.h"

using ::testing::A;
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

        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        m_Meteor = std::make_shared<Meteor>(m_raylibMock);
        ASSERT_TRUE(m_Meteor != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(MeteorTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Meteor->update(), "Assertion failed");
}

TEST_F(MeteorTest, update)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Meteor->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(1));
    m_Meteor->update();
}

TEST_F(MeteorTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Meteor->draw(), "Assertion failed");
}

TEST_F(MeteorTest, draw)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Meteor->setTextures({fakeTexture});

    EXPECT_CALL((*m_raylibMock), drawTexturePro(A<Texture2D>(),
                                                FieldsAre(0, 0, 4, 4),
                                                A<Rectangle>(),
                                                FieldsAre(2, 2),
                                                A<float>(),
                                                FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Meteor->draw();
}

TEST_F(MeteorTest, getCenterWithoutTextures_death)
{
    EXPECT_DEATH(m_Meteor->getCenter(), "Assertion failed");
}

TEST_F(MeteorTest, getCenter)
{
    Texture2D fakeTexture = {0, 4, 4, 0, 0};
    m_Meteor->setTextures({fakeTexture});

    EXPECT_THAT(m_Meteor->getCenter(), A<Vector2>());
}

TEST_F(MeteorTest, getRadiusWithoutTextures_death)
{
    EXPECT_DEATH(m_Meteor->getRadius(), "Assertion failed");
}

TEST_F(MeteorTest, getRadius)
{
    Texture2D fakeTexture = {0, 4, 15, 0, 0};
    m_Meteor->setTextures({fakeTexture});

    EXPECT_EQ(m_Meteor->getRadius(), 2);
}

TEST_F(MeteorTest, getRect_death)
{
    EXPECT_DEATH(m_Meteor->getRect(), "Assertion failed");
}

TEST_F(MeteorTest, setTextures_death)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    EXPECT_DEATH(m_Meteor->setTextures(fakeTextures), "Assertion failed");
}

} // namespace MeteorTest
