#include <stdio.h>
#include "dynamic_array.h"

int main(void)
{
    int *arr = NULL;

    da_push(arr, 69);
    da_push(arr, 420);
    da_push(arr, 67);
    da_push(arr, 80085);

    for (size_t i = 0; i < da_count(arr); i++) {
        printf("%d\n", arr[i]);
    }

    printf("Arr: %p\n", (void*)arr);

    printf("Popped: %d\n", da_pop(arr));
    printf("Popped: %d\n", da_pop(arr));

    da_free(arr);

    return 0;
}
