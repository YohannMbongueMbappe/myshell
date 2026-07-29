#include <stdio.h>      // printf(), fgets(), fflush()
#include <stdlib.h>     // exit(), EXIT_FAILURE
#include <string.h>     // strcmp(), strtok(), strcspn()
#include <unistd.h>     // fork(), execvp()
#include <sys/wait.h>   // wait()

// Taille maximale d'une ligne saisie
#define MAX_LINE 1024

// Nombre maximal d'arguments d'une commande
#define MAX_ARGS 64

int main() {

    // Buffer qui contiendra la commande tapée par l'utilisateur
    char buffer[MAX_LINE];

    // Tableau de pointeurs vers les arguments
    // Exemple :
    // ls -la /home
    //
    // args[0] = "ls"
    // args[1] = "-la"
    // args[2] = "/home"
    // args[3] = NULL
    char *args[MAX_ARGS];

    // Boucle infinie : le shell reste lancé jusqu'à "exit"
    while (1) {

        // Affiche l'invite de commande
        printf("myshell> ");

        // Force l'affichage immédiat
        fflush(stdout);

        // Lit une ligne tapée au clavier
        // Si CTRL+D est pressé, fgets renvoie NULL
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        // Remplace le '\n' par '\0'
        // Exemple :
        //
        // "ls\n"
        //
        // devient
        //
        // "ls"
        buffer[strcspn(buffer, "\n")] = '\0';

        // Si l'utilisateur tape "exit", on quitte le shell
        if (strcmp(buffer, "exit") == 0)
            break;

        //---------------------------------------------------
        // Découpage de la commande en arguments
        //---------------------------------------------------

        int i = 0;

        // Découpe la chaîne selon les espaces
        char *token = strtok(buffer, " ");

        // Continue tant qu'il reste des mots
        while (token != NULL && i < MAX_ARGS - 1) {

            // Stocke chaque argument
            args[i++] = token;

            // Passe au mot suivant
            token = strtok(NULL, " ");
        }

        // execvp exige que le tableau se termine par NULL
        args[i] = NULL;

        //---------------------------------------------------
        // Création d'un nouveau processus
        //---------------------------------------------------

        pid_t pid = fork();

        //---------------------------------------------------
        // PROCESSUS ENFANT
        //---------------------------------------------------

        if (pid == 0) {

            // Remplace le processus enfant
            // par la commande demandée
            //
            // Exemple :
            // execvp("ls", args)
            //
            // Le programme devient /bin/ls
            execvp(args[0], args);

            // Si execvp revient ici,
            // c'est qu'une erreur s'est produite
            perror("execvp");

            // Termine le processus enfant
            exit(EXIT_FAILURE);
        }

        //---------------------------------------------------
        // PROCESSUS PARENT
        //---------------------------------------------------

        else if (pid > 0) {

            // Attend que l'enfant termine
            wait(NULL);
        }

        //---------------------------------------------------
        // Erreur de fork()
        //---------------------------------------------------

        else {

            perror("fork");
        }
    }

    // Fin du shell
    return 0;
}