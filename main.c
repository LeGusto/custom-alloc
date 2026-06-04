#define HEAP_SIZE (1024 * 1024)
#define end_sentinel (0xEE)
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct Node {
    int space;
    struct Node* next;
};



char heap[HEAP_SIZE];
int left = HEAP_SIZE;

void* free_ptr = &heap[0];
void* h_end = &heap[0] + HEAP_SIZE;


void* c_malloc(int req) {
    if (req  + 1 > left) return NULL;
    void* rt = free_ptr;
    free_ptr += req;
    *(char*)free_ptr = end_sentinel;
    free_ptr++;
    left -= req + 1;

    return rt;
}

void c_reset(void) {
    free_ptr = &heap[0];
    left = HEAP_SIZE;
}

void c_free(unsigned char* ptr) {
    while (ptr < (unsigned char*)h_end
        && *ptr != end_sentinel) {
        *ptr = 0;
        ptr++;
    }
}

int main(int argc, char** argv[]) {
    memset(&heap[0], 0, HEAP_SIZE);

    // alloc succeeds and returns pointer into heap
    unsigned char* rt = c_malloc(16);
    assert(rt != NULL);
    assert((char*)rt >= heap && (char*)rt < heap + HEAP_SIZE);

    // write and read back
    rt[0] = 'A';
    rt[15] = 'Z';
    assert(rt[0] == 'A' && rt[15] == 'Z');

    // sentinel placed right after allocation
    assert(((unsigned char*)rt)[16] == end_sentinel);

    // second alloc lands after first + sentinel
    unsigned char* rt2 = c_malloc(8);
    assert(rt2 == rt + 17);

    // oversized alloc fails
    assert(c_malloc(HEAP_SIZE) == NULL);

    // free zeroes the region
    c_free(rt);
    assert(rt[0] == 0 && rt[15] == 0);

    printf("all checks passed\n");
}