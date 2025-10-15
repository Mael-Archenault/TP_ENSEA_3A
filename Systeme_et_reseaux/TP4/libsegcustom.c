#include "segdef.h"

void acq_sem_custom(int semid, int semnum){
    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    semop(semid, &sops, 1);
}

void wait_sem_custom(int semid, int semnum){
    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = 0;
    sops.sem_flg = 0;
    semop(semid, &sops, 1);
}

void lib_sem_custom(int semid, int semnum) {
    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = +1;
    sops.sem_flg = 0;
    semop(semid, &sops, 1);
}
