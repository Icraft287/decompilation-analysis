---
title: How to Reproduce This Experiment
short_title: Experiment Guide
---

## Step-by-Step Guide

Follow these steps to reproduce the decompilation analysis on your own system.

### Prerequisites

::::{grid} 2

:::{card} Required Tools
- GCC compiler (any recent version)
- Text editor (VSCode, vim, etc.)
- Web browser
- Internet connection for Dogbolt
:::

:::{card} Optional Tools
- Git (for version control)
- `objdump` (for assembly inspection)
- Ghidra (for local decompilation)
:::

::::

## Step 1: Save the Source Code

:::{tip}
Download or copy the complete source code from the [source code page](source.md).
:::

🛠 **Save the source code** as `experiment.c`:

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

// ... (rest of code - see source.md)
```

## Step 2: Compile with Different Optimizations

🛠 **Open your terminal** and navigate to the directory containing `experiment.c`:

```bash
cd /path/to/your/experiment
```

🛠 **Compile with three different optimization levels**:

```bash
# No optimization - easiest to decompile
gcc -O0 experiment.c -o experiment_O0

# Moderate optimization
gcc -O2 experiment.c -o experiment_O2

# Aggressive optimization - hardest to decompile
gcc -O3 experiment.c -o experiment_O3
```

:::{note}
The `-O0`, `-O2`, and `-O3` flags control how aggressively GCC optimizes your code. `-O0` means no optimization, while `-O3` applies the most aggressive optimizations.
:::

### Optional: Generate Assembly Files

🛠 **Generate human-readable assembly** for comparison:

```bash
gcc -O0 -S experiment.c -o experiment_O0.s
gcc -O2 -S experiment.c -o experiment_O2.s
gcc -O3 -S experiment.c -o experiment_O3.s
```

🛠 **Compare assembly line counts**:

```bash
wc -l experiment_O*.s
```

You should see output similar to:
```text
  234 experiment_O0.s
  392 experiment_O2.s
  454 experiment_O3.s
 1080 total
```

## Step 3: Visit Dogbolt

:::{important}
[Dogbolt](https://dogbolt.org) is an online decompiler aggregator that lets you compare results from multiple decompilers including Ghidra, IDA, Binary Ninja, and more.
:::

🛠 **Open your web browser** and navigate to:

```text
https://dogbolt.org
```

## Step 4: Upload and Decompile Each Binary

### Upload experiment_O0

🛠 **Click "Upload Binary"** on the Dogbolt homepage

🛠 **Select** `experiment_O0` from your file system

🛠 **Wait** for decompilation (typically 30-60 seconds)

🛠 **Select "Ghidra"** from the decompiler options on the left sidebar

:::{tip}
You can also select other decompilers like "Hex-Rays" or "Binary Ninja" to compare results!
:::

### Analyze the Output

🛠 **Find the `main` function** in the decompiled code

🛠 **Compare with your original source**:
- Are variable names the same?
- Is the control flow recognizable?
- Are functions still separate?

🛠 **Look for `calculate_fibonacci` and `sum_array`**:
- Do they still exist as separate functions?
- Can you identify the algorithm?

### Repeat for O2 and O3

🛠 **Upload** `experiment_O2` and `experiment_O3` separately

🛠 **Compare** how optimizations transformed the code:
- Which functions disappeared (inlined)?
- How did loops change?
- Is the Fibonacci algorithm still recognizable?

## Step 5: Document Your Observations

Create a comparison table like this:

| Aspect | -O0 | -O2 | -O3 |
|--------|-----|-----|-----|
| Variable names preserved? | ❌ No | ❌ No | ❌ No |
| Functions separate? | ✅ Yes | ⚠️ Partially | ❌ No (inlined) |
| Fibonacci recognizable? | ✅ Clear | ⚠️ Harder | ❌ Unrecognizable |
| Array sum loop clear? | ✅ Yes | ⚠️ Unrolled | ❌ Constant folded |

## Step 6: Deep Dive Analysis (Optional)

### Using objdump for Assembly Inspection

🛠 **View the assembly** of a specific function:

```bash
objdump -d experiment_O0 | grep -A 30 "calculate_fibonacci"
```

### Using Ghidra Locally

If you have [Ghidra](https://ghidra-sre.org) installed:

1. Create a new project in Ghidra
2. Import your binary: File → Import File
3. Analyze with default options
4. Navigate to the Functions list in the Symbol Tree
5. Compare the Decompile view with your original source

### Compare Assembly Line Counts

🛠 **View specific sections** of assembly:

```bash
# See how Fibonacci changes across optimization levels
grep -A 50 "calculate_fibonacci:" experiment_O0.s > fib_O0.txt
grep -A 50 "calculate_fibonacci:" experiment_O3.s > fib_O3.txt
diff fib_O0.txt fib_O3.txt
```

## What to Look For

::::{grid} 2

:::{card} Easy to Recover ✅
- Control flow (if/else/loops)
- String literals
- Numeric constants
- Function boundaries (-O0)
- Basic algorithms
:::

:::{card} Lost Forever ❌
- Variable names
- Comments
- Type definitions (struct names)
- Developer reasoning
- Code organization
:::

::::

## Common Observations

### At -O0 Level

```c
// Original
int total = sum_array(numbers, array_size);

// Decompiled - very similar!
uint uVar2;
uVar2 = sum_array(numbers, 10);
```

### At -O2/-O3 Level

```c
// Original
int total = sum_array(numbers, array_size);

// Decompiled - computed at compile time!
__printf_chk(1, "Sum of array: %d\n", 0x37);
```

:::{warning}
The entire function call was eliminated and replaced with the hardcoded result `0x37` (55 in hex).
:::

## Troubleshooting

### Binary Too Large for Dogbolt

If your binary is too large, try:
- Compiling with `-s` to strip symbols: `gcc -O0 -s experiment.c -o experiment_O0_stripped`
- Using a local decompiler instead (Ghidra, IDA)

### Can't Find Functions

- Make sure you compiled without stripping: don't use `-s` flag
- Look for function addresses in the assembly output
- In Ghidra, functions might be auto-named like `FUN_00401234`

### Decompiled Code Looks Wrong

- Remember: decompilers are approximations, not perfect
- Try different decompilers on Dogbolt
- Compare with the assembly to verify logic

## Additional Experiments to Try

1. **Add more complex algorithms**: Try quicksort, binary search, or linked lists
2. **Use different compilers**: Compare GCC vs Clang vs MSVC
3. **Try obfuscation**: Use tools like `llvm-obfuscator`
4. **Strip symbols**: Compile with `-s` flag to see how it affects analysis
5. **Add static analysis**: Use `cppcheck` or `clang-tidy` on decompiled code

## Resources

- **Dogbolt**: [https://dogbolt.org](https://dogbolt.org)
- **Compiler Explorer**: [https://godbolt.org](https://godbolt.org) (see assembly + source side-by-side)
- **Ghidra**: [https://ghidra-sre.org](https://ghidra-sre.org) (full-featured reverse engineering)
- **GCC Optimization Docs**: [https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

---

:::{tip}
Once you've completed the experiment, compare your findings with the main [analysis report](index.md)!
:::