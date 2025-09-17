#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int global_variable = 64;
int not_initialized_variable;


void dummy_function(){
    return;
}


int main(){

    char* string_variable = "This is a string";
    char* allocated_variable = malloc(sizeof(char)*128);

    int stack_variable = 64;
    void* mmap_variable = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);



    printf("Global variable adress: %p\n\r", &global_variable); 
    printf("Not initialized variable adress: %p\n\r", &not_initialized_variable); 
    printf("String variable adress: %p\n\r", string_variable);
    printf("Allocated variable adress: %p\n\r", allocated_variable); 
    printf("Stack variable adress: %p\n\r", &stack_variable); 
    printf("Code address: %p\n\r", &dummy_function); 
    printf("Standard lib address: %p\n\r", &printf);
    printf("Mmap allocation address: %p\n\r", mmap_variable);



    //return 0;

    // Call of the pmap function (with exec function)

    int pid = getpid();

    char* pid_str = malloc(sizeof(char)*128);
    snprintf(pid_str, sizeof(pid_str), "%d", pid);


    char* args[] = {"pmap", "-X", pid_str, NULL};

    execvp(args[0], args);

    printf("Test\n\r");
   
    return 0;
}