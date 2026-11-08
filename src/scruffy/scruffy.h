
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

#define SCRUFFY_MAX_TESTS 300

// Structure to hold test metadata
typedef struct {
  const char *test_name;
  void (*test_func)(void);
} TestCase;

typedef struct {
  const char *suite_name;
  uint16_t max_test_cases;
  TestCase test_cases[SCRUFFY_MAX_TESTS];
} SuiteTestCase;

// Global array to store test cases
extern SuiteTestCase test_registry[SCRUFFY_MAX_TESTS];
extern size_t test_registry_size;
extern int total_test_count;
extern int total_test_failures;
extern int suite_failures;
extern bool SCRUFFY_VERBOSE_OUTPUT;

int get_time_diff(struct timespec start, struct timespec end);
void push_test_case(const char* suite, TestCase testcase);
int run_all();

static
const char* get_target() {
  #if defined(__x86_64__) || defined(_M_X64)
    return "Target is 64-bit Intel/AMD x86";
  #elif defined(__i386__) || defined(_M_IX86)
    return "Target is 32-bit Intel/AMD x86";
  #elif defined(__aarch64__) || defined(_M_ARM64)
    return "Target is 64-bit ARM";
  #elif defined(__arm__) || defined(_M_ARM)
    return "Target is 32-bit ARM";
  #else
    return "Target is Uknown";
  #endif
}

// fixme: this and test-color-message are the same
#define TEST_MESSAGE(color, format, ...)                     \
  do {                                                       \
    printf("%s" format SCRUFFY_RESET, color, ##__VA_ARGS__); \
  } while (0)

static
void print_info() {
  const char* c = SCRUFFY_CYAN;
  TEST_MESSAGE(c, "======================================\n");
  #ifdef OS_INFO
  TEST_MESSAGE(c, "%s\n", OS_INFO);
  TEST_MESSAGE(c, "%s\n", CPU_INFO);
  TEST_MESSAGE(c, "%s\n", RAM_INFO);
  #endif

  TEST_MESSAGE(c, "Compilers:\n");
  #if defined(C_INFO)
  TEST_MESSAGE(c, "  %s\n", C_INFO);
  #endif
  #if defined(CXX_INFO)
  TEST_MESSAGE(c, "  %s\n", CXX_INFO);
  #endif

  #if __clang__
  TEST_MESSAGE(c, "%s\n", get_target());
  #endif
  TEST_MESSAGE(c, "======================================\n");
}

// keep do loop - ensures test_failure++ is scoped with fprintf()
#define TEST_DEBUG_MESSAGE(format, ...)                                        \
  do {         \
    if (SCRUFFY_VERBOSE_OUTPUT) {                                          \
    printf("\n%sfile: %s line: %d %s\n", SCRUFFY_CYAN, __FILE__, __LINE__,     \
           SCRUFFY_RESET);                                                     \
    printf(format "\n", ##__VA_ARGS__);                                        \
    } \
    total_test_failures++;                                                     \
    suite_failures++; \
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

#define TEST_COLOR_MESSAGE(color, format, ...)                                         \
  do {                                                                         \
    printf("%s" format "%s\n", color, ##__VA_ARGS__, SCRUFFY_RESET);   \
  } while (0)

// Pointers comparisons -----------------------------------
#define EXPECT_NULL(ptr)                                                       \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((ptr) != NULL) {                                                       \
      TEST_DEBUG_MESSAGE("Expected %s[%p] == NULL", #ptr,ptr);                 \
    }                                                                          \
  } while (0)

#define EXPECT_NOT_NULL(ptr)                                                   \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((ptr) == NULL) {                                                       \
      TEST_DEBUG_MESSAGE("Expected %s[%p] != NULL", #ptr,ptr);                 \
    }                                                                          \
  } while (0)

// Binary -------------------------------------------------
#define EXPECT_TRUE(cond)                                                      \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((cond) == false) {                                                     \
      TEST_DEBUG_MESSAGE("Expected (%s) == true", #cond);                      \
    }                                                                          \
  } while (0)

#define EXPECT_FALSE(cond)                                                     \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((cond) == true) {                                                      \
      TEST_DEBUG_MESSAGE("Expected (%s) == false", #cond);                     \
    }                                                                          \
  } while (0)

#define EXPECT_EQ(actual, expected)                                            \
  do {                                                                         \
    total_test_count++;                                                        \
    if ((actual) != (expected)) {                                              \
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
      TEST_DEBUG_MESSAGE("Assertion %s == %s failed, got '%s' != '%s'",        \
                         #actual, #expected, (a ? a : "NULL"),                 \
                         (e ? e : "NULL"));                                    \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

// Test registration macro
#define TEST(suite, name)                                                      \
  void suite##_##name(void);                                                   \
  static TestCase test_##suite##_##name = {#name, suite##_##name};             \
  __attribute__((constructor)) static void register_##suite##_##name(void) {   \
    if (test_registry_size < SCRUFFY_MAX_TESTS) {                              \
      push_test_case(#suite, test_##suite##_##name);                           \
    }                                                                          \
    else {                                                                     \
      fprintf(stderr, "Test registry full!\n");                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  }                                                                            \
  void suite##_##name(void)


// RUN_ALL macro
#define RUN_ALL(verbose)          \
int main(void) {                  \
  print_info();                   \
  SCRUFFY_VERBOSE_OUTPUT=verbose; \
  return run_all();               \
}

#if defined __cplusplus
}
#endif