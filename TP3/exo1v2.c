#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


struct s_arg {
    int rang;
    int NA;
};

void * f_thread(void * p) {
    long unsigned mon_id = pthread_self();
    printf("Thread %d : mon identificateur est %d\n", (*(struct s_arg *)p).rang, mon_id);
    int * res = malloc(sizeof(int));
    *res = (*(struct s_arg *)p).rang;
    pthread_exit((void*)res);
}


int main(int argc, char * argv[]) {

    if (argc != 3) {
        printf("Error : 3 arg required\n");
        exit(1);
    }

    int NF = atoi(argv[1]);
    int NA = atoi(argv[2]);
    pthread_t ptids[NF];
    void * ret_values[NF];
    int * res;


    struct s_arg mes_arg[NF];

    for(int i = 0; i < NF; i++) {
        mes_arg[i].rang = i;
        mes_arg[i].NA = NA;
        if (pthread_create(&ptids[i], NULL, f_thread, &mes_arg[i]) != 0) {
            perror("Error pthread_create().\n");
            exit(1);
        }
    }

    for (int i = 0; i < NF; i++) {
        pthread_join(ptids[i], (void**)&res);
        printf("Valeur retournee par le thread %d = %d\n", ptids[i], *res);
    }

}