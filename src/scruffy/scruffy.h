
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once

#if defined __cplusplus
extern "C" {
#endif

// For environments that don't support color
// #define RED (getenv("NO_COLOR") ? "" : "\033[31m")
// #define GREEN (getenv("NO_COLOR") ? "" : "\033[32m")
// #define YELLOW (getenv("NO_COLOR") ? "" : "\033[33m")
// #define CYAN (getenv("NO_COLOR") ? "" : "\033[36m")
// #define RESET (getenv("NO_COLOR") ? "" : "\033[0m")

// Color codes
#define SCRUFFY_RED "\033[31m"    // error
#define SCRUFFY_GREEN "\033[32m"  // success
#define SCRUFFY_YELLOW "\033[33m" // warning
#define SCRUFFY_CYAN "\033[36m"   // debug
#define SCRUFFY_RESET "\033[0m"

// Structure to hold test metadata
typedef struct {
  const char *suite_name;
  const char *test_name;
  void (*test_func)(void);
} TestCase;

// Global array to store test cases
#define SCRUFFY_MAX_TESTS 300
extern TestCase test_registry[SCRUFFY_MAX_TESTS];
extern size_t test_registry_size;
extern int total_test_count;
extern int total_test_failures;
extern int suite_failures;

static int get_time_diff(struct timespec start, struct timespec end) {
  return (int)((end.tv_sec - start.tv_sec) +
               (end.tv_nsec - start.tv_nsec) / 1e3);
}

// keep do loop - ensures test_failure++ is scoped with fprintf()
#define TEST_DEBUG_MESSAGE(format, ...)                                        \
  do {                                                                         \
    printf("\n%sfile: %s line: %d %s\n", SCRUFFY_CYAN, __FILE__, __LINE__,       \
           SCRUFFY_RESET);                                                     \
    printf(format "\n", ##__VA_ARGS__);                                        \
    total_test_failures++;                                                     \
  } while (0)

// keep do loop - I really don't know why it doesn't work without it
#define TEST_FAIL_MESSAGE(format, ...)                                         \
  do {                                                                         \
    printf("%s" format "%s\n", SCRUFFY_RED, ##__VA_ARGS__, SCRUFFY_RESET);     \
  } while (0)

#define TEST_PASS_MESSAGE(format, ...)                                         \
  do {                                                                         \
    printf("%s" format "%s\n", SCRUFFY_GREEN, ##__VA_ARGS__, SCRUFFY_RESET);   \
  } while (0)

// Pointers comparisons -----------------------------------
#define EXPECT_NULL(ptr)                                                       \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((ptr) != NULL) {                                                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) != NULL", #ptr);                       \
    }                                                                          \
  } while (0)

#define EXPECT_NOT_NULL(ptr)                                                   \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((ptr) == NULL) {                                                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == NULL", #ptr);                       \
    }                                                                          \
  } while (0)

// Binary -------------------------------------------------
#define EXPECT_TRUE(cond)                                                      \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((cond) == false) {                                                     \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == true", #cond);                      \
    }                                                                          \
  } while (0)

#define EXPECT_FALSE(cond)                                                     \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((cond) == true) {                                                      \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == false", #cond);                     \
    }                                                                          \
  } while (0)

#define EXPECT_EQ(actual, expected)                                            \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((actual) != (expected)) {                                              \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected %s == %s, got %lld != %lld", #actual,       \
                         #expected, (long long)(actual),                       \
                         (long long)(expected));                               \
    }                                                                          \
  } while (0)

