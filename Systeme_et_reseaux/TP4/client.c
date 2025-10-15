#include "segdef.h"
#include <unistd.h>
#include <stdio.h>

#define MAX_REQUESTS 5

struct sembuf sop;

void init(int *shmid, int* semid, segment **seg) {
    *shmid = shmget(cle, segsize, 0);
    if (*shmid == -1) {
        perror("shmget error");
    }

    *semid = semget(cle,3,0);
    if (*semid == -1) {
        perror("semget error");
    }

    *seg = shmat(*shmid, NULL, 0);
    if (*seg == (void*)-1) {
        perror("shmat error");
    }

    init_rand();
}

long mean(long* arr, int size) {
    long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

int main() {



    int shmid;
    int semid;
    segment* seg;
    init(&shmid, &semid, &seg);
    printf("shmid: %d,    semid: %d,    address of attachment: %p\n\r", shmid,semid,seg);

    for (int i = 0; i < MAX_REQUESTS; i++) {
        printf("\n\rRequest number : %d\n\r", i);
        acq_sem(semid, seg_dispo);
        seg->pid = getpid();
        seg->req = i;
        long local_tab[maxval];
        for (int j=0; j<maxval; j++){
            seg->tab[j] = getrand();
        }
        long local_mean = mean(seg->tab, maxval);

        acq_sem(semid, seg_init);
        wait_sem(semid, res_ok);
        long server_mean = seg->result;
        lib_sem(semid, seg_init);
        lib_sem(semid,seg_dispo);
        
        printf("Server mean: %d,   Local mean: %d,  Validation: %d\n\r", server_mean, local_mean, (server_mean==local_mean));
    }
    

    return 0;
}
