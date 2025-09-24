#include <stdio.h>
#include <unistd.h>

#define DEFAULT_BLOCK_SIZE 1024
#define MAGIC_NUMBER 0x0123456789ABCDEFL


typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre */
    size_t block_size; /* taille du free_memory_list bloc en octets*/
    long magic_number; /* 0x0123456789ABCDEFL */
} HEADER;


HEADER* free_memory_list = NULL;


void* mallloc_3is(size_t block_size){
    HEADER* current = free_memory_list;
    HEADER* previous = NULL;

    while(current != NULL){
        if (current->block_size>=block_size){
            HEADER* new =(HEADER* ) ((char*)current + sizeof(HEADER)+block_size+sizeof(MAGIC_NUMBER));

            new->ptr_next = current->ptr_next;
            new->block_size = current->block_size-block_size;
            new->magic_number = MAGIC_NUMBER;

            if (previous!=NULL){
                previous->ptr_next = new;
            }
            else{
                free_memory_list = new;
            }

            current->ptr_next = NULL;
            current->block_size = block_size;

            
        }
        previous = current;
        current = current->ptr_next;

    }

    // If no place is available
    HEADER* new = sbrk(sizeof(HEADER)+block_size+sizeof(MAGIC_NUMBER));
    if (new == -1){
        perror("Couldn't increase heap size");
        return (void *)-1;
    }
    new->ptr_next = NULL;
    new->block_size = block_size;
    new->magic_number = MAGIC_NUMBER;
    return (void*)(new+1);
};


void* free_3is(void* ptr){
    HEADER* header = (HEADER*)ptr -1;

    HEADER* current = free_memory_list;
    HEADER* previous = NULL;

    while(current != NULL && current < header){
        previous = current;
        current = current->ptr_next;
    }

    if (previous == NULL){
        free_memory_list = header;
    }
    else{
        previous->ptr_next = header;
    }
    header->ptr_next = current;
}

void display_free_memory_list(){
    HEADER* current = free_memory_list;
    while(current != NULL){
        display_memory_block(current);
        current = current->ptr_next;
    }
}

void display_memory_block(HEADER* head){
    printf("| Addr: %p | Block size: %d | Next: %p |\n\r", head, head->block_size, head->ptr_next);
}





int main(void) {
    int* tab = mallloc_3is(3*sizeof(int));

    for (int i = 0; i<3; i++){
        tab[i] = i;
    }
    for (int i = 0; i<3; i++){
        printf("%d\n\r", tab[i]);
    }

    int* tab2 = mallloc_3is(5*sizeof(int));

    for (int i = 0; i<5; i++){
        tab2[i] = i;
    }
    for (int i = 0; i<5; i++){
        printf("%d\n\r", tab2[i]);
    }

    int* tab3 = mallloc_3is(5*sizeof(int));

    for (int i = 0; i<5; i++){
        tab3[i] = i;
    }
    for (int i = 0; i<5; i++){
        printf("%d\n\r", tab2[i]);
    }

    free_3is(tab2);
    free_3is(tab3);

    display_free_memory_list();
}