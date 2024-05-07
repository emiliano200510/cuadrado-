#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

#define ROWS 6
#define COLUMNS (ROWS * 2)

typedef struct
{
  int x;
  int y;
} tCoordinate;

typedef char** tSquare;

typedef struct
{
  tCoordinate *currentState;
  tCoordinate *pastState;
  tSquare square;
} tGame;

tCoordinate *initializeCoordinate();
tSquare createSquare();
void gotoxy(int, int);
void printSquare(tCoordinate*, tSquare);
void destroyCoordinate(tCoordinate*);
void destroySquare(tSquare);
void activate_no_echo_mode();
void deactivate_no_echo_mode();

int main(int argc, char *argv[])
{
  char key = '\n', opt = '\n', c;
  tGame game;
  game.currentState = initializeCoordinate();
  game.pastState = NULL;
  game.square = createSquare();

  if(game.currentState == NULL || game.square == NULL)
    return EXIT_FAILURE;

  system("clear");
  printSquare(game.currentState, game.square);

  while(1)
  {
    activate_no_echo_mode();
    key = getchar();
    deactivate_no_echo_mode();
    opt = tolower(key);

    switch(opt)
    {
      case 'a': game.currentState->x--; break;
      case 'd': game.currentState->x++; break;
      case 'w': game.currentState->y--; break;
      case 's': game.currentState->y++; break;
      default: game.currentState = NULL;
    }

    while((c = getchar()) != '\n' && c != EOF);
    if(game.currentState == NULL)
      break;

    system("clear");
    printSquare(game.currentState, game.square);
  }

  destroyCoordinate(game.currentState);
  destroyCoordinate(game.pastState);
  destroySquare(game.square);

  if(game.currentState == NULL)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

tCoordinate *initializeCoordinate()
{
  tCoordinate *coordinate = (tCoordinate *)malloc(sizeof(tCoordinate));

  coordinate->x = 1;
  coordinate->y = 1;

  return coordinate;
}

tSquare createSquare()
{
  int i, j;
  char **square = (tSquare)malloc(sizeof(char *) * ROWS);

  if(square == NULL)
    return NULL;

  for(i = 0; i < ROWS; i++)
  {
    square[i] = (char *)malloc(sizeof(char) * COLUMNS);

    if(square[i] == NULL)
      return NULL;
  }

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLUMNS; j++)
      ((i == 0 || i == ROWS - 1) || (j == 0 || j == COLUMNS - 1)) ? (square[i][j] = '.') : (square[i][j] = ' ');
  }

  return square;
}

void gotoxy(int x, int y)
{
  printf("%c[%d;%df", 0x1B, x, y);
}

void printSquare(tCoordinate *coordinate, tSquare square)
{
  int i, j;

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLUMNS; j++)
    {
      gotoxy(i + coordinate->y, j + coordinate->x);
      printf("%c", square[i][j]);
    }

    puts("");
  }
}

void destroyCoordinate(tCoordinate *coordinate)
{
  free(coordinate);
}

void destroySquare(tSquare square)
{
  int i;

  for(i = 0; i < ROWS; i++)
    free(square[i]);

  free(square);
}

void activate_no_echo_mode()
{
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void deactivate_no_echo_mode()
{
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= (ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}