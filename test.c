#define DEBUG 1
#include "chess.c"

typedef void (*ft_action)(gamestate, move, int);

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

char piece_char(int p, bool is_white) {
  switch (p) {
  case PIECE_EMPTY: return '.';
  case PIECE_ROOK: return is_white ? 'R' : 'r';
  case PIECE_KNIGHT: return is_white ? 'N' : 'n';
  case PIECE_BISHOP: return is_white ? 'B' : 'b';
  case PIECE_QUEEN: return is_white ? 'Q' : 'q';
  case PIECE_KING: return is_white ? 'K' : 'k';
  case PIECE_PAWN: return is_white ? 'P' : 'p';
  default: abort();
  }
}

void print_gamestate(gamestate g)
{
  for (int r = 8; r --> 0;) {
    for (int f = 0; f < 8; f++) {
      int pos = mkPosition(f, r);
      int piece = get_piece(g, pos);
      bool is_white = is_bit_set(g.current_player_bb, pos);
      char c = piece_char(piece, is_white);
      putchar(c);
    }
    putchar('\n');
  }
}

void print_pos(int pos) {
  printf("%c%d", file(pos) + 'a', rank(pos)+1);
}

void print_move(move m) {
  print_pos(m.from);
  print_pos(m.to);
}

uint64_t n;

void tick_counter(gamestate x, move m, int d) {
  if (d == 1) 
    n++;
}

