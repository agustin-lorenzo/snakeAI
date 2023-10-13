#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <utility>
#include <random>
#include <cmath>

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

// Initalize constants
int headx = 0;
int heady = 0;
int snakeLength = 1;
Direction snakeDirection = DOWN;
Direction previousDirection = DOWN;
bool gameOver = false;
std::list<std::pair<int, int>> snakeHistory;
std::random_device rd;

// Helper/debug function to print a 2D array
void print2DArray(int arr[][12]) {
  int numRows = 9;
  int numCols = 12;
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      std::cout << arr[i][j] << '\t'; // Adjust formatting as needed
    }
    std::cout << std::endl;
  }
}

// Helper function for assigning food to a random location
void assignFood() {
  std::cout << "ASSIGNFOOD() ENTERED" << std::endl;
  std::mt19937 gen(rd());  
  std::uniform_int_distribution<int> dist1(0, 11); 
  std::uniform_int_distribution<int> dist2(0, 8);
  
  int x = dist1(gen);
  int y = dist2(gen);

  // Loop over until random location isn't part of a snake
  while (board[x][y] == 1
  && (board[x-1][y] == 1 || board[x+1][y] == 1
  || board[x][y-1] == 1 || board[x][y+1] == 1)) {
  
    x = dist1(gen);
    y = dist2(gen);
    
  }

  std::cout << "FOOD X = " << x << std::endl;
  std::cout << "FOOD Y = " << y << std::endl;
  board[y][x] = 2;
  std::cout << "board[x][y] = " << board[x][y] << std::endl;
} // assignFood



int main() {
  
  RenderWindow window(VideoMode(600, 455), "Snake");
  Clock clock;
  Time deltaTime = seconds(1.0f / 5.0f);
  bool needFood = false;

  
  while(window.isOpen()) {
    
    Time elapsed = clock.getElapsedTime();
    while (elapsed >= deltaTime) {
      
      if (!gameOver) {
	std::cout << "PREVIOUS HEAD\n";
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
	std::cout << "NEXT HEAD\n";
	std::cout << "headx = " << headx << std::endl;
	std::cout << "heady = " << heady << std::endl;
	
	if (board[headx][heady] == 1) gameOver = true;
	previousDirection = snakeDirection;
	snakeHistory.push_back(std::make_pair(headx, heady));
	if (board[headx][heady] == 2) {
	  snakeLength++;
	  needFood = true;
	}
	
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
	} 
	
	// Check for out of bounds
	if (headx >= 9 || headx < 0 || heady >= 12 || heady < 0) {
	  std::cout << "OUT OF BOUNDS" << std::endl;
	  gameOver = true;
	}

	// Check if there is no food on the board
	needFood = true;
	for (int i = 0; i < 9; i++) {
	  for (int j = 0; j < 12; j++) {
	    if (board[i][j] == 2) {
	      needFood = false;
	    }
	  }
	}

	// Assign the food to a random location if needed
	if (needFood) {
	  std::cout << "\n\nFOOD NEEDED/ASSIGNED\n\n\n";
	  assignFood();
	  needFood = false;
	  print2DArray(board);
	}
	
	// game over
      } else {
	std:: cout << "GAME OVER!" << std::endl;
	return 0;
      }

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

      if ((Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::A)) &&
      previousDirection != RIGHT) {
	snakeDirection = LEFT;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Right) ||
      Keyboard::isKeyPressed(Keyboard::D)) &&
      previousDirection != LEFT) {
	snakeDirection = RIGHT;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Up) ||
      Keyboard::isKeyPressed(Keyboard::W)) &&
      previousDirection != DOWN) {
	snakeDirection = UP;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Down) ||
      Keyboard::isKeyPressed(Keyboard::S)) &&
      previousDirection != UP) {
	snakeDirection = DOWN;
      }
    
    } // while pollEvent()
      
  } // while window.isOpen()
    
  return 0;
 
}
