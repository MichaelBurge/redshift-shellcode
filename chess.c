#include <stdint.h>

#ifdef DEBUG
#define abort() __builtin_trap();
//#define abort() throw "derp";
#define private static __attribute__((always_inline)) inline
#else
#define private static __attribute__((always_inline)) inline
#define abort() __builtin_unreachable();
#endif

// bb is 'bitboard'
typedef struct gamestate {
  uint64_t rooks_bb;
  uint64_t knights_bb;
  uint64_t bishops_bb;
  uint64_t queens_bb;
  uint64_t kings_bb;
  uint64_t pawns_bb;
  union {
    uint64_t current_player_bb;
    uint64_t current_piece_bb; // For iterators
  };
  int en_passant_sq;
  union {
    uint64_t castle_flags;
    uint64_t promotion_piece; // For iterators
  };
} gamestate;

struct move {
  int from;
  int to;
};

typedef gamestate iterator;

const int PIECE_EMPTY  = 0;
const int PIECE_ROOK   = 1;
const int PIECE_KNIGHT = 2;
const int PIECE_BISHOP = 3;
const int PIECE_QUEEN  = 4;
const int PIECE_KING   = 5;
const int PIECE_PAWN   = 6;

const int VALUE_PAWN   = 100;
const int VALUE_KNIGHT = 300;
const int VALUE_BISHOP = 300;
const int VALUE_ROOK   = 500;
const int VALUE_QUEEN  = 900;
const int VALUE_AVAILABLE_MOVE = 5;
const int VALUE_CHECKMATE = -1000000; 
const int VALUE_NEGAMAX_START = 0x8F000000;
const int VALUE_CENTER = 10;

const int DIRECTION_EAST  = 0;
const int DIRECTION_WEST  = 1;
const int DIRECTION_NORTH = 2;
const int DIRECTION_SOUTH = 3;
const int DIRECTION_NORTHEAST = 4;
const int DIRECTION_NORTHWEST = 5;
const int DIRECTION_SOUTHEAST = 6;
const int DIRECTION_SOUTHWEST = 7;

// CASTLE FLAGS
const int CASTLE_WHITE_KINGSIDE = 0x1;
const int CASTLE_WHITE_QUEENSIDE = 0x2;
const int CASTLE_BLACK_KINGSIDE = 0x4;
const int CASTLE_BLACK_QUEENSIDE = 0x8;

const int CASTLE_KINGSIDE_KPOS = 6;
const int CASTLE_QUEENSIDE_KPOS = 2;
const int CASTLE_KINGSIDE_RPOS = 5;
const int CASTLE_QUEENSIDE_RPOS = 3;

const int NUM_BOARD_SQUARES = 8*8;

const int RANK = 8;

const int POSITION_INVALID = 255;

const uint64_t RAY_A1A8 = 0x0101010101010101;
const uint64_t RAY_H1H8 = RAY_A1A8 << (RANK-1);

// Private functions

private uint64_t bit(uint64_t idx) {
  if (idx >= 64 || idx < 0) {
    return 0;
  }
  return ((uint64_t)(1) << idx);
}
private uint64_t clear_bit(uint64_t x, uint64_t idx) { return x & ~bit(idx); }
private bool is_bit_set(uint64_t x, uint64_t idx) {
  if (idx >= 64) { return false; }
  return x & bit(idx);
}
private uint64_t set_bit(uint64_t x, uint64_t idx) { return x | bit(idx); }
private uint64_t lsb_first_set(uint64_t x) { return __builtin_ctzll(x); }
private uint64_t msb_first_set(uint64_t x) { return (63 - __builtin_clzll(x)); }

private int rank(int x) { return x / 8; }
private int file(int x) { return x % 8; }
private uint64_t offset(uint64_t x, int os)
{
#ifdef DEBUG
  if (os >= 64 || os <= -64) {
    throw "???";
  }
#endif
  if (os < 0) {
    return (x >> -os);
  } else {
    return (x << os);
  }
}

private gamestate zerostate()
{
  gamestate x;
  x.current_piece_bb = 0;
  x.rooks_bb = 0;
  x.knights_bb = 0;
  x.bishops_bb = 0;
  x.queens_bb = 0;
  x.kings_bb = 0;
  x.pawns_bb = 0;
  x.en_passant_sq = POSITION_INVALID;
  x.castle_flags = 0;
  return x;
}


private uint64_t mkPosition(int file, int rank)
{
  return rank * RANK + file;
}

const uint64_t RAY_A1H8 =
  bit(mkPosition(0,0)) |
  bit(mkPosition(1,1)) |
  bit(mkPosition(2,2)) |
  bit(mkPosition(3,3)) |
  bit(mkPosition(4,4)) |
  bit(mkPosition(5,5)) |
  bit(mkPosition(6,6)) |
  bit(mkPosition(7,7))
  ;

#ifdef DEBUG
#include <stdio.h>
void print_pos(int pos) {
  printf("%c%d", file(pos) + 'a', rank(pos)+1);
}
private void print_bitboard(uint64_t bb)
{
  for (int r = 8; r --> 0;) {
    for (int f = 0; f < 8; f++) {
      char c = is_bit_set(bb, mkPosition(f, r)) ? '1' : '0';
      putchar(c);
    }
    putchar('\n');
  }
}
#endif

