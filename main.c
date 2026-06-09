#define HEAP_SIZE (1024 * 1024)
#define end_sentinel (0xEE)
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

typedef struct Node {
    int space;
    int free;
    struct Node* next;
} Node;



char heap[HEAP_SIZE];
int left = HEAP_SIZE;

void* free_ptr = &heap[0];
void* h_end = &heap[0] + HEAP_SIZE;


void* c_malloc(int req) {
    if (req <= 0) return NULL;

    Node* head = (Node*)&heap[0];

    while (head) {
        if (head->free && head->space >= req) {
            int remaining = head->space - req;
            if (remaining > (int)sizeof(Node)) {
                Node* split = (Node*)((char*)(head + 1) + req);
                split->space = remaining - sizeof(Node);
                split->free = 1;
                split->next = head->next;

                head->next = split;
                head->space = req;
            }

            head->free = 0;
            return (void*)(head + 1);  // payload sits right after the header
        }
        head = head->next;
    }

    return NULL;
}

void c_reset(void) {
    // free_ptr = &heap[0];
    // left = HEAP_SIZE;
}

void c_free(void* ptr) {
    if (!ptr) return;

    Node* node = (Node*)ptr - 1;  
    node->free = 1;

    while (node->next && node->next->free) {
        node->space += sizeof(Node) + node->next->space;
        node->next = node->next->next;
    }
}

int main(int argc, char** argv[]) {
    memset(&heap[0], 0, HEAP_SIZE);
    Node* head = (Node*)&heap[0];

    head->space = HEAP_SIZE - sizeof(Node);
    head->free = 1;
    head->next = NULL;
    
    

    // // alloc succeeds and returns pointer into heap
    // unsigned char* rt = c_malloc(16);
    // assert(rt != NULL);
    // assert((char*)rt >= heap && (char*)rt < heap + HEAP_SIZE);

    // // write and read back
    // rt[0] = 'A';
    // rt[15] = 'Z';
    // assert(rt[0] == 'A' && rt[15] == 'Z');

    // // sentinel placed right after allocation
    // assert(((unsigned char*)rt)[16] == end_sentinel);

    // // second alloc lands after first + sentinel
    // unsigned char* rt2 = c_malloc(8);
    // assert(rt2 == rt + 17);

    // // oversized alloc fails
    // assert(c_malloc(HEAP_SIZE) == NULL);

    // // free zeroes the region
    // c_free(rt);
    // assert(rt[0] == 0 && rt[15] == 0);

    // printf("all checks passed\n");
}