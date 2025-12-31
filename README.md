# Asteroids
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
