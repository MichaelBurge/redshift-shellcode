#define DEBUG 1
#include "chess.c"

typedef void (*ft_action)(gamestate x);

void print_iterator(iterator i)
{
  printf("ITERATOR\n");
  printf("== Current pieces\n");
  print_bitboard(i.current_piece_bb);
  printf("== Rooks\n");
  print_bitboard(i.rooks_bb);
  printf("== Knights\n");
  print_bitboard(i.knights_bb);
  printf("== Bishops\n");
  print_bitboard(i.bishops_bb);
  printf("== Queens\n");
  print_bitboard(i.queens_bb);
  printf("== Kings\n");
  print_bitboard(i.kings_bb);
  printf("== Pawns\n");
  print_bitboard(i.pawns_bb);
}

uint64_t n;

void tick_counter(gamestate x) {
  n++;
}

void walk_game_tree(gamestate init, int depth, ft_action f)
{
  if (depth <= 0) {
    f(init);
    return;
  } else {
    iterator i = mkIterator(init);
    while (! is_iterator_finished(i)) {
      move m = dereference_iterator(i);
      gamestate g = apply_move(init, m);
      walk_game_tree(g, depth-1, f);
      iterator i2 = advance_iterator(g, i);
      if (! iter_lt(i2, i)) {
        printf("%lu\n", n);
        print_iterator(i);
        print_iterator(i2);
        throw "derp";
      }
      i = i2;
    }
  }
}

void assert_equal_bb(const char* message, uint64_t x, uint64_t y)
{
  if (x != y) {
    printf("ASSERTION FAILURE: %s\n", message);
    printf("== expected\n");
    print_bitboard(x);
    printf("== actual\n");
    print_bitboard(y);
    throw "derp";
  }
}
void assert_equal_int(const char* message, int x, int y)
{
  if (x != y) {
    printf("ASSERTION_FAILURE: %s\n", message);
    printf("%d != %d", x,y);
    throw "derp";
  }
}

void assert(const char* message, bool x)
{
  if (! x) {
    printf("ASSERTION_FAILURE: %s\n", message);
    throw "derp";
  }
}

void test_ray()
{
  uint64_t center = mkPosition(3,3);
  uint64_t nw_ray =
    bit(mkPosition(0,6)) |
    bit(mkPosition(1,5)) |
    bit(mkPosition(2,4));
  uint64_t n_ray =
    bit(mkPosition(3,4)) |
    bit(mkPosition(3,5)) |
    bit(mkPosition(3,6)) |
    bit(mkPosition(3,7));
  uint64_t ne_ray =
    bit(mkPosition(4,4)) |
    bit(mkPosition(5,5)) |
    bit(mkPosition(6,6)) |
    bit(mkPosition(7,7));
  uint64_t e_ray =
    bit(mkPosition(4,3)) |
    bit(mkPosition(5,3)) |
    bit(mkPosition(6,3)) |
    bit(mkPosition(7,3));
  uint64_t se_ray =
    bit(mkPosition(4,2)) |
    bit(mkPosition(5,1)) |
    bit(mkPosition(6,0));
  uint64_t s_ray =
    bit(mkPosition(3,2)) |
    bit(mkPosition(3,1)) |
    bit(mkPosition(3,0));
  uint64_t sw_ray =
    bit(mkPosition(2,2)) |
    bit(mkPosition(1,1)) |
    bit(mkPosition(0,0));
  uint64_t w_ray =
    bit(mkPosition(2,3)) |
    bit(mkPosition(1,3))|
    bit(mkPosition(0,3));
  /* printf("== RAY_A1A8\n"); */
  /* print_bitboard(RAY_A1A8); */
  /* printf("== RAY_H1H8\n"); */
  /* print_bitboard(RAY_H1H8); */
  /* printf("== offset(RAY, 1)\n"); */
  /* print_bitboard(offset(RAY_A8H1, 1)); */

  /* printf("== offset(RAY, RANK)\n"); */
  /* print_bitboard(offset(RAY_A8H1, RANK)); */

  /*   printf("== offset(RAY, -RANK)\n"); */
  /* print_bitboard(offset(RAY_A8H1, -RANK)); */

  assert_equal_bb("nw", nw_ray, mkRay(center, DIRECTION_NORTHWEST));
  assert_equal_bb("n",  n_ray,  mkRay(center, DIRECTION_NORTH));
  assert_equal_bb("ne", ne_ray, mkRay(center, DIRECTION_NORTHEAST));
  assert_equal_bb("e",  e_ray,  mkRay(center, DIRECTION_EAST));
  assert_equal_bb("se", se_ray, mkRay(center, DIRECTION_SOUTHEAST));
  assert_equal_bb("s",  s_ray,  mkRay(center, DIRECTION_SOUTH));
  assert_equal_bb("sw", sw_ray, mkRay(center, DIRECTION_SOUTHWEST));
  assert_equal_bb("w",  w_ray,  mkRay(center, DIRECTION_WEST));
}

