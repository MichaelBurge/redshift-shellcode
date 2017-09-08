int x(int l);
static int y(int x);
static int factorial(int x);

int x(int l) {
  int ret = 0;
  for (int i = 0; i < l; i++) {
    ret += factorial(i);
  }
  return ret;
}
// 1 + 1 + 2 + 6 + 24 = 33

static int y(int x) { return x+42; }
static int factorial(int x) {
  if (x == 0) { return 1; }
  return x * factorial(x-1);
}
