#include <scruffy/scruffy.h>
#include <unistd.h>
// #include <stdlib.h>

TEST(scruffy_pass, floats) {
  float a = 3.12;
  EXPECT_FLOAT_EQ(a, 3.12f);

  double b = 12.34567;
  EXPECT_DOUBLE_EQ(b, 12.34567);
  EXPECT_DOUBLE_EQ(b, 12.34567);
  EXPECT_DOUBLE_EQ(b, 12.34567);
  // sleep(2);
}

TEST(scruffy_pass, equals) {
  EXPECT_EQ(1, 1);
  EXPECT_EQ(-1, -1);
  EXPECT_EQ(1, 1);
}


TEST(scruffy_pass_alt, equals) {
  EXPECT_EQ(1, 1);
}


TEST(scruffy_pass, bools) {
  EXPECT_TRUE(1 == 1);
  EXPECT_FALSE(1 == 2);
  EXPECT_FALSE(1 == -1);
}

TEST(scruffy_pass, pointers) {
  int *p = NULL;
  EXPECT_NULL(p);

  int a = 3;
  p     = &a;
  EXPECT_NOT_NULL(p);
}

TEST(scruffy_pass, strings) {
  EXPECT_STREQ("hi", "hi");
  EXPECT_STREQ(NULL, NULL);
  EXPECT_STREQ(nullptr, nullptr);

  const char* s = "hello there brown cow!?";
  EXPECT_STREQ(s, "hello there brown cow!?");
}
