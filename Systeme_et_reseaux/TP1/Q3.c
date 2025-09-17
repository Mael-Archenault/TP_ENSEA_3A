#include <stdlib.h>
#include <stdio.h>

#define N 10


typedef struct integer{
    int n;
    struct integer* next;
}LINKED_INTEGER;


int length(LINKED_INTEGER* list){
    int res = 0;
    while(list!=NULL){
        res++;
        list = list->next;
    }
    return res;
}

void display(LINKED_INTEGER* list){
    while(list!= NULL){
        printf("<%p> %d\n\r", list, list->n);
        list = list->next;
    }
    return;
}

LINKED_INTEGER* remove_first(LINKED_INTEGER* list){
    LINKED_INTEGER* next = list->next;
    free(list);
    return next;
}

LINKED_INTEGER* remove_last(LINKED_INTEGER* list){
    LINKED_INTEGER* temp = list;
    while((temp->next)->next!=NULL){
        temp = temp->next;
    }

    free(temp->next->next);
    temp->next = NULL;
    return list;
}

LINKED_INTEGER* add_one_element(LINKED_INTEGER* list){
    LINKED_INTEGER* temp = list;
    while(temp->next!=NULL){
        temp = temp->next;
    }
    
    LINKED_INTEGER* new = malloc(sizeof(LINKED_INTEGER));
    printf("OK\n\r");
    new->n = temp->n +1;
    new->next = NULL;
    temp->next = new;
    return list;

}
int main(){

    LINKED_INTEGER* previous_ptr = NULL;
    LINKED_INTEGER* current_ptr = NULL;
    for (int i=N-1; i>-1; i--){
        current_ptr = malloc(sizeof(LINKED_INTEGER));
        current_ptr->n = i;
        current_ptr->next = previous_ptr;

        previous_ptr = current_ptr;
    }
    printf("%d\n\r", length(current_ptr));
    display(current_ptr);
    //current_ptr = remove_first(current_ptr);
    //display(current_ptr);
    current_ptr = add_one_element(current_ptr);
    display(current_ptr);
    return 0;
}