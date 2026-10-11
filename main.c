#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Incomplete starter — implement the required behavior.
 * Stdin modes (if any): alloc, free, merge, edge, multi
 * Match the sample test output exactly. Hidden tests use the other inputs listed above.
 */

/* Core types from the exercise (keep these) */
#define HEAP_SIZE 4096

struct Header {
    bool used;
    size_t size;
    struct Header* next;
};

static char HEAP[HEAP_SIZE] = {};
static char* HEAD = HEAP;

static void print_sizes() {
    printf("Size of char: %zu\n", sizeof(char));
    printf("Size of size_t: %zu\n", sizeof(size_t));
    printf("Size of int: %zu\n", sizeof(int));
    printf("Size of void*: %zu\n", sizeof(void*));
    printf("Size of Header: %zu\n", sizeof(struct Header));
}

/* TODO: implement init_heap */
/* TODO: implement my_malloc */
/* TODO: implement my_free */
/* TODO: implement print_heap */
/* TODO: implement count_free_blocks */

static size_t get_remaining_memory() {
     return (size_t) HEAP + HEAP_SIZE - (size_t) HEAD;
}

static void init_heap() {
    print_sizes();

    printf("\nStart allocation\n");
    HEAD = HEAP;
}

static struct Header* create_header(const bool used, const size_t size, struct Header* next) {
    const auto ptr = (struct Header*) HEAD;
    ptr->used = used;
    ptr->size = size;
    ptr->next = next;
    HEAD += sizeof(struct Header);
    return ptr;
}

static void* my_malloc(const size_t bytes) {
    printf("\nTrying to allocate %zu bytes\n", bytes);

    if (bytes == 0)
        return nullptr;

    if (get_remaining_memory() < (bytes + sizeof(struct Header)))
        return nullptr;

    struct Header* new_header = create_header(true, bytes, nullptr);
    void* new_data = HEAD;
    HEAD += bytes;
    new_header->next = (struct Header*) HEAD;
    return new_data;
}

static void my_free(void* ptr) {
    printf("Free memory\n");
}

int main() {
    init_heap();

    printf("\nRemaining memory: %zu\n", get_remaining_memory());

    int* array = my_malloc(10 * sizeof(int));

    printf("\nRemaining memory: %zu\n", get_remaining_memory());

    printf("Add data to memory\n");
    for (int i = 0; i < 10; i++)
        array[i] = i;

    printf("Read data from memory\n");
    for (int i = 0; i < 10; i++)
        printf("i: %d\n", array[i]);

    my_free(array);

    printf("\nRemaining memory: %zu\n", get_remaining_memory());

    return 0;
}
