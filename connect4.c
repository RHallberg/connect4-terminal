#include <ctype.h>
#include <stdio.h>
#define TB_IMPL
#include "termbox2.h"


typedef struct {
    int x;
    int y;
} Point;

void dropDisc(int pos, int player, int (*grid)[6]);

int main()
{
  int ret;
  ret = tb_init();
  if (ret) {
    printf("tb_init() failed with error code %d\n", ret);
    return 1;
  }

  const int colors[3] = {TB_WHITE, TB_RED, TB_YELLOW};
  int player = 1;
  int grid [7][6];
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 6; j++) {
      grid[i][j] = 0;
    }
  }

  struct tb_event e;
  while(1){
    tb_peek_event(&e, 10);
    if(e.ch == 'q')break;
    if(e.ch == 'r'){
      for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
          grid[i][j] = 0;
        }
      }
    }

    if(isdigit(e.ch) && e.ch - '0' <= 7){
      dropDisc(e.ch - '0', player, grid);
      player = player == 1 ? 2 : 1;
    }

    Point pos ={0,0};
    tb_clear();

    // Print the grid
    tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|1 2 3 4 5 6 7|");
    pos.y++;
    for(int i = 0; i < 6; i++){
      tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|             |");
      pos.y++;
    }
    tb_print(pos.x, pos.y, TB_WHITE, TB_BLUE, "|1 2 3 4 5 6 7|");

    // Print the discs
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 6; j++) {
        int color = colors[grid[i][j]];
        int offset = i + 1;
        tb_print(i+offset,j+1, color, TB_BLUE, "⬤");
      }
    }

    tb_present();
  }

  tb_shutdown();
  return 0;
}

void dropDisc(int pos, int player, int (*grid)[6]){
  pos--;
  Point location = {pos,0};
  while(grid[location.x][location.y] == 0 && location.x < 7 && location.x >= 0 && location.y < 6){
    location.y++;
  }
  location.y--;
  if(location.y < 0) return;
  grid[location.x][location.y] = player;
}
