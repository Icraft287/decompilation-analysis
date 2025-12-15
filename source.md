---
title: Complete Source Code
subtitle: experiment.c - Test Program for Decompilation Analysis
---

## Overview

This is the complete C source code used in the decompilation experiment. The program demonstrates various common programming patterns to test how well they survive compilation and decompilation at different optimization levels.

## Features Tested

::::{grid} 2

:::{card} Recursive Functions
The Fibonacci function tests how recursion is preserved or transformed during optimization.
:::

:::{card} Array Processing
The sum_array function tests loop handling and array manipulation.
:::

:::{card} Struct Operations
The Point struct and update_point function test structure handling and pointer operations.
:::

:::{card} Control Flow
The main function tests conditional statements and printf calls.
:::

::::

## Complete Source Code

Save this as `experiment.c`:

```c
#include <stdio.h>
#include <stdlib.h>

// Function for Fibonacci sequence
int calculate_fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return calculate_fibonacci(n - 1) + calculate_fibonacci(n - 2);
}

// Function with loop and local variables to compute sum of a given array
int sum_array(int *arr, int size) {
    int sum = 0;
    int i;
    
    for (i = 0; i < size; i++) {
        sum += arr[i];
    }
    
    return sum;
}

// Function with struct and pointer manipulation
typedef struct {
    int x;
    int y;
    char name[20];
} Point;

void update_point(Point *p, int new_x, int new_y) {
    p->x = new_x;
    p->y = new_y;
    sprintf(p->name, "Point(%d,%d)", new_x, new_y);
}

//Main
int main() {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int array_size = 10;
    
    // Call sum function
    int total = sum_array(numbers, array_size);
    printf("Sum of array: %d\n", total);
    
    // Call fibonacci
    int fib_result = calculate_fibonacci(10);
    printf("Fibonacci(10): %d\n", fib_result);
    
    // Work with struct
    Point my_point;
    update_point(&my_point, 42, 84);
    printf("Point: %s at (%d, %d)\n", my_point.name, my_point.x, my_point.y);
    
    // Comparing values test
    if (total > fib_result) {
        printf("Array sum is greater\n");
    } else {
        printf("Fibonacci is greater or equal\n");
    }
    
    return 0;
}
```

## Function Breakdown

### calculate_fibonacci()

```c
int calculate_fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return calculate_fibonacci(n - 1) + calculate_fibonacci(n - 2);
}
```

**What it tests**:
- Recursive function calls
- Base case handling
- Integer arithmetic
- Return value propagation

**Expected behavior at different optimization levels**:
- **-O0**: Preserved as-is
- **-O2/-O3**: May be converted to iterative loops, heavily unrolled

### sum_array()

```c
int sum_array(int *arr, int size) {
    int sum = 0;
    int i;
    
    for (i = 0; i < size; i++) {
        sum += arr[i];
    }
    
    return sum;
}
```

**What it tests**:
- For loop handling
- Array indexing and pointer arithmetic
- Local variable scoping
- Accumulator pattern

**Expected behavior at different optimization levels**:
- **-O0**: Loop preserved exactly
- **-O2**: Loop may be unrolled 2-4 times
- **-O3**: May be completely eliminated if array is constant

### update_point()

```c
typedef struct {
    int x;
    int y;
    char name[20];
} Point;

void update_point(Point *p, int new_x, int new_y) {
    p->x = new_x;
    p->y = new_y;
    sprintf(p->name, "Point(%d,%d)", new_x, new_y);
}
```

**What it tests**:
- Struct definitions and field access
- Pointer dereferencing
- String formatting with sprintf
- Multiple field updates

**Expected behavior at different optimization levels**:
- **-O0**: Function boundary preserved, field accesses clear
- **-O2/-O3**: May be inlined, struct layout becomes offset calculations

### main()

```c
int main() {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int array_size = 10;
    
    int total = sum_array(numbers, array_size);
    printf("Sum of array: %d\n", total);
    
    int fib_result = calculate_fibonacci(10);
    printf("Fibonacci(10): %d\n", fib_result);
    
    Point my_point;
    update_point(&my_point, 42, 84);
    printf("Point: %s at (%d, %d)\n", my_point.name, my_point.x, my_point.y);
    
    if (total > fib_result) {
        printf("Array sum is greater\n");
    } else {
        printf("Fibonacci is greater or equal\n");
    }
    
    return 0;
}
```

**What it tests**:
- Stack-allocated arrays
- Function call sequences
- Multiple variable declarations
- Conditional branching
- Standard library calls (printf)

**Expected behavior at different optimization levels**:
- **-O0**: All function calls visible, variables on stack
- **-O2/-O3**: Heavy inlining, constant folding (e.g., array sum becomes 0x37)

## Expected Output

When you compile and run this program, you should see:

```text
Sum of array: 55
Fibonacci(10): 55
Point: Point(42,84) at (42, 84)
Fibonacci is greater or equal
```

:::{note}
The array sum (1+2+...+10 = 55) equals Fibonacci(10) = 55, so the condition `total > fib_result` is false.
:::

## Compilation Instructions

See the [experiment guide](guide.md) for detailed compilation instructions. Quick reference:

```bash
# Compile without optimization
gcc -O0 experiment.c -o experiment_O0

# Compile with moderate optimization
gcc -O2 experiment.c -o experiment_O2

# Compile with aggressive optimization
gcc -O3 experiment.c -o experiment_O3
```

## What Makes This a Good Test Case?

::::{grid} 2

:::{card} Diverse Patterns
Combines recursion, iteration, structs, and I/O - common real-world patterns.
:::

:::{card} Measurable Results
Clear expected output makes it easy to verify correctness after optimization.
:::

:::{card} Optimization-Friendly
Contains opportunities for constant folding, inlining, and loop unrolling.
:::

:::{card} Compact Size
Small enough to analyze manually but complex enough to show interesting transformations.
:::

::::

