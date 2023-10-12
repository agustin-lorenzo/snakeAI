#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

using namespace sf;

enum Direction {
  UP, DOWN, LEFT, RIGHT
};

int board[][12] = {
  {1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,2,0},
  {0,0,0,0,0,0,0,0,0,0,0,0},
};

int headx = 0;
int heady = 0;
int snakeLength = 1;
Direction snakeDirection = DOWN;
Direction previousDirection = DOWN;
bool gameOver = false;
std::list<std::pair<int, int>> snakeHistory;

void assignFood() {
  
} // assignFood

int main() {
  
  RenderWindow window(VideoMode(600, 455), "Snake");
  Clock clock;
  Time deltaTime = seconds(1.0f / 5.0f);
  
  while(window.isOpen()) {
    
    Time elapsed = clock.getElapsedTime();
    while (elapsed >= deltaTime) {
      
      if (!gameOver) {
	std::cout << "Previous Direction: ";
	std::cout << std::to_string(previousDirection);
	std::cout << "  Snake Direction: ";
	std::cout << std::to_string(snakeDirection) << std::endl;
	std::cout << "headx = " << headx << std::endl;
	std::cout << "heady = " << heady << std::endl;
      
	// Update board array based on direction
	if (snakeDirection == UP) {
	  headx--;
	} else if (snakeDirection == DOWN) {
	  headx++;
	} else if (snakeDirection == LEFT) {
	  heady--;
	} else if (snakeDirection == RIGHT) {
	  heady++;
	}

	if (board[headx][heady] == 1) gameOver = true;
	if (board[headx][heady] == 2) {
	  snakeLength++;
	  assignFood();
	}
	previousDirection = snakeDirection;
	snakeHistory.push_back(std::make_pair(headx, heady));

	// Board has to be "cleared" momentarily before snake is rewritten/food is written
	// Otherwise blocks wouldn't revert back from the snake value
        for (int i = 0; i < 9; i++) {
	  for (int j = 0; j < 12; j++) {
	    if (board[i][j] == 1) {
	      board[i][j] = 0;
	    }
	  }
	} // for

	// Read from the end of the snakeHistory list
	// Set blocks in the board to snake value (1) for "snakeLength" number of blocks
	auto iter = snakeHistory.rbegin();
        for (int i = 0; i < snakeLength && iter != snakeHistory.rend(); ++i, ++iter) {
	  std::pair<int, int> location = *iter;
	  int x = location.first;
	  int y = location.second;
	  board[x][y] = 1;
	} // for
	
	// Check for collisions
	if (headx >= 9 || heady >= 12) {
	  std::cout << "OUT OF BOUNDS" << std::endl;
	  gameOver = true;
	}
	
	
      } else { // if !gameOver
	std:: cout << "GAME OVER!" << std::endl;
	return 0;
      } // else gameOver

      elapsed -= deltaTime;
      clock.restart();
    } // while elapsed >= deltaTime: game tick
    

    // Drawing the board between window.clear() and window.display
    window.clear(Color(44,44,44));
    // Draw the parts of the board set to 1(snake value) or 2(food)
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 12; j++) {
	if (board[i][j] == 1) {
	  RectangleShape block(Vector2f(45.f, 45.f));
	  block.setFillColor(Color(48,94,59));
	  block.setOutlineThickness(5.f);
	  block.setOutlineColor(Color(0,0,0));
	  int x = 50*j;
	  int y = 50*i;
	  block.setPosition(x+5, y+5);
	  window.draw(block);
	  
	} // if board[][] == 1: part of snake

	if (board[i][j] == 2) {
	  RectangleShape block(Vector2f(45.f, 45.f));
	  block.setFillColor(Color(255,223,55));
	  block.setOutlineThickness(5.f);
	  block.setOutlineColor(Color(0,0,0));
	  int x = 50*j;
	  int y = 50*i;
	  block.setPosition(x+5, y+5);
	  window.draw(block);   
	} // if board[][] == 2: food

      } // for
	
    } // for
    window.display();

    
    // Checking for keyboard events
    Event event;
    while(window.pollEvent(event)) {
      
      if (event.type == Event::Closed) {
	window.close();
      }

      if (Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::A)) {
	snakeDirection = LEFT;
      }

      if (Keyboard::isKeyPressed(Keyboard::Right) ||
      Keyboard::isKeyPressed(Keyboard::D)) {
	snakeDirection = RIGHT;
      }

      if (Keyboard::isKeyPressed(Keyboard::Up) ||
      Keyboard::isKeyPressed(Keyboard::W)) {
	snakeDirection = UP;
      }

      if (Keyboard::isKeyPressed(Keyboard::Down) ||
      Keyboard::isKeyPressed(Keyboard::S)) {
	snakeDirection = DOWN;
      }
    
    } // while pollEvent()
      
  } // while window.isOpen()
    
  return 0;
 
}
