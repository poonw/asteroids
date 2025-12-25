#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include <memory>
#include "GameSettings.h"
#include "RaylibMock.h"
#include "SpriteMock.h"

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
    std::shared_ptr<SpriteMock>                          m_playerMock = nullptr;
    std::array<std::shared_ptr<Sprite>, NUMBER_OF_STARS> m_starMocksList;
    std::shared_ptr<NiceMock<SpriteMock>>                m_laserMock     = nullptr;
    std::shared_ptr<NiceMock<SpriteMock>>                m_meteorMock    = nullptr;
    std::shared_ptr<NiceMock<SpriteMock>>                m_explosionMock = nullptr;

    std::shared_ptr<SpriteMock> createLaser(std::shared_ptr<RaylibInterface> raylibPtr,
                                            Vector2                          position)
    {
        m_laserMock = std::make_shared<NiceMock<SpriteMock>>();
        return (std::dynamic_pointer_cast<SpriteMock>(m_laserMock));
    }

    std::shared_ptr<SpriteMock> createMeteor(std::shared_ptr<RaylibInterface> raylibPtr)
    {
        m_meteorMock = std::make_shared<NiceMock<SpriteMock>>();
        return (std::dynamic_pointer_cast<SpriteMock>(m_meteorMock));
    }

    std::shared_ptr<SpriteMock> explodeMeteor(std::shared_ptr<RaylibInterface> raylibPtr,
                                              Vector2                          position)
    {
        m_explosionMock = std::make_shared<NiceMock<SpriteMock>>();
        return (std::dynamic_pointer_cast<SpriteMock>(m_explosionMock));
    }

    void SetUp(void)
    {
        m_raylibMock = std::make_shared<RaylibMock>();
        m_playerMock = std::make_shared<SpriteMock>();
        ASSERT_TRUE(m_raylibMock != nullptr);
        ASSERT_TRUE(m_playerMock != nullptr);

        for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
        {
            m_starMocksList[n] = std::make_shared<SpriteMock>();
            ASSERT_TRUE(m_starMocksList[n] != nullptr);
        }

        EXPECT_CALL((*m_raylibMock), initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game"))
            .Times(Exactly(1))
            .InSequence(seq);
        EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);

        EXPECT_CALL((*m_raylibMock), initAudioDevice())
            .Times(Exactly(1))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadTexture(A<std::string>()))
            .Times(Exactly(32))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadFontEx(_, _, _, _))
            .Times(Exactly(1))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadSound(A<std::string>()))
            .Times(Exactly(2))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadMusicStream(A<std::string>()))
            .Times(Exactly(1))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), playMusicStream(A<Music>()))
            .Times(Exactly(1))
            .InSequence(seq);

        std::function<std::shared_ptr<SpriteMock>(
            std::shared_ptr<RaylibInterface> raylibPtr)>
            f_createMeteor = std::bind(&GameTest::createMeteor, this, std::placeholders::_1);

        std::function<std::shared_ptr<SpriteMock>(
            std::shared_ptr<RaylibInterface> raylibPtr,
            Vector2                          position)>
            f_createLaser = std::bind(&GameTest::createLaser, this, std::placeholders::_1, std::placeholders::_2);

        std::function<std::shared_ptr<SpriteMock>(
            std::shared_ptr<RaylibInterface> raylibPtr,
            Vector2                          position)>
            f_explodeMeteor = std::bind(&GameTest::explodeMeteor, this, std::placeholders::_1, std::placeholders::_2);

        m_Game = std::make_shared<Game>(m_raylibMock, f_createLaser, f_createMeteor, f_explodeMeteor);
        ASSERT_TRUE(m_Game != nullptr);
    }

    void TearDown(void)
    {
        EXPECT_CALL((*m_raylibMock), closeAudioDevice()).Times(Exactly(1)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), unloadMusicStream(A<Music>())).Times(Exactly(1)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), closeWindow()).Times(Exactly(1)).InSequence(seq);
        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(GameTest, playerIsNotSet)
{
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
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

TEST_F(GameTest, loopWithOnlyPlayerAndStars)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "0",
                                            FieldsAre(50, 50),
                                            FONT_SIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
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
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();

    EXPECT_NE(m_meteorMock, nullptr);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "0",
                                            FieldsAre(50, 50),
                                            FONT_SIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_meteorMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), getCenter()).InSequence(seq);
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
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "0",
                                            FieldsAre(50, 50),
                                            FONT_SIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_meteorMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), getCenter()).InSequence(seq);
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

TEST_F(GameTest, meteorLaserNoCollisionTest)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();
    Vector2 dummyPosition(0, 0);
    m_Game->shootLaser(dummyPosition);

    EXPECT_NE(m_meteorMock, nullptr);
    EXPECT_NE(m_laserMock, nullptr);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_laserMock), update()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "0",
                                            FieldsAre(50, 50),
                                            FONT_SIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_laserMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(true));

    m_Game->run();

    EXPECT_FALSE(m_laserMock->m_discard);
    EXPECT_FALSE(m_meteorMock->m_discard);
    EXPECT_EQ(m_explosionMock, nullptr);
}

TEST_F(GameTest, meteorLaserCollisionTest)
{
    EXPECT_CALL((*m_playerMock), setTextures(_)).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), setTextures(_)).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->setPlayer(m_playerMock);
    m_Game->setStarsList(m_starMocksList);
    m_Game->createMeteor();
    Vector2 dummyPosition(0, 0);
    m_Game->shootLaser(dummyPosition);

    EXPECT_NE(m_meteorMock, nullptr);
    EXPECT_NE(m_laserMock, nullptr);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    EXPECT_CALL((*m_laserMock), update()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(std::dynamic_pointer_cast<SpriteMock>(m_starMocksList[n]))), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "0",
                                            FieldsAre(50, 50),
                                            FONT_SIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_laserMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_meteorMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .InSequence(seq)
        .WillOnce(Return(true));

    m_Game->run();

    EXPECT_TRUE(m_laserMock->m_discard);
    EXPECT_TRUE(m_meteorMock->m_discard);
    EXPECT_NE(m_explosionMock, nullptr);
}

} // namespace GameTest
