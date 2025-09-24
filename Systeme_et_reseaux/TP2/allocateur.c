#include <stdio.h>
#include <unistd.h>

#define DEFAULT_BLOCK_SIZE 1024
#define MAGIC_NUMBER 0x0123456789ABCDEFL


typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; 
    size_t block_size; 
    long magic_number; 
} HEADER;


HEADER* free_memory_list = NULL;


void* malloc_3is(size_t block_size){
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


            *((char*)current + sizeof(HEADER) + block_size) = MAGIC_NUMBER;

            return (void*)(current +1);

            
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

    
    *((char*)new + sizeof(HEADER) + block_size) = MAGIC_NUMBER;

    return (void*)(new+1);
};


void* free_3is(void* ptr){
    HEADER* header = (HEADER*)ptr -1;

    // Magic number Verification (not working)

    // long magic_number = *(long*)((char*)header + sizeof(HEADER) + header->block_size);
    // if (header->magic_number != magic_number){
    //     printf("%d\n\r%d\n\r", magic_number, header->magic_number);
    // };

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
    printf("-----Free Memory Blocks ------\n\r");
    HEADER* current = free_memory_list;
    while(current != NULL){
        display_memory_block(current);
        current = current->ptr_next;
    }
    printf("-----------------------\n\r\n\r");
}

void display_memory_block(HEADER* head){
    printf("| Addr: %p | Block size: %d | Next: %p |\n\r", head, head->block_size, head->ptr_next);
}





int main(void) {

    // 1.Multiple allocations

    int* tab1 = malloc_3is(3*sizeof(int));
    int* tab2 = malloc_3is(3*sizeof(int));

    for (int i = 0; i<3; i++){
        tab1[i] = i;
        tab2[i] = i;
    }
    for (int i = 0; i<4; i++){
        printf("%d %d\n\r", tab1[i], tab2[i]);
    }

    // 2. Block liberation

    free_3is(tab2);
    display_free_memory_list();

    // 3. Memory overflow verification

    // Not working

    // 4. Reuse free memory

    int* tab2_bis = malloc_3is(2*sizeof(int));
    // tab2_bis will reuse the 12 bytes place that was liberated in .2
    // but it only needs 8 bytes so, the free memory list will have a block of size 4
    display_free_memory_list();

    // 5. High size blocks cutting

    // Works automatically (see previous question)


}