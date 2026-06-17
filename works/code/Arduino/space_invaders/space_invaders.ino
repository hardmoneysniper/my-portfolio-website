#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 13;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();
 
class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg; 
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg; 
      y = y_arg; 
      strength = strength_arg; 
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg; 
      y = y_arg; 
      strength = strength_arg; 
    }
    
    // getters
    int get_x() const {
      return x; 
    }
    int get_y() const {
      return y; 
    }
    int get_strength() const {
      return strength; 
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
    
          y = y + 1;
      
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength > 0) { 
        if (strength == 1) {
        draw_with_rgb(RED,BLUE);
        }
        if (strength == 2) {
        draw_with_rgb(ORANGE,BLUE);
        }
        if (strength == 3) {
        draw_with_rgb(YELLOW,BLUE);
        }
        if (strength == 4) {
        draw_with_rgb(GREEN,BLUE);
        }
        if (strength == 5) {
        draw_with_rgb(BLUE,BLUE);
        }
        if (strength == 6) {
        draw_with_rgb(PURPLE,BLUE);
        }
        if (strength == 7) {
        draw_with_rgb(WHITE,BLUE);
        }
      }
      else {
        return;
      }
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK); 
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      if (strength > 1) {
        erase();
        strength--;
        draw();
      }

      else {
        erase();
        strength--;
      }
    }

  private:
    int x;
    int y;
    int strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      
      
       matrix.drawPixel(x + 1, y, body_color.to_333());
       matrix.drawPixel(x + 2, y, body_color.to_333());
       

       matrix.drawPixel(x, y + 1, body_color.to_333());

       //eyes
       matrix.drawPixel(x + 1, y + 1, eye_color.to_333());
       matrix.drawPixel(x + 2, y + 1, eye_color.to_333());

       matrix.drawPixel(x + 3, y + 1, body_color.to_333());

       matrix.drawPixel(x , y + 2, body_color.to_333());
       matrix.drawPixel(x + 1, y + 2, body_color.to_333());
       matrix.drawPixel(x + 2, y + 2, body_color.to_333());
       matrix.drawPixel(x + 3, y + 2, body_color.to_333());

       matrix.drawPixel(x, y + 3, body_color.to_333());
       matrix.drawPixel(x + 3, y + 3, body_color.to_333());
      

     
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0; 
      y = 0; 
      fired = false; 
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired; 
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg; 
      fired = true;
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      if ((get_y() - 1) >= 0) {
        y--;
      }
      else {
        erase();
        reset();
      }
    }
    
    // resets private data members to initial values
    void hit() {
      reset();
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      if (fired) {
        matrix.drawPixel(get_x(), get_y(), ORANGE.to_333());
        matrix.drawPixel(get_x(), get_y() + 1, ORANGE.to_333());
      }
    }
    
    // draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(get_x(), get_y(), BLACK.to_333());
      matrix.drawPixel(get_x(), get_y() + 1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 0;
      y = 14;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives; 
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      lives--; 
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      
      draw_with_rgb(BLACK); 
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg; 
      y = y_arg; 
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
      
      matrix.drawPixel(x + 1, y, color.to_333());
      matrix.drawPixel(x + 1, y + 1, color.to_333());
      matrix.drawPixel(x, y + 1, color.to_333());
      matrix.drawPixel(x + 2, y + 1, color.to_333());

    }
};

class Boss {
  public:

    Boss() {}

    void initialize(int left_in, int right_in, int top_in, int bottom_in, int strength_in) {
      left = left_in;
      right = right_in;
      top = top_in;
      bottom = bottom_in;
      strength = strength_in;
    }

    int get_left() {return left;}

    int get_right() {return right;}

    int get_top() {return top;}

    int get_bottom() {return bottom;}

    int get_strength() {return strength;}

    void move_left() {
      if (left != 0) {
        left -= 1;
        right -= 1;
        top += 1;
        bottom += 1;
      }

      else {
        go_left = false;
      }       
    }

