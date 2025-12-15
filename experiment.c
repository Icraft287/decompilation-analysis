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