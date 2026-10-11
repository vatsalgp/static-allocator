#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Incomplete starter — implement the required behavior.
 * Stdin modes (if any): alloc, free, merge, edge, multi
 * Match the sample test output exactly. Hidden tests use the other inputs listed above.
 */

#define HEAP_SIZE (4 * 1024)

struct Header {
    bool used;
    size_t size;
    struct Header* next;
};

static struct Header* get_header(void* data) {
    return (struct Header*) ((uint8_t*) data - sizeof(struct Header));
}

static void print_header(const struct Header* header) {
    printf("Printing header information:\n");
    printf("Where in memory is the header at: %p\n", header);
    printf("Is the following memory used: %s\n", header->used ? "Yes" : "No");
    printf("How big is the following memory: %zu bytes\n", header->size);
    printf("Where in memory is the next header at: %p\n", header->next);
    printf("\n");
}

static uint8_t HEAP[HEAP_SIZE] = {};
static uint8_t* NEXT_HEADER = HEAP;

static void print_sizes() {
    printf("Printing sizes information:\n");
    printf("Size of uint8_t: %zu\n", sizeof(uint8_t));
    printf("Size of size_t: %zu\n", sizeof(size_t));
    printf("Size of int: %zu\n", sizeof(int));
    printf("Size of void*: %zu\n", sizeof(void*));
    printf("Size of Header: %zu\n", sizeof(struct Header));
    printf("\n");
}

/* TODO: implement my_free */
/* TODO: implement print_heap */
/* TODO: implement count_free_blocks */

static size_t get_remaining_memory() {
     return (size_t) HEAP + HEAP_SIZE - (size_t) NEXT_HEADER;
}

static void print_remaining_memory() {
    printf("Remaining memory: %zu\n", get_remaining_memory());
}

static void print_memory_info(void* data) {
    print_header(get_header(data));
}

static void init_heap() {
    print_sizes();

    printf("Start allocation\n");
    NEXT_HEADER = HEAP;
}

static struct Header* create_header(const bool used, const size_t size, struct Header* next) {
    const auto ptr = (struct Header*) NEXT_HEADER;
    ptr->used = used;
    ptr->size = size;
    ptr->next = next;
    NEXT_HEADER += sizeof(struct Header);
    return ptr;
}

static void* my_malloc(const size_t bytes) {
    printf("Trying to allocate %zu bytes\n", bytes);

    if (bytes == 0)
        return nullptr;

    if (get_remaining_memory() < (bytes + sizeof(struct Header)))
        return nullptr;

    struct Header* new_header = create_header(true, bytes, nullptr);
    void* new_data = NEXT_HEADER;
    NEXT_HEADER += bytes;
    new_header->next = (struct Header*) NEXT_HEADER;
    return new_data;
}

static void my_free(void* data) {
    printf("Free memory at %p\n", data);

    struct Header* header = get_header(data);
    header->used = false;
    memset(data, 0, header->size);
    struct Header* next_header = header->next;

    const bool isLast = next_header == (struct Header*) NEXT_HEADER;
    if (!isLast) {
        if (!next_header->used) {
            struct Header* final_header = next_header->next;
            const size_t next_size = next_header->size;
            memset(next_header, 0, sizeof(struct Header));
            header->next = final_header;
            header->size += sizeof(struct Header) + next_size;
        }
    }

    print_memory_info(data);
}

int main() {
    init_heap();

    print_remaining_memory();

    int* array1 = my_malloc(10 * sizeof(int));
    print_memory_info(array1);

    print_remaining_memory();

    printf("Add data to memory\n");
    for (int i = 0; i < 10; i++)
        array1[i] = i;

    printf("Read data from memory\n");
    for (int i = 0; i < 10; i++)
        printf("i: %d\n", array1[i]);
    printf("\n");


    int* array2 = my_malloc(10 * sizeof(int));
    print_memory_info(array2);

    print_remaining_memory();

    printf("Add data to memory\n");
    for (int i = 0; i < 10; i++)
        array2[i] = i + 20;

    printf("Read data from memory\n");
    for (int i = 0; i < 10; i++)
        printf("i: %d\n", array2[i]);
    printf("\n");


    int* array3 = my_malloc(10 * sizeof(int));
    print_memory_info(array3);

    print_remaining_memory();

    printf("Add data to memory\n");
    for (int i = 0; i < 10; i++)
        array3[i] = i + 40;

    printf("Read data from memory\n");
    for (int i = 0; i < 10; i++)
        printf("i: %d\n", array3[i]);
    printf("\n");

    my_free(array3);
    my_free(array2);
    my_free(array1);

    print_remaining_memory();

    return 0;
}