    void move_right() {
      if (right != 32) {
        left += 1;
        right += 1;
        top += 1;
        bottom += 1;
      }

      else {
        go_left = true;
      }
    }

    void move() {
      if (go_left) {
        move_left();
      }

      else {
        move_right();
      }
    }

    void erase() {
      draw_with_rgb(BLACK, BLACK); 
    }

    void hit() {
      if (strength > 1) {
        erase();
        strength--;
        draw();
      }
      else {
        erase();
        strength--;
      }
    }

    void draw() {
      if (strength == 0) {
        erase();
        return;
      }

      else if (strength <= 20 && strength > 10) {
        draw_with_rgb(RED,BLUE);
      }
      
      else if (strength <= 30 && strength > 20) {
        draw_with_rgb(ORANGE,BLUE);
      }
          
      else if (strength <= 40 && strength > 30) {
        draw_with_rgb(YELLOW,BLUE);
      }
          
      else if (strength <= 50 && strength > 40) {
        draw_with_rgb(GREEN,BLUE);
      }
          
      else if (strength <= 60 && strength > 50) {
        draw_with_rgb(BLUE,BLUE);
      }
          
      else if (strength <= 70 && strength > 60) {
        draw_with_rgb(PURPLE,BLUE);
      }
          
      else {
        draw_with_rgb(GREEN, PURPLE);
      }

    }

    private:
      int left = 0;
      int right = 6;
      int top = 0;
      int bottom = 6;
      int strength = 60;
      bool go_left = false;

      // draws the Boss
      void draw_with_rgb(Color body_color, Color eye_color) {
        
        
        matrix.drawPixel(left + 2, top, body_color.to_333());
        matrix.drawPixel(left + 3, top, body_color.to_333());
        matrix.drawPixel(left + 4, top, body_color.to_333());
            

        matrix.drawPixel(left + 1, top + 1, body_color.to_333());
        matrix.drawPixel(left + 3, top + 1, body_color.to_333());
        matrix.drawPixel(left + 5, top + 1, body_color.to_333());

        matrix.drawPixel(left, top + 2, body_color.to_333());
        matrix.drawPixel(left + 1, top + 2, body_color.to_333());
        matrix.drawPixel(left + 3, top + 2, body_color.to_333());
        matrix.drawPixel(left + 5, top + 2, body_color.to_333());
        matrix.drawPixel(left + 6, top + 2, body_color.to_333());

        matrix.drawPixel(left + 1, top + 3, body_color.to_333());
        matrix.drawPixel(left + 2, top + 3, body_color.to_333());
        matrix.drawPixel(left + 4, top + 3, body_color.to_333());
        matrix.drawPixel(left + 5, top + 3, body_color.to_333());

        matrix.drawPixel(left + 2, top + 4, body_color.to_333());
        matrix.drawPixel(left + 3, top + 4, body_color.to_333());
        matrix.drawPixel(left + 4, top + 4, body_color.to_333());

        matrix.drawPixel(left + 1, top + 5, body_color.to_333());
        matrix.drawPixel(left + 2, top + 5, body_color.to_333());
        matrix.drawPixel(left + 4, top + 5, body_color.to_333());
        matrix.drawPixel(left + 5, top + 5, body_color.to_333());

        matrix.drawPixel(left, top + 6, body_color.to_333());
        matrix.drawPixel(left + 2, top + 6, body_color.to_333());
        matrix.drawPixel(left + 4, top + 6, body_color.to_333());
        matrix.drawPixel(left + 6, top + 6, body_color.to_333());

        // eyes
        matrix.drawPixel(left + 2, top + 1, eye_color.to_333());
        matrix.drawPixel(left + 4, top + 1, eye_color.to_333());
        matrix.drawPixel(left + 2, top + 2, eye_color.to_333());
        matrix.drawPixel(left + 4, top + 2, eye_color.to_333());
        matrix.drawPixel(left + 3, top + 3, eye_color.to_333());

      }
};





class Game {
  public:
    Game() {
      level = 1;
      time = 0;
      time2 = 0;
      time3 = 0;
    }

