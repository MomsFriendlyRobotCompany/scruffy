![](./docs/scruffy.png)
# Scruffy

**Why not use `gtest`?** Well I do on C++ code, but I also 
write a lot of C code and wanted somethig similar. This 
provides the majority of what I do for testing.

> Grok helped me write these and I fine tuned and adjusted
> them to my liking

## CMake

```cmake
# Fetch scruffy for testing
include(FetchContent)
FetchContent_Declare(
    scruffy
    URL https://github.com/MomsFriendlyRobotCompany/scruffy/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(scruffy)

# run all tests
add_executable(test-all ${stests})
target_link_libraries(test-all scruffy ... other libs ...)
```

## Simple Example

```c
#include <scruffy/scruffy.h>

TEST(mylib, dummy) {
  EXPECT_TRUE(true);
}

TEST(mylib, somthing) {
  EXPECT_FALSE(1, 22);
  EXPECT_EQ(true, true);
}

TEST(mylib, other_thing) {
  int *a = NULL
  EXPECT_NULL(a);

  float b = 1.2345;
  float c = b * 2.0f / 2.0f;
  EXPECT_FLOAT_EQ(b,c);
}

RUN_ALL()
```

## Output

Success

```bash
======================================
macOS 27.0.1 [26A434]
CPU: 12 core Apple M4 Pro
RAM: 49152 MB
Compilers:
  C23 AppleClang: /usr/bin/cc
Target is 64-bit ARM
======================================
[==========] Starting tests
[//////////] Suite scruffy_pass with 5 test cases
[----------]
[ RUN      ]  1. scruffy_pass.floats
[       OK ]     Tests passed 4 (1 usec)
[----------]
[ RUN      ]  2. scruffy_pass.equals
[       OK ]     Tests passed 3 (0 usec)
[----------]
[ RUN      ]  3. scruffy_pass.bools
[       OK ]     Tests passed 3 (1 usec)
[----------]
[ RUN      ]  4. scruffy_pass.pointers
[       OK ]     Tests passed 2 (0 usec)
[----------]
[ RUN      ]  5. scruffy_pass.strings
[       OK ]     Tests passed 4 (0 usec)
[----------]
[//////////] Suite scruffy_pass_alt with 1 test cases
[----------]
[ RUN      ]  1. scruffy_pass_alt.equals
[       OK ]     Tests passed 1 (0 usec)
[----------]
[//////////] End all tests.
[==========] 17 out of 17 total tests passed (34 usec total)
```

Failure

