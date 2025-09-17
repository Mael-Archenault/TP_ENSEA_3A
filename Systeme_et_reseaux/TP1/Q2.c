#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>


int main(){

    // Opening file
    
    char* path = "./test.txt";
    int fd = open(path, O_RDWR);
    if (fd==-1){
        perror("File could not be opened\n\r");
        exit(EXIT_FAILURE);
    }


    // Getting file stats
    struct stat file_stat;
    fstat(fd, &file_stat);

    size_t file_size = file_stat.st_size;


    // Mapping memory

    char* file = (char*) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    char* temp = malloc(sizeof(char)*file_size);
    for (int i = 0; i<file_size; i++){
        temp[i] = file[i];
    }

    // Inverting file
    for (int i=0; i<file_size; i++){
        file[i] = temp[file_size-i-1];
    }

    // Freeing memory 
    free(temp);
    if (munmap((void*)file, file_size)==-1){
        perror("Couldn't unmap memory");
        return -1;
    };

    printf("File inverted successfully\n\r");

    return 0;


}