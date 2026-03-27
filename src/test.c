#include <stdio.h>
#include "dynamic_array.h"

int main(void)
{
    int *arr = NULL;

    da_push(arr, 69);
    da_push(arr, 420);
    da_push(arr, 67);
    da_push(arr, 80085);

    da_foreach(arr, batman) {
        printf("%d\n", *batman);
    }

    da_foreach(arr, batman) {
        *batman *= 10;
    }

    da_foreach(arr, batman) {
        printf("%d\n", *batman);
    }

    printf("Arr: %p\n", (void*)arr);

    printf("Popped: %d\n", da_pop(arr));
    printf("Popped: %d\n", da_pop(arr));

    da_free(arr);

    return 0;
}
