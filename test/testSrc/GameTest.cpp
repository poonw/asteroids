#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Game.h"
#include <memory>
#include "GameSettings.h"
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
Sequence seq;

namespace GameTest
{
class GameTest : public ::testing::Test
{
public:
    std::shared_ptr<Game>              m_Game              = nullptr;
    std::shared_ptr<RaylibMock>        m_raylibMock        = nullptr;
    std::shared_ptr<SpriteFactoryFake> m_spriteFactoryFake = nullptr;

    void SetUp(void)
    {
        m_raylibMock        = std::make_shared<RaylibMock>();
        m_spriteFactoryFake = std::make_shared<SpriteFactoryFake>();

        ASSERT_TRUE(m_raylibMock != nullptr);
        ASSERT_TRUE(m_spriteFactoryFake != nullptr);

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
        ASSERT_TRUE(m_spriteFactoryFake->m_playerMocksList.size() == 1);
        ASSERT_TRUE(m_spriteFactoryFake->m_starMocksList.size() == NUMBER_OF_STARS);
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

TEST_F(GameTest, loopWithOnlyPlayerAndStars)
{
    m_Game->setState(Game::PLAYING);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), update()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), draw()).InSequence(seq);
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
}

TEST_F(GameTest, playerMeteorNoCollisionTest)
{
    m_Game->setState(Game::PLAYING);
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), update()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), draw()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();
}

TEST_F(GameTest, playerMeteorCollisionTest)
{
    m_Game->setState(Game::PLAYING);
    m_Game->createMeteor();

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), update()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), draw()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(true));
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, meteorLaserNoCollisionTest)
{
    m_Game->setState(Game::PLAYING);
    m_Game->createMeteor();
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), update()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), draw()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_FALSE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_FALSE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 0);
}

TEST_F(GameTest, meteorLaserCollisionTest)
{
    m_Game->setState(Game::PLAYING);
    m_Game->createMeteor();
    Sprite::SpriteAttr_t attr;
    m_Game->playerShootLaser(attr);

    EXPECT_TRUE(m_spriteFactoryFake->m_meteorMocksList.size() == 1);
    EXPECT_TRUE(m_spriteFactoryFake->m_playerLaserMocksList.size() == 1);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), update()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), draw()).InSequence(seq);
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getRadius()).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMocksList[0])), getCenter()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionCircles(A<Vector2>(), A<float>(), A<Vector2>(), A<float>()))
        .InSequence(seq)
        .WillOnce(Return(false));

    m_Game->run();

    EXPECT_TRUE((m_spriteFactoryFake->m_playerLaserMocksList[0])->m_discard);
    EXPECT_TRUE((m_spriteFactoryFake->m_meteorMocksList[0])->m_discard);
    EXPECT_TRUE(m_spriteFactoryFake->m_explosionMocksList.size() == 1);
}

TEST_F(GameTest, settingsPage_mouseNotPointingToBackButton)
{
    m_Game->setState(Game::SETTINGS);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Back",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, settingsPage_mousePointingToBackButtonButNotClick)
{
    m_Game->setState(Game::SETTINGS);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Back",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, settingsPage_mousePointingToBackButtonAndClick)
{
    m_Game->setState(Game::SETTINGS);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<std::string>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Back",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToNothing)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToStartButtonButNotClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToStartButtonAndClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToSettingsButtonButNotClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToSettingsButtonAndClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToQuitButtonButNotClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, welcomePage_mousePointingToQuitButtonAndClick)
{
    m_Game->setState(Game::WELCOME);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Asteroids",
                                            A<Vector2>(),
                                            GAME_TITLE_FONTSIZE,
                                            0,
                                            FieldsAre(255, 203, 0, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Start",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Settings",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), closeWindow()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, gameoverPage_mousePointingToNothing)
{
    m_Game->setState(Game::GAME_OVER);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    //loop 1
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

    //loop 2
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
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
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Retry",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, gameoverPage_mousePointingToRetryButtonButNotClick)
{
    m_Game->setState(Game::GAME_OVER);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    //loop 1
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

    //loop 2
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
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
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Retry",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, gameoverPage_mousePointingToRetryButtonAndClick)
{
    m_Game->setState(Game::GAME_OVER);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    //loop 1
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

    //loop 2
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
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
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Retry",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, gameoverPage_mousePointingToQuitButtonButNotClick)
{
    m_Game->setState(Game::GAME_OVER);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    //loop 1
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

    //loop 2
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
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
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Retry",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameTest, gameoverPage_mousePointingToQuitButtonAndClick)
{
    m_Game->setState(Game::GAME_OVER);

    EXPECT_CALL((*m_raylibMock), windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    //loop 1
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

    //loop 2
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), update()).InSequence(seq);
    }
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), getMousePosition()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), checkCollisionPointRec(A<Vector2>(), A<Rectangle>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), isMouseButtonPressed(MOUSE_BUTTON_LEFT)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), playSound(A<Sound>())).InSequence(seq);
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
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Retry",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Quit",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 30,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

} // namespace GameTest
