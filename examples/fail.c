#include "scruffy/scruffy.h"
#include <stdio.h>
#include <stdlib.h>

TEST(scruffy, floats) {
  float a = 3.12;
  EXPECT_FLOAT_EQ(a, 3.1f);

  double b = 12.34567;
  EXPECT_DOUBLE_EQ(b, 12.345);
}

TEST(scruffy, equals) {
  EXPECT_EQ(1, 2);
}

// calling a function
bool test_true() { return true; }

TEST(scruffy, bools) {
  EXPECT_TRUE(test_true());
  EXPECT_FALSE(test_true());
  EXPECT_TRUE(1 == 2);
  EXPECT_FALSE(1 == 1);
}

TEST(scruffy, pointers) {
  int *p = NULL;
  EXPECT_NOT_NULL(p);

  int a = 3;
  p     = &a;
  EXPECT_NULL(p);
}

RUN_ALL();