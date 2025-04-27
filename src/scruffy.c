#include "scruffy/scruffy.h"

TestCase test_registry[SCRUFFY_MAX_TESTS];

size_t test_registry_size = 0;
int total_test_count      = 0; // total tests executed
int suite_failures        = 0; // number failed tests in suite
int total_test_failures   = 0; // total number failed tests

// double get_time_diff(struct timespec start, struct timespec end) {
//   return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;
// }