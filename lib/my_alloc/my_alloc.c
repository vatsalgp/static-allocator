#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "my_alloc.h"

#define HEAP_SIZE (4 * 1024)

typedef struct Header {
    bool used;
    size_t size;
    struct Header* next;
} Header;

static Header* get_header(void* data) {
    return (Header*) ((uint8_t*) data - sizeof(Header));
}

static void print_header(const Header* header) {
    printf("Printing header information:\n");
    printf("Where in memory is the header at: %p\n", header);
    printf("Is the following memory used: %s\n", header->used ? "Yes" : "No");
    printf("How big is the following memory: %zu bytes\n", header->size);
    printf("Where in memory is the next header at: %p\n", header->next);
}

static uint8_t HEAP[HEAP_SIZE] = {};
static uint8_t* NEXT_HEADER = HEAP;

static void print_sizes() {
    printf("Printing sizes information:\n");
    printf("Size of uint8_t: %zu\n", sizeof(uint8_t));
    printf("Size of size_t: %zu\n", sizeof(size_t));
    printf("Size of int: %zu\n", sizeof(int));
    printf("Size of void*: %zu\n", sizeof(void*));
    printf("Size of Header: %zu\n", sizeof(Header));
    printf("\n");
}

static size_t get_remaining_memory() {
     return (size_t) HEAP + HEAP_SIZE - (size_t) NEXT_HEADER;
}

static void print_remaining_memory() {
    printf("Remaining memory: %zu\n", get_remaining_memory());
}

static void print_memory_info(void* data) {
    print_header(get_header(data));
}

static Header* create_header(const size_t size) {
    const auto ptr = (Header*) NEXT_HEADER;
    ptr->used = true;
    ptr->size = size;
    ptr->next = nullptr;
    NEXT_HEADER += sizeof(Header);
    return ptr;
}

static bool defrag() {
    const auto header = (Header*) HEAP;

    if (header == (Header*) NEXT_HEADER) return false;

    if (header->used) return false;

    if (header->next == (Header*) NEXT_HEADER) {
        memset(header, 0, sizeof(Header));
        NEXT_HEADER = (u_int8_t*) header;
        return true;
    }

    if (header->next->used) return false;

    Header* final_header = header->next->next;
    const size_t next_size = header->next->size;
    memset(header->next, 0, sizeof(Header));
    header->next = final_header;
    header->size += sizeof(Header) + next_size;

    return true;
}

void init_heap() {
    print_sizes();

    printf("Start allocation\n");
    NEXT_HEADER = HEAP;
}

void* my_malloc(const size_t bytes) {
    printf("Trying to allocate %zu bytes\n", bytes);

    if (bytes == 0)
        return nullptr;

    if (get_remaining_memory() < bytes + sizeof(Header))
        return nullptr;

    Header* new_header = create_header(bytes);
    void* new_data = NEXT_HEADER;
    NEXT_HEADER += bytes;
    new_header->next = (Header*) NEXT_HEADER;
    return new_data;
}

void my_free(void* data) {
    if (!data) return;

    printf("Freeing memory at header %p\n", get_header(data));
    Header* header = get_header(data);
    header->used = false;
    memset(data, 0, header->size);

    while (defrag()) {}

    print_memory_info(data);
    printf("Freed memory at header %p\n", get_header(data));
    print_remaining_memory();
    printf("\n");
}
