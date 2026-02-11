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

namespace GameTest
{
extern Sequence                           seq;
extern std::shared_ptr<Game>              m_Game;
extern std::shared_ptr<RaylibMock>        m_raylibMock;
extern std::shared_ptr<SpriteFactoryFake> m_spriteFactoryFake;
extern std::shared_ptr<PlayerMock>        m_playerMock;

extern void gameCommonSetup(void);
extern void gameCommonTeardown(void);

class GameGameoverStateTest : public ::testing::Test
{
public:
    void SetUp(void)
    {
        gameCommonSetup();
        m_Game->setState(Game::GAME_OVER);
        EXPECT_CALL((*m_playerMock), setTextures(A<std::vector<Texture2D>>())).InSequence(seq);
        m_Game->setPlayer(m_playerMock);
    }

    void TearDown(void)
    {
        gameCommonTeardown();
    }
};

TEST_F(GameGameoverStateTest, mousePointingToNothing)
{
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

TEST_F(GameGameoverStateTest, mousePointingToRetryButtonButNotClick)
{
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

TEST_F(GameGameoverStateTest, mousePointingToRetryButtonAndClickAndTransitionToWelcome)
{
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

TEST_F(GameGameoverStateTest, mousePointingToQuitButtonButNotClick)
{
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

TEST_F(GameGameoverStateTest, mousePointingToQuitButtonAndClickAndTransitionToQuit)
{
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
