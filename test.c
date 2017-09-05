#include "chess.c"
#include <stdio.h>

typedef void (*ft_action)(gamestate x);

void walk_game_tree(gamestate init, int depth, ft_action f)
{
  if (depth <= 0) {
    ft_action(init);
    return;
  } else {
    iterator i = mkIterator(init);
    while (! is_iterator_finished(i)) {
      move m = dereference_iterator(i);
      gamestate g = apply_move(init, m);
      walk_game_tree(g, depth-1, f);
      i = advance_iterator(i);
    }
  }
}

void print_bitboard(uint64_t bb)
{
  for (int i = NUM_BOARD_SQUARES; i --> 0;) {
    char c = is_bit_set(bb, mkPosition(_file(i), 7 - _rank(i))) ? '1' : '0';
    putchar(c);
    if (i % 8 == 0)
      putchar('\n');
  }
}

uint64_t n;

void tick_counter(gamestate x) { n++; }

int perft(int depth)
{
  n = 0;
  gamestate init = new_game();
  walk_game_tree(init, depth, tick_counter);
  return n;
}

int main() {
  gamestate g = new_game();
  
  
  printf("%lu\n", x);
  print_bitboard(x);
  printf("Perft(0): %d\n", perft(0));
  printf("Perft(1): %d\n", perft(1));
  printf("Perft(2): %d\n", perft(2));
  printf("Perft(3): %d\n", perft(3));
}