void walk_game_tree(gamestate init, int depth, ft_action f)
{
  if (depth <= 0) {
    return;
  } else {
    iterator i = mkLegalIterator(init);
    while (! is_iterator_finished(i)) {
      move m = dereference_iterator(i);
      gamestate g = swap_board(apply_move(init, m));

      f(init, m, depth);
      walk_game_tree(g, depth-1, f);
      iterator i2 = advance_iterator_legal(init, i);
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

void print_gamestate_action(gamestate g, move m, int d) { print_move(m); putchar('\n');}

void print_moves(gamestate g)
{
  walk_game_tree(g, 1, print_gamestate_action);
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
  // Base rays
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

    assert_equal_bb("nw", nw_ray, mkRay(center, DIRECTION_NORTHWEST));
    assert_equal_bb("n",  n_ray,  mkRay(center, DIRECTION_NORTH));
    assert_equal_bb("ne", ne_ray, mkRay(center, DIRECTION_NORTHEAST));
    assert_equal_bb("e",  e_ray,  mkRay(center, DIRECTION_EAST));
    assert_equal_bb("se", se_ray, mkRay(center, DIRECTION_SOUTHEAST));
    assert_equal_bb("s",  s_ray,  mkRay(center, DIRECTION_SOUTH));
    assert_equal_bb("sw", sw_ray, mkRay(center, DIRECTION_SOUTHWEST));
    assert_equal_bb("w",  w_ray,  mkRay(center, DIRECTION_WEST));
  }
  // Rays with blockers
  {
    uint64_t center = mkPosition(3,3);
    // W
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(1,3)) |
        bit(mkPosition(0,3));
      uint64_t expected =
        bit(mkPosition(2,3)) |
        bit(mkPosition(1,3));
      /* printf("== expected:\n"); print_bitboard(expected); printf("\n"); */
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_WEST);
      /* printf("== actual:\n"); print_bitboard(actual); printf("\n"); */
      /* uint64_t base_ray = mkRay(center, DIRECTION_WEST); */
      /* printf("== base_ray:\n"); print_bitboard(base_ray); printf("\n"); */
      /* uint64_t blockers = base_ray & all_pieces(g); */
      /* printf("== blockers:\n"); print_bitboard(blockers); printf("\n"); */
      /* int blocker = closest_blocker(blockers, DIRECTION_WEST); */
      /* printf("== blocker: "); print_pos(blocker); printf("\n"); */
      /* uint64_t blocker_ray = mkRay(blocker, DIRECTION_WEST); */
      /* printf("== blocker_ray:\n"); print_bitboard(blocker_ray); */
      /* uint64_t movable_squares_without_capture = base_ray ^ blocker_ray ^ bit(blocker); */
      /* printf("== movable_squares_without_capture\n"); print_bitboard(movable_squares_without_capture); printf("\n"); */

      assert_equal_int("w_blocker", mkPosition(1,3), closest_blocker(g.pawns_bb, DIRECTION_WEST));
      assert_equal_bb("w", expected, actual);
    }
    // NW
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(1,5)) |
        bit(mkPosition(0,6));
      uint64_t expected =
        bit(mkPosition(2,4)) |
        bit(mkPosition(1,5));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_NORTHWEST);
      assert_equal_int("nw_blocker", mkPosition(1,5), closest_blocker(g.pawns_bb, DIRECTION_NORTHWEST));
      assert_equal_bb("nw", expected, actual);
    }
    // N
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(3,6)) |
        bit(mkPosition(3,7));
      uint64_t expected =
        bit(mkPosition(3,4)) |
        bit(mkPosition(3,5)) |
        bit(mkPosition(3,6));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_NORTH);
      assert_equal_int("n_blocker", mkPosition(3,6), closest_blocker(g.pawns_bb, DIRECTION_NORTH));
      assert_equal_bb("n", expected, actual);
    }
    // NE
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(6,6)) |
        bit(mkPosition(7,7));
      uint64_t expected =
        bit(mkPosition(4,4)) |
        bit(mkPosition(5,5)) |
        bit(mkPosition(6,6));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_NORTHEAST);
      assert_equal_int("ne_blocker", mkPosition(6,6), closest_blocker(g.pawns_bb, DIRECTION_NORTHEAST));
      assert_equal_bb("ne", expected, actual);
    }
    // NE 2
    {
      uint64_t center = mkPosition(0,2);
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(4,6)) |
        bit(mkPosition(5,7));
      uint64_t expected =
        bit(mkPosition(1,3)) |
        bit(mkPosition(2,4)) |
        bit(mkPosition(3,5)) |
        bit(mkPosition(4,6));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_NORTHEAST);
      assert_equal_bb("ne_2", expected, actual);
    }
    // NE 3
    {
      uint64_t bb = bit(mkPosition(4,6)) | bit(mkPosition(5,7));
      int i = closest_blocker(bb, DIRECTION_NORTHEAST);
      assert_equal_int("ne_3", mkPosition(4,6), i);
    }
    // E
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(6,3)) |
        bit(mkPosition(7,3));
      uint64_t expected =
        bit(mkPosition(4,3)) |
        bit(mkPosition(5,3)) |
        bit(mkPosition(6,3)); 
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_EAST);
      assert_equal_int("e_blocker", mkPosition(6,3), closest_blocker(g.pawns_bb, DIRECTION_EAST));
      assert_equal_bb("e", expected, actual);
    }
    // SE
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(5,1)) |
        bit(mkPosition(6,0));
      uint64_t expected =
        bit(mkPosition(4,2)) |
        bit(mkPosition(5,1));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_SOUTHEAST);
      assert_equal_int("se_blocker", mkPosition(5,1), closest_blocker(g.pawns_bb, DIRECTION_SOUTHEAST));
      assert_equal_bb("se", expected, actual);
    }
    // S
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(3,1)) |
        bit(mkPosition(3,0));
      uint64_t expected =
        bit(mkPosition(3,2)) |
        bit(mkPosition(3,1));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_SOUTH);
      assert_equal_int("s_blocker", mkPosition(3,1), closest_blocker(g.pawns_bb, DIRECTION_SOUTH));
      assert_equal_bb("s", expected, actual);
    }
    // SW
    {
      gamestate g = zerostate();
      g.pawns_bb =
        bit(mkPosition(1,1)) |
        bit(mkPosition(0,0));
      uint64_t expected =
        bit(mkPosition(2,2)) |
        bit(mkPosition(1,1));
      uint64_t actual = shoot_ray_until_blocker(g, center, DIRECTION_SOUTHWEST);
      assert_equal_int("sw_blocker", mkPosition(1,1), closest_blocker(g.pawns_bb, DIRECTION_SOUTHWEST));
      assert_equal_bb("sw", expected, actual);
    }
  }
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
  // Blocked by another piece
  {
    uint64_t center = mkPosition(0,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center) | bit(center + RANK);
    g.current_piece_bb = g.pawns_bb;
    
    uint64_t expected = 0;
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_5", expected, actual);
  }
  // Blocked by another piece when double-jumping
  {
    uint64_t center = mkPosition(0,1);
    gamestate g = zerostate();
    g.pawns_bb = bit(center);
    g.current_piece_bb = g.pawns_bb;
    g.pawns_bb |=  bit(mkPosition(0,3));
    
    uint64_t expected = bit(mkPosition(0,2));
    uint64_t actual = valid_pawn_moves(g, center);
    assert_equal_bb("test_pawn_6", expected, actual);
  }
  // En Passant
  {
    gamestate g = zerostate();
    g.pawns_bb =
      bit(mkPosition(0,1)) |
      bit(mkPosition(1,3));
    g.current_piece_bb = bit(mkPosition(0,1));

    iterator i = mkIterator(g);
    {
      uint64_t expected =
        bit(mkPosition(0,2)) |
        bit(mkPosition(0,3));
      assert_equal_bb("test_pawn_en_passant_1", expected, i.current_piece_bb);
    }
    // Double jump allows en passant
    move m; m.from = mkPosition(0,1); m.to = mkPosition(0,3);
    g = swap_board(apply_move(g, m));
    i = mkIterator(g);
    {
      uint64_t expected =
        bit(mkPosition(0,5)) |
        bit(mkPosition(1,5));
      assert_equal_int("test_pawn_en_passant_target", mkPosition(0,5), g.en_passant_sq);
      assert_equal_bb("test_pawn_en_passant_2", expected, i.current_piece_bb);
    }
    // The actual capture
    move m2; m2.from = mkPosition(1,4); m2.to = mkPosition(0,5);
    g = swap_board(apply_move(g, m2));
    {
      assert_equal_int("test_pawn_en_passant_target_2", POSITION_INVALID, g.en_passant_sq);
      assert_equal_bb("test_pawn_en_passant_3", bit(mkPosition(0,2)), g.pawns_bb);
    }
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
  // Knight in left bottom
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
  // Knight in right bottom
  {
    uint64_t center = mkPosition(6,0);
    gamestate g = zerostate();
    g.knights_bb = bit(center);
    g.current_piece_bb = g.knights_bb;

    uint64_t expected =
      bit(mkPosition(7,2)) |
      bit(mkPosition(5,2)) |
      bit(mkPosition(4,1));
    uint64_t actual = valid_knight_moves(g, center);
    assert_equal_bb("test_knight_2", expected, actual);
  }
}

