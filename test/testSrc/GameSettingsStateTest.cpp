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

extern void gameCommonSetup(void);
extern void gameCommonTeardown(void);

class GameSettingsStateTest : public ::testing::Test
{
public:
    void SetUp(void)
    {
        gameCommonSetup();
        m_Game->setState(Game::SETTINGS);
    }

    void TearDown(void)
    {
        gameCommonTeardown();
    }
};

TEST_F(GameSettingsStateTest, mouseNotPointingToBackButton)
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

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiSetFont(A<Font>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), guiSetStyle(DEFAULT, TEXT_SIZE, SETTINGS_FONTSIZE)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(true));

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "OK",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Reset",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameSettingsStateTest, mousePointingToResetButtonButNotClick)
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

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiSetFont(A<Font>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), guiSetStyle(DEFAULT, TEXT_SIZE, SETTINGS_FONTSIZE)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "OK",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Reset",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameSettingsStateTest, mousePointingToResetButtonAndClickAndTransitionToWelcome)
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

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiSetFont(A<Font>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), guiSetStyle(DEFAULT, TEXT_SIZE, SETTINGS_FONTSIZE)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "OK",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Reset",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameSettingsStateTest, mousePointingToOKButtonButNotClick)
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

    EXPECT_CALL((*m_raylibMock), updateMusicStream(A<Music>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), beginDrawing()).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), clearBackground(FieldsAre(0, 0, 0, 255))).InSequence(seq);
    for (uint32_t n = 0; n < NUMBER_OF_STARS; n++)
    {
        EXPECT_CALL((*(m_spriteFactoryFake->m_starMocksList[n])), draw()).InSequence(seq);
    }

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiSetFont(A<Font>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), guiSetStyle(DEFAULT, TEXT_SIZE, SETTINGS_FONTSIZE)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "OK",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Reset",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

TEST_F(GameSettingsStateTest, mousePointingToOKButtonAndClickAndTransitionToWelcome)
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
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMock)), setLeftKey(A<KeyboardKey>())).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMock)), setRightKey(A<KeyboardKey>())).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMock)), setUpKey(A<KeyboardKey>())).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMock)), setDownKey(A<KeyboardKey>())).InSequence(seq);
    EXPECT_CALL((*(m_spriteFactoryFake->m_playerMock)), setShootKey(A<KeyboardKey>())).InSequence(seq);
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

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), A<float>(), A<int>(), A<Color>())).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            A<const std::string&>(),
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE,
                                            0,
                                            FieldsAre(245, 245, 245, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiSetFont(A<Font>())).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), guiSetStyle(DEFAULT, TEXT_SIZE, SETTINGS_FONTSIZE)).InSequence(seq);

    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));
    EXPECT_CALL((*m_raylibMock), guiDropdownBox(A<Rectangle>(), A<const std::string&>(), A<int*>(), A<bool>())).InSequence(seq).WillOnce(Return(false));

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(80, 80, 80, 255))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "OK",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), drawRectangleRounded(A<Rectangle>(), 0.2, 0, FieldsAre(0, 0, 0, 0))).InSequence(seq);
    EXPECT_CALL((*m_raylibMock), drawTextEx(A<Font>(),
                                            "Reset",
                                            A<Vector2>(),
                                            MENU_ITEM_FONTSIZE + 10,
                                            0,
                                            FieldsAre(200, 200, 200, 255)))
        .InSequence(seq);

    EXPECT_CALL((*m_raylibMock), endDrawing()).InSequence(seq);

    m_Game->run();
}

} // namespace GameTest
