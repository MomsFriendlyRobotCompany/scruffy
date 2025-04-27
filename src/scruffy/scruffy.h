
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

#if defined __cplusplus
extern "C" {
#endif

// Color codes
#define SCRUFFY_RED "\033[31m"
#define SCRUFFY_GREEN "\033[32m"
#define SCRUFFY_YELLOW "\033[33m"
#define SCRUFFY_CYAN "\033[36m"
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
extern int test_count;
extern int test_failures;
extern int suite_failures;

// keep do loop - ensures test_failure++ is scoped with fprintf()
#define TEST_DEBUG_MESSAGE(format, ...)                                        \
  do {                                                                         \
    fprintf(stdout, "line %d: " format "\n", __LINE__, ##__VA_ARGS__);         \
    test_failures++;                                                           \
  } while (0)

// keep do loop - I really don't know why it doesn't work without it
#define TEST_FAIL_MESSAGE(format, ...)                                         \
  do {                                                                         \
    fprintf(stderr, "%sline %d: " format "%s\n", SCRUFFY_RED, __LINE__,        \
            ##__VA_ARGS__, SCRUFFY_RESET);                                     \
  } while (0)

#define TEST_PASS_MESSAGE(format, ...)                                         \
  do {                                                                         \
    fprintf(stdout, "%s" format "%s\n", SCRUFFY_GREEN, ##__VA_ARGS__,          \
            SCRUFFY_RESET);                                                    \
  } while (0)

// Pointers comparisons -----------------------------------
#define EXPECT_NULL(ptr)                                                       \
  do {                                                                         \
    test_count++;                                                              \
    if ((ptr) != NULL) {                                                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) != NULL", #ptr);                       \
    }                                                                          \
  } while (0)

#define EXPECT_NOT_NULL(ptr)                                                   \
  do {                                                                         \
    test_count++;                                                              \
    if ((ptr) == NULL) {                                                       \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == NULL", #ptr);                       \
    }                                                                          \
  } while (0)

// Binary -------------------------------------------------
#define EXPECT_TRUE(cond)                                                      \
  do {                                                                         \
    test_count++;                                                              \
    if ((cond) == false) {                                                     \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == true", #cond);                      \
    }                                                                          \
  } while (0)

#define EXPECT_FALSE(cond)                                                     \
  do {                                                                         \
    test_count++;                                                              \
    if ((cond) == true) {                                                      \
      suite_failures++;                                                        \
      TEST_DEBUG_MESSAGE("Expected (%s) == false", #cond);                     \
    }                                                                          \
  } while (0)

#define EXPECT_EQ(actual, expected)                                            \
  do {                                                                         \
    test_count++;                                                              \
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
    test_count++;                                                              \
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
    test_count++;                                                              \
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
    test_count++;                                                              \
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
    test_count++;                                                              \
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
  do {                                                                         \
    test_count     = 0;                                                        \
    test_failures  = 0;                                                        \
    suite_failures = 0;                                                        \
    TEST_PASS_MESSAGE("[----------] Global test environment set-up.");         \
    for (size_t i = 0; i < test_registry_size; i++) {                          \
      test_failures = 0;                                                       \
      TEST_PASS_MESSAGE("[----------] %d tests from %s", test_count,           \
                        test_registry[i].suite_name);                          \
      TEST_PASS_MESSAGE("[ RUN      ] %s.%s", test_registry[i].suite_name,     \
                        test_registry[i].test_name);                           \
      test_registry[i].test_func();                                            \
      if (test_failures > 0)                                                   \
        TEST_FAIL_MESSAGE("[  FAILED  ] %s.%s", test_registry[i].suite_name,   \
                          test_registry[i].test_name);                         \
      else                                                                     \
        TEST_PASS_MESSAGE("[       OK ] %s.%s", test_registry[i].suite_name,   \
                          test_registry[i].test_name);                         \
      TEST_PASS_MESSAGE("[----------] %d tests from %s", test_count,           \
                        test_registry[i].suite_name);                          \
    }                                                                          \
    TEST_PASS_MESSAGE("[----------] Global test environment tear-down");       \
    TEST_PASS_MESSAGE("[==========] %d out of %d failed", suite_failures,      \
                      test_count);                                             \
  } while (0)

#if defined __cplusplus
}
#endif