void test_king()
{
  // King in middle
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.kings_bb = bit(center);
    g.current_piece_bb = g.kings_bb;

    uint64_t expected =
      bit(mkPosition(4,3)) |
      bit(mkPosition(4,4)) |
      bit(mkPosition(3,4)) |
      bit(mkPosition(2,4)) |
      bit(mkPosition(2,3)) |
      bit(mkPosition(2,2)) |
      bit(mkPosition(3,2)) |
      bit(mkPosition(4,2));
    uint64_t actual = valid_king_moves(g, center);
    assert_equal_bb("test_king_1", expected, actual);
  }
  // King blocked on 3 sides in left corner
  {
    uint64_t center = mkPosition(0,0);
    gamestate g = zerostate();
    g.kings_bb = bit(center);
    g.pawns_bb =
      bit(mkPosition(1,0)) |
      bit(mkPosition(1,1)) |
      bit(mkPosition(0,1));
    g.current_piece_bb = all_pieces(g);

    uint64_t expected = 0;
    uint64_t actual = valid_king_moves(g, center);
    assert_equal_bb("test_king_2", expected, actual);
  }
}

void test_bishop()
{
  // Bishop southwest blocker
  {
    gamestate g = zerostate();
    g.pawns_bb = bit(mkPosition(3,1));
    g.kings_bb = bit(mkPosition(2,0));
    g.current_piece_bb = bit(mkPosition(7,5));
    g.bishops_bb = bit(mkPosition(7,5));

    {
      uint64_t expected =
        bit(mkPosition(6,6)) |
        bit(mkPosition(5,7)) |
        bit(mkPosition(6,4)) |
        bit(mkPosition(5,3)) |
        bit(mkPosition(4,2)) |
        bit(mkPosition(3,1));
      uint64_t actual = valid_bishop_moves(g, mkPosition(7,5));
      assert_equal_bb("test_bishop_1", expected, actual);
    }
    // King not in check from bishop
    g.current_piece_bb ^= all_pieces(g);
    assert("test_bishop_1_check", ! is_in_check(g));
  }
  // Bishop northeast blocker
  {
    gamestate g = zerostate();
    g.pawns_bb = bit(mkPosition(3,1));
    g.kings_bb = bit(mkPosition(2,0));
    g.current_piece_bb = all_pieces(g);
    g.bishops_bb = bit(mkPosition(7,5));
    {
      uint64_t expected =
        bit(mkPosition(6,6)) |
        bit(mkPosition(5,7)) |
        bit(mkPosition(6,4)) |
        bit(mkPosition(5,3)) |
        bit(mkPosition(4,2));
      uint64_t actual = valid_bishop_moves(g, mkPosition(7,5));
      assert_equal_bb("test_bishop_2", expected, actual);
    }
    // King not in check from bishop
    assert("test_bishop_2_check", ! is_in_check(g));
  } 
}

