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

    da_insert(arr, 96, 0);
    printf("Inserted 96 at 0\n");
    da_print(arr, "%d");

    da_insert(arr,  4, 5);
    printf("Inserted 24 at 5\n");
    da_print(arr, "%d");

    printf("Inserted 76 at %zu\n", da_count(arr));
    da_insert(arr, 76, da_count(arr));
    da_print(arr, "%d");

    printf("Inserted 333 at %zu\n", da_count(arr) - 1);
    da_insert(arr, 333, da_count(arr) - 1);
    da_print(arr, "%d");

    int arr2[] = {0, 1, 9, 6, 3};
    int *new = da_from_arr(arr2, sizeof(arr2) / sizeof(arr2[0]));
    da_print(new, "%d");

    da_free(arr);
    da_free(new);

    return 0;
}
