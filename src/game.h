#ifndef GAME_H
#define GAME_H

#include <random>
#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "features.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  
  void CheckSuperFoodCell(const SDL_Point &newCell);
  void CheckPoisonCell(const SDL_Point &newCell);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  // Made food instances as unique pointer
  std::unique_ptr<SDL_Point> food = std::make_unique<SDL_Point>();

  // Created new items
  std::shared_ptr<SuperFood> superFood = std::make_shared<SuperFood>();
  std::unique_ptr<Poisons> poisons = std::make_unique<Poisons>(_PoisonCount);

  int score{0};
  int _PoisonCount{12};
  int _hitCount{0};
  bool _isGameOver{false};
 
  // Features placed in game
  void PlaceFood();
  void PlaceSuperFood();
  void PlacePoisons();
  void Update();

  std::mutex m_superFood;
  std::mutex m_poisons;
  
  // For holding all threads of the items the game 
  std::vector<std::thread> threads; 

};

#endif