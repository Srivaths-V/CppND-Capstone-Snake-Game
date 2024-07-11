#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  
  PlaceFood();
  // Place the new features
  PlacePoisons();
  threads.emplace_back(std::thread(&Game::PlaceSuperFood, this));
  
}

Game::~Game()
{
  std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
      t.join();
  });
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, superFood, poisons);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
        if(_isGameOver)
        {
          renderer.GameOverTitle(score, snake.size);
        }
        else
        {
          renderer.UpdateWindowTitle(score, frame_count);
          frame_count = 0;
          title_timestamp = frame_end;
        }
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food->x = x;
      food->y = y;
      return;
    }
  }
}

// thread waits for 1ms between two cycles of the loop
//  generates the super food after a random waiting time ranging from 4s to 10s
void Game::PlaceSuperFood() {
  
  int x, y;
  std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> randomValue(4.0,10.0);
  double cycleDuration = randomValue(gen);

  auto updateStart = std::chrono::high_resolution_clock::now();

  while (true) {

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto stop = std::chrono::high_resolution_clock::now();
    double cycleLoop = std::chrono::duration_cast<std::chrono::seconds>(stop-updateStart).count();

    x = random_w(engine);
    y = random_h(engine);

    SDL_Point superFoodCell{
    static_cast<int>(x),
    static_cast<int>(y)};  
    
    if(cycleLoop > cycleDuration)
    {
      // Location should be free for placing super food
      if (!snake.SnakeCell(x, y) && !(food->x == x && food->y ==y) && !poisons->isPresentInPosition(superFoodCell)) {
        
        std::lock_guard<std::mutex> lock(m_superFood);
        superFood->addPosition(superFoodCell);
        
        return;
      }
    }  

  }
}

void Game::PlacePoisons() {
  int x, y;
  int replacePoisonCount = 0;
  int onScreenPoisonCount = poisons->getPoisonsCoordinates().size();
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    SDL_Point PoisonCell{
    static_cast<int>(x),
    static_cast<int>(y)};  

    // Location should be free for placing the poison
    if (!snake.SnakeCell(x, y) && !(food->x == x && food->y ==y) && !superFood->isPresentInPosition(PoisonCell)) {
      {
        std::lock_guard<std::mutex> lock(m_superFood);
        poisons->addPosition(PoisonCell);
      }
      
      if(poisons->getPoisonsCoordinates().empty())
      {
        if(_PoisonCount == poisons->getPoisonsCoordinates().size()){ 
          return;
        }
      } 
      else
      {
        replacePoisonCount++;
        if(_PoisonCount == (onScreenPoisonCount + replacePoisonCount)){
          return;
        }
      }

    }
  }
}

void Game::CheckSuperFoodCell(const SDL_Point &newCell)
{   
    if (superFood->isPresentInPosition(newCell)){
    
    // Increase the speed of the snake to constant value of 0.4
    // Let snake grow
    snake.speed = 0.4;
    snake.GrowBody();

    // Double the score
    if(score!= 0)
    {
      score = score * 2;
    }
    else
    {
      score = 2;
    }
    // Reset the hit count
    _hitCount = 0;
    {
      std::lock_guard<std::mutex> lock(m_superFood);
      superFood->deletePosition(newCell);
    }
    threads.emplace_back(std::thread(&Game::PlaceSuperFood, this));

    return; 
  }
}

void Game::CheckPoisonCell(const SDL_Point &newCell)
{
    
    if (poisons->isPresentInPosition(newCell)){
    
    // Snake speed reduces to constant value of 0.04 if it hits any Poison
    snake.speed = 0.04;
    {
      std::lock_guard<std::mutex> lock(m_superFood);
      poisons->deletePosition(newCell);
    }
    _hitCount++;
    PlacePoisons();
    return; 
  }
}

void Game::Update() {
  if (!snake.alive) 
  {
    _isGameOver = true;
    return;
  }

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food->x == new_x && food->y == new_y) {
    score++;
    PlaceFood();

    // Grow the snake body 
    snake.GrowBody();
    snake.speed = 0.2;

    // Reset the hit count
    _hitCount = 0;
  }

  // Convert coordinates to SDL_Point value
  SDL_Point newCell{
    static_cast<int>(new_x),
    static_cast<int>(new_y)};  

  // Check if speed increaser is present in the cell
  CheckSuperFoodCell(newCell);

  // Check if Poison is present in the cell
  if(!poisons->getPoisonsCoordinates().empty())
  {
    CheckPoisonCell(newCell);
  }

  // If the snake with head alone eats poison twice, then the snake dies
  if(_hitCount == 2 && snake.body.empty())
  {
    snake.alive = false;
    _isGameOver = true;
    return ;
  }
  
  // Snake with body eats a posion and size and score are decremented by 1 unit
  if(_hitCount == 1 && !snake.body.empty() && snake.size>1 && score>0)
  {
    snake.body.pop_back();
    snake.size--;
    score--;
    _hitCount = 0;
    return ;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }