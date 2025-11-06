#include "Logger.h"
#include "raylib.h"
#include "settings.h"

int main(void)
{
    Logger::getInstance().log(Logger::DEBUG, "asteroids game");
    return 0;
}
