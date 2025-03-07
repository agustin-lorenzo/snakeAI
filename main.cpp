#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
bool aiMode = false;
int headRow = 0;
int headCol = 0;
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

// Helper function for checking if a row and column are out of bounds.
bool inBounds(int row, int col) {
  if (row >= 9 || row < 0 || col >= 12 || col < 0) {
    return false;
  } else return true;
}

// Helper function for calculating the distance between two points on the board.
double getDistance(int startRow, int startCol, int endRow, int endCol) {
  // account for out of bounds and snake-body options
  if (!inBounds(startRow, startCol) || board[startRow][startCol] == 1) {
    return 1000;
  }
  int a = endRow - startRow;
  int b = endCol - startCol;
  return std::sqrt(a * a + b * b);
} // getDistance

// Helper function for checking both bounds and part of snake
bool validMove(int row, int col) {
  if (!inBounds(row, col) || board[row][col] == 1) return false;
  return true;
}



int main(int argc, char*argv[]) {
  // check for "AI mode"
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--ai") == 0) {
      aiMode = true;
    }
  }

  SoundBuffer buffer;
  if (!buffer.loadFromFile("food.wav")) return 1;
  Sound sound;
  sound.setBuffer(buffer);
  RenderWindow window(VideoMode(600, 455), "Snake");
  Clock clock;
  Time deltaTime = seconds(1.0f / 10.0f);
  bool needFood = false;

  
  while(window.isOpen()) {
    
    Time elapsed = clock.getElapsedTime();
    while (elapsed >= deltaTime) {
      
      if (!gameOver) {
	std::cout << "PREVIOUS HEAD\n";
	std::cout << "headRow = " << headRow << std::endl;
	std::cout << "headCol = " << headCol << std::endl;
      
	// Update board array based on direction
	if (snakeDirection == UP) {
	  headRow--;
	} else if (snakeDirection == DOWN) {
	  headRow++;
	} else if (snakeDirection == LEFT) {
	  headCol--;
	} else if (snakeDirection == RIGHT) {
	  headCol++;
	}
	std::cout << "NEXT HEAD\n";
	std::cout << "headRow = " << headRow << std::endl;
	std::cout << "headCol = " << headCol << std::endl;

	// Check for out of bounds
	if (!inBounds(headRow, headCol)) {
	  std::cout << "OUT OF BOUNDS" << std::endl;
	  std:: cout << "GAME OVER!" << std::endl;
	  return 0;
	}

	// Evaluate based on new snake head location
	if (board[headRow][headCol] == 1) gameOver = true;
	previousDirection = snakeDirection;
	snakeHistory.push_back(std::make_pair(headRow, headCol));
	if (board[headRow][headCol] == 2) {
	  sound.play();
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
      previousDirection != RIGHT && !aiMode) {
	snakeDirection = LEFT;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Right) ||
      Keyboard::isKeyPressed(Keyboard::D)) &&
      previousDirection != LEFT && !aiMode) {
	snakeDirection = RIGHT;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Up) ||
      Keyboard::isKeyPressed(Keyboard::W)) &&
      previousDirection != DOWN && !aiMode) {
	snakeDirection = UP;
      }

      if ((Keyboard::isKeyPressed(Keyboard::Down) ||
      Keyboard::isKeyPressed(Keyboard::S)) &&
      previousDirection != UP && !aiMode) {
	snakeDirection = DOWN;
      }
    
    } // while pollEvent()

    // A* pathfinding algorithm
    if (aiMode) {
      int headUpRow = headRow - 1;
      int headUpCol = headCol;

      int headDownRow = headRow + 1;
      int headDownCol = headCol;

      int headLeftRow = headRow;
      int headLeftCol = headCol - 1;

      int headRightRow = headRow;
      int headRightCol = headCol + 1;

      // int headUpRow = headCol--; //TODO: check if there was any reason for old variables being wrong
      // int headUpCol = headRow;

      // int headDownRow = headCol++;
      // int headDownCol = headRow;

      // int headLeftRow = headCol;
      // int headLeftCol = headRow--;

      // int headRightRow = headCol;
      // int headRightCol = headRow++;

      int foodRow = -1; // get location of food
      int foodCol = -1;
      for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 12; j++) {
	  if (board[i][j] == 2) {
	    foodRow = i;
	    foodCol = j;
	    break;
	  }
	}
      }
      
      // double distFromUp = getDistance(headUpRow, headUpCol, foodRow, foodCol);
      // double distFromDown = getDistance(headDownRow, headDownCol, foodRow, foodCol);
      // double distFromLeft = getDistance(headLeftRow, headLeftCol, foodRow, foodCol);
      // double distFromRight = getDistance(headRightRow, headRightCol, foodRow, foodCol);


      snakeDirection = previousDirection;

      double distFromUp = getDistance(headUpRow, headUpCol, foodRow, foodCol);
      std::cout << "distance from up = " << distFromUp << std::endl;
      if (validMove(headUpRow, headUpCol)) {
	snakeDirection = UP;
      }
      double distFromDown = getDistance(headDownRow, headDownCol, foodRow, foodCol);
      if ((distFromDown < distFromUp) && validMove(headDownRow, headDownCol)) {
	snakeDirection = DOWN;
      }
      std::cout << "distance from down = " << distFromDown << std::endl;
      double distFromLeft = getDistance(headLeftRow, headLeftCol, foodRow, foodCol);
      if ((distFromLeft < distFromDown) && (distFromLeft < distFromUp) && validMove(headLeftRow, headLeftCol)) {
	snakeDirection = LEFT;
      }
      std::cout << "distance from left = " << distFromLeft << std::endl;
      double distFromRight = getDistance(headRightRow, headRightCol, foodRow, foodCol);
      if ((distFromRight < distFromLeft) && (distFromRight < distFromDown) && (distFromRight < distFromUp) && validMove(headRightRow, headRightCol)) {
	snakeDirection = RIGHT;
      }
      std::cout << "distance from right = " << distFromRight << std::endl;
      std::cout << "SNAKE DIRECTION = " << snakeDirection << std::endl;
       
    } // aiMode
      
  } // while window.isOpen()
    
  return 0;
 
}
