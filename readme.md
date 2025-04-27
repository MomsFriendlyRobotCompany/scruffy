![](./docs/scruffy.png)
# Scruffy

**Why not use `gtest`?** Well I do on C++ code, but I also 
write a lot of C code and wanted somethig similar. This 
provides the majority of what I do for testing.

> Grok helped me write these and I fine tuned them

## Output

Success

```bash
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from SampleTest
[ RUN      ] SampleTest.Test1
[       OK ] SampleTest.Test1 (0 ms)
[ RUN      ] SampleTest.Test2
[       OK ] SampleTest.Test2 (0 ms)
[----------] 2 tests from SampleTest (0 ms total)
[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

Failure

```bash
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from SampleTest
[ RUN      ] SampleTest.Test3
path/to/your/test.cpp:5: Failure
Value of: 1
Expected: 2
To be equal to: 2
[  FAILED  ] SampleTest.Test3 (0 ms)
[----------] 1 test from SampleTest (0 ms total)
[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  FAILED  ] 1 test, listed below:
[  FAILED  ] SampleTest.Test3

1 FAILED TEST
```

Each macro is wrapped in a `do {...} while(0)` loop to avoid problems. 

- Ensures `EXPECT_` acts like single statement
- Avoids dangling `else` problems (see below)
- Ensures proper scope for temporary variables in the testing framework
- Properly consumes the semicolon at the end of `EXPECT_` statements
- No performance overhead, since compilers will optimize it away

## Tests

- `EXPECT_NULL(pointer)`: pass if `ptr == NULL`
- `EXPECT_NOT_NULL(pointer)`: pass if `ptr != NULL`
- `EXPECT_TRUE(condition)`: pass if condition is `true`
- `EXPECT_FALSE(condition)`: pass if condition is `false`
- `EXPECT_EQ(int, int)`: pass if `int == int`
- `EXPECT_FLOAT_EQ(float, float)`: pass if `float == float`
- `EXPECT_DOUBLE_EQ(double, double)`: pass if `double == double`
- `TEST(suite, test)`: creates a test function named 
  `void suite_test(void)` and registers the function 
- `RUN_ALL()`: runs all test functions and prints results

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

int main() {
  RUN_ALL();
  return test_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

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