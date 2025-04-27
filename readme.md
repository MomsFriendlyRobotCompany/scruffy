![](./docs/scruffy.png)
# Scruffy

**Why not use `gtest`?** Well I do on C++ code, but I also 
write a lot of C code and wanted somethig similar. This 
provides the majority of what I do for testing.

> Grok helped me write these and I fine tuned and adjusted
> them to my liking

## Output

Success

```bash
[==========] Starting tests
[----------] Global test environment set-up.
[ RUN      ] scruffy.floats
[       OK ] 2 / 2 tests passed (1 usec)
[----------]
[ RUN      ] scruffy.equals
[       OK ] 3 / 3 tests passed (1 usec)
[----------]
[ RUN      ] scruffy.bools
[       OK ] 5 / 5 tests passed (2 usec)
[----------]
[ RUN      ] scruffy.pointers
[       OK ] 7 / 7 tests passed (1 usec)
[----------]
[----------] Global test environment tear-down
[==========] 7 out of 7 total tests passed (28 usec total)
```

Failure

```bash
[==========] Starting tests
[----------] Global test environment set-up.
[ RUN      ] scruffy.floats

file: /Users/kevin/github/scruffy/examples/fail.c line: 7 
Expected a == 3.1f, got 3.120000 != 3.100000 (diff 0.020000 > 0.000010)

file: /Users/kevin/github/scruffy/examples/fail.c line: 10 
Expected b == 12.345, got 12.345670 != 12.345000 (diff 0.000670 > 0.000000)

[  FAILED  ] 0 / 2 tests passed (9 usec)
[----------]
[ RUN      ] scruffy.equals

file: /Users/kevin/github/scruffy/examples/fail.c line: 14 
Expected 1 == 2, got 1 != 2

[  FAILED  ] 0 / 3 tests passed (3 usec)
[----------]
[ RUN      ] scruffy.bools

file: /Users/kevin/github/scruffy/examples/fail.c line: 22 
Expected (test_true()) == false

file: /Users/kevin/github/scruffy/examples/fail.c line: 23 
Expected (1 == 2) == true

file: /Users/kevin/github/scruffy/examples/fail.c line: 24 
Expected (1 == 1) == false

[  FAILED  ] 1 / 7 tests passed (4 usec)
[----------]
[ RUN      ] scruffy.pointers

file: /Users/kevin/github/scruffy/examples/fail.c line: 29 
Expected (p) == NULL

file: /Users/kevin/github/scruffy/examples/fail.c line: 33 
Expected (p) != NULL

[  FAILED  ] 1 / 9 tests passed (6 usec)
[----------]
[----------] Global test environment tear-down
[==========] 8 out of 9 total tests failed (40 usec total)
```

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

RUN_ALL();
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