---
title: Decompilation Analysis Report
subtitle: Examining Code Recovery Across Compiler Optimization Levels
short_title: Overview
authors:
  - name: Your Name
date: 2024-12-14
keywords:
  - decompilation
  - reverse engineering
  - compiler optimization
  - ghidra
exports:
  - format: pdf
    template: arxiv_two_column
  - format: docx
---

## Overview

This experiment analyzes how compiler optimizations affect the ability to recover source code through decompilation. A simple C program was compiled with three optimization levels (`-O0`, `-O2`, `-O3`) and decompiled using Ghidra via [Dogbolt](https://dogbolt.org) to compare what information can and cannot be reconstructed.

:::{important}
**Key Finding**: While control flow and logic are largely recoverable, semantic information like variable names, comments, and developer intent are permanently lost. Higher optimizations dramatically transform code structure.
:::

## Experimental Setup

### Source Code

A 65-line C program with:
- Recursive Fibonacci function
- Array sum with loop
- Struct manipulation with pointers
- Conditional logic and I/O

### Compilation Commands

```bash
gcc -O0 experiment.c -o experiment_O0
gcc -O2 experiment.c -o experiment_O2
gcc -O3 experiment.c -o experiment_O3
```

### Decompilation Tool

[**Dogbolt**](https://dogbolt.org) - Online decompiler aggregator
- Uploaded each binary
- Selected Ghidra decompiler
- Compared output with original source

## Results Summary

### Assembly Line Counts

```text
  234 experiment_O0.s
  392 experiment_O2.s
  454 experiment_O3.s
```

:::{note}
**Surprising result**: Higher optimizations produced *more* assembly lines, not fewer. This is due to aggressive loop unrolling and inlining creating repetitive but efficient code patterns.
:::

### What Was Successfully Recovered

::::{grid} 2

:::{card} ✅ Control Flow (All Levels)
- If/else branches correctly identified
- Loop structures recognizable at `-O0`
- Function call relationships preserved at `-O0` and `-O2`
- String literals intact: `"Sum of array: %d\n"`, `"Point(%d,%d)"`
:::

:::{card} ✅ Basic Data Types
- Integer operations clear
- Pointer arithmetic identifiable
- Struct field offsets calculable (though not named)
:::

::::

### What Was Lost Forever

::::{grid} 2

:::{card} ❌ All Semantic Information
- **Variable names**: `total` → `uVar2`, `fib_result` → `uVar3`
- **Function names**: Preserved only by symbol table
- **Comments**: All removed during compilation
- **Type definitions**: `typedef struct { ... } Point;` → anonymous struct
:::

:::{card} ❌ Developer Intent
- Variable `total` becomes `local_18` - purpose unclear
- Function purposes must be inferred from behavior
- Design patterns invisible
- Optimization rationale lost
:::

::::

## Detailed Analysis by Optimization Level

### -O0: Most Recoverable

**Fibonacci Function - Original**:

```c
int calculate_fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return calculate_fibonacci(n - 1) + calculate_fibonacci(n - 2);
}
```

**Decompiled**:

```c
int calculate_fibonacci(int n) {
  int iVar1;
  int iVar2;
  
  if (1 < n) {
    iVar1 = calculate_fibonacci(n + -1);
    iVar2 = calculate_fibonacci(n + -2);
    n = iVar2 + iVar1;
  }
  return n;
}
```

**Analysis**:

::::{grid} 2
:::{card} What Survived ✅
- Algorithm clearly recognizable as Fibonacci
- Recursive structure preserved
- Control flow nearly identical
:::

:::{card} What Was Lost ❌
- Variable names changed to generic `iVar1`, `iVar2`
- Subtle logic inversion: `n <= 1` became `1 < n`
:::
::::

### Array Sum Comparison

**Original**:
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

**Decompiled (-O0)**:
```c
int sum_array(int *arr, int size) {
  int sum;
  int i;
  
  sum = 0;
  for (i = 0; i < size; i = i + 1) {
    sum = sum + arr[i];
  }
  return sum;
}
```

:::{tip}
**Near-perfect recovery** - only `i++` became `i = i + 1`.
:::

## -O2 & -O3: Aggressive Transformation

### Key Changes

1. **Function Inlining**: `sum_array()` and `update_point()` disappeared entirely - inlined into `main()`
2. **Constant Folding**: Array sum computed at compile time as `0x37` (55 in decimal)
3. **Loop Unrolling**: Simple for-loop became unrecognizable

### Main Function - Critical Difference

Original called `sum_array(numbers, 10)` and stored result in `total`.

**Decompiled at -O2/-O3**:
```c
__printf_chk(1, "Sum of array: %d\n", 0x37);  // Sum pre-calculated!
```

:::{warning}
The entire array and summation loop were **eliminated**. The compiler computed 1+2+...+10=55 at compile time and hardcoded `0x37` (55).
:::

### Fibonacci at -O2/-O3 - Catastrophic Transformation

The simple recursive function became 100+ lines of deeply nested loops and bitwise operations:

```c
int calculate_fibonacci(int n) {
  // ... initialization ...
  if (1 < n) {
    do {
      local_64 = 0;
      local_70 = local_74 + -1;
      if (local_70 != 1) {
        do {
          local_60 = 0;
          iVar1 = local_70 + -1;
          if (iVar1 == 1) {
            local_60 = 1;
          }
          else {
            do {
              // ... 80 more lines of nested loops ...
              iVar7 = (iVar5 - 3U & 1) + iVar7;
            } while (1 < iVar5);
          }
        } while (1 < local_70);
      }
    } while (1 < local_74);
  }
  return local_6c;
}
```

:::{danger}
**Analysis**: The compiler transformed recursion into iteration with loop unrolling. While functionally equivalent, the **algorithm is unrecognizable** without extensive reverse engineering.
:::

### Array Sum at -O2/-O3 - Loop Unrolling

```c
// Original: simple for loop
// Decompiled: 4-way unrolled loop
do {
  iVar1 = *piVar5;
  piVar2 = piVar5 + 1;
  piVar3 = piVar5 + 2;
  piVar4 = piVar5 + 3;
  piVar5 = piVar5 + 4;
  iVar7 = iVar7 + iVar1;
  iVar8 = iVar8 + *piVar2;
  iVar9 = iVar9 + *piVar3;
  iVar10 = iVar10 + *piVar4;
} while (piVar5 != arr + ...);
```

Processes 4 array elements per iteration for performance, but obscures the original simple loop.

## Key Findings

### The Semantic Gap

Even with perfect decompilation, there's an **unbridgeable semantic gap**:

| Recoverable | Lost Forever |
|-------------|--------------|
| What the code does | Why it does it |
| Control flow logic | Developer reasoning |
| Literal values | Variable purposes |
| Function structure (-O0) | Design patterns |
| Basic algorithms | Optimization intent |

### Optimization Impact

| Level | Recoverability | Key Changes |
|-------|----------------|-------------|
| **-O0** | ~85% structure | Minimal transformation, clear logic |
| **-O2** | ~50% structure | Inlining, constant folding, some unrolling |
| **-O3** | ~30% structure | Aggressive transformation, unrecognizable algorithms |

### Example: Variable Name Loss

```c
// You write:
int total = sum_array(numbers, array_size);

// Decompiler sees:
uint uVar2;
uVar2 = sum_array(local_58, 10);
```

:::{important}
The name `total` conveyed **meaning** - it's a sum, a total. `uVar2` is just a register placeholder. This semantic information is **irrecoverable**.
:::

## Conclusions

### Can You Recover Source Code?

**Short answer**: Partially, depending on optimization level.

- **-O0**: Yes, structure is ~85% intact. Readable with effort.
- **-O2/-O3**: Maybe 30-50%. Requires significant reverse engineering expertise.

### What This Means

::::{grid} 2

:::{card} For Proprietary Code Protection
Algorithms *can* be reverse-engineered, but require expertise. `-O3` adds significant protection through obfuscation.
:::

:::{card} For Software Archaeology
Lost source code can be partially reconstructed, but will never match the original in clarity or intent.
:::

:::{card} For Reverse Engineering
Lower optimization levels are **dramatically** easier to analyze. A `-O0` binary might take hours to understand; `-O3` could take weeks.
:::

:::{card} The Bottom Line
Decompilation recovers the **what**, rarely the **why**. The logic is there, but the story behind it is lost forever.
:::

::::

## Next Steps

- [Follow the experiment guide](guide.md) to reproduce these results
- [View the complete source code](source.md)
- Try the [Dogbolt decompiler](https://dogbolt.org) yourself

---

**Experiment Conducted**: December 2024  
**Tools**: GCC, Dogbolt (Ghidra decompiler)  
**Platform**: Linux x86_64