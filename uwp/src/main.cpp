#include <Windows.h>
#define SDL_MAIN_HANDLED
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

int ShimMain();

int bootstrap(int argc, char** argv)
{
  // todo: Force these in SDL-UWP? (at least wgi)
  SDL_SetHint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, "true");
  SDL_SetHint(SDL_HINT_JOYSTICK_WGI, "true");

  return ShimMain();
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
  return SDL_RunApp(0, 0, bootstrap, NULL);
}
