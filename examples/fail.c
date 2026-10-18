#include "scruffy/scruffy.h"
#include <stdio.h>
#include <stdlib.h>

TEST(scruffy_fail, floats) {
  float a = 3.12;
  EXPECT_FLOAT_EQ(a, 3.1f);

  double b = 12.34567;
  EXPECT_DOUBLE_EQ(b, 12.345);
}

TEST(scruffy_fail, equals) {
  EXPECT_EQ(1, 2);
}

// calling a function
bool test_true() { return true; }

TEST(scruffy_fail, bools) {
  EXPECT_TRUE(test_true());
  EXPECT_FALSE(test_true());
  EXPECT_TRUE(1 == 2);
  EXPECT_FALSE(1 == 1);
}

TEST(scruffy_fail, pointers) {
  int *p = NULL;
  EXPECT_NOT_NULL(p);

  int a = 3;
  p     = &a;
  EXPECT_NULL(p);
}