// Floating-point equality --------------------------------
#define EXPECT_FLOAT_EQ(actual, expected)                                      \
  do {                                                                         \
    total_test_count++;                                                        \
    float epsilon = 1e-5f;                                                     \
    if (fabsf((actual) - (expected)) > epsilon) {                              \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected %s == %s, got %f != %f (diff %f > %f)",     \
                         #actual, #expected, (float)(actual),                  \
                         (float)(expected), fabsf((actual) - (expected)),      \
                         epsilon);                                             \
    }                                                                          \
  } while (0)

#define EXPECT_DOUBLE_EQ(actual, expected)                                     \
  do {                                                                         \
    total_test_count++;                                                        \
    double epsilon = 1e-9;                                                     \
    if (fabs((actual) - (expected)) > epsilon) {                               \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected %s == %s, got %lf != %lf (diff %lf > %lf)", \
                         #actual, #expected, (double)(actual),                 \
                         (double)(expected), fabs((actual) - (expected)),      \
                         epsilon);                                             \
    }                                                                          \
  } while (0)

// Strings ------------------------------------------------
#define EXPECT_STREQ(actual, expected)                                         \
  do {                                                                         \
    total_test_count++;                                                        \
    const char *a = (actual);                                                  \
    const char *e = (expected);                                                \
    if ((a == NULL && e != NULL) || (a != NULL && e == NULL) ||                \
        (a != NULL && e != NULL && strcmp(a, e) != 0)) {                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected %s == %s, got '%s' != '%s'", #actual,       \
                         #expected, (a ? a : "NULL"), (e ? e : "NULL"));       \
    }                                                                          \
  } while (0)

#define ASSERT_STREQ(actual, expected)                                         \
  do {                                                                         \
    total_test_count++;                                                        \
    const char *a = (actual);                                                  \
    const char *e = (expected);                                                \
    if ((a == NULL && e != NULL) || (a != NULL && e == NULL) ||                \
        (a != NULL && e != NULL && strcmp(a, e) != 0)) {                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Assertion %s == %s failed, got '%s' != '%s'",        \
                         #actual, #expected, (a ? a : "NULL"),                 \
                         (e ? e : "NULL"));                                    \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

// Test registration macro
#define TEST(suite, name)                                                      \
  void suite##_##name(void);                                                   \
  static TestCase test_##suite##_##name = {#suite, #name, suite##_##name};     \
  __attribute__((constructor)) static void register_##suite##_##name(void) {   \
    if (test_registry_size < SCRUFFY_MAX_TESTS) {                              \
      test_registry[test_registry_size++] = test_##suite##_##name;             \
    }                                                                          \
    else {                                                                     \
      fprintf(stderr, "Test registry full!\n");                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  }                                                                            \
  void suite##_##name(void)

// RUN_ALL macro
#define RUN_ALL()                                                              \
  int main(void) {                                                             \
    total_test_count    = 0;                                                   \
    total_test_failures = 0;                                                   \
    suite_failures      = 0;                                                   \
    int diff;                                                                  \
    struct timespec start, end;                                                \
    struct timespec total_start, total_end;                                    \
    clock_gettime(CLOCK_MONOTONIC, &total_start);                              \
    TEST_PASS_MESSAGE("[==========] Starting tests");                          \
    TEST_PASS_MESSAGE("[----------] Global test environment set-up.");         \
    for (size_t i = 0; i < test_registry_size; i++) {                          \
      clock_gettime(CLOCK_MONOTONIC, &start);                                  \
      TEST_PASS_MESSAGE("[ RUN      ] %s.%s", test_registry[i].suite_name,     \
                        test_registry[i].test_name);                           \
      test_registry[i].test_func();                                            \
      clock_gettime(CLOCK_MONOTONIC, &end);                                    \
      diff = get_time_diff(start, end);                                        \
      if (total_test_failures > 0)                                             \
        TEST_FAIL_MESSAGE("\n[  FAILED  ] %d / %d tests passed (%d usec)",       \
                          total_test_count - total_test_failures,              \
                          total_test_count, diff);                             \
      else                                                                     \
        TEST_PASS_MESSAGE("[       OK ] %d / %d tests passed (%d usec)",       \
                          total_test_count - total_test_failures,              \
                          total_test_count, diff);                             \
      TEST_PASS_MESSAGE("[----------]");                                       \
    }                                                                          \
    TEST_PASS_MESSAGE("[----------] Global test environment tear-down");       \
    clock_gettime(CLOCK_MONOTONIC, &total_end);                                \
    diff = get_time_diff(total_start, total_end);                              \
                                                                               \
    if (total_test_failures)                                                   \
      TEST_FAIL_MESSAGE(                                                       \
          "[==========] %d out of %d total tests failed (%d usec total)",      \
          total_test_failures, total_test_count, diff);                        \
    else                                                                       \
      TEST_PASS_MESSAGE(                                                       \
          "[==========] %d out of %d total tests passed (%d usec total)",      \
          total_test_count, total_test_count, diff);                           \
    return total_test_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;             \
  }

#if defined __cplusplus
}
#endif