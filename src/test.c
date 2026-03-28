#include <stdio.h>
#define DA_INITIAL_CAPACITY 10
#include "dynamic_array.h"

int main(void)
{
    int *arr = NULL;

    da_push(arr, 69);
    da_push(arr, 420);
    da_push(arr, 67);
    da_push(arr, 80085);

    da_print(arr, "%d");

    for (int i = 0; i < 30; i++)
        da_push(arr, i);

    da_print(arr, "%d");

    da_foreach(arr, batman)
        *batman *= 10;

    da_print(arr, "%d");

    printf("Popped: %d\n", da_pop(arr));
    printf("Popped: %d\n", da_pop(arr));

    printf("arr[3]: %d\n", arr[3]);
    printf("Removed: %d\n", da_fast_remove(arr, 3));

    da_print(arr, "%d");

    printf("arr[2]: %d\n", arr[2]);
    printf("Removed: %d\n", da_remove(arr, 2));

    da_print(arr, "%d");

    for (size_t i = 0; i < 19; i++) {
        (void)da_pop(arr);
    }

    da_print(arr, "%d");

    return 0;
}
