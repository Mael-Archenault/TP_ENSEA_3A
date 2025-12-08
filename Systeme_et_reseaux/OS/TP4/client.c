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
        pid_t pid = getpid();
        acq_sem_custom(semid, seg_dispo);
        seg->pid = pid;
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
        else {
            printf("iteration: %d    server mean: %d    local mean: %d\n\r",i,server_mean,local_mean);
        }
        sleep(0.01); //the sleep accomodates for the speed of communication between client and server
    }
    return (float)count/n_iterations*100;
}

float test_communication_with_fork(int semid, segment* seg, int n_children){
    int process_count = 0;
    int success_count = 0;
    for (int k = 0; k < n_children; k++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }
        if (pid == 0) { // child process
            int i = getpid();
            acq_sem_custom(semid, seg_dispo);
            seg->pid = i;
            seg->req = i;
            long local_tab[maxval];
            for (int j=0; j<maxval; j++) {
                seg->tab[j] = getrand()%100;
            }
            long local_mean = mean(seg->tab, maxval);

            acq_sem_custom(semid, seg_init);
            wait_sem_custom(semid, res_ok);
            long server_mean = seg->result;
            lib_sem_custom(semid, seg_init);
            lib_sem_custom(semid, seg_dispo);

            if (server_mean == local_mean) {
                exit(1);
            }
            else {
                exit(0);
            }
        }
        else { // parent process
            process_count++;
        }
    }

    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            success_count += WEXITSTATUS(status);
        }
    }
    if (process_count == 0) return 0.0f;
    return (float)success_count / process_count * 100.0f;
}

int main() {
    int shmid;
    int semid;
    segment* seg;
    init(&shmid, &semid, &seg);
    printf("shmid: %d,    semid: %d,    address of attachment: %p\n\r", shmid,semid,seg);

    float percent_success_with_for = test_communication_with_for(semid, seg, 4000);
    printf("With for loop: percent of success: %f\n\r", percent_success_with_for);
    float percent_success_with_fork = test_communication_with_fork(semid, seg, 4000);
    printf("With forked processes: percent of success: %f\n\r", percent_success_with_fork);
    shmdt(seg); //detach memory
    return 0;
}

