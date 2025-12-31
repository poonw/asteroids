#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Explosion.h"
#include <memory>
#include "GameSettings.h"
#include "RaylibMock.h"

using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::Return;

namespace ExplosionTest
{
class ExplosionTest : public ::testing::Test
{
public:
    std::shared_ptr<Explosion>  m_Explosion  = nullptr;
    std::shared_ptr<RaylibMock> m_raylibMock = nullptr;

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);

        Vector2 startPos = {0, WINDOW_HEIGHT};
        EXPECT_CALL((*m_raylibMock), isWindowReady()).WillOnce(Return(true));

        m_Explosion = std::make_shared<Explosion>(m_raylibMock, startPos);
        ASSERT_TRUE(m_Explosion != nullptr);
    }

    void TearDown(void)
    {
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(ExplosionTest, updateWithoutTextures_death)
{
    EXPECT_DEATH(m_Explosion->update(), "Assertion failed");
}

TEST_F(ExplosionTest, update)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    m_Explosion->setTextures(fakeTextures);

    EXPECT_FALSE(m_Explosion->m_discard);

    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(0.001)); //m_index = 1
    m_Explosion->update();
    EXPECT_FALSE(m_Explosion->m_discard);

    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(0.001)); //m_index = 2
    m_Explosion->update();
    EXPECT_TRUE(m_Explosion->m_discard);
}

TEST_F(ExplosionTest, drawWithoutTextures_death)
{
    EXPECT_DEATH(m_Explosion->draw(), "Assertion failed");
}

TEST_F(ExplosionTest, draw)
{
    Texture2D              fakeTexture  = {0, 0, 0, 0, 0};
    std::vector<Texture2D> fakeTextures = {fakeTexture, fakeTexture};
    m_Explosion->setTextures(fakeTextures);

    EXPECT_CALL((*m_raylibMock), drawTextureV(A<Texture2D>(), A<Vector2>(), FieldsAre(255, 255, 255, 255)))
        .Times(Exactly(1));
    m_Explosion->draw();
}

TEST_F(ExplosionTest, getCenter_death)
{
    EXPECT_DEATH(m_Explosion->getCenter(), "Assertion failed");
}

TEST_F(ExplosionTest, getRadius_death)
{
    EXPECT_DEATH(m_Explosion->getRadius(), "Assertion failed");
}

TEST_F(ExplosionTest, getRect_death)
{
    EXPECT_DEATH(m_Explosion->getRect(), "Assertion failed");
}

TEST_F(ExplosionTest, setTextures_death)
{
    Texture2D fakeTexture = {0, 0, 0, 0, 0};
    EXPECT_DEATH(m_Explosion->setTextures({fakeTexture}), "Assertion failed");
}

} // namespace ExplosionTest
