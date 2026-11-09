#include "scruffy/scruffy.h"
#include <stdarg.h>

#define TEST_COLOR_MESSAGE(color, format, ...) printf("%s" format "%s\n", color, ##__VA_ARGS__, SCRUFFY_RESET)

size_t test_registry_size   = 0;
int total_test_count        = 0; // total tests executed
int suite_failures          = 0; // number failed tests in suite
int total_test_failures     = 0; // total number failed tests
bool SCRUFFY_VERBOSE_OUTPUT = false;

SuiteTestCase test_registry[SCRUFFY_MAX_TESTS];

typedef struct {
  char msgs[SCRUFFY_MAX_DEBUG_MSGS][SCRUFFY_MAX_DEBUG_MSG_LEN];
  const uint16_t max_msgs;
  uint16_t num_msgs;
} scruffy_debug_t;

static scruffy_debug_t debug = {.max_msgs = SCRUFFY_MAX_DEBUG_MSGS, .num_msgs = 0};

void push_debug_msg(const char *msg, uint8_t size) {
  if (debug.num_msgs == SCRUFFY_MAX_DEBUG_MSGS) return;
  size = (size < SCRUFFY_MAX_DEBUG_MSG_LEN) ? size : SCRUFFY_MAX_DEBUG_MSG_LEN - 1;
  strncpy(debug.msgs[debug.num_msgs++], msg, size);
}

