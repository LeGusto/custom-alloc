#define HEAP_SIZE (1024 * 1024)
#include <stddef.h>

char heap[HEAP_SIZE];
int left = HEAP_SIZE;

void* free_ptr = &heap[0];


void* c_malloc(int req) {
    if (req > left) return NULL;
    void* rt = free_ptr;
    free_ptr += req;
    left -= req;

    return rt;
}

int main(int argc, void* argv[]) {

}