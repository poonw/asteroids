#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include <memory>
#include "GameSettings.h"
#include "PlayerMock.h"
#include "RaylibMock.h"
#include "SpriteFactoryFake.h"
#include "SpriteMock.h"

using ::testing::_;
using ::testing::A;
using ::testing::Exactly;
using ::testing::FieldsAre;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Sequence;

namespace GamePlayingStateTest
{
class GamePlayingStateTest : public ::testing::Test
{
public:
    Sequence                           seq;
    std::shared_ptr<Game>              m_Game              = nullptr;
    std::shared_ptr<RaylibMock>        m_raylibMock        = nullptr;
    std::shared_ptr<SpriteFactoryFake> m_spriteFactoryFake = nullptr;
    std::shared_ptr<PlayerMock>        m_playerMock        = nullptr;

    void SetUp(void)
    {
        m_raylibMock        = std::make_shared<RaylibMock>();
        m_spriteFactoryFake = std::make_shared<SpriteFactoryFake>();
        m_playerMock        = std::make_shared<PlayerMock>();

        ASSERT_TRUE(m_raylibMock != nullptr);
        ASSERT_TRUE(m_spriteFactoryFake != nullptr);
        ASSERT_TRUE(m_playerMock != nullptr);

        EXPECT_CALL((*m_raylibMock), initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Asteroids"))
            .Times(Exactly(1))
            .InSequence(seq);

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
            .Times(Exactly(3))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), loadMusicStream(A<std::string>()))
            .Times(Exactly(1))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), playMusicStream(A<Music>()))
            .Times(Exactly(1))
            .InSequence(seq);

        EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Back", (MENU_ITEM_FONTSIZE + 10), 0)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Game Over", GAME_OVER_FONTSIZE, 0)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Retry", (MENU_ITEM_FONTSIZE + 30), 0)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Quit", (MENU_ITEM_FONTSIZE + 30), 0)).InSequence(seq);

        EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);

        m_Game = std::make_shared<Game>(m_raylibMock, m_spriteFactoryFake);

        ASSERT_TRUE(m_Game != nullptr);
        ASSERT_TRUE(m_spriteFactoryFake->m_starMocksList.size() == NUMBER_OF_STARS);

        m_Game->setState(Game::PLAYING);

        EXPECT_CALL((*m_playerMock), setTextures(A<std::vector<Texture2D>>())).InSequence(seq);
        m_Game->setPlayer(m_playerMock);
    }

    void TearDown(void)
    {
        EXPECT_CALL((*m_raylibMock), unloadMusicStream(A<Music>())).Times(Exactly(1)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), unloadSound(A<Sound>())).Times(Exactly(3)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), unloadFont(A<Font>())).Times(Exactly(1)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), unloadTexture(A<Texture2D>())).Times(Exactly(32)).InSequence(seq);
        EXPECT_CALL((*m_raylibMock), closeAudioDevice()).Times(Exactly(1)).InSequence(seq);

        Mock::VerifyAndClearExpectations(&m_raylibMock);
    }
};

TEST_F(GamePlayingStateTest, loopWithOnlyPlayerAndStars)
{

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_FALSE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, playerMeteorNoCollisionTest)
{
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_FALSE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, playerMeteorCollisionTest)
{
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ////////// 1st loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    ////////// 2nd loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     2",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GamePlayingStateTest, meteorLaserNoCollisionTest)
{
    m_Game->createMeteor();

    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_FALSE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, meteorLaserCollisionTest)
{
    m_Game->createMeteor();

    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ////////// 1st loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
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

    ////////// 2nd loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    1",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GamePlayingStateTest, playerOpponentNoCollisionTest)
{
    m_Game->createOpponent();

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_opponentMocksList[0])->m_discard);
    EXPECT_FALSE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, playerOpponentCollisionTest)
{
    m_Game->createOpponent();

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ////////// 1st loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    ////////// 2nd loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     2",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_opponentMocksList[0])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GamePlayingStateTest, opponentLaserNoCollisionTest)
{
    m_Game->createOpponent();

    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(false));
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_FALSE((m_spriteFactoryFake->m_opponentMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, opponentLaserCollisionTest)
{
    m_Game->createOpponent();

    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ////////// 1st loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
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

    ////////// 2nd loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:   10",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_TRUE((m_spriteFactoryFake->m_opponentMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GamePlayingStateTest, playerOpponentLaserNoCollisionTest)
{
    Sprite::SpriteAttr_t attr;
    m_Game->opponentShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), getRect()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_opponentLaserMocksList[0])->m_discard);
    EXPECT_FALSE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GamePlayingStateTest, playerOpponentLaserCollisionTest)
{
    Sprite::SpriteAttr_t attr;
    m_Game->opponentShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_opponentLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    ////////// 1st loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_opponentLaserMocksList[0])), getRect()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircleRec(A<Vector2>(), A<float>(), A<Rectangle>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    ////////// 2nd loop //////////
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     2",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_opponentLaserMocksList[0])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GamePlayingStateTest, playerMeteor3xCollisionsTestAndTransitionToGameOver)
{
    ////////// 1st loop: collision //////////
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);

    ////////// 2nd loop: collision //////////
    m_playerMock->m_discard = false;
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 2);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[1])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[1])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     2",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[1])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[1])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[1])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 2);

    ////////// 3rd loop: collision and rampdowntimer triggered //////////
    m_playerMock->m_discard = false;
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 3);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[2])), update()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[2])), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     1",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[2])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_meteorMocksList[2])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getRadius()).InSequence(seq);
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq).WillOnce(Return(1));
    EXPECT_CALL((*m_playerMock), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[2])->m_discard);
    EXPECT_TRUE(m_playerMock->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 3);

    ////////// 4th loop: transition to GAME_OVER state //////////
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq).WillOnce(Return(2));
    EXPECT_CALL((*m_playerMock), update()).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_playerMock), draw()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "lives:     3",
                                            FieldsAre((WINDOW_WIDTH - 150), 30),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "score:    0",
                                            FieldsAre((WINDOW_WIDTH - 150), (30 + STAT_FONTSIZE)),
                                            STAT_FONTSIZE,
                                            0,
                                            FieldsAre(255, 255, 255, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();

    ////////// 5th loop: GAME_OVER state /////////
    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getFrameTime()).WillOnce(Return(5));
    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Game Over",
                                            A<Vector2>(),
                                            GAME_OVER_FONTSIZE,
                                            0,
                                            FieldsAre(230, 41, 55, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

} // namespace GamePlayingStateTest