int get_time_diff(struct timespec start, struct timespec end) { return (int)((end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3); }

void push_test_case(const char *suite, TestCase testcase) {
  for (size_t i = 0; i < test_registry_size; ++i) {
    SuiteTestCase *stc = &test_registry[i];
    if (strcmp(suite, stc->suite_name) == 0) {
      if (stc->max_test_cases == SCRUFFY_MAX_TESTS) return;
      stc->test_cases[stc->max_test_cases++] = testcase;
      return;
    }
  }
  if (test_registry_size == SCRUFFY_MAX_TESTS) return;
  test_registry[test_registry_size].suite_name     = suite;
  test_registry[test_registry_size].test_cases[0]  = testcase;
  test_registry[test_registry_size].max_test_cases = 1;
  test_registry_size++;
}

int run_all(void) {
  total_test_count    = 0;
  total_test_failures = 0;
  suite_failures      = 0;
  int diff;
  struct timespec start, end;
  struct timespec total_start, total_end;
  clock_gettime(CLOCK_MONOTONIC, &total_start);

  TEST_COLOR_MESSAGE(SCRUFFY_GREEN, "[==========] Starting tests");

  for (size_t i = 0; i < test_registry_size; i++) {
    const size_t tc_cnt = test_registry[i].max_test_cases;
    size_t cnt          = 0;

    TEST_COLOR_MESSAGE(SCRUFFY_GREEN, "[//////////] Suite %s with %zu test cases", test_registry[i].suite_name, tc_cnt);

    for (size_t c = 0; c < tc_cnt; ++c) {
      int success_cnt = total_test_count;
      int fail_cnt    = total_test_failures;
      TestCase *tc    = &test_registry[i].test_cases[c];
      int tests       = 0;
      bool fail       = false;

      clock_gettime(CLOCK_MONOTONIC, &start);
      tc->test_func();
      clock_gettime(CLOCK_MONOTONIC, &end);
      diff = get_time_diff(start, end);

      if (total_test_failures > 0) {
        tests = suite_failures - fail_cnt;
        fail  = true;
      } else {
        tests = total_test_count - success_cnt;
      }

      TEST_COLOR_MESSAGE(fail ? SCRUFFY_RED : SCRUFFY_GREEN, "[----------]");
      TEST_COLOR_MESSAGE(fail ? SCRUFFY_RED : SCRUFFY_GREEN, "[ RUN      ] %2zu. %s.%s", ++cnt, test_registry[i].suite_name, tc->test_name);
      TEST_COLOR_MESSAGE(fail ? SCRUFFY_RED : SCRUFFY_GREEN, "[     %4s ]     Tests %s %d (%d usec)", fail ? "FAIL" : "OK", fail ? "failed" : "passed", tests, diff);
    }
    if (total_test_failures > 0) TEST_COLOR_MESSAGE(SCRUFFY_RED, "[----------]");
    else TEST_COLOR_MESSAGE(SCRUFFY_GREEN, "[----------]");
  }

  clock_gettime(CLOCK_MONOTONIC, &total_end);
  diff = get_time_diff(total_start, total_end);

  TEST_COLOR_MESSAGE(SCRUFFY_GREEN, "[//////////] End all tests.");

  if (total_test_failures) TEST_COLOR_MESSAGE(SCRUFFY_RED, "[==========] %d out of %d total tests failed (%d usec total)", total_test_failures, total_test_count, diff);
  else TEST_COLOR_MESSAGE(SCRUFFY_GREEN, "[==========] %d out of %d total tests passed (%d usec total)", total_test_count, total_test_count, diff);

  if (debug.num_msgs > 0) {
    printf("\n%sErrors Found:%s\n", SCRUFFY_CYAN, SCRUFFY_RESET);
    for (int i = 0; i < debug.num_msgs; ++i)
      printf("%s", debug.msgs[i]);
  }

  return total_test_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

//////////////////

static void error_msg_c(const char *file, int line, const char *format, ...) {
  va_list args;
  va_start(args, format);

  char msg[SCRUFFY_MAX_DEBUG_MSG_LEN];
  snprintf(msg, SCRUFFY_MAX_DEBUG_MSG_LEN, "%s--------------------------------------------------%s\n", SCRUFFY_CYAN, SCRUFFY_RESET);
  push_debug_msg(msg, SCRUFFY_MAX_DEBUG_MSG_LEN);

  snprintf(msg, SCRUFFY_MAX_DEBUG_MSG_LEN, "%sfile: %s line: %d %s\n", SCRUFFY_CYAN, file, line, SCRUFFY_RESET);
  push_debug_msg(msg, SCRUFFY_MAX_DEBUG_MSG_LEN);

  vsnprintf(msg, SCRUFFY_MAX_DEBUG_MSG_LEN, format, args);

  push_debug_msg(msg, SCRUFFY_MAX_DEBUG_MSG_LEN);
  total_test_failures++;
  suite_failures++;

  va_end(args);
}

void null_c(const char *file, int line, const char *ptr, void *p) {
  total_test_count++;
  if ((p) != NULL) {
    error_msg_c(file, line, "Expected %s[%p] == NULL\n", ptr, p);
  }
}

void not_null_c(const char *file, int line, const char *ptr, void *p) {
  total_test_count++;
  if ((p) == NULL) {
    error_msg_c(file, line, "Expected %s[%p] != NULL\n", ptr, p);
  }
}

void true_c(const char *file, int line, const char *var, bool cond) {
  total_test_count++;
  if ((cond) == false) {
    error_msg_c(file, line, "Expected (%s) == true\n", var);
  }
}

void false_c(const char *file, int line, const char *var, bool cond) {
  total_test_count++;
  if ((cond) == true) {
    error_msg_c(file, line, "Expected (%s) == false\n", var);
  }
}

void streq_c(const char *file, int line, const char *l, const char *r, const char *left, const char *right) {
  total_test_count++;
  if ((left == NULL && right != NULL) || (left != NULL && right == NULL) || (left != NULL && right != NULL && strcmp(left, right) != 0)) {
    error_msg_c(file, line, "Expected %s == %s, got '%s' != '%s'\n", l, r, (left ? left : "NULL"), (right ? right : "NULL"));
  }
}

void float_eq_c(const char *file, int line, const char *l, const char *r, float left, float right) {
  total_test_count++;
  float epsilon = 1e-9;
  float diff    = fabsf((left) - (right));
  if (diff > epsilon) {
    error_msg_c(file, line, "Expected %s == %s, got %f != %f (diff %f > %f)", l, r, (float)(left), (float)(right), diff, epsilon);
  }
}

void double_eq_c(const char *file, int line, const char *l, const char *r, double left, double right) {
  total_test_count++;
  double epsilon = 1e-9;
  double diff    = fabs((left) - (right));
  if (diff > epsilon) {
    error_msg_c(file, line, "Expected %s == %s, got %lf != %lf (diff %lf > %lf)", l, r, (double)(left), (double)(right), diff, epsilon);
  }
}

void int_eq_c(const char *file, int line, const char *l, const char *r, long long left, long long right) {

  total_test_count++;
  if ((left) != (right)) {
    error_msg_c(file, line, "Expected %s == %s, got %lld != %lld\n", l, r, left, right);

    total_test_failures++;
    suite_failures++;
  }
}