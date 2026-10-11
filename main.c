#include "lib/my_alloc/my_alloc.h"

#define CHUNKS 10
#define CHUNK_SIZE 16

int main() {
    init_heap();

    int* ptrs[CHUNKS];

    for (int i = 0; i < CHUNKS; i++) {
        ptrs[i] = my_malloc(CHUNK_SIZE * sizeof(int));

        printf("Add data to memory\n");
        for (int j = 0; j < CHUNK_SIZE; j++)
            ptrs[i][j] = j + CHUNK_SIZE * i;

        printf("Read data from memory\n");
        for (int j = 0; j < CHUNK_SIZE; j++)
            printf("i: %d\n", ptrs[i][j]);

        printf("\n");
    }

    my_free(ptrs[9]);
    my_free(ptrs[1]);
    my_free(ptrs[4]);
    my_free(ptrs[8]);
    my_free(ptrs[0]);
    my_free(ptrs[6]);
    my_free(ptrs[7]);
    my_free(ptrs[3]);
    my_free(ptrs[2]);
    my_free(ptrs[5]);

    return 0;
}