void test_apply_move()
{
  // Knight in middle
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.knights_bb = bit(center);
    g.current_piece_bb = g.knights_bb;

    iterator i = mkIterator(g);
    move m = dereference_iterator(i);
    assert_equal_int("test_apply_move_from", center, m.from);
    assert_equal_int("test_apply_move_to", mkPosition(2,1), m.to);
    gamestate g2 = apply_move(g, m);

    assert_equal_bb("test_apply_move", bit(mkPosition(2,1)), g2.knights_bb);
  }
  // Doesn't affect other pieces
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.knights_bb = bit(center);
    g.current_piece_bb = g.knights_bb;
    g.knights_bb |=
      bit(mkPosition(1,0)) |
      bit(mkPosition(6,0));

    iterator i = mkIterator(g);
    move m = dereference_iterator(i);
    assert_equal_int("test_apply_move_from", center, m.from);
    assert_equal_int("test_apply_move_to", mkPosition(2,1), m.to);
    gamestate g2 = apply_move(g, m);

    uint64_t expected = bit(m.to) | bit(mkPosition(1,0)) | bit(mkPosition(6,0));
    assert_equal_bb("test_apply_move_2", expected, g2.knights_bb);
  }
}

void test_check()
{
  // King in check 1
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.kings_bb = bit(center);
    g.rooks_bb = bit(center+2);
    g.current_player_bb = g.kings_bb;

    assert("test_check_1", is_in_check(g));
  }
  // King in check 2
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.kings_bb = bit(center);
    g.rooks_bb = bit(center-2*RANK);
    g.current_player_bb = g.kings_bb;

    assert("test_check_2", is_in_check(g));
  }
  // King blocked by rooks
  {
    uint64_t center = mkPosition(3,3);
    gamestate g = zerostate();
    g.kings_bb = bit(center);
    g.rooks_bb =
      bit(mkPosition(0,4)) |
      bit(mkPosition(0,2)) |
      bit(mkPosition(2,0));
    //bit(mkPosition(0,2));
    g.current_player_bb = g.kings_bb;
    assert_equal_int("test_check_3", 1, num_legal_moves(g));
  }
  // King can safely move to a square protected by own unit.
  {
    gamestate g = zerostate();
    g.pawns_bb =
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,0)) |
      bit(mkPosition(4,1)) |
      bit(mkPosition(2,1));
    g.kings_bb = bit(mkPosition(3,0));
    g.bishops_bb =
      bit(mkPosition(0,2)) |
      bit(mkPosition(0,5));
    g.knights_bb =
      bit(mkPosition(1,0)) |
      bit(mkPosition(6,0));
    g.current_piece_bb = all_pieces(g);
    g.bishops_bb |= bit(mkPosition(7,5));
    {
      uint64_t expected =
        bit(mkPosition(6,6)) |
        bit(mkPosition(5,7)) |
        bit(mkPosition(6,4)) |
        bit(mkPosition(5,3)) |
        bit(mkPosition(4,2));
      uint64_t actual = valid_bishop_moves(g, mkPosition(7,5));
      assert_equal_bb("test_check_4_bishop", expected, actual);
    }
    // King not in check from bishop
    assert("test_check_4_check", ! is_in_check(g));
    {
      int from = mkPosition(3,0);
      int target = mkPosition(2,0);
      uint64_t expected = bit(target);
      uint64_t actual = piece_legal_movepoints(g, from);
      assert_equal_bb("test_check_4_king_valid", bit(target), valid_king_moves(g, from));
      iterator i = zerostate();
      int piece = get_piece(g, from);
      i = set_piece_bb(i, piece, bit(from));
      i = reset_iterator_moves(g,i);
      i = legalize(g,i);

      assert_equal_bb("test_check_4_next", bit(target), i.current_piece_bb);
      assert_equal_bb("test_check_4_moves", expected, actual);
    }
  }
}

