#include "scruffy/scruffy.h"
#include <stdio.h>
#include <stdlib.h>

TEST(scruffy, floats) {
  float a = 3.12;
  EXPECT_FLOAT_EQ(a, 3.12f);

  double b = 12.34567;
  EXPECT_DOUBLE_EQ(b, 12.34567);
}

TEST(scruffy, equals) {
  EXPECT_EQ(1, 1);
}

TEST(scruffy, bools) {
  EXPECT_TRUE(1 == 1);
  EXPECT_FALSE(1 == 2);
}

TEST(scruffy, pointers) {
  int *p = NULL;
  EXPECT_NULL(p);

  int a = 3;
  p     = &a;
  EXPECT_NOT_NULL(p);
}

RUN_ALL();