
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

// seems to be compiler specific
#if !defined(__FILE_NAME__)
  #define __FILE_NAME__ __FILE__
#endif

// Color codes
#define SCRUFFY_RED "\033[31m"    // error
#define SCRUFFY_GREEN "\033[32m"  // success
#define SCRUFFY_YELLOW "\033[33m" // warning
#define SCRUFFY_CYAN "\033[36m"   // debug
#define SCRUFFY_RESET "\033[0m"

#define SCRUFFY_MAX_TESTS 300
#define SCRUFFY_MAX_DEBUG_MSGS (3 * SCRUFFY_MAX_TESTS)
#define SCRUFFY_MAX_DEBUG_MSG_LEN 128

// keep do loop - I really don't know why it doesn't work without it
#define SWRAP(test)                                                            \
  do {                                                                         \
    test;                                                                      \
  } while (0)

// test function defines
#define EXPECT_EQ(left, right)                                                 \
  SWRAP(int_eq_c(__FILE_NAME__, __LINE__, #left, #right, left, right))
#define EXPECT_TRUE(cond) SWRAP(true_c(__FILE_NAME__, __LINE__, #cond, cond))
#define EXPECT_FALSE(cond) SWRAP(false_c(__FILE_NAME__, __LINE__, #cond, cond))
#define EXPECT_FLOAT_EQ(left, right)                                           \
  SWRAP(float_eq_c(__FILE_NAME__, __LINE__, #left, #right, left, right))
#define EXPECT_DOUBLE_EQ(left, right)                                          \
  SWRAP(double_eq_c(__FILE_NAME__, __LINE__, #left, #right, left, right))
#define EXPECT_STREQ(left, right)                                              \
  SWRAP(streq_c(__FILE_NAME__, __LINE__, #left, #right, left, right))
#define EXPECT_NULL(ptr) SWRAP(null_c(__FILE_NAME__, __LINE__, #ptr, p))
#define EXPECT_NOT_NULL(ptr) SWRAP(not_null_c(__FILE_NAME__, __LINE__, #ptr, p))

// Test registration macro to build test suite and cases
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
#define RUN_ALL()                                                              \
  int main(void) {                                                             \
    scruffy_sys_info();                                                        \
    return run_all();                                                          \
  }

//////////////////////////////////////////////////////
// Functions and data structures
//
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

int get_time_diff(struct timespec start, struct timespec end);
void push_test_case(const char *suite, TestCase testcase);
void push_debug_msg(const char *msg, uint8_t size);
int run_all();

// Integer comparison -------------------------------------
void int_eq_c(const char *file, int line, const char *l, const char *r,
              long long left, long long right);
// Boolean comparison -------------------------------------
void true_c(const char *file, int line, const char *var, bool cond);
void false_c(const char *file, int line, const char *var, bool cond);
// Floating-point equality --------------------------------
void float_eq_c(const char *file, int line, const char *l, const char *r,
                float left, float right);
void double_eq_c(const char *file, int line, const char *exp, const char *act,
                 double actual, double expected);
// String comparison --------------------------------------
void streq_c(const char *file, int line, const char *l, const char *r,
             const char *left, const char *right);
// Pointers comparisons -----------------------------------
void null_c(const char *file, int line, const char *ptr, void *p);
void not_null_c(const char *file, int line, const char *ptr, void *p);

//////////////////////////////////////////////////////
// These stay in header so #defines work correctely
//

static void scruffy_sys_info() {
  printf("%s======================================\n", SCRUFFY_CYAN);
#ifdef OS_INFO
  printf("%s\n", OS_INFO);
  printf("%s\n", CPU_INFO);
  printf("%s\n", RAM_INFO);
#endif

  printf("Compilers:\n");
#if defined(C_INFO)
  printf("  %s\n", C_INFO);
#endif
#if defined(CXX_INFO)
  printf("  %s\n", CXX_INFO);
#endif

#if __clang__
  #if defined(__x86_64__) || defined(_M_X64)
  const char *target = "Target is 64-bit Intel/AMD x86";
  #elif defined(__i386__) || defined(_M_IX86)
  const char *target = "Target is 32-bit Intel/AMD x86";
  #elif defined(__aarch64__) || defined(_M_ARM64)
  const char *target = "Target is 64-bit ARM";
  #elif defined(__arm__) || defined(_M_ARM)
  const char *target = "Target is 32-bit ARM";
  #else
  const char *target = "Target is Uknown";
  #endif
  printf("%s\n", target);
#endif
  printf("======================================\n%s", SCRUFFY_RESET);
}
//////////////////////////////////////////////////////

#if defined __cplusplus
}
#endif