void test_castling()
{
  // Kingside castle 1
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(7,0));
    g.castle_flags = CASTLE_WHITE_KINGSIDE;
    g.current_player_bb = all_pieces(g);

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0)) |
      bit(mkPosition(6,0));
    uint64_t actual = valid_king_moves(g, mkPosition(4,0));
    assert_equal_bb("test_castling_1_moves", expected, actual);
    move m; m.from = mkPosition(4,0); m.to = CASTLE_KINGSIDE_KPOS;
    gamestate g2 = apply_move(g, m);
    assert_equal_bb("test_castling_1_king", bit(CASTLE_KINGSIDE_KPOS), g2.kings_bb);
    assert_equal_bb("test_castling_1_rook", bit(CASTLE_KINGSIDE_RPOS), g2.rooks_bb);
  }
  // Queenside castle 1
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(0,0));
    g.castle_flags = CASTLE_WHITE_QUEENSIDE;
    g.current_player_bb = all_pieces(g);

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0)) |
      bit(mkPosition(2,0));
    uint64_t actual = valid_king_moves(g, mkPosition(4,0));
    assert_equal_bb("test_castling_2_moves", expected, actual);
    move m; m.from = mkPosition(4,0); m.to = mkPosition(2,0);
    gamestate g2 = apply_move(g, m);
    assert_equal_bb("test_castling_2_king", bit(mkPosition(2,0)), g2.kings_bb);
    assert_equal_bb("test_castling_2_rook", bit(mkPosition(3,0)), g2.rooks_bb);
  }
  // Kingside castle blocked by check 1
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(7,0));
    g.castle_flags = CASTLE_WHITE_KINGSIDE;
    g.current_player_bb = all_pieces(g);
    g.rooks_bb |= bit(mkPosition(5,7));

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,1));
    uint64_t actual = piece_legal_movepoints(g, mkPosition(4,0));
    move not_castle; not_castle.from = mkPosition(4,0); not_castle.to = mkPosition(4,1);
    move castle_kingside; castle_kingside.from = mkPosition(4,0); castle_kingside.to = mkPosition(6,0);
    assert("test_castling_3_not_kingside_1", ! is_kingside_castle(g, not_castle));
    assert("test_castling_3_not_kingside_2", ! is_queenside_castle(g, not_castle));
    assert("test_castling_3_not_kingside_3", ! results_in_check(g, not_castle));
    assert("test_castling_3_not_kingside_4", is_legal(g, not_castle));
    assert("test_castling_3_illegal", ! is_legal(g, castle_kingside));
    assert_equal_bb("test_castling_3_moves", expected, actual);
  }
  // Queenside castle blocked by check 1
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(0,0));
    g.castle_flags = CASTLE_WHITE_QUEENSIDE;
    g.current_player_bb = all_pieces(g);
    g.rooks_bb |= bit(mkPosition(3,7));

    uint64_t expected =
      bit(mkPosition(4,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0));
    uint64_t actual = piece_legal_movepoints(g, mkPosition(4,0));
    assert_equal_bb("test_castling_4_moves", expected, actual);
  }
  // Kingside castle blocked by check 2
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(7,0));
    g.castle_flags = CASTLE_WHITE_KINGSIDE;
    g.current_player_bb = all_pieces(g);
    g.rooks_bb |= bit(mkPosition(4,7));

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0));
    uint64_t actual = piece_legal_movepoints(g, mkPosition(4,0));
    assert_equal_bb("test_castling_5_moves", expected, actual);
  }
  // Queenside castle blocked by check 2
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(0,0));
    g.castle_flags = CASTLE_WHITE_QUEENSIDE;
    g.current_player_bb = all_pieces(g);
    g.rooks_bb |= bit(mkPosition(4,7));

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0));
    uint64_t actual = piece_legal_movepoints(g, mkPosition(4,0));
    assert_equal_bb("test_castling_6_moves", expected, actual);
  }
  // Kingside castle blocked by unit
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(7,0));
    g.bishops_bb |= bit(mkPosition(5,0));
    g.castle_flags = CASTLE_WHITE_KINGSIDE;
    g.current_player_bb = all_pieces(g);

    uint64_t expected =
      bit(mkPosition(3,0)) |
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,1)) |
      bit(mkPosition(5,1));
    uint64_t actual = valid_king_moves(g, mkPosition(4,0));
    assert_equal_bb("test_castling_7_moves", expected, actual);
  }
  // Queenside castle blocked by unit
  {
    gamestate g = zerostate();
    g.kings_bb = bit(mkPosition(4,0));
    g.rooks_bb = bit(mkPosition(0,0));
    g.bishops_bb = bit(mkPosition(3,0));
    g.castle_flags = CASTLE_WHITE_QUEENSIDE;
    g.current_player_bb = all_pieces(g);

    uint64_t expected =
      bit(mkPosition(3,1)) |
      bit(mkPosition(4,1)) |
      bit(mkPosition(5,1)) |
      bit(mkPosition(5,0));
    uint64_t actual = valid_king_moves(g, mkPosition(4,0));
    assert_equal_bb("test_castling_8_moves", expected, actual);
  }
}

