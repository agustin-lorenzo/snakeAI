#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

enum Direction {
  UP, DOWN, LEFT, RIGHT
};

int board[][8] = {
  {1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
};

int headx = 0;
int heady = 0;
int snakelength = 1;
Direction snakeDirection = DOWN;
Direction previousDirection = DOWN;

int main() {
  
  RenderWindow window(VideoMode(400, 300), "Snake");
  
  while(window.isOpen()) {
    
    Event event;
    while(window.pollEvent(event)) {
      
      if (event.type == Event::Closed) {
	window.close();
      }

      window.clear();
      
      // Update board based on direction
      if (snakeDirection == previousDirection) {
	// do nothing
      } else if (snakeDirection == UP) {
	heady--;
	board[headx][heady] = 1;
	previousDirection = snakeDirection;
      } else if (snakeDirection == DOWN) {
	heady++;
	board[headx][heady] = 1;
	previousDirection = snakeDirection;
      } else if (snakeDirection = LEFT) {
	headx--;
	board[headx][heady] = 1;
	previousDirection = snakeDirection;
      } else if (snakeDirection = RIGHT) {
	headx++;
	board[headx][heady] = 1;
	previousDirection = snakeDirection;
      }
      
      

      // Draw the parts of the board set to 1
      for (int i = 0; i < 6; i++) {
	for (int j = 0; j < 8; j++) {
	  if (board[i][j] == 1) {
	    RectangleShape block(Vector2f(45.f, 45.f));
	    block.setFillColor(Color(48,94,59));
	    block.setOutlineThickness(5.f);
	    block.setOutlineColor(Color(0,0,0));
	    int x = 50*j;
	    int y = 50*i;
	    block.setPosition(x, y);
	    window.draw(block);   
	  }

	} // for
	
      } // for
      
      window.display();

    }
    
  }
    
  return 0;
}
