#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "gamefeatures.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();
  
  void Render(Snake const snake, std::unique_ptr<SDL_Point> &food, 
              std::shared_ptr<SuperFood> &superFood, std::unique_ptr<Poisons> &poisons);

  void UpdateWindowTitle(int score, int fps);
  void GameOverTitle(int score, int size);
  bool _gameStatus{false};

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  bool _isRenderRequired{true};
};

#endif