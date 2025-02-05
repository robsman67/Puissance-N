#include <stdio.h>
#include "IA.h"
#include "Jouer.h"
#include "Demarrage_partie.h"
#include "SauvegardeEtchargement.h"

int main() {

    Grid grille;
    char start;
    int player=0;
    char prenom1[100];
    char prenom2[100];
    int size;
    char jeton = 0;
    int c = 0;
    int nb_jetons;
    int retrait = -1;


    printf("\n=============PUISSANCE N============= \n\n");
    //Démarrage du jeu
    start = demarrage();

    switch (start) {
        case 'N': printf("\n=============NOUVELLE PARTIE============= \n\n");
            //Choix de l'adversaire
            player = demarrage_nouvelle_partie(prenom1, prenom2);
            //Choix du nombre de jetons à aligner et définition de la taille de la grille
            size = taille();
            //Initialisation de la grille avec des '_'
            init_grid(&grille, size);
            //Affichage de la grille initialisée
            show_grid(grille);
            printf("\n");
            play(grille, jeton, player, prenom1, prenom2, c, size, 0, retrait);
            break;
        case 'C': printf("\n=============PARTIE EN COURS============= \n\n");

            FILE* fichier = fopen("fichier.txt", "r");
            if(fichier != NULL) {
                load(fichier,&grille,prenom1,prenom2,&player,&retrait,&nb_jetons,&size); // Charge la partie sauvegardée
                fclose(fichier);
                show_grid(grille);
                printf("\n\n");
                play(grille, jeton, player, prenom1, prenom2, c, size, nb_jetons,retrait);
            } else {
                printf("Aucune partie sauvegardee");
            }

            break;
        case 'Q': printf("\n=============FIN DE LA PARTIE============= \n");
            break;
    }
    return 0;
}
