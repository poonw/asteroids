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
Sequence                           seq;
std::shared_ptr<Game>              m_Game              = nullptr;
std::shared_ptr<RaylibMock>        m_raylibMock        = nullptr;
std::shared_ptr<SpriteFactoryFake> m_spriteFactoryFake = nullptr;

void gameCommonSetup(void)
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

    EXPECT_CALL((*m_raylibMock), loadTexture(A<const std::string&>()))
        .Times(Exactly(34))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), loadFontEx(_, _, _, _))
        .Times(Exactly(1))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), loadSound(A<const std::string&>()))
        .Times(Exactly(6))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), loadMusicStream(A<const std::string&>()))
        .Times(Exactly(1))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), playMusicStream(A<Music>()))
        .Times(Exactly(1))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Reset", (MENU_ITEM_FONTSIZE + 10), 0)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "OK", (MENU_ITEM_FONTSIZE + 10), 0)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Game Over", GAME_OVER_FONTSIZE, 0)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Retry", (MENU_ITEM_FONTSIZE + 30), 0)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), measureTextEx(A<Font>(), "Quit", (MENU_ITEM_FONTSIZE + 30), 0)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), getTime()).InSequence(seq);

    m_Game = std::make_shared<Game>(m_raylibMock, m_spriteFactoryFake);

    ASSERT_TRUE(m_Game != nullptr);
    ASSERT_TRUE(m_spriteFactoryFake->m_starMocksList.size() == NUMBER_OF_STARS);
}

void gameCommonTeardown(void)
{
    EXPECT_CALL((*m_raylibMock), unloadMusicStream(A<Music>())).Times(Exactly(1)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), unloadSound(A<Sound>())).Times(Exactly(6)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), unloadFont(A<Font>())).Times(Exactly(1)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), unloadTexture(A<Texture2D>())).Times(Exactly(34)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), closeAudioDevice()).Times(Exactly(1)).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), closeWindow()).Times(Exactly(1)).InSequence(seq);

    Mock::VerifyAndClearExpectations(&m_raylibMock);
    m_Game              = nullptr;
    m_raylibMock        = nullptr;
    m_spriteFactoryFake = nullptr;
}

class GameWelcomeStateTest : public ::testing::Test
{
public:
    void SetUp(void)
    {
        gameCommonSetup();
    }

    void TearDown(void)
    {
        gameCommonTeardown();
    }
};

TEST_F(GameWelcomeStateTest, mousePointingToNothing)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToStartButtonButNotClick)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToStartButtonAndClickAndTransitionToPlaying)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToSettingsButtonButNotClick)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToSettingsButtonAndClickAndTransitionToSettings)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToQuitButtonButNotClick)
{
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

TEST_F(GameWelcomeStateTest, mousePointingToQuitButtonAndClickAndTransitionToQuit)
{
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

    m_Game->run();
}

} // namespace GameTest
