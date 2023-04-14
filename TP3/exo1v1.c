#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



void child_work(int NF, int rang, pid_t pid) {
    for (int i = 0; i < NF; i++) {
        printf("Activite rang %d : identifiant = %d\n", rang, pid);
    }
}


int main(int argc, char const *argv[]) {
    
    if (argc != 3) {
        exit(1);
        printf("Error, 2 arg required.\n");
    }

    int NA = atoi(argv[1]);
    int NF = atoi(argv[2]);
    pid_t child_pid;
    pid_t pid;

    for (int i = 0; i < NA; i++) {
        pid = fork();
        if (pid == -1) {
            perror("Error fork()\n");
            exit(1);
        } 
        
        else if (pid == 0) {
            pid = getpid();
            child_work(NF, i, pid);
            exit(i);
        } 
    }

    int status;
    int valeur_retour;
    
    while (wait(&status) != -1) {
        if (WIFEXITED(status)) {
            valeur_retour = WEXITSTATUS(status);
            printf("Fin processus : %d\n", valeur_retour);
        }
    }

    return 0;
}
