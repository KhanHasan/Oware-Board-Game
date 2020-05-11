#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "oware.h"

const int HOUSE_COUNT = 12;

// see oware.h
void oware_new_game(struct oware_board *board) {
  for(int i = 0; i<12; i++){
    board->houses[i] = 4;
  }
  board->turn = 1;
  board->scores[0] = 0;
  board->scores[1] = 0;
}

// see oware.h
int oware_seed_count(const struct oware_board *board, int player) {
  int sum = 0;
  if(player == 1){
    for(int i = 0; i<6; i++){
      sum += board->houses[i];
    }
    return sum;
  }else{
    for(int i = 6; i<12; i++){
      sum += board->houses[i];
    }
    return sum;
  }
}

// see oware.h
bool oware_valid_move(const struct oware_board *board, int house) {
  assert(board);
  if(board->turn == 1){
    if(house < 6 && house >= 0){
      if(board->houses[house] > 0){
        if((oware_seed_count(board, 2)) > 0){
          return true;
        }else if((house + board->houses[house]) > 5){
          return true;
        }else{
          return false;
        }
      }else{
        return false;
      }
    }else {
      return false;
    }
  }

  if(board->turn == 2){
    if(house < 12 && house >= 6){
      if(board->houses[house] > 0){
        if((oware_seed_count(board, 1)) > 0){
          return true;
        }else if((house + board->houses[house]) > 12){
          return true;
        }else{
          return false;
        }
      }else{
        return false;
      }
    }else {
      return false;
    }
  }

  return false;

}


// see oware.h
void oware_make_move(struct oware_board *board, int house) {
  int pos = house;
  int final = house;
  int times = board->houses[house];

  if((oware_valid_move(board, house)) == true){
    board->houses[house] = 0;

    for(int i = 0; i < times; i++){
      pos++;
      if(pos == 12){
        pos = 0;
      }
      board->houses[pos]++;
      final++;
    }
  }

  final = final%12;

  if(board->turn == 1){
    while(final > 5){
      if(board->houses[final] == 2 ||board->houses[final] == 3){
        if((oware_seed_count(board, 2) - board->houses[final]) == 0)          {
          break;
        }     
        board->scores[0] += board->houses[final];
        board->houses[final] = 0;
        final--;
      }else{
        break;
      }
    }
  }

  if(board->turn == 2){
    while(final >= 0 && final < 6){
     if(board->houses[final] == 2 ||board->houses[final] == 3){
      if((oware_seed_count(board, 1) - board->houses[final]) == 0){
          break;
        }  
        board->scores[1] += board->houses[final];
        board->houses[final] = 0;
        final--;
      }else{
        break;
      }
    }
  }


  if(board->turn == 1){
    board->turn = 2;
  }else{
    board->turn = 1;
  }
}

// see oware.h
bool oware_game_over(const struct oware_board *board) {
  if((board->scores[0]) >= 25){
    return true;
  }
  if((board->scores[1]) >= 25){
    return true;
  }
  if(((oware_seed_count(board, 1)) == 0) && (board->turn == 1)){
    return true;
  }
  if(((oware_seed_count(board, 2)) == 0) && (board->turn == 2)){
    return true;
  }
  if(board->turn == 1 && !(oware_valid_move(board, 0)) &&           !(oware_valid_move(board, 1)) && !(oware_valid_move(board, 2)) &&  !(oware_valid_move(board, 3)) && !(oware_valid_move(board, 4))  && !(oware_valid_move(board, 5))){
    return true;
  }
  if(board->turn == 2 && !(oware_valid_move(board, 6)) &&           !(oware_valid_move(board, 7)) && !(oware_valid_move(board, 8)) &&  !(oware_valid_move(board, 9)) && !(oware_valid_move(board, 10))  && !(oware_valid_move(board, 11))){
    return true;
  }
  return false;

}

// see oware.h
void oware_end_game(struct oware_board *board) {
  int sum1 = 0;
  int sum2 = 0;
  for(int i = 0; i < 12; i++){
    if(i < 6){
      sum1 += board->houses[i];
    }else{
      sum2 += board->houses[i];
    }
  }

  board->scores[0] += sum1;
  board->scores[1] += sum2;
}

// draw_lines() draws a +--+ line on the screen
// effects: outputs a line of text

static void draw_lines(void) {
  for (int i = 0; i < HOUSE_COUNT / 2; ++i) {
    printf("+--"); 
  }
  printf("+\n");
}

// draw_labels(player) draws the house labels for player
//  (A-F for player 1, G-L for player 2)
// requires: player is 1 or 2
// effects: ouputs a line of text

static void draw_labels(int player) {
  for (int i = 0; i < HOUSE_COUNT / 2; ++i) {
    int house;
    if (player == 1) {
      house = i;
    } else {
      house = HOUSE_COUNT - i - 1;
    }
    printf(" %c ", 'A' + house); 
  }
  printf("\n");
}

// see oware.h
void oware_draw(const struct oware_board *board) {
  draw_labels(2);
  draw_lines();
  for (int i = 0; i < HOUSE_COUNT / 2; ++i) {
    printf("|%2d", board->houses[HOUSE_COUNT - 1 - i]); 
  }
  printf("|\n");
  draw_lines();
  for (int i = 0; i < HOUSE_COUNT / 2; ++i) {
    printf("|%2d", board->houses[i]); 
  }
  printf("|\n");
  draw_lines();
  draw_labels(1);
  printf("\n");
  printf("Player 1: %d\n", board->scores[0]);
  printf("Player 2: %d\n", board->scores[1]); 
}
