#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <conio.h>
#include <time.h>
#include "univf.h"


// Misc Consts
#define DELAY 1000/10 // ms | 1sec/fps
#define VECZERO {0, 0}
#define FOOD_INDENT 1

// Graphics constants
#define TITLE "Snake Game"
#define SYMB_WALL '#'
#define SYMB_SPACE ' '
#define SNAKE_HEAD '0'
#define SNAKE_BODY 'X'
#define FOOD '*'

// Game constants
#define F_WIDTH (60 + 1) // 1 for NULL
#define F_HEIGHT 30

#define START_POS {F_WIDTH / 2, F_HEIGHT / 2, NULL}

// Enums
enum movement {UP, DOWN, LEFT, RIGHT, IDLE};

// Custom Types
typedef char game_field;
typedef struct vector {int x, y;} vector;
typedef struct llist_snake {int x, y; int *next;} llist_snake;

// Global variables
char GAME_OVER[F_HEIGHT][F_WIDTH];

// FUNCTIONS
static void fill_field(game_field (*f)[F_WIDTH]) {
  char row[2][F_WIDTH]; // row[0] - top and bottom, row[1] - left and right

  // Makes the row template
  for (int i = 0; i < (F_WIDTH - 1); i++) {
    if (i == 0 || i == (F_WIDTH - 2)) {
      row[1][i] = SYMB_WALL;
    } else {
      row[1][i] = SYMB_SPACE;
    }
    row[0][i] = SYMB_WALL; 
  }  
  row[0][F_WIDTH - 1] = '\0';
  row[1][F_WIDTH - 1] = '\0';
  
  // Fill the game field
  for (int i = 0; i < F_HEIGHT; i++) {
    if (i == 0 || i == (F_HEIGHT - 1)) {
      strcpy(f[i], row[0]);
    } else {
      strcpy(f[i], row[1]);
    }
  }  
}

static void fill_gameover(game_field (*f)[F_WIDTH]) {
  const char SCORE[3][17] = {
    "   GAME OVER!   ",
    "                ",
    " score:         "
  };
  char wall_row[F_WIDTH];

  for (int i = 0; i < (F_WIDTH - 1); i++) {
    wall_row[i] = SYMB_WALL;
  }  
  for (int i = 0; i < F_HEIGHT; i++) {
    strcpy(f[i], wall_row);
  }  

  int first_line = (F_HEIGHT / 2) - 2;
  for (int fi = (F_WIDTH / 2) - ((17 - 1) / 2), i = 0; i < (17 - 1); fi++, i++) {
    f[first_line][fi] = SCORE[1][i];
    f[first_line+1][fi] = SCORE[0][i];
    f[first_line+2][fi] = SCORE[1][i];
    f[first_line+4][fi] = SCORE[2][i];
  }  
}

static void place_snake(game_field (*f)[F_WIDTH], llist_snake *s) {
  // Place the head
  f[s->y][s->x] = SNAKE_HEAD;

  // Place the snake

}

static void move_snake(llist_snake *v, int direction) {
  if ((v->x) < (F_WIDTH - 1) &&
      (v->x) > 0 &&
      (v->y) < (F_HEIGHT) &&
      (v->y) > 0
      ) {
    switch (direction) {
      case 0: // UP
        v->y--;
        break;
      case 1: // DOWN
        v->y++;
        break;
      case 2: // LEFT
        v->x--;
        break;
      case 3: // RIGHT
        v->x++;
        break;
    }
  }
}

//  NOTE(Pavel): make safe-check to prevent food spawning on the snake
static void place_food(bool *consumed, vector *f_pos, game_field (*f)[F_WIDTH], llist_snake *s) {
  if (*consumed) { // set new position
    vector pos = {-1, -1};
    
    // Randomize X and Y positions
    do { // X
     pos.x = rand();
    } while (pos.x > (F_WIDTH - 2 - FOOD_INDENT) || pos.x < (1 + FOOD_INDENT));
    do { // Y
      pos.y = rand();
    } while (pos.y > (F_HEIGHT - 1 - FOOD_INDENT) || pos.y < (1 + FOOD_INDENT));

    f_pos->x = pos.x;
    f_pos->y = pos.y;

    *consumed = false;
  }
  
  f[f_pos->y][f_pos->x] = FOOD;
}

static void draw_game(game_field (*f)[F_WIDTH]) {
  clear_screen(true);
  //Printing Title
  puts("\n" TITLE "\n");

  for (int i = 0; i < F_HEIGHT; i++) {
    puts(f[i]);
  }  
}

static void check_input(enum movement *move_dir) {
  if (GetAsyncKeyState('W') & 0x8000) {
    *move_dir = UP;
  }
  else if (GetAsyncKeyState('S') & 0x8000) {
    *move_dir = DOWN;
  }
  else if (GetAsyncKeyState('A') & 0x8000) {
    *move_dir = LEFT;
  }
  else if (GetAsyncKeyState('D') & 0x8000) {
    *move_dir = RIGHT;
  }
}

// MAIN LOOP OF THE SNAKE GAME
int game_loop() {
  enum movement move_direction = IDLE;
  llist_snake snake = START_POS;
  char input;
  vector food_pos = {-1, -1};
  bool food_consumed = true;

  srand(time(0));

  // Building visuals
  game_field field[F_HEIGHT][F_WIDTH];
  game_field game_over[F_HEIGHT][F_WIDTH];

  // Game Loop
  while (true) {

    fill_field(field);
    fill_gameover(game_over);
    move_snake(&snake, move_direction);
    place_snake(field, &snake);
    place_food(&food_consumed, &food_pos, field, &snake);
    check_input(&move_direction);
    draw_game(field);

    //Game over condition
    //Checks if the snake has hit a wall
    if (snake.x == 0 || 
        snake.y == 0 || 
        snake.x == (F_WIDTH - 2) ||
        snake.y == (F_HEIGHT - 1)
        ) {
      break;
    }

    Sleep(DELAY);
  }

  draw_game(game_over);
  puts("\n" "Press any key to escape...");
  getch();
  
  return 0;
}