```bash
======================================
macOS 27.0.1 [26A434]
CPU: 12 core Apple M4 Pro
RAM: 49152 MB
Compilers:
  C23 AppleClang: /usr/bin/cc
Target is 64-bit ARM
======================================
[==========] Starting tests
[//////////] Suite scruffy_pass with 5 test cases
[----------]
[ RUN      ]  1. scruffy_pass.floats
[       OK ]     Tests passed 4 (0 usec)
[----------]
[ RUN      ]  2. scruffy_pass.equals
[       OK ]     Tests passed 3 (0 usec)
[----------]
[ RUN      ]  3. scruffy_pass.bools
[       OK ]     Tests passed 3 (0 usec)
[----------]
[ RUN      ]  4. scruffy_pass.pointers
[       OK ]     Tests passed 2 (0 usec)
[----------]
[ RUN      ]  5. scruffy_pass.strings
[       OK ]     Tests passed 4 (0 usec)
[----------]
[//////////] Suite scruffy_pass_alt with 1 test cases
[----------]
[ RUN      ]  1. scruffy_pass_alt.equals
[       OK ]     Tests passed 1 (0 usec)
[----------]
[//////////] Suite scruffy_fail with 5 test cases
[----------]
[ RUN      ]  1. scruffy_fail.floats
[     FAIL ]     Tests failed 2 (7 usec)
[----------]
[ RUN      ]  2. scruffy_fail.equals
[     FAIL ]     Tests failed 2 (1 usec)
[----------]
[ RUN      ]  3. scruffy_fail.bools
[     FAIL ]     Tests failed 3 (1 usec)
[----------]
[ RUN      ]  4. scruffy_fail.pointers
[     FAIL ]     Tests failed 2 (1 usec)
[----------]
[ RUN      ]  5. scruffy_fail.strings
[     FAIL ]     Tests failed 6 (6 usec)
[----------]
[//////////] End all tests.
[==========] 15 out of 32 total tests failed (123 usec total)

Errors Found:
--------------------------------------------------
file: fail.c line: 7 
Expected a == 3.1f, got 3.120000 != 3.100000 (diff 0.020000 > 0.000000)--------------------------------------------------
file: fail.c line: 10 
Expected b == 12.345, got 12.345670 != 12.345000 (diff 0.000670 > 0.000000)--------------------------------------------------
file: fail.c line: 13 
Expected 1 == 2, got 1 != 2
--------------------------------------------------
file: fail.c line: 20 
Expected (test_true()) == false
--------------------------------------------------
file: fail.c line: 21 
Expected (1 == 2) == true
--------------------------------------------------
file: fail.c line: 22 
Expected (1 == 1) == false
--------------------------------------------------
file: fail.c line: 27 
Expected p[0x0] != NULL
--------------------------------------------------
file: fail.c line: 31 
Expected p[0x16d5a2d74] == NULL
--------------------------------------------------
file: fail.c line: 35 
Expected "Hi" == "hi", got 'Hi' != 'hi'
--------------------------------------------------
file: fail.c line: 36 
Expected "Hi" == NULL, got 'Hi' != 'NULL'
--------------------------------------------------
file: fail.c line: 37 
Expected NULL == "hi", got 'NULL' != 'hi'
--------------------------------------------------
file: fail.c line: 38 
Expected "Hi" == nullptr, got 'Hi' != 'NULL'
--------------------------------------------------
file: fail.c line: 39 
Expected nullptr == "hi", got 'NULL' != 'hi'
--------------------------------------------------
file: fail.c line: 42 
Expected s == "hello there brown cow", got 'hello there brown cow!?' != 'hello there brown cow'
```

## Tests

- `EXPECT_NULL(pointer)`: pass if `ptr == NULL`
- `EXPECT_NOT_NULL(pointer)`: pass if `ptr != NULL`
- `EXPECT_TRUE(condition)`: pass if condition is `true`
- `EXPECT_FALSE(condition)`: pass if condition is `false`
- `EXPECT_EQ(int, int)`: pass if `int == int`
- `EXPECT_FLOAT_EQ(float, float)`: pass if `float == float`
- `EXPECT_DOUBLE_EQ(double, double)`: pass if `double == double`
- `TEST(suite, test)`: creates a test function named `void suite_test(void)` and registers the function 
- `EXPECT_STREQ(actual, expected)`: compares two strings
- `RUN_ALL()`: runs all test functions and prints failure descriptions at end


Each macro is wrapped in a `do {...} while(0)` loop to avoid problems. 

- Ensures `EXPECT_` acts like single statement
- Avoids dangling `else` problems (see below)
- Ensures proper scope for temporary variables in the testing framework
- Properly consumes the semicolon at the end of `EXPECT_` statements
- No performance overhead, since compilers will optimize it away

## Limitations

- You can only write 300 `TEST()` functions in your code.

## Dangling `else` Problem

```c
if (some_condition)
    EXPECT_TRUE(x == y);
else
    printf("Condition false\n");
```

After macro expansion:

```c
if (some_condition)
    test_count++;
    if (!(x == y)) {
        TEST_FAIL_MESSAGE("Expected x == y to be true", "x == y");
    };
else
    printf("Condition false\n");
```

The `else` now gets associated with the second `if`, which is not what was planned.

While the `do {...} while(0)` is correct. 

```c
if (some_condition)
    do {
        test_count++;
        if (!(x == y)) {
            TEST_FAIL_MESSAGE("Expected x == y to be true", "x == y");
        }
    } while (0);
else
    printf("Condition false\n");
```

## Examples

See `examples` folder, but here is a simple example:

```c
TEST(scruffy, pointers) {
  EXPECT_TRUE(1 == 1);
  EXPECT_FALSE(1 == 2);

  int *p = NULL;
  EXPECT_NULL(p);

  int a = 3;
  p = &a;
  EXPECT_NOT_NULL(p);
}

RUN_ALL(true);
```

## ToDo:

- [ ] Work more on test cases and suite tests status to make it clearer

# MIT License

**Copyright (c) 2025 Mom's Friendly Robot Company**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.