void test_rook()
{
  // Rook in middle
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.rooks_bb = bit(center);
    g.current_piece_bb = g.rooks_bb;
    
    uint64_t expected = clear_bit(mkRank(3) | mkFile(3), center);
    uint64_t actual = valid_rook_moves(g, center);
    assert_equal_bb("test_rook_middle", expected, actual);
  }
  // Rook in corner
  {
    uint64_t center = mkPosition(0,0);
    gamestate g = zerostate();
    g.rooks_bb = bit(center);
    g.current_piece_bb = g.rooks_bb;
    
    uint64_t expected = clear_bit(mkRank(0) | mkFile(0), center);
    uint64_t actual = valid_rook_moves(g, center);
    assert_equal_bb("test_rook_corner", expected, actual);
  }
  // Rook blocked in left corner
  {
    uint64_t center = mkPosition(0,0);
    gamestate g = zerostate();
    g.rooks_bb = bit(center) | bit(mkPosition(0,1)) | bit(mkPosition(1,0));
    g.current_piece_bb = g.rooks_bb;

    assert_equal_bb("test_rook_blocked_1", 0, valid_rook_moves(g, center));
  }
  // Rook blocked in right corner
  {
    uint64_t center = mkPosition(7,0);
    gamestate g = zerostate();
    g.rooks_bb = bit(center) | bit(mkPosition(7,1)) | bit(mkPosition(6,0));
    g.current_piece_bb = g.rooks_bb;

    assert_equal_bb("test_rook_blocked_2", 0, valid_rook_moves(g, center));
  }
}

void test_pawn()
{
  // Left edge
  {
    uint64_t center = mkPosition(0,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center);
    g.current_piece_bb = g.pawns_bb;

    uint64_t expected = bit(mkPosition(0,2)) | bit(mkPosition(0,3));
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_1", expected, actual);

  }
  // Right edge
  {
    uint64_t center = mkPosition(7,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center);
    g.current_piece_bb = g.pawns_bb;

    uint64_t expected = bit(mkPosition(7,2)) | bit(mkPosition(7,3));
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_2", expected, actual);
  }
  // Middle
  {
    uint64_t center = mkPosition(3,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center);
    g.current_piece_bb = g.pawns_bb;

    uint64_t expected = bit(mkPosition(3,2)) | bit(mkPosition(3,3));
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_3", expected, actual);
  }
  // Capture
  {
    uint64_t center = mkPosition(7,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center);
    g.current_piece_bb = g.pawns_bb;
    g.pawns_bb |= bit(move_direction(center, DIRECTION_NORTHWEST));
    
    uint64_t expected = bit(mkPosition(7,2)) | bit(mkPosition(7,3)) | bit(mkPosition(6,2));
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_4", expected, actual);
  }
}

void test_iterator()
{
  // Piece has valid moves
  {
    uint64_t center = mkPosition(0,0);
    gamestate g = zerostate();
    g.rooks_bb = bit(center);
    g.current_piece_bb = g.rooks_bb;

    uint64_t expected =
      mkRay(center, DIRECTION_NORTH) |
      mkRay(center, DIRECTION_EAST);
    iterator i = mkIterator(g);
    uint64_t actual = i.current_piece_bb;
    assert_equal_bb("test_iterator_1", expected, actual);
  }
  // Piece has no valid moves; skip to next
  {
    uint64_t center = mkPosition(0,0);
    gamestate g = zerostate();
    g.pawns_bb = bit(mkPosition(0,1));
    g.rooks_bb = bit(center);
    g.knights_bb = bit(mkPosition(1,0));
    g.current_piece_bb = all_pieces(g);

    iterator i = mkIterator(g);    
    assert_equal_bb("test_iterator_2_rooks", 0, i.rooks_bb);
    assert_equal_bb("test_iterator_2_knights", bit(mkPosition(1,0)), i.knights_bb);
    uint64_t expected =
      bit(mkPosition(0,2)) |
      bit(mkPosition(2,2)) |
      bit(mkPosition(3,1));
    assert_equal_bb("test_iterator_2_knight_moves", expected, i.current_piece_bb);
  }
  // Two pieces have no valid moves; skip to next
  {
    gamestate g = zerostate();
    g.pawns_bb =
      bit(mkPosition(0,1)) |
      bit(mkPosition(7,1));
    g.rooks_bb =
      bit(mkPosition(0,0)) |
      bit(mkPosition(7,0));
    g.knights_bb =
      bit(mkPosition(1,0)) |
      bit(mkPosition(6,0));
    g.current_piece_bb = all_pieces(g);
    iterator i = mkIterator(g);
    assert_equal_bb("test_iterator_3_rooks", 0, i.rooks_bb);
    assert_equal_bb("test_iterator_3_knights", bit(mkPosition(1,0)) | bit(mkPosition(6,0)), i.knights_bb);
    uint64_t expected = valid_knight_moves(g, mkPosition(1,0));
    assert_equal_bb("test_iterator_3_knight_moves", expected, i.current_piece_bb);
      
  }
  // Empty iterator is finished
  {
    gamestate g = zerostate();
    iterator i = zerostate();
    assert("test_iterator_empty_1", is_iterator_finished(i));
    iterator j = advance_iterator(g, i);
    assert("test_iterator_empty_2", is_iterator_finished(j));
  }
}
    
void test_knight()
{
  // Knight in middle
  {
    uint64_t center = mkPosition(1,0);
    gamestate g = zerostate();
    g.knights_bb = bit(center);
    g.current_piece_bb = g.knights_bb;

    uint64_t expected =
      bit(mkPosition(0,2)) |
      bit(mkPosition(2,2)) |
      bit(mkPosition(3,1));
    uint64_t actual = valid_knight_moves(g, center);
    assert_equal_bb("test_knight_1", expected, actual);
  }
}

int perft(int depth)
{
  n = 0;
  gamestate init = new_game();
  walk_game_tree(init, depth, tick_counter);
  return n;
}


int main() {
  test_ray();
  test_rook();
  test_pawn();
  test_knight();
  test_iterator();
  /* gamestate g = new_game(); */
  /* iterator i = mkIterator(g); */
  
  /* print_bitboard(i.current_piece_bb); */
  
  printf("Perft(0): %d\n", perft(0));
  printf("Perft(1): %d\n", perft(1));
  /* printf("Perft(2): %d\n", perft(2)); */
  /* printf("Perft(3): %d\n", perft(3)); */
}