    int temp1 = 0;
    int temp2 = 0;
       
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {

      if (player.get_lives() == 0) {
        // game over!
        matrix.fillScreen(BLACK.to_333());
        delay(1000);
        game_over();
        delay(5000);
        matrix.fillScreen(BLACK.to_333());

        while (10 > 9) {
          delay(1000);
        }
        
      }

      // resets level
      matrix.fillScreen(BLACK.to_333());
      delay(1000);
      print_level(level);
      delay(2000);
      matrix.fillScreen(BLACK.to_333());
      temp1 = 0;
      temp2 = 0;
      // prints lives when life >= 1
      print_lives(player.get_lives());
      delay(2000);
      matrix.fillScreen(BLACK.to_333());
      delay(1000);
      ball.reset();
      reset_level();
      player.set_x(-1);
      delay(1000);
      player.draw();
      
    }



    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
    

      // create constant to determing the moving speed of ball and invader

      unsigned long currentTime = millis();
      unsigned long currentTime2 = millis();
      unsigned long currentTime3 = millis();
      const unsigned long moveInterval = 2000;
      const unsigned long cannonInterval = 50;
      
      
      // change amount of invaders based on levels
      int max_index = 0;
      if (level <= 1) {
        max_index = 8;
      }

      else {
        max_index = 16;
      }

      if (/* menu is selected to hand-operated mode*/) {
        // player move
        
        int pos1 = player.get_x() - 1;
        int pos = potentiometer_value / 32 - 1;
        player.set_x(pos);
        int pos2 = player.get_x() - 1;
        player.draw();

        if (pos2 > pos1) {
          player.set_x(pos - 1);
          player.erase(); 

          player.set_x(pos);
          player.draw(); 
        }

        if (pos2 < pos1) {
          player.set_x(pos + 1);
          player.erase(); 

          player.set_x(pos);
          player.draw(); 
        }


        // move the invader

        int firstRowDead = 0;

        if (level == 1) { 
          if (currentTime - time >= moveInterval) {
            for (int i = 0; i < max_index; i++) {
              enemies[i].erase();
              enemies[i].move();
              enemies[i].draw();
            
            }

            for (int i = 0; i < max_index; i++) {
              if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }    

              else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
              } 

              else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();

              }

              else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }

