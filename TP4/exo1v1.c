#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>


struct s_thread_args {
    int numero_afficheur;
    pthread_mutex_t * mutex;
    int NBM;
    int NBL;
};


void * thread_routine(void * args) {
    struct s_thread_args * thread_args = args;

    int numero_afficheur = thread_args->numero_afficheur;
    int NBM = thread_args->NBM;
    int NBL = thread_args->NBL;
    pthread_mutex_t * mutex = thread_args->mutex;

    pthread_t thread_id = pthread_self();

    int nb_msg_affiches = 0;
    int nb_lignes_affichees = 0;

    for (int i = 0; i < NBM; i++) {

        if (pthread_mutex_lock(mutex) != 0) {
            perror("Error locking mutex.\n");
            exit(1);
        }

        for (int j = 0; j < NBL; j++) {
            printf("Afficheur %d (%ld), j'affiche la ligne %d/%d du message %d/%d.\n", numero_afficheur, thread_id, nb_lignes_affichees+1, NBL, nb_msg_affiches+1, NBM);
            nb_lignes_affichees++;
        }
        nb_msg_affiches++;
        nb_lignes_affichees = 0;

        if (pthread_mutex_unlock(mutex) != 0) {
            perror("Error unlocking mutex.\n");
            exit(1);
        }
    }

    free(args);

}


int main(int argc, char const *argv[]) {

    // Test nb arguments
    if (argc != 4) {
        fprintf(stderr, "usage: %s nbt nbl nbm\n",argv[0]);
        exit(1);
    }
    
    // Recuperation des arguments
    int NBT = atoi(argv[1]);
    int NBL = atoi(argv[2]);
    int NBM = atoi(argv[3]);

    // Creation du tableau contenant les id des threads
    pthread_t threads_ids[NBT];

    // Creation et initialisation des mutex
    pthread_mutex_t mutex;

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Error pthread_mutex_init()\n");
        exit(1);
    }

    for (int i = 0; i < NBT; i++) {
        if (i == 0) {
            if (pthread_mutex_lock(&mutex) != 0) {
                perror("Error pthread_mutex_lock()");
            }
        }
    } 


    void * return_value;

    // Boucle de création des threads
    for (int i = 0; i < NBT; i++) {

        // Creation et initialisation des arguments a passer a la fonction thread_routine()
        struct s_thread_args * thread_args = malloc(sizeof(struct s_thread_args));
        thread_args->NBL = NBL;
        thread_args->NBM = NBM;
        thread_args->numero_afficheur = i;
        thread_args->mutex = &mutex;

        // Creation des threads
        if (pthread_create(&threads_ids[i], NULL, &thread_routine, thread_args) != 0) {
            perror("Error pthread_create().\n");
            exit(1);
        }
    }

    for (int i = 0; i < NBT; i++) {
        if (pthread_join(threads_ids[i], &return_value) != 0) {
            perror("Error join().\n");
            exit(1);
        }
    }
    
    if (pthread_mutex_destroy(&mutex) != 0) {
        perror("Error pthread_mutex_detroy().\n");
        exit(1);
    }
    
    return 0;
}
