#include <assert.h>
#include <stdbool.h>
#include "oware.h"

// board_eq(a, b) returns true if and only if a and b represent
//   the exact same oware game state

bool board_eq(const struct oware_board *a,
              const struct oware_board *b) {
  assert(a);
  assert(b);
  if (a->scores[0] != b->scores[0] ||
      a->scores[1] != b->scores[1] ||
      a->turn != b->turn) {
    return false;
  }
  for (int i = 0; i < HOUSE_COUNT; ++i) {
    if (a->houses[i] != b->houses[i]) {
      return false;
    }
  }
  return true;
}

int main(void) {
  struct oware_board b;
  struct oware_board init = {{4, 4, 4, 4, 4, 4,
                              4, 4, 4, 4, 4, 4}, 
                             {0, 0}, 
                             1};
  oware_new_game(&b);
  assert(board_eq(&b, &init)); // does init initilize properly?
  
  assert(!oware_valid_move(&b, 6)); // one of player 2's houses
  
  oware_make_move(&b, 0); // player 1 sows from house 0 (aka A)
  
  struct oware_board expected_1 = 
    {{0, 5, 5, 5, 5, 4, // 4 seeds sowed into houses 1 - 4
      4, 4, 4, 4, 4, 4},// didn't reach player 2
     {0,0}, // no houses captured
     2};  // now it's player 2's turn
   
  assert(board_eq(&b, &expected_1));
  assert(!oware_game_over(&b));
  oware_end_game(&b);
  // ending game should set both to be 0...
  assert(oware_seed_count(&b, 1) == oware_seed_count(&b, 2));
  