              else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }

              else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die();
                setupGame();
                
              }

              else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
              } 

              else if (enemies[i].get_y() + 3 > 16) {
                  player.die(); 
                  setupGame();
              
              }

              else {
                continue;
              }

            }
            time = currentTime;
          }

        }

        // level > 1

        else { 

          if (level % 5 == 0) {
            if (currentTime - time >= moveInterval) {
              boss.erase();
              boss.move();
              boss.draw();

              if (player.get_x() <= boss.get_right() && player.get_x() >= boss.get_left() 
                  && player.get_y() == boss.get_bottom()) {
                  player.die(); 
                  setupGame();
              }
              else if (boss.get_bottom() == 15) {
                player.die(); 
                setupGame();
              }
              time = currentTime;
            }
          }
          else {
            for (int i = 8; i < max_index; i++) { 
              if (enemies[i].get_strength() == 0) { 
                firstRowDead++; 
              }
            }

            if (currentTime - time >= moveInterval) {
              if (firstRowDead < 8) {
                for (int i = 8; i < max_index; i++) {
                  enemies[i].erase();
                  enemies[i].move();
                  enemies[i].draw();
                
                }

                for (int i = 0; i < max_index; i++) {
                
                  if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }    

                  else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  } 

                  else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (enemies[i].get_y() + 3 > 15) {
                    player.die(); 
                    setupGame();
                
                  }

                  else {
                    continue;
                  }
                }
              }

              else {
                for (int i = 0; i < 8; i++) {
                  enemies[i].erase();
                  enemies[i].move();
                  enemies[i].draw();
                }

                for (int i = 0; i < max_index; i++) {
                
                  if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }    

                  else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  } 

                  else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (enemies[i].get_y() + 3 > 15) {
                    player.die(); 
                    setupGame();
                
                  }

                  else {
                    continue;
                  }
                }
              }
              
              time = currentTime;

            }
          }
        }

        // move the ball

        if (button_pressed && !ball.has_been_fired()) {
          ball.fire(player.get_x() + 1, player.get_y() - 2);
          ball.draw();

        }
        
        if (currentTime2 - time2 >= cannonInterval) {
          //if(ballMoveDis == 13){
            //ball.erase();
            //ball.hit();
          //}
          
          ball.erase();
          ball.move();
          ball.draw();
          //ballMoveDis++;

          if (level % 5 == 0) {
            if (ball.get_x() <= boss.get_right() && ball.get_x() >= boss.get_left() 
                  && ball.get_y() == boss.get_bottom()) {
                Ball.erase();
                ball.hit();
                boss.hit();  
            }
          }
          else {        
            for (int i = 0; i < max_index; i++) {
              if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0 ){
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                ball.erase();
                ball.hit();
                enemies[i].hit();
              }

              else {
                continue;
              }
            }
          }
          time2 = currentTime2;
        }


      }



      // ---------------------Divides human-controlled with ai-controlled------------------------------


      else {

        // enemies move and collision between player and enemy

        int firstRowDead = 0;

        if (level == 1) { 
          if (currentTime - time >= moveInterval) {
            for (int i = 0; i < max_index; i++) {
              enemies[i].erase();
              enemies[i].move();
              enemies[i].draw();
            
            }

            for (int i = 0; i < max_index; i++) {
              if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }    

              else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
              } 

              else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();

              }

              else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }

              else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
                
              }

              else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                player.die();
                setupGame();
                
              }

              else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                player.die(); 
                setupGame();
              } 

              else if (enemies[i].get_y() + 3 > 15) {
                  player.die(); 
                  setupGame();
              
              }

              else {
                continue;
              }

            }
            time = currentTime;
          }

        }

        // level > 1

        else {

          if (level % 5 == 0) { 
            if (currentTime - time >= moveInterval) {
              boss.erase();
              boss.move();
              boss.draw();

              if (player.get_x() <= boss.get_right() && player.get_x() >= boss.get_left() 
                  && player.get_y() == boss.get_bottom()) {
                player.die(); 
                setupGame();
              }

              else if (boss.get_bottom() == 15) {
                player.die(); 
                setupGame();
              }
              time = currentTime;
            }
          }

          else {
            for (int i = 8; i < max_index; i++) { 
              if (enemies[i].get_strength() == 0) { 
                firstRowDead++; 
              }
            }

            if (currentTime - time >= moveInterval) {
              if (firstRowDead < 8) {
                for (int i = 8; i < max_index; i++) {
                  enemies[i].erase();
                  enemies[i].move();
                  enemies[i].draw();
                
                }

                for (int i = 0; i < max_index; i++) {
                
                  if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }    

                  else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  } 

                  else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                    
                  }

                  else if (enemies[i].get_y() + 3 > 15) {
                    player.die(); 
                    setupGame();
                
                  }

                  else {
                    continue;
                  }
                }
              }

              else {
                for (int i = 0; i < 8; i++) {
                  enemies[i].erase();
                  enemies[i].move();
                  enemies[i].draw();
                }

                for (int i = 0; i < max_index; i++) {
                
                  if (player.get_x() + 1 == enemies[i].get_x() && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }    

                  else if (player.get_x() + 1 == enemies[i].get_x() + 3 && player.get_y() == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  } 

                  else if (player.get_x() == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (player.get_x() + 2 == enemies[i].get_x() + 3 && player.get_y() + 1 == enemies[i].get_y() + 3) {
                    player.die(); 
                    setupGame();
                  }

                  else if (enemies[i].get_y() + 3 > 15) {
                    player.die(); 
                    setupGame();
                
                  }

                  else {
                    continue;
                  }
                }
              }
              
              time = currentTime;

            }
          }
        }





        // AI and ball
        bool isEnemyAbove1 = false;
        bool isEnemyAbove2 = false;
        bool isBossAbove = false;

        if (level % 5 == 0) {
          if (boss.get_left() <= player.get_x() + 1 && boss.get_right() >= player.get_x() + 1) {
            isBossAbove = true;
          }

          if (isBossAbove) {
            if (!ball.has_been_fired()) {
                ball.fire(player.get_x() + 1, player.get_y() - 2);
                ball.draw();
            }

            if (currentTime3 - time3 >= cannonInterval) {
              
              if (ball.get_x() <= boss.get_right() && ball.get_x() >= boss.get_left() 
                && (ball.get_y() == boss.get_bottom() || ball.get_y() <= boss.get_top())) {
                ball.erase();
                ball.hit();
                boss.hit();  
              }

              ball.erase();
              ball.move();
              ball.draw();

              time3 = currentTime3;
            }

            
          }

          else {
              player.erase();
              player.set_x(player.get_x() + 7);
              player.draw();
            } 

        }

        else {
          if (max_index == 8) {
            for (int k = 0; k < max_index; k++) {
              if (enemies[k].get_x() == player.get_x() + 1 && enemies[k].get_strength() != 0) {
                isEnemyAbove2 = true;
              }
            }
            
            if (isEnemyAbove2) {

              if (!ball.has_been_fired()) {
                ball.fire(player.get_x() + 1, player.get_y() - 2);
                ball.draw();
              }

              if (currentTime2 - time2 >= cannonInterval) {
                ball.erase();
                ball.move();
                ball.draw();

                for (int i = 0; i < max_index; i++) {
                  if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0){
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else {
                    continue;
                  }
                }              

                time2 = currentTime2;
              }

            }

            else {
              player.erase();
              player.set_x(player.get_x() + 1);
              player.draw();
            } 
          }


          if (max_index == 16) {
            for (int k = 0; k < 8; k++) {
              if (enemies[k].get_x() == player.get_x() + 1 && enemies[k].get_strength() != 0) {
                isEnemyAbove1 = true;
              }
            }

            for (int m = 8; m < 16; m++) {
              if (enemies[m].get_x() == player.get_x() + 1 && enemies[m].get_strength() != 0) {
                isEnemyAbove2 = true;
              }
            }



            if ((!isEnemyAbove2 && isEnemyAbove1) && player.get_x() < 24 && temp1 <= temp2) {
              player.erase();
              player.set_x(player.get_x() + 4);
              player.draw();
            } 

            else if ((!isEnemyAbove2 && isEnemyAbove1) && player.get_x() >= 28) {
              player.erase();
              temp1 = player.get_x();
              player.set_x(player.get_x() - 4);
              temp2 = player.get_x();
              player.draw();
            }

            else if ((!isEnemyAbove2 && !isEnemyAbove1)) {
              player.erase();
              temp1 = player.get_x();
              player.set_x(player.get_x() - 4);
              temp2 = player.get_x();
              player.draw();
            } 


            else {
              if (!ball.has_been_fired()) {
                ball.fire(player.get_x() + 1, player.get_y() - 2);
                ball.draw();
              }

              if (currentTime2 - time2 >= cannonInterval) {
                ball.erase();
                ball.move();
                ball.draw();

                for (int i = 0; i < 16; i++) {
                  if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0){
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 3 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 2 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 1 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 2 && ball.get_y() == enemies[i].get_y() && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else if (ball.get_x() == enemies[i].get_x() + 3 && ball.get_y() == enemies[i].get_y() + 1 && enemies[i].get_strength() > 0) {
                    ball.erase();
                    ball.hit();
                    enemies[i].hit();
                  }

                  else {
                    continue;
                  }
                }              

                time2 = currentTime2;
              }
            }
          }
        }
      }



      // If all enemies is cleared, clear the screen and move to the next level
      bool ifAllDestory = level_cleared();
      if (ifAllDestory) {
        level++;
        setupGame();
      }
}
    
    




  private:
    int level;
    //int ballMoveDis = 0;
    unsigned long time = 0;
    unsigned long time2 = 0;
    unsigned long time3 = 0;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];
    Boss boss;

    
      // check if Player defeated all Invaders in current level
    bool level_cleared() {
      // 8 enemies in level 1
      if (level == 1) {
        for (int i = 0; i < 8; ++i) {
          if (enemies[i].get_strength() != 0) {
            return false;
          }
        }
      }

      else if (level % 5 == 0) {
        if (boss.get_strength() != 0) {
          return false;
        }
      }

      else {        
        // 16 enemies in all other levels
        for (int i = 0; i < NUM_ENEMIES; ++i) {
          if(enemies[i].get_strength() != 0) {
            return false;
          }
        }
      }
      return true;
      
    }


    // set up a level
    void reset_level() {
      if (level == 1) {
        int a1 = 0;
        for (int i = 0; i < 8; i++) {
          enemies[i].initialize(a1, 0, 1);
          a1 = a1 + 4;
          enemies[i].draw();
        }
      }
      
      else if(level == 2) {
        int horizontal = 0;
        int vertical = 0;
          for (int i = 0; i < NUM_ENEMIES; i++) {
            if (i == 8) {
              horizontal = 0;
              vertical += 4;
            }
            if (i % 2 == 0) {
              enemies[i].initialize(horizontal, vertical, 1);
            }
            else {
              enemies[i].initialize(horizontal, vertical, 2);
            }
            horizontal += 4;
            enemies[i].draw();
          }
      }

      else if (level == 3) {
        int horizontal = 0;
        int vertical = 0;
          for (int i = 0; i < NUM_ENEMIES; i++) {
            if (i == 8) {
              horizontal = 0;
              vertical += 4;
            }
            int st = (i + 1) % 5;
            if (st == 0) {
              st = 5;
            }
            enemies[i].initialize(horizontal, vertical, st);
            horizontal += 4;
            enemies[i].draw();
          }
      }

      else if (level == 4) {
        int horizontal = 0;
        int vertical = 0;
          for (int i = 0; i < NUM_ENEMIES; i++) {
            if (i == 8) {
              horizontal = 0;
              vertical += 4;
            }
            if (i % 2 == 0 && i < 8) {
              enemies[i].initialize(horizontal, vertical, 5);
            }
            else if (i % 2 == 1 && i < 8) {
              enemies[i].initialize(horizontal, vertical, 4);
            }
            else if (i % 2 == 0 && i >= 8) {
              enemies[i].initialize(horizontal, vertical, 2);
            }
            else if (i % 2 == 1 && i >= 8) {
              enemies[i].initialize(horizontal, vertical, 3);
            }
            horizontal += 4;
            enemies[i].draw();
          }
      }

      else if (level % 5 == 0) {
        int strength = (level - 5) * 2 + 60;
        boss.initialize(0, 6, 0, 6, strength);
      }

      else {
        int horizontal = 0;
        int vertical = 0;
          for (int i = 0; i < NUM_ENEMIES; i++) {
            if (i == 8) {
              horizontal = 0;
              vertical += 4;
            }
            enemies[i].initialize(horizontal, vertical, random(1, 7));
            horizontal += 4;
            enemies[i].draw();
          }
      }
  }
    

};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  
  //exp
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.update(potentiometer_value, button_pressed);
  delay(2);
}

// displays Level
void print_level(int level) {
  matrix.setCursor(2, 0);
  matrix.setTextSize(1);
  matrix.print("Level: "); 
  matrix.setCursor(6, 8);
  matrix.print(level); 

}

// displays number of lives
void print_lives(int lives) {
  matrix.setCursor(2, 0);
  matrix.setTextSize(1);
  matrix.print("Lives: "); 
  matrix.setCursor(6, 8);
  matrix.print(lives); 

}

// displays "game over"
void game_over() {
  matrix.setCursor(2, 0);
  matrix.setTextSize(1);
  matrix.print("Game Over");
}