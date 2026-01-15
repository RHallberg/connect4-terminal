#include <ctype.h>
#include <stdio.h>
#define TB_IMPL
#include "termbox2.h"

#define BOARD_W 7
#define BOARD_H 6

#define PLAYER1 1
#define PLAYER2 2

#define WIN 1

typedef struct {
    int x;
    int y;
} Point;

int dropDisc(int pos, int player, int (*grid)[6]);

int countDiscs(int (*grid)[6], int x, int y, int player, int dx, int dy);
int checkWin(int (*grid)[6], int x, int y, int player);

int main()
{
  int ret;
  ret = tb_init();
  if (ret) {
    printf("tb_init() failed with error code %d\n", ret);
    return 1;
  }

  const int colors[3] = {TB_WHITE, TB_RED, TB_YELLOW};
  const int textcolors[3] = {0, TB_WHITE, TB_BLACK};
  int player = PLAYER1;
  int gameover = 0;
  int grid [BOARD_W][BOARD_H];
  for (int i = 0; i < BOARD_W; i++) {
    for (int j = 0; j < BOARD_H; j++) {
      grid[i][j] = 0;
    }
  }

  struct tb_event e;
  while(1){
    tb_peek_event(&e, 10);
    if(e.ch == 'q')break;
    if(e.ch == 'r'){
      gameover = 0;
      player = PLAYER1;
      for (int i = 0; i < BOARD_W; i++) {
        for (int j = 0; j < BOARD_H; j++) {
          grid[i][j] = 0;
        }
      }
    }

    if(isdigit(e.ch) && e.ch - '0' <= BOARD_W && !gameover){
      int result = dropDisc(e.ch - '0', player, grid);
      if(result == WIN){
        gameover = 1;
      }
      else {
        player = player == PLAYER1 ? PLAYER2 : PLAYER1;
      }
    }

    Point pos ={0,0};
    tb_clear();

    // Print the grid
    tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|1 2 3 4 5 6 7|");
    pos.y++;
    for(int i = 0; i < BOARD_H; i++){
      tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|             |");
      pos.y++;
    }
    tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|1 2 3 4 5 6 7|");

    // Print the discs
    for (int i = 0; i < BOARD_W; i++) {
      for (int j = 0; j < BOARD_H; j++) {
        int color = colors[grid[i][j]];
        int offset = i + 1;
        tb_print(i+offset,j+1, color, TB_BLUE, "⬤");
      }
    }

    if(gameover){
      char winstring[25];
      sprintf(winstring, "PLAYER %d WINS!!", player);
      tb_print(pos.x, pos.y+1, textcolors[player], colors[player], winstring);
      tb_print(pos.x, pos.y+2, TB_BLACK, TB_DEFAULT, "Press 'r' to reset");
    }

    tb_present();
  }

  tb_shutdown();
  return 0;
}

int dropDisc(int pos, int player, int (*grid)[BOARD_H]){
  pos--;
  Point location = {pos,0};
  while(grid[location.x][location.y] == 0 && location.x < BOARD_W && location.x >= 0 && location.y < BOARD_H){
    location.y++;
  }
  location.y--;
  if(location.y < 0) return 0;
  grid[location.x][location.y] = player;
  // return 0;
  return checkWin(grid, location.x, location.y, player);
}

int countDiscs(int (*grid)[6], int x, int y, int player, int dx, int dy){

  int nbr = 0;
  while(x >= 0 && y >= 0 && x < BOARD_W && y < BOARD_H && grid[x][y] == player){
    nbr++;
    x+= dx; y+= dy;
  }
  return nbr;
}

int checkWin(int (*grid)[6], int x, int y, int player){

  // Vertical
  if((countDiscs(grid, x, y+1, player, 0, 1) + 1) == 4) 
    return 1;

  // Horizontal
  if((countDiscs(grid, x+1, y, player, 1, 0) + 1 + countDiscs(grid, x-1, y, player, -1, 0) ) == 4)
    return 1;

  // Diagonal right
  if((countDiscs(grid,x+1,y-1, player, 1, -1) + 1 + countDiscs(grid, x-1, y+1, player, -1, 1)) == 4)
    return 1;

  // Diagonal left
  if((countDiscs(grid,x-1,y-1, player, -1, -1) + 1 + countDiscs(grid, x+1, y+1, player, 1, 1)) == 4)
    return 1;

  return 0;
}
