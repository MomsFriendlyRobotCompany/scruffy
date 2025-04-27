#include "scruffy/scruffy.h"

TestCase test_registry[SCRUFFY_MAX_TESTS];

size_t test_registry_size = 0;
int test_count            = 0;
int suite_failures        = 0;
int test_failures         = 0;