// https://chessprogramming.wikispaces.com/On+an+empty+Board#RayAttacks
private uint64_t diagonal_mask(int center)
{
   const uint64_t maindia = 0x8040201008040201;
   int diag =8*(center & 7) - (center & 56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

private uint64_t antidiagonal_mask(int center)
{
  const uint64_t maindia = 0x0102040810204080;
  int diag =56- 8*(center&7) - (center&56);
  int nort = -diag & ( diag >> 31);
  int sout =  diag & (-diag >> 31);
  return (maindia >> sout) << nort;
}

// https://chessprogramming.wikispaces.com/Flipping+Mirroring+and+Rotating
// See 'flipDiagA1H8'
private uint64_t flipDiagA1H8(uint64_t x)
{
  uint64_t t;
  const uint64_t k1 = 0x5500550055005500;
  const uint64_t k2 = 0x3333000033330000;
  const uint64_t k4 = 0x0f0f0f0f00000000;
  t  = k4 & (x ^ (x << 28));
  x ^=       t ^ (t >> 28) ;
  t  = k2 & (x ^ (x << 14));
  x ^=       t ^ (t >> 14) ;
  t  = k1 & (x ^ (x <<  7));
  x ^=       t ^ (t >>  7) ;
  return x;
}

private uint64_t mirror_horizontal (uint64_t x) {
   const uint64_t k1 = 0x5555555555555555;
   const uint64_t k2 = 0x3333333333333333;
   const uint64_t k4 = 0x0f0f0f0f0f0f0f0f;
   x = ((x >> 1) & k1) +  2*(x & k1);
   x = ((x >> 2) & k2) +  4*(x & k2);
   x = ((x >> 4) & k4) + 16*(x & k4);
   return x;
}

private uint64_t flip_vertical(uint64_t x)
{
  return
    ( (x << 56)                        ) |
    ( (x << 40) & (0x00ff000000000000) ) |
    ( (x << 24) & (0x0000ff0000000000) ) |
    ( (x <<  8) & (0x000000ff00000000) ) |
    ( (x >>  8) & (0x00000000ff000000) ) |
    ( (x >> 24) & (0x0000000000ff0000) ) |
    ( (x >> 40) & (0x000000000000ff00) ) |
    ( (x >> 56)                        );
}

// We use a monochrome board, so white is always the one to move, and we flip the board after each move.
private uint64_t flip_bb(uint64_t x)
{
  return __builtin_bswap64(x);
  //return mirror_horizontal(__builtin_bswap64(x));
}

private uint64_t rotate_bb(uint64_t x)
{
  return flip_vertical(flipDiagA1H8(x));
}

const uint64_t RAY_A8H1 = rotate_bb(RAY_A1H8);

private uint64_t get_piece_bb(gamestate x, int piece)
{
  switch (piece) {
  case PIECE_ROOK:
    return x.rooks_bb;
  case PIECE_KNIGHT:
    return x.knights_bb;
  case PIECE_BISHOP:
    return x.bishops_bb;
  case PIECE_QUEEN:
    return x.queens_bb;
  case PIECE_KING:
    return x.kings_bb;
  case PIECE_PAWN:
    return x.pawns_bb;
  }
  abort();
}

private gamestate set_piece_bb(gamestate x, int piece, uint64_t bb)
{
  switch (piece) {
  case PIECE_ROOK:
    x.rooks_bb = bb;
    return x;
  case PIECE_KNIGHT:
    x.knights_bb = bb;
    return x;
  case PIECE_BISHOP:
    x.bishops_bb = bb;
    return x;
  case PIECE_QUEEN:
    x.queens_bb = bb;
    return x;
  case PIECE_KING:
    x.kings_bb = bb;
    return x;
  case PIECE_PAWN:
    x.pawns_bb = bb;
    return x;
  default:
    abort();
  }
}

private int next_iterator_piece(iterator x)
{
  if (x.rooks_bb)
    return PIECE_ROOK;
  if (x.knights_bb)
    return PIECE_KNIGHT;
  if (x.bishops_bb)
    return PIECE_BISHOP;
  if (x.queens_bb)
    return PIECE_QUEEN;
  if (x.kings_bb)
    return PIECE_KING;
  if (x.pawns_bb)
    return PIECE_PAWN;
  return PIECE_EMPTY;
}

private int iterator_position(iterator x)
{
  int piece = next_iterator_piece(x);
  if (piece == PIECE_EMPTY) {
    return POSITION_INVALID;
  } 
  uint64_t piece_bb = get_piece_bb(x, piece);
  int idx = lsb_first_set(piece_bb);
  return idx;
}

private int promotion_piece(move m)
{
  return (m.to >> 6);
}

private move dereference_iterator(iterator i)
{
  move m;
  m.from = iterator_position(i);
  m.to = lsb_first_set(i.current_piece_bb);
  m.to |= (i.promotion_piece << 6);
  return m;
}

private uint64_t advance_bb_iterator(uint64_t x)
{
  return x & (x-1);
}

private bool is_iterator_finished(iterator x)
{
  return
    ! x.rooks_bb &&
    ! x.knights_bb &&
    ! x.bishops_bb &&
    ! x.queens_bb &&
    ! x.kings_bb &&
    ! x.pawns_bb &&
    ! x.current_piece_bb
    ;
}

// When a piece is moving west, it may wrap around to be east.
// So we mask out the eastmost column, since it's impossible anyways.
private uint64_t guard_west(uint64_t x) {
  return x & ~RAY_H1H8;
}

private uint64_t guard_east(uint64_t x) {
  return x & ~RAY_A1A8;
}

private int move_direction(int idx, int direction)
{
  if (idx < 0 || idx >= 64) {
    return POSITION_INVALID;
  }
  switch (direction) {
  case DIRECTION_EAST:
    if (idx % 8 == 7)
      return POSITION_INVALID;
    return idx + 1;
  case DIRECTION_WEST:
    if (idx % 8 == 0)
      return POSITION_INVALID;
    return idx - 1;
  case DIRECTION_NORTH:
    return idx + RANK;
  case DIRECTION_SOUTH:
    return idx - RANK;
  case DIRECTION_NORTHEAST:
    if (file(idx) == 7) {
      return POSITION_INVALID;
    }
    return (idx + RANK + 1);
  case DIRECTION_NORTHWEST:
    if (file(idx) == 0) {
      return POSITION_INVALID;
    }
    return (idx + RANK - 1);
  case DIRECTION_SOUTHEAST:
    if (file(idx) == 7) {
      return POSITION_INVALID;
    }
    return (idx - RANK + 1);
  case DIRECTION_SOUTHWEST:
    if (file(idx) == 0) {
      return POSITION_INVALID;
    }
    return (idx - RANK - 1);
  default:
    abort();
  }
}

private uint64_t all_pieces(gamestate x)
{
  return
    x.rooks_bb |
    x.knights_bb |
    x.bishops_bb |
    x.queens_bb |
    x.kings_bb |
    x.pawns_bb
    ;
}

private uint64_t enemy_pieces(gamestate x)
{
  return all_pieces(x) ^ x.current_player_bb;
}

private uint64_t valid_pawn_moves(gamestate x, int center)
{
  uint64_t moves_bb = 0;
  // Non-captures
  if (! is_bit_set(all_pieces(x), center + RANK)) {
    uint64_t noncaptures_bb = bit(center + RANK);
    uint64_t pcs = all_pieces(x);
    if (rank(center) == 1 && ! is_bit_set(pcs, center + RANK) && ! is_bit_set(pcs, center + RANK*2)) {
      noncaptures_bb |= bit(center + RANK + RANK);
    }
    moves_bb |= noncaptures_bb;
  }
  // Captures
  {
    uint64_t captures_bb = 0;
    if (file(center) > 0) {
      captures_bb |= bit(move_direction(center + RANK, DIRECTION_WEST));
    }
    if (file(center) < 7) {
      captures_bb |= bit(move_direction(center + RANK, DIRECTION_EAST));
    }
    captures_bb &= all_pieces(x) ^ x.current_player_bb;
    if ((file(center) < 7 && x.en_passant_sq == center + RANK + 1) ||
        (file(center) > 0 && x.en_passant_sq == center + RANK - 1)) {
      captures_bb |= bit(x.en_passant_sq);
    }
    moves_bb |= captures_bb;
  }
  return moves_bb;
}

private uint64_t valid_knight_moves(gamestate x, int idx)
{
  uint64_t moves_bb = 0;
  moves_bb |= guard_west(bit(move_direction(idx + 2 * RANK, DIRECTION_WEST)));
  moves_bb |= guard_east(bit(move_direction(idx + 2 * RANK, DIRECTION_EAST)));
  
  moves_bb |= guard_west(bit(move_direction(idx - 2 * RANK, DIRECTION_WEST)));
  moves_bb |= guard_east(bit(move_direction(idx - 2 * RANK, DIRECTION_EAST)));
  
  moves_bb |= bit(move_direction(move_direction(move_direction(idx, DIRECTION_EAST), DIRECTION_EAST), DIRECTION_NORTH));
  moves_bb |= bit(move_direction(move_direction(move_direction(idx, DIRECTION_EAST), DIRECTION_EAST), DIRECTION_SOUTH));

  moves_bb |= bit(move_direction(move_direction(move_direction(idx, DIRECTION_WEST), DIRECTION_WEST), DIRECTION_NORTH));
  moves_bb |= bit(move_direction(move_direction(move_direction(idx, DIRECTION_WEST), DIRECTION_WEST), DIRECTION_SOUTH));

  moves_bb &= ~x.current_player_bb;
  return moves_bb;
}

private uint64_t bitrange(int start, int end)
{
  uint64_t end_bb = bit(end+1)-1;
  uint64_t start_bb = bit(start)-1;
  return end_bb ^ start_bb;
}

private uint64_t mkRay(int center, int direction)
{
  int rOs, fOs;
  switch (direction) {
  case DIRECTION_NORTHWEST: rOs =  1; fOs = -1; break;
  case DIRECTION_NORTH:     rOs =  1; fOs =  0; break;
  case DIRECTION_NORTHEAST: rOs =  1; fOs =  1; break;
  case DIRECTION_EAST:      rOs =  0; fOs =  1; break;
  case DIRECTION_SOUTHEAST: rOs = -1; fOs =  1; break;
  case DIRECTION_SOUTH:     rOs = -1; fOs =  0; break;
  case DIRECTION_SOUTHWEST: rOs = -1; fOs = -1; break;
  case DIRECTION_WEST:      rOs =  0; fOs = -1; break;
  default: abort();
  }
  uint64_t ray;
  uint64_t next = bit(center);
  do {
    ray = next;
    next = offset(next, RANK * rOs);
    next = offset(next, fOs);
    switch (fOs) {
    case 1:  next &= ~RAY_A1A8; break;
    case -1: next &= ~RAY_H1H8; break;
    default: break;
    }
    next |= ray;
  } while (ray != next);
  ray = clear_bit(ray, center);
  return ray;
}

private int closest_blocker(uint64_t blockers_ray, int direction)
{
  if (blockers_ray == 0)
    return POSITION_INVALID;
  switch (direction) {
  case DIRECTION_NORTHWEST:
  case DIRECTION_NORTH:
  case DIRECTION_NORTHEAST:
  case DIRECTION_EAST:
    return lsb_first_set(blockers_ray);
  case DIRECTION_SOUTHEAST:
  case DIRECTION_SOUTH:
  case DIRECTION_SOUTHWEST:
  case DIRECTION_WEST:
    return msb_first_set(blockers_ray);
  default:
    abort();
  }  
}

private uint64_t shoot_ray_until_blocker(gamestate state, int idx, int direction)
{
  uint64_t pieces = all_pieces(state);
  uint64_t base_ray = mkRay(idx, direction);
  uint64_t blockers = base_ray & pieces;
  int blocker = closest_blocker(blockers, direction);
  if (blocker == POSITION_INVALID) {
    return base_ray;
  } else {
    uint64_t blocker_ray = mkRay(blocker, direction);
    uint64_t movable_squares_without_capture = base_ray ^ blocker_ray;
    bool allow_capture = ! is_bit_set(state.current_player_bb, blocker);
    if (allow_capture)
      return movable_squares_without_capture | bit(blocker);
    else
      return movable_squares_without_capture & ~bit(blocker);
  }
}

private uint64_t valid_bishop_moves(gamestate x, int idx)
{
  return
    shoot_ray_until_blocker(x, idx, DIRECTION_NORTHEAST) |
    shoot_ray_until_blocker(x, idx, DIRECTION_NORTHWEST) |
    shoot_ray_until_blocker(x, idx, DIRECTION_SOUTHEAST) |
    shoot_ray_until_blocker(x, idx, DIRECTION_SOUTHWEST)
    ;
}

private uint64_t valid_rook_moves(gamestate x, int idx)
{
  return
    shoot_ray_until_blocker(x, idx, DIRECTION_NORTH) |
    shoot_ray_until_blocker(x, idx, DIRECTION_WEST) |
    shoot_ray_until_blocker(x, idx, DIRECTION_EAST) |
    shoot_ray_until_blocker(x, idx, DIRECTION_SOUTH)
    ;
}

private uint64_t valid_king_moves(gamestate g, int idx)
{
  uint64_t ret =
    bit(move_direction(idx, DIRECTION_EAST)) |
    bit(move_direction(idx, DIRECTION_WEST)) |
    bit(move_direction(idx, DIRECTION_NORTH)) |
    bit(move_direction(idx, DIRECTION_SOUTH)) |
    bit(move_direction(idx, DIRECTION_NORTHEAST)) |
    bit(move_direction(idx, DIRECTION_SOUTHEAST)) |
    bit(move_direction(idx, DIRECTION_NORTHWEST)) |
    bit(move_direction(idx, DIRECTION_SOUTHWEST))
    ;
  if ((g.castle_flags & CASTLE_WHITE_KINGSIDE)) {
    uint64_t pcs = all_pieces(g);
    if (is_bit_set(pcs, CASTLE_KINGSIDE_KPOS) ||
        is_bit_set(pcs, CASTLE_KINGSIDE_RPOS)) {
      goto skip_kingside;
    }
    ret |= bit(CASTLE_KINGSIDE_KPOS);
  }
 skip_kingside:
  if ((g.castle_flags & CASTLE_WHITE_QUEENSIDE)) {
    uint64_t pcs = all_pieces(g);
    if (is_bit_set(pcs, CASTLE_QUEENSIDE_RPOS) ||
        is_bit_set(pcs, CASTLE_QUEENSIDE_KPOS) ||
        is_bit_set(pcs, CASTLE_QUEENSIDE_KPOS-1)) {
      goto skip_queenside;
    }
    ret |= bit(CASTLE_QUEENSIDE_KPOS);
  }
 skip_queenside:
  ret &= ~ g.current_piece_bb;
  return ret;
}

private uint64_t valid_queen_moves(gamestate x, int idx)
{
  return
    valid_bishop_moves(x, idx) |
    valid_rook_moves(x, idx)
    ;
}

private int get_piece(gamestate x, int idx)
{
  if (is_bit_set(x.rooks_bb, idx))
    return PIECE_ROOK;
  if (is_bit_set(x.knights_bb, idx))
    return PIECE_KNIGHT;
  if (is_bit_set(x.bishops_bb, idx))
    return PIECE_BISHOP;
  if (is_bit_set(x.queens_bb, idx))
    return PIECE_QUEEN;
  if (is_bit_set(x.kings_bb, idx))
    return PIECE_KING;
  if (is_bit_set(x.pawns_bb, idx))
    return PIECE_PAWN;
  return PIECE_EMPTY;
}

private uint64_t valid_piece_moves(gamestate x, int idx)
{
  int piece = get_piece(x, idx);
  switch (piece) {
  case PIECE_ROOK:
    return valid_rook_moves(x, idx);
  case PIECE_KNIGHT:
    return valid_knight_moves(x, idx);
  case PIECE_BISHOP:
    return valid_bishop_moves(x, idx);
  case PIECE_QUEEN:
    return valid_queen_moves(x, idx);
  case PIECE_KING:
    return valid_king_moves(x, idx);
  case PIECE_PAWN:
    return valid_pawn_moves(x, idx);
  default:
    abort();
  }
}

private iterator reset_iterator_promotion_piece(gamestate g, iterator i)
{
  if (is_iterator_finished(i)) {
    return i;
  }
  int idx = iterator_position(i);
  int piece = get_piece(g, idx);
  if (piece == PIECE_PAWN &&
      rank(idx) == 6 &&
      i.current_piece_bb != 0) {
    i.promotion_piece = PIECE_QUEEN;
  }
  return i;  
}

private iterator reset_iterator_moves(gamestate g, iterator i)
{
  if (is_iterator_finished(i)) {
    return zerostate();
  } else {
    int idx = iterator_position(i);
    uint64_t moves = valid_piece_moves(g, idx);
    i.current_piece_bb = moves;
    i = reset_iterator_promotion_piece(g, i);
    return i;
  }
}

private iterator advance_iterator(gamestate g, iterator i)
{
  if (i.current_piece_bb) {
    if (i.promotion_piece) {
      i.promotion_piece--;
    }
    if (!i.promotion_piece) {
      i.current_piece_bb = advance_bb_iterator(i.current_piece_bb);
      i = reset_iterator_promotion_piece(g, i);
    }
  }

  while (! is_iterator_finished(i) && ! i.current_piece_bb) {
    if (i.rooks_bb) {
      i.rooks_bb = advance_bb_iterator(i.rooks_bb);
    } else if (i.knights_bb) {
      i.knights_bb = advance_bb_iterator(i.knights_bb);
    } else if (i.bishops_bb) {
      i.bishops_bb = advance_bb_iterator(i.bishops_bb);
    } else if (i.queens_bb) {
      i.queens_bb = advance_bb_iterator(i.queens_bb);
    } else if (i.kings_bb) {
      i.kings_bb = advance_bb_iterator(i.kings_bb);
    } else if (i.pawns_bb) {
      i.pawns_bb = advance_bb_iterator(i.pawns_bb);
    }
    i = reset_iterator_moves(g, i);
  }
  if (is_iterator_finished(i) && i.current_piece_bb != 0) {
    abort();
  }
  
  return i;
}

private gamestate switch_sides(gamestate x)
{
  uint64_t enemy_pieces_bb = enemy_pieces(x);
  x.current_player_bb = enemy_pieces_bb;

  x.rooks_bb   = __builtin_bswap64(x.rooks_bb);
  x.knights_bb = __builtin_bswap64(x.knights_bb);
  x.bishops_bb = __builtin_bswap64(x.bishops_bb);
  x.queens_bb  = __builtin_bswap64(x.queens_bb);
  x.kings_bb   = __builtin_bswap64(x.kings_bb);
  x.pawns_bb   = __builtin_bswap64(x.pawns_bb);
  return x;
}
                 
private iterator mkIterator(gamestate g)
{
  iterator x = g;
  x.rooks_bb   &= x.current_player_bb;
  x.knights_bb &= x.current_player_bb;
  x.bishops_bb &= x.current_player_bb;
  x.queens_bb  &= x.current_player_bb;
  x.kings_bb   &= x.current_player_bb;
  x.pawns_bb   &= x.current_player_bb;
  x.promotion_piece = 0;
  
  x = reset_iterator_moves(g, x);
  if (! x.current_piece_bb) {
    x = advance_iterator(g, x);
  }
  
  return x;
}

private uint64_t all_rotations(uint64_t bb)
{
  bb |= rotate_bb(bb);
  bb |= rotate_bb(bb);
  bb |= rotate_bb(bb);
  return bb;
}

private gamestate new_game()
{
  gamestate x;
  x.rooks_bb =
    bit(mkPosition(0,0)) |
    bit(mkPosition(7,0)) |
    bit(mkPosition(0,7)) |
    bit(mkPosition(7,7));
  x.knights_bb =
    bit(mkPosition(1,0)) |
    bit(mkPosition(6,0)) |
    bit(mkPosition(1,7)) |
    bit(mkPosition(6,7));
  x.bishops_bb =
    bit(mkPosition(2,0)) |
    bit(mkPosition(5,0)) |
    bit(mkPosition(2,7)) |
    bit(mkPosition(5,7));
  x.queens_bb =
    bit(mkPosition(3,0)) |
    bit(mkPosition(3,7));
  x.kings_bb =
    bit(mkPosition(4,0)) |
    bit(mkPosition(4,7));
  x.pawns_bb =
    ((uint64_t)0xFF << RANK) |
    ((uint64_t)0xFF << (6*RANK));
  x.current_player_bb = 0xFFFF;
  x.en_passant_sq = POSITION_INVALID;
  x.castle_flags = 0xF;
  return x;
}

// Flips the board so white is black and black is white.
// In our model, it is always white's turn to move.
private gamestate swap_board(gamestate g)
{
  g.current_player_bb ^= all_pieces(g);
  g.rooks_bb = flip_bb(g.rooks_bb);
  g.knights_bb = flip_bb(g.knights_bb);
  g.bishops_bb = flip_bb(g.bishops_bb);
  g.queens_bb = flip_bb(g.queens_bb);
  g.kings_bb = flip_bb(g.kings_bb);
  g.pawns_bb = flip_bb(g.pawns_bb);
  g.current_player_bb = flip_bb(g.current_player_bb);
  if (g.en_passant_sq != POSITION_INVALID) {
    g.en_passant_sq = mkPosition(file(g.en_passant_sq), 7 - rank(g.en_passant_sq));
  }
  {
    int flags = 0;
    if (g.castle_flags & CASTLE_WHITE_KINGSIDE)
      flags |= CASTLE_BLACK_KINGSIDE;
    if (g.castle_flags & CASTLE_WHITE_QUEENSIDE)
      flags |= CASTLE_BLACK_QUEENSIDE;
    if (g.castle_flags & CASTLE_BLACK_KINGSIDE)
      flags |= CASTLE_WHITE_KINGSIDE;
    if (g.castle_flags & CASTLE_BLACK_QUEENSIDE)
      flags |= CASTLE_WHITE_QUEENSIDE;
    g.castle_flags = flags;
  }
  return g;
}

private bool is_kingside_castle(gamestate g, move m)
{
  int from_piece = get_piece(g, m.from);
  return
    from_piece == PIECE_KING &&
    m.to == CASTLE_KINGSIDE_KPOS &&
    (g.castle_flags & CASTLE_WHITE_KINGSIDE);
}

private bool is_queenside_castle(gamestate g, move m)
{
  int from_piece = get_piece(g, m.from);
  return
    from_piece == PIECE_KING &&
    m.to == CASTLE_QUEENSIDE_KPOS &&
    (g.castle_flags & CASTLE_WHITE_QUEENSIDE);
}

private bool is_promotion(move m) { return promotion_piece(m) == PIECE_EMPTY; }

private gamestate apply_move(gamestate g, move m)
{
  gamestate g_orig = g;
  int from_piece = get_piece(g, m.from);
  int prom_piece = promotion_piece(m);
  m.to &= 0x3F;
  int to_piece = get_piece(g, m.to);
  if (to_piece != PIECE_EMPTY) {
    uint64_t to_bb = get_piece_bb(g, to_piece);
    g = set_piece_bb(g, to_piece, clear_bit(to_bb, m.to));
  }
  // Non-promotion moves
  if (prom_piece == PIECE_EMPTY) {
    uint64_t from_bb = get_piece_bb(g, from_piece);
    g = set_piece_bb(g, from_piece, clear_bit(from_bb, m.from) | bit(m.to));
    // Promotions
  } else {
    uint64_t piece_bb = get_piece_bb(g, prom_piece);
    g.pawns_bb = clear_bit(g.pawns_bb, m.from);
    g = set_piece_bb(g, prom_piece, piece_bb | bit(m.to));
  }
  // Capture the pawn properly during En Passant capture
  if (from_piece == PIECE_PAWN && m.to == g.en_passant_sq) {
    g.pawns_bb = clear_bit(g.pawns_bb, g.en_passant_sq - RANK);
  }
  // Set En Passant target square on double-jump
  if (from_piece == PIECE_PAWN && rank(m.to) - rank(m.from) == 2) {
    g.en_passant_sq = m.from + RANK;
  } else {
    g.en_passant_sq = POSITION_INVALID;
  }
  // Check for kingside castle.
  if (is_kingside_castle(g_orig, m)) {
    g.castle_flags &= ~CASTLE_WHITE_KINGSIDE;
    g.rooks_bb = clear_bit(g.rooks_bb, mkPosition(7,0));
    g.current_piece_bb = clear_bit(g.current_piece_bb, mkPosition(7,0));
    g.rooks_bb = set_bit(g.rooks_bb, CASTLE_KINGSIDE_RPOS);
    g.current_piece_bb = set_bit(g.current_piece_bb, CASTLE_KINGSIDE_RPOS);
  }
  // Check for queenside castle
  if (is_queenside_castle(g_orig,m)) {
    g.castle_flags &= ~CASTLE_WHITE_QUEENSIDE;
    g.rooks_bb = clear_bit(g.rooks_bb, mkPosition(0,0));
    g.current_piece_bb = clear_bit(g.current_piece_bb, mkPosition(0,0));
    g.rooks_bb = set_bit(g.rooks_bb, CASTLE_QUEENSIDE_RPOS);
    g.current_piece_bb = set_bit(g.current_piece_bb, CASTLE_QUEENSIDE_RPOS);
  }
  // Clear kingside rook flags
  if (g.castle_flags & CASTLE_WHITE_KINGSIDE &&
      from_piece == PIECE_ROOK &&
      m.from == mkPosition(7,0)) {
    g.castle_flags &= ~CASTLE_WHITE_KINGSIDE;
  }
  // Clear queenside rook flags
  if (g.castle_flags & CASTLE_WHITE_QUEENSIDE &&
      from_piece == PIECE_ROOK &&
      m.from == mkPosition(0,0)) {
    g.castle_flags &= ~CASTLE_WHITE_QUEENSIDE;
  }
  // Clear eaten kingside rook flags
  if (to_piece == PIECE_ROOK &&
      m.to == mkPosition(7,7)) {
    g.castle_flags &= ~CASTLE_BLACK_KINGSIDE;
  }
  // Clear eaten queenside rook flags
  if (to_piece == PIECE_ROOK &&
      m.to == mkPosition(0,7)) {
    g.castle_flags &= ~CASTLE_BLACK_QUEENSIDE;
  }
  // Clear king's castling flags
  if (from_piece == PIECE_KING) {
    g.castle_flags &= ~(CASTLE_WHITE_KINGSIDE | CASTLE_WHITE_QUEENSIDE);
  }
  // Set colors
  g.current_player_bb = clear_bit(g.current_player_bb, m.from) | bit(m.to);
  
  return g;
}

private uint64_t mkRank(int idx) { return ((uint64_t)0xFF << (idx*RANK)); }
private uint64_t mkFile(int idx) {
  return
    bit(mkPosition(idx,0)) |
    bit(mkPosition(idx,1)) |
    bit(mkPosition(idx,2)) |
    bit(mkPosition(idx,3)) |
    bit(mkPosition(idx,4)) |
    bit(mkPosition(idx,5)) |
    bit(mkPosition(idx,6)) |
    bit(mkPosition(idx,7));
}

private bool iter_lt(iterator x, iterator y)
{
  return
    (x.rooks_bb < y.rooks_bb ||
     (x.rooks_bb == y.rooks_bb &&
      (x.knights_bb < y.knights_bb ||
       (x.knights_bb == y.knights_bb &&
        (x.bishops_bb < y.bishops_bb ||
         (x.bishops_bb == y.bishops_bb &&
          (x.queens_bb < y.queens_bb ||
           (x.queens_bb == y.queens_bb &&
            (x.kings_bb < y.kings_bb ||
             (x.kings_bb == y.kings_bb &&
              (x.pawns_bb < y.pawns_bb ||
               (x.pawns_bb == y.pawns_bb &&
                (x.current_piece_bb < y.current_piece_bb ||
                 (x.current_piece_bb == y.current_piece_bb &&
                  (x.promotion_piece < y.promotion_piece)))))))))))))));
}

private uint64_t movepoints(gamestate g)
{
  uint64_t movepoints = 0;
  iterator i = mkIterator(g);
  while (! is_iterator_finished(i)) {
    movepoints |= i.current_piece_bb;
    i.current_piece_bb = 0;
    i = advance_iterator(g, i);
  }
  return movepoints;
}

private uint64_t vulnerables(gamestate g)
{
  g = swap_board(g);
  uint64_t ret = movepoints(g) & enemy_pieces(g);
  ret = flip_bb(ret);
  return ret;
}

private bool is_in_check(gamestate g)
{
  uint64_t white_kings = g.kings_bb & g.current_player_bb;
  return ((white_kings & vulnerables(g)) != 0);
}

private bool results_in_check(gamestate g, move m)
{
  gamestate g2 = apply_move(g,m);
  return is_in_check(g2);
}

private bool is_legal(gamestate g, move m)
{
  if (results_in_check(g,m)) {
    return false;
  }
  move kingside_castle_part; kingside_castle_part.from = mkPosition(4,0); kingside_castle_part.to = mkPosition(5,0);
  move queenside_castle_part; queenside_castle_part.from = mkPosition(4,0); queenside_castle_part.to = mkPosition(3,0);
  bool valid_kingside_castle =
    is_kingside_castle(g, m)
    ? (! results_in_check(g, kingside_castle_part) &&
       ! is_in_check(g))
    : true;
  bool valid_queenside_castle =
    is_queenside_castle(g, m)
    ? (! results_in_check(g, queenside_castle_part) &&
       ! is_in_check(g))
    : true;
  return
    valid_kingside_castle &&
    valid_queenside_castle;
}

// Like advance_iterator, but skip moves that leave the king in check
private iterator advance_iterator_legal(gamestate g, iterator i)
{
  while (1) {
    i = advance_iterator(g, i);
    if (is_iterator_finished(i)) {
      break;
    }
    move m = dereference_iterator(i);
    if (is_legal(g, m)) {
      break;
    }
  }
  return i;
}

private iterator mkLegalIterator(gamestate g)
{
  iterator i = mkIterator(g);
  move m = dereference_iterator(i);
  if (! is_legal(g, m)) {
    i = advance_iterator_legal(g, i);
  }
  return i;
}

private uint64_t legal_movepoints(gamestate g)
{
  uint64_t movepoints = 0;
  iterator i = mkLegalIterator(g);
  while (! is_iterator_finished(i)) {
    move m = dereference_iterator(i);
    movepoints |= bit(m.to);
    i = advance_iterator_legal(g, i);
  }
  return movepoints;
}

private int num_legal_moves(gamestate g)
{
  iterator i = mkLegalIterator(g);
  int count = 0;
  while (! is_iterator_finished(i)) {
    i = advance_iterator_legal(g, i);
    count++;
  }
  return count;
}

private iterator legalize(gamestate g, iterator i)
{
  while (! is_iterator_finished(i)) {
    move m = dereference_iterator(i);
    if (is_legal(g, m)) {
      break;
    }
    i = advance_iterator(g, i);
  }
  return i;
}

private uint64_t piece_legal_movepoints(gamestate g, int idx)
{
  iterator i = zerostate();
  int piece = get_piece(g, idx);
  i = set_piece_bb(i, piece, bit(idx));
  i = reset_iterator_moves(g, i);
  i = legalize(g, i);
  uint64_t ret = 0;
  while (! is_iterator_finished(i)) {
    move m = dereference_iterator(i);
    ret |= bit(m.to);
    i = advance_iterator_legal(g, i);
  }
  return ret;
}

private move mkPromotion(gamestate g, int from, int piece)
{
  if (rank(from) != 6)
    abort();
  move m;
  m.from = from;
  m.to = from + RANK;
  m.to |= piece << 6;
  return m;
}

// Public functions

private int num_bits(uint64_t x) { return __builtin_popcountl(x); }

private int score_pieces(gamestate g)
{
  return
    num_bits(g.pawns_bb & g.current_player_bb) * VALUE_PAWN +
    num_bits(g.knights_bb & g.current_player_bb) * VALUE_KNIGHT +
    num_bits(g.bishops_bb & g.current_player_bb) * VALUE_BISHOP +
    num_bits(g.rooks_bb & g.current_player_bb) * VALUE_ROOK +
    num_bits(g.queens_bb & g.current_player_bb) * VALUE_QUEEN;
}

private int score_availability(gamestate g)
{
  int num_moves = num_legal_moves(g);
  if (num_moves == 0)
    return VALUE_CHECKMATE;
  return VALUE_AVAILABLE_MOVE * num_moves;
}

private uint64_t center()
{
  return
    bit(mkPosition(2,2)) |
    bit(mkPosition(2,3)) |
    bit(mkPosition(2,4)) |
    bit(mkPosition(2,5)) |
    
    bit(mkPosition(3,2)) |
    bit(mkPosition(3,3)) |
    bit(mkPosition(3,4)) |
    bit(mkPosition(3,5)) |
    
    bit(mkPosition(4,2)) |
    bit(mkPosition(4,3)) |
    bit(mkPosition(4,4)) |
    bit(mkPosition(4,5)) |
    
    bit(mkPosition(5,2)) |
    bit(mkPosition(5,3)) |
    bit(mkPosition(5,4)) |
    bit(mkPosition(5,5));    
}

private int score_center(gamestate g)
{
  int num_centers = num_bits(g.current_player_bb & center());
  return num_centers * VALUE_CENTER;
}

private int evaluate(gamestate g)
{
  return score_pieces(g) + score_availability(g);
}


typedef struct negamax_ret {
  move m;
  int score;
} negamax_ret;

template<int depth> private int negamax(gamestate g, int color);

template<> int negamax<0>(gamestate g, int color)
{
  return color * evaluate(g);
}

template<int depth> int negamax(gamestate g, int color)
{
  if (depth == 0)
    return color*evaluate(g);
  int max = VALUE_NEGAMAX_START;
  for (iterator i = mkLegalIterator(g); ! is_iterator_finished(i); i = advance_iterator_legal(g, i))  {
    move m = dereference_iterator(i);
    gamestate g2 = apply_move(g, m);
    int score = -negamax<depth-1>(g2, color*-1);
    if (score > max)
      max = score;
  }
  return max;
}

move best_move(gamestate g)
{
  int max = VALUE_NEGAMAX_START;
  move ret; ret.from = POSITION_INVALID; ret.to = POSITION_INVALID;
  for (iterator i = mkLegalIterator(g); ! is_iterator_finished(i); i = advance_iterator_legal(g, i)) {
    move m = dereference_iterator(i);
    gamestate g2 = apply_move(g, m);
    int score = negamax<3>(g, 1);
    if (score > max) {
      max = score;
      ret = m;
    }
  }
  return ret;
}

private int negamax_original(gamestate g1)
{
  // depth 1
  int max1 = VALUE_NEGAMAX_START;
  for (iterator i1 = mkLegalIterator(g1); ! is_iterator_finished(i1); i1 = advance_iterator_legal(g1, i1)) {
    move m1 = dereference_iterator(i1);
    gamestate g2 = apply_move(g1, m1);
    // depth 2
    int max2 = VALUE_NEGAMAX_START;
    for (iterator i2 = mkLegalIterator(g2); ! is_iterator_finished(i2); i2 = advance_iterator_legal(g2, i2)) {
      move m2 = dereference_iterator(i2);
      gamestate g3 = apply_move(g2, m2);
      // depth 3
      int max3 = VALUE_NEGAMAX_START;
      for (iterator i3 = mkLegalIterator(g3); ! is_iterator_finished(i3); i3 = advance_iterator_legal(g3, i3)) {
        move m3 = dereference_iterator(i3);
        gamestate g4 = apply_move(g3, m3);
        // depth 4
        int score = evaluate(g4);
        score = -score;
        if (score > max3) {
          max3 = score;
        }
      }
      max3 = -max3;
      if (max3 > max2) {
        max2 = max2;
      }
    }
    max2 = -max2;
    if (max2 > max1) {
      max1 = max2;
    }
  }
  return max1;
}
typedef int continuation;
private negamax_ret negamax(gamestate g1)
{
  // Arguments shared between all trampolines
  continuation tr_cont;
  gamestate tr_g;
  gamestate g2;
  gamestate g3;
  gamestate g4;
  move tr_m;
  iterator tr_i;
  iterator i1;
  iterator i2;
  iterator i3;
  int max1;
  int max2;
  int max3;
  int score;
  
  move best_move;
  move m1;
  move m2;
  move m3;
  goto begin_loop;
  // Trampoline return destinations
#define DEST_I1 0
#define DEST_G2 1
#define DEST_I2 2   
#define DEST_G3 3 
#define DEST_I3 4
#define DEST_G4 5
#define DEST_ADV1 6
#define DEST_ADV2 7
#define DEST_ADV3 8  

 apply_move_trampoline:
  tr_g = apply_move(tr_g, tr_m);
  goto exit_tr;
 mkLegalIterator_trampoline:
  tr_i = mkLegalIterator(tr_g);
  goto exit_tr;  
 advance_iterator_legal_trampoline:
  tr_i = advance_iterator_legal(tr_g, tr_i);
  goto exit_tr;
 exit_tr:
   switch (tr_cont) {
   case DEST_I1: goto dest_i1;
   case DEST_G2: goto dest_g2;
   case DEST_I2: goto dest_i2;
   case DEST_G3: goto dest_g3;
   case DEST_I3: goto dest_i3;
   case DEST_G4: goto dest_g4;
   case DEST_ADV1: goto dest_adv1;
   case DEST_ADV2: goto dest_adv2;
   case DEST_ADV3: goto dest_adv3;
   default: abort();
   }
 begin_loop:
  // depth 1
   max1 = VALUE_NEGAMAX_START;
  // trampolined: iterator i1 = mkLegalIterator(g1);
  tr_g = g1; tr_cont = DEST_I1; goto mkLegalIterator_trampoline;
 dest_i1: i1 = tr_i;
  while (! is_iterator_finished(i1)) {
    m1 = dereference_iterator(i1);
    // trampolined: gamestate g2 = apply_move(g1, m1);
    tr_g = g1; tr_m = m1; tr_cont = DEST_G2; goto apply_move_trampoline;
  dest_g2: g2 = tr_g;
    // depth 2
    max2 = VALUE_NEGAMAX_START;
    // trampolined: iterator i2 = mkLegalIterator(g2)
    tr_g = g2; tr_cont = DEST_I2; goto mkLegalIterator_trampoline;
  dest_i2: i2 = tr_i;
    while (! is_iterator_finished(i2)) {
      m2 = dereference_iterator(i2);
      // trampolined: gamestate g3 = apply_move(g2, m2);
      tr_g = g2; tr_m = m2; tr_cont = DEST_G3; goto apply_move_trampoline;
    dest_g3: g3 = tr_g;
      // depth 3
      max3 = VALUE_NEGAMAX_START;
      // trampolined: iterator i3 = mkLegalIterator(g3)
      tr_g = g3; tr_cont = DEST_I3; goto mkLegalIterator_trampoline;
    dest_i3: i3 = tr_i;
      while (! is_iterator_finished(i3)) {
        m3 = dereference_iterator(i3);
        // trampolined: gamestate g4 = apply_move(g3, m3);
        tr_g = g3; tr_m = m3; tr_cont = DEST_G4;  goto apply_move_trampoline;
      dest_g4: g4 = tr_g;
        // depth 4
        score = evaluate(g4);
        score = -score;
        if (score > max3) {
          max3 = score;
        }
        // trampolined: i3 = advance_iterator_legal(g3, i3)) {
        tr_g = g3; tr_i = i3; tr_cont = DEST_ADV3; goto advance_iterator_legal_trampoline;
      dest_adv3:;
      }
      max3 = -max3;
      if (max3 > max2) {
        max2 = max2;
      }
      // trampolined: i2 = advance_iterator_legal(i2, i2);
      tr_g = g2; tr_i = i2; tr_cont = DEST_ADV2; goto advance_iterator_legal_trampoline;
    dest_adv2:;
    }
    max2 = -max2;
    if (max2 > max1) {
      max1 = max2;
      best_move = m1;
    }
    // trampolined: i1 = advance_iterator_legal(g1, i1)
    tr_g = g1; tr_i = i1; tr_cont = DEST_ADV1; goto advance_iterator_legal_trampoline;
  dest_adv1:;
  }
  negamax_ret ret;
  ret.m = best_move;
  ret.score = max1;
  return ret;
}
