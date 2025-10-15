#include "segdef.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 5

void init(int *shmid, int* semid, segment **seg) {
    *shmid = shmget(cle, segsize, 0); // get shared memory id
    if (*shmid == -1) {
        perror("shmget error");
    }

    *semid = semget(cle,3,0); // get semaphores id
    if (*semid == -1) {
        perror("semget error");
    }

    *seg = shmat(*shmid, NULL, 0); // attach memory
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

float test_communication_with_for(int semid, segment* seg, int n_iterations){
    int count = 0;
    for (int i = 0; i < n_iterations; i++) {
        int i = getpid();
        acq_sem_custom(semid, seg_dispo);
        seg->pid = i;
        seg->req = i;
        long local_tab[maxval];
        for (int j=0; j<maxval; j++){
            seg->tab[j] = getrand()%100;
        }
        long local_mean = mean(seg->tab, maxval);

        acq_sem_custom(semid, seg_init);
        wait_sem_custom(semid, res_ok);
        long server_mean = seg->result;
        lib_sem_custom(semid, seg_init);
        lib_sem_custom(semid,seg_dispo);
        if(server_mean==local_mean){
            count++;
        }
    }
    return (float)count/n_iterations*100;
}

void test_communication_with_fork(int semid, segment* seg){
    pid_t pid = fork();
    int status;
    if (pid != 0) { // parent process
        wait(&status);
        exit(EXIT_SUCCESS);
    }
    else {
        fork();fork();fork();fork();fork();fork();
        int i = getpid();
        acq_sem_custom(semid, seg_dispo);
        seg->pid = i;
        seg->req = i;
        long local_tab[maxval];
        for (int j=0; j<maxval; j++){
            seg->tab[j] = getrand()%100;
        }
        long local_mean = mean(seg->tab, maxval);

        acq_sem_custom(semid, seg_init);
        wait_sem_custom(semid, res_ok);
        long server_mean = seg->result;
        lib_sem_custom(semid, seg_init);
        lib_sem_custom(semid,seg_dispo);
        printf("Client ID: %d, Validation: %d\n\r",i,(server_mean==local_mean));
        wait(&status);
        exit(EXIT_SUCCESS);
    }

}


int main() {
    int shmid;
    int semid;
    segment* seg;
    init(&shmid, &semid, &seg);
    printf("shmid: %d,    semid: %d,    address of attachment: %p\n\r", shmid,semid,seg);

    float percent_success = test_communication_with_for(semid, seg, 4000);
    //test_communication_with_fork(semid, seg);
    printf("Percent of success: %f\n\r", percent_success);
    shmdt(seg); //detach memory
    return 0;
}
