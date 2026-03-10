# Asteroids
![gameDemo](https://github.com/user-attachments/assets/7effaaaf-b91d-4f96-baa2-d088a27fd9f6)

The main purposes of this project are:
- to try out the raylib library by following closely this tutorial [here](https://www.youtube.com/watch?v=UoAsDlUwjy0);
- to setup a Makefile from scratch to build a C++ project;
- to setup unit testing environment with googletest framework to test the C++ code.

The following instructions are meant for development environment in Windows x64. [Raylib version 5.5](https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip) is used for the development of this project.

### Install development toolchain from w64devkit
1. download [here](https://github.com/skeeto/w64devkit/releases/download/v2.4.0/w64devkit-x64-2.4.0.7z.exe)
2. run the downloaded .exe and a folder called "w64devkit" is extracted
3. place the folder "w64devkit" under C:\
4. add "C:\w64devkit\bin" to %PATH%
5. make sure to disable of the other dev toolchain such as mingw-x64 if applicable

### Install vscode extension C++ TestMate
- [link](https://marketplace.visualstudio.com/items?itemName=matepek.vscode-catch2-test-adapter)

### init googletest repo
- call git clone https://github.com/google/googletest.git in the same level directory where asteroids in initialized


## Architecture
### Game loop
To use the raylib effectively, the order of following function calls must be respected:
<img width="624" height="907" alt="image" src="https://github.com/user-attachments/assets/2b23ce86-4c81-48ae-a112-23e2191eefc5" />

### Sprite and SpriteFactory classes
- Sprite - a computer graphic which may be moved on-screen and otherwise manipulated as a single entity.
- SpriteFactory to encapsulate the Sprite object creation - getSprite function.
- SpriteFactoryFake class for UT purpose.
- <img width="238" height="329" alt="image" src="https://github.com/user-attachments/assets/1bd86bed-d3e5-4418-aad6-d469b0f836d1" />
- Types of Sprites defined:
  - Explosion
  - Laser
  - Meteor
  - Opponent
  - Powerup
  - Star
  - SpriteMock (UT purpose)
- Each sprite defines own update and draw methods.
- <img width="1128" height="316" alt="image" src="https://github.com/user-attachments/assets/bbc88af9-5014-47bc-bba7-605b21da8c0d" />

### PlayerInterface and Player classes
- Player class is a special type of Sprite, with more unique enhancements.
- PlayerMock for UT purpose.
- <img width="477" height="269" alt="image" src="https://github.com/user-attachments/assets/789a43a9-8d34-4b4e-8d0d-4bb8f2456fd6" />

### RaylibInterface
- Raylib is included into project as a static library.
- In order to be able to mock out raylib functions, a wrapper class is created to wrap the original C functions and a mock class inherited from the interface.
- <img width="428" height="277" alt="image" src="https://github.com/user-attachments/assets/490b7769-7271-41af-9c3f-7271cca4a0cc" />

### Game class
- Keep track of the all the game objects in different lists (creation and destruction)
- Responsible to check for collisions in between game objects
- Composed of:
  - SpriteFactory
  - RaylibInterface
  - PlayerInterface
- <img width="672" height="480" alt="image" src="https://github.com/user-attachments/assets/0ea2ebdb-3d16-4453-93da-78d00846455c" />

### Game multi-page handling
- There are many “pages” in the Game:
  - WELCOME page - the landing page when user launch the program
  - SETTINGS page - to set the user controls
  - PLAYING page - where the actual game is rolled out
  - GAME_OVER page
- Button clicks to trigger the transition among the pages.
- A state machine is used to handle all these pages and the transitions, each page is a state.
- EXIT_GAME - state to consolidate the handling of the window closing and resource deallocation.
- <img width="945" height="1101" alt="image" src="https://github.com/user-attachments/assets/2d527c9b-3ac7-4933-96e9-f6edfda13b6d" />

### Player state machine
- Define a transition phase to ease in the user, to let the first time user to get to know the controls, and to prevent the case where Player collides with other objects as soon as the game is loaded.
- Player class contain different states:
  - PLAYABLE - this is the state where the PLAYER would be for the majority of the time.
  - INVISIBLE - PLAYER is invisible to the user and check for collision is skipped.
  - MOVE_IN - PLAYER is visible but greyed-out, and check for collision is skipped (invincible), and user control is ignored.
  - WARMUP - Just like MOVE_IN state, PLAYER is visible but greyed-out, and check for collision is skipped (invincible), BUT user control is enabled.
- <img width="789" height="1105" alt="image" src="https://github.com/user-attachments/assets/17050165-98f9-4f96-98c4-55cccbf1aab5" />

