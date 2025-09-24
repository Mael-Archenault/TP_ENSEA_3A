#include <stdio.h>
#include <unistd.h>
#define MAGIC_NUMBER 0x123456789ABCDEFL
#define DEFAULT_BLOC_SIZE 1024

typedef struct HEADER_TAG {
    struct HEADER_TAG* ptr_next;
    size_t bloc_size;
    long magic_number;
} HEADER;

HEADER *memory = NULL;


void *malloc_3is(int size) {
    HEADER *temp = memory;
    while(temp != NULL){

    }

    HEADER *new = sbrk(sizeof(HEADER) + size + sizeof(MAGIC_NUMBER));
    if(new == (void *)-1) {
        perror("Could not increase heap size");
        return (void *)-1;
    }
    new->ptr_next = NULL;
    new->bloc_size = size;
    new->magic_number = MAGIC_NUMBER;
    return (void *)(new + 1 + sizeof(HEADER));
}

int main(void) {
    int *i = malloc_3is(sizeof(int));
    printf("%p \n\r",i);
    return 0;
};