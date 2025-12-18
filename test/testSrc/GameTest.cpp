#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include "GameSettings.h"
#include <memory>
#include "ExplosionMock.h"
#include "LaserMock.h"
#include "MeteorMock.h"
#include "PlayerMock.h"
#include "RaylibMock.h"
#include "StarMock.h"

using ::testing::_;
using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Sequence;
Sequence seq;

namespace GameTest
{
class GameTest : public ::testing::Test
{
public:
    std::shared_ptr<Game>                                m_Game       = nullptr;
    std::shared_ptr<RaylibMock>                          m_raylibMock = nullptr;
    std::shared_ptr<PlayerMock>                          m_playerMock = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starMocksList;
    std::vector<std::shared_ptr<LaserMock>>              m_lasersList;
    std::vector<std::shared_ptr<NiceMock<MeteorMock>>>   m_meteorsList;
    std::vector<std::shared_ptr<ExplosionMock>>          m_explosionsList;

    std::shared_ptr<LaserMock> createLaser(std::shared_ptr<RaylibInterface> raylibPtr,
                                           Vector2                          position)
    {
        std::shared_ptr<LaserMock> laserPtr = std::make_shared<LaserMock>();
        m_lasersList.push_back(laserPtr);
        return (laserPtr);
    }

    std::shared_ptr<MeteorMock> createMeteor(std::shared_ptr<RaylibInterface> raylibPtr)
    {
        std::shared_ptr<NiceMock<MeteorMock>> meteorPtr = std::make_shared<NiceMock<MeteorMock>>();
        m_meteorsList.push_back(meteorPtr);
        return (std::dynamic_pointer_cast<MeteorMock>(meteorPtr));
    }

    std::shared_ptr<ExplosionMock> explodeMeteor(std::shared_ptr<RaylibInterface> raylibPtr,
                                                 Vector2                          position)
    {
        std::shared_ptr<ExplosionMock> explosionPtr = std::make_shared<ExplosionMock>();
        m_explosionsList.push_back(explosionPtr);
        return (explosionPtr);
    }

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

        EXPECT_CALL((*m_raylibMock), initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game"))
            .Times(Exactly(1))
            .InSequence(seq);
        EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadTexture(A<std::string>()))
            .Times(Exactly(32))
            .InSequence(seq);

        std::function<std::shared_ptr<MeteorMock>(
            std::shared_ptr<RaylibInterface> raylibPtr)>
            f_createMeteor = std::bind(&GameTest::createMeteor, this, std::placeholders::_1);

        std::function<std::shared_ptr<LaserMock>(
            std::shared_ptr<RaylibInterface> raylibPtr,
            Vector2                          position)>
            f_createLaser = std::bind(&GameTest::createLaser, this, std::placeholders::_1, std::placeholders::_2);

        std::function<std::shared_ptr<ExplosionMock>(
            std::shared_ptr<RaylibInterface> raylibPtr,
            Vector2                          position)>
            f_explodeMeteor = std::bind(&GameTest::explodeMeteor, this, std::placeholders::_1, std::placeholders::_2);

        m_Game = std::make_shared<Game>(m_raylibMock, f_createLaser, f_createMeteor, f_explodeMeteor);
        ASSERT_TRUE(m_Game != nullptr);
    }

    void TearDown(void)
    {
        EXPECT_CALL((*m_raylibMock), closeWindow()).Times(Exactly(1)).InSequence(seq);
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(GameTest, playerIsNotSet)
{
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setStarsList(m_starMocksList);
    EXPECT_DEATH(m_Game->run(), "Assertion failed");
}

TEST_F(GameTest, starsAreNotSet)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);

    m_Game->setPlayer(m_playerMock);
    EXPECT_DEATH(m_Game->run(), "Assertion failed");
}

TEST_F(GameTest, loop)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(true));

    m_Game->run();
}

TEST_F(GameTest, playerMeteorNoCollisionTest)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(true));

    m_Game->run();
}

TEST_F(GameTest, playerMeteorCollisionTest)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<StarMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_meteorsList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), closeWindow()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(true));

    m_Game->run();
}

} // namespace GameTest
