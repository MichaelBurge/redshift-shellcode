// gcc -Os simple-4.c -c -fno-jump-tables
int prime(int n) {
  switch (n) {
  case 1: return 2;
  case 2: return 3;
  case 3: return 5;
  case 4: return 7;
  case 5: return 11;
  case 6: return 13;
  case 7: return 17;
  case 8: return 19;
  default: __builtin_trap();
  }
}