int perft(int depth)
{
  n = 0;
  gamestate init = new_game();
  walk_game_tree(init, depth, tick_counter);
  return n;
}

int perft_divide_depth;


void perft_divide_helper1(gamestate g, move m, int d)
{
  n = 0;

  gamestate g2 = swap_board(apply_move(g, m));
  walk_game_tree(g2, perft_divide_depth, tick_counter);
  print_move(m);
  printf(": %lu\n", n);
}

int perft_divide(gamestate g, int depth)
{
  perft_divide_depth = depth;
  walk_game_tree(g, 1, perft_divide_helper1);
}

void print_fen(gamestate g)
{
  int empty_squares = 0;
  for (int r = 8; r --> 0;) {
    for (int f = 0; f < 8; f++) {
      int pos = mkPosition(f, r);
      int piece = get_piece(g, pos);
      if (piece == PIECE_EMPTY) {
        empty_squares++;
        continue;
      } else if (empty_squares > 0) {
        printf("%d", empty_squares);
        empty_squares = 0;
      }
      bool is_white = is_bit_set(g.current_player_bb, pos);
      char c = piece_char(piece, is_white);
      putchar(c);
    }
    if (empty_squares > 0) {
      printf("%d", empty_squares);
      empty_squares = 0;
    }
    if (r > 0)
      putchar('/');
  }
  printf(" w ");
  // Castle Flags
  {
    if (g.castle_flags & CASTLE_WHITE_KINGSIDE)
      putchar('K');
    if (g.castle_flags & CASTLE_WHITE_QUEENSIDE)
      putchar('Q');
    if (g.castle_flags & CASTLE_BLACK_KINGSIDE)
      putchar('k');
    if (g.castle_flags & CASTLE_BLACK_QUEENSIDE)
      putchar('q');
    if (! g.castle_flags)
      putchar('-');
  }
  putchar(' ');
  if (g.en_passant_sq == POSITION_INVALID) {
    putchar('-');
  } else {
    //printf("%d---", g.en_passant_sq);
    print_pos(g.en_passant_sq);
  }
  printf(" 0 1\n");
}

