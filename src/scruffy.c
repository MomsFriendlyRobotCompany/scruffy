#include "scruffy/scruffy.h"

// TestCase test_registry[SCRUFFY_MAX_TESTS];

size_t test_registry_size = 0;
int total_test_count      = 0; // total tests executed
int suite_failures        = 0; // number failed tests in suite
int total_test_failures   = 0; // total number failed tests
bool SCRUFFY_VERBOSE_OUTPUT = false;


SuiteTestCase test_registry[SCRUFFY_MAX_TESTS];


typedef struct {
  char msgs[SCRUFFY_MAX_DEBUG_MSGS][SCRUFFY_MAX_DEBUG_MSG_LEN];
  const uint16_t max_msgs;
  uint16_t num_msgs;
} scruffy_debug_t;

static scruffy_debug_t debug = {.max_msgs=SCRUFFY_MAX_DEBUG_MSGS, .num_msgs=0};


void push_debug_msg(const char* msg, uint8_t size) {
  if (debug.num_msgs == SCRUFFY_MAX_DEBUG_MSGS) return;
  size = (size < SCRUFFY_MAX_DEBUG_MSG_LEN)? size : SCRUFFY_MAX_DEBUG_MSG_LEN-1;
  strncpy(debug.msgs[debug.num_msgs++], msg, size);
}

int get_time_diff(struct timespec start, struct timespec end) {
  return (int)((end.tv_sec - start.tv_sec) * 1e6 +
               (end.tv_nsec - start.tv_nsec) / 1e3);
}

void push_test_case(const char* suite, TestCase testcase) {
  for (size_t i=0; i < test_registry_size; ++i) {
    SuiteTestCase *stc = &test_registry[i];
    if (strcmp(suite, stc->suite_name) == 0) {
      if (stc->max_test_cases == SCRUFFY_MAX_TESTS) return;
      stc->test_cases[stc->max_test_cases++] = testcase;
      return;
    }
  }
  if (test_registry_size == SCRUFFY_MAX_TESTS) return;
  test_registry[test_registry_size].suite_name = suite;
  test_registry[test_registry_size].test_cases[0] = testcase;
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
                                                                        
  TEST_PASS_MESSAGE("[==========] Starting tests");                     
  // TEST_PASS_MESSAGE("[----------] Global test environment set-up.");    
  for (size_t i = 0; i < test_registry_size; i++) {
    const size_t tc_cnt = test_registry[i].max_test_cases;
    size_t cnt = 0;
   
    TEST_PASS_MESSAGE("[//////////] Suite %s with %zu test cases",
      test_registry[i].suite_name,
      tc_cnt
    );

    for (size_t c=0; c<tc_cnt; ++c) {
      int success_cnt = total_test_count;
      int fail_cnt = total_test_failures;
      TestCase *tc = &test_registry[i].test_cases[c];
      int tests = 0;
      bool fail = false;
      
      clock_gettime(CLOCK_MONOTONIC, &start);
      tc->test_func();                                       
      clock_gettime(CLOCK_MONOTONIC, &end);                               
      diff = get_time_diff(start, end);   
                                       
      if (total_test_failures > 0) {
        tests = suite_failures - fail_cnt;
        fail = true;
      }                        
      else {
        tests = total_test_count - success_cnt;
      }

      TEST_COLOR_MESSAGE(
        fail? SCRUFFY_RED : SCRUFFY_GREEN,
        "[----------]"
      );
      TEST_COLOR_MESSAGE(
        fail? SCRUFFY_RED : SCRUFFY_GREEN,
        "[ RUN      ] %2zu. %s.%s",
        ++cnt,
        test_registry[i].suite_name,
        tc->test_name
      );
      TEST_COLOR_MESSAGE(
        fail? SCRUFFY_RED : SCRUFFY_GREEN,
        "[     %4s ]     Tests %s %d (%d usec)", 
        fail? "FAIL" : "OK",
        fail? "failed" : "passed",
        tests,
        diff
      );
    }
    if (total_test_failures > 0) TEST_FAIL_MESSAGE("[----------]");
    else  TEST_PASS_MESSAGE("[----------]");
  } 

  // TEST_PASS_MESSAGE("[----------] Global test environment tear-down");  
  clock_gettime(CLOCK_MONOTONIC, &total_end);                           
  diff = get_time_diff(total_start, total_end);    
  
  
    TEST_PASS_MESSAGE("[//////////] End all tests.");
                                                                        
  if (total_test_failures)                                              
    TEST_FAIL_MESSAGE(                                                  
        "[==========] %d out of %d total tests failed (%d usec total)", 
        total_test_failures, total_test_count, diff);                   
  else                                                                  
    TEST_PASS_MESSAGE(                                                  
        "[==========] %d out of %d total tests passed (%d usec total)", 
        total_test_count, total_test_count, diff);
  
  if (debug.num_msgs > 0) {
    printf("\n%sErrors Found:%s\n", SCRUFFY_CYAN, SCRUFFY_RESET);
    for (int i=0; i<debug.num_msgs; ++i) printf("%s", debug.msgs[i]);
  }

  return total_test_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;        
}
