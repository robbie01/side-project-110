#include <iostream>
#include <SDL2/SDL.h>

#include "primitives.hpp"

#define FAIL() return ( \
  std::cout << "SDL Error: " << SDL_GetError() << std::endl, \
  1 \
)

constexpr int w = 640, h = 480;

void drawTriangle(SDL_Surface* sfc, const Triangle& tri, int color) {
  Uint32* pixels = (Uint32*)sfc->pixels;
  Rect bbox = tri.getBoundingRect();

  std::cout << bbox.x1 << ' ' << bbox.y1 << ' ' << bbox.x2 << ' ' << bbox.y2 << std::endl;

  for (int x = bbox.x1; x <= bbox.x2 && x < sfc->w; ++x) {
    if (x < 0) continue;
    for (int y = bbox.y1; y <= bbox.y2 && y < sfc->h; ++y) {
      if (y < 0) continue;
      if (tri.hasPoint(Vector2I{x, y})) pixels[x+y*w] = color;
    }
  }
}

void drawQuad(SDL_Surface* sfc, const QuadGeometry& quad) {
  drawTriangle(sfc, quad.t1, quad.color);
  drawTriangle(sfc, quad.t2, quad.color);
}

int main() {
  SDL_Window *win = nullptr;
  SDL_Surface *winSfc = nullptr, *renderSfc = nullptr;

  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("yo",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    w, h, 0);
  if (win == nullptr) {
    FAIL();
  }

  winSfc = SDL_GetWindowSurface(win);

  renderSfc = SDL_CreateRGBSurface(0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  if (renderSfc == nullptr) {
    FAIL();
  }

  Camera cam { w, h, 1, 0, 0, 0, 0, 0, 0 };

  drawQuad(renderSfc, QuadGeometry::projectRect(cam, { -100, 100, -10, 100, 0, -10 }, 0x00FF00FF));

  SDL_BlitSurface(renderSfc, nullptr, winSfc, nullptr);
  SDL_UpdateWindowSurface(win);

  SDL_Delay(3000);

  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