int main() {
  test_ray();
  test_rook();
  test_pawn();
  test_knight();
  test_king();
  test_bishop();
  test_iterator();
  test_apply_move();
  test_check();
  test_castling();
  /* gamestate g = new_game(); */
  /* iterator i = mkIterator(g); */
  
  /* print_bitboard(i.current_piece_bb); */

  gamestate g = new_game();
  // 1: b1a3
  move m1; m1.from = mkPosition(1,0); m1.to = mkPosition(0,3);
  /* g = swap_board(apply_move(g, m1)); */
  /* // 2: b2b3 */
  /* move m2; m2.from = mkPosition(1,1); m2.to = mkPosition(1,2); */
  /* g = swap_board(apply_move(g, m2)); */
  /* // 3: a1b1 */
  /* move m3; m3.from = mkPosition(0,0); m3.to = mkPosition(1,0); */
  /* g = swap_board(apply_move(g, m3)); */
  /* // 4: c1a3 */
  /* move m4; m4.from = mkPosition(2,0); m4.to = mkPosition(0,2); */
  /* g = swap_board(apply_move(g, m4)); */
  /* // 5: a2a3 */
  /* move m5; m5.from = mkPosition(0,1); m5.to = mkPosition(0,2); */
  /* g = swap_board(apply_move(g, m4)); */

  // NOTE: D1C1 as m6 is in roce, but not the redshift shellcode.
  
  /* g = swap_board(apply_move(g, m2)); */
  /* move m3; m3.from = mkPosition(0,3); m3.to = mkPosition(0,4); */
  /* g = swap_board(apply_move(g, m3)); */
  /* g = apply_move(g, m3); */
  /* perft_divide(g,3); */
  /* print_fen(g); */
  /* // 1: B1A3 */

  /* g = swap_board(apply_move(g, m1)); */
  /* /\* // 2: B1A3 *\/ */
  /* g = swap_board(apply_move(g, m1)); */
  print_fen(g);
  /* /\* print_gamestate(g); *\/ */
  perft_divide(g, 0);;
  /* print_gamestate(g); */
  /* printf("== rooks\n"); print_bitboard(g.rooks_bb); */
  /* printf("== pawns\n"); print_bitboard(g.pawns_bb); */

  /* print_moves(g); */
  
  printf("Perft(0): %d\n", perft(0));
  printf("Perft(1): %d\n", perft(1));
  printf("Perft(2): %d\n", perft(2));
  printf("Perft(3): %d\n", perft(3));
  printf("Perft(4): %d\n", perft(4));
  printf("Perft(5): %d\n", perft(5));
  printf("Perft(6): %d\n", perft(6)); 
}
