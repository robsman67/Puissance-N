//
// Created by coco on 30/05/2021.
//

#include "Jouer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "IA.h"
#include "Demarrage_partie.h"
#include "SauvegardeEtchargement.h"
#include "Demarrage_partie.h"

int add_token(Grid grille, int c, char jetons, bool test_ordi){
    int l=0;
    int insertion = 0;

    // boucle afin de trouver la premiere case vide de la colonne
    while(l < grille.hauteur && grille.tableau [l][c] != '_'){
        l++;
    }
    if(l < grille.hauteur){
        // La hauteur de la grille n'est pas atteinte on remplit donc la première case vide de la colonne par un jeton
        grille.tableau[l][c] = jetons;
        insertion = 1;
    }
    else{
        // On a atteind le haut de la colonne et donc pas de jeton à ajouter
        insertion = 0;
        if (test_ordi == false) {
            // Ce n'est pas l'ordi qui fait un test donc on peut afficher le message
            printf("ERREUR, colonne pleine \n");
        }
    }

    return insertion;
}

int check_winner(Grid grille, int alignement_gagant)
{
    int gagnant;
    bool alignement_en_cours;
    int l; // ligne
    int c = 0;  // colone
    int indice_c, indice_l;
    int jetons_alignes = 1;

    // Parcours de la grille horizonatlement tant qu'il n'y a pas de gagnant et que nous ne somme pas au bout de la grille
    do {

        l = 0;
        // Parcours de la grille verticalement tant qu'il n'y a pas de gagnant,
        // que nous ne sommes pas au bout de la grille
        // et qu'il y a un jeton dans la case
        while (jetons_alignes != alignement_gagant && l < grille.hauteur && grille.tableau[l][c] != '_'){

            indice_c = c;
            indice_l = l;
            alignement_en_cours = true;

            // Vérification d'un éventuel alignement gagnant horizontalement
            while (alignement_en_cours == true && jetons_alignes != alignement_gagant && c <= (grille.largeur - alignement_gagant)){
                if(grille.tableau[l][c] != '_' && (grille.tableau[l][c] == grille.tableau[l][indice_c + 1])) {
                    // La case à droite contient le même jeton que la case actuelle
                    // On incrémente l'alignement
                    jetons_alignes++;
                    indice_c++;

                } else {
                    jetons_alignes = 1;
                    alignement_en_cours = false;
                }
            }

            if (jetons_alignes != alignement_gagant)
            {
                // Il n'y a pas encore d'alignement gagnant en horizontale
                indice_c = c;
                indice_l = l;
                alignement_en_cours = true;
                // Vérification d'un éventuel alignement gagnant diagonnale droite
                while (alignement_en_cours == true && jetons_alignes != alignement_gagant && c <= grille.largeur - alignement_gagant && l <= grille.hauteur - alignement_gagant) {
                    if(grille.tableau[l][c] != '_' && grille.tableau[l][c] == grille.tableau[indice_l + 1][indice_c + 1]){
                        // La case du dessus à droite contient le même jeton que la case actuelle
                        // On incrémente l'alignement
                        jetons_alignes++;
                        indice_c++;
                        indice_l++;
                    } else {
                        jetons_alignes = 1;
                        alignement_en_cours = false;
                    }
                }
            }

            if (jetons_alignes != alignement_gagant) {
                // Il n'y a pas encore d'alignement gagnant en horizontale
                // ni en diagonale droite
                indice_c = c;
                indice_l = l;
                alignement_en_cours = true;
                // Vérification d'un éventuel alignement gagnant verticalement
                while (alignement_en_cours == true && jetons_alignes != alignement_gagant && l <= grille.hauteur - alignement_gagant) {
                    if (grille.tableau[l][c] != '_' && grille.tableau[l][c] == grille.tableau[indice_l + 1][c]) {
                        // La case du dessus contient le même jeton que la case actuelle
                        // On incrémente l'alignement
                        indice_l++;
                        jetons_alignes++;
                    } else {
                        jetons_alignes = 1;
                        alignement_en_cours = false;
                    }
                }
            }

            if (jetons_alignes != alignement_gagant){
                // Il n'y a pas encore d'alignement gagnant en horizontale
                // ni en diagonale droite
                // ni en verticale
                indice_c = c;
                indice_l = l;
                alignement_en_cours = true;
                // Vérification d'un éventuel alignement gagnant diagonale gauche
                while (alignement_en_cours == true && jetons_alignes != alignement_gagant && l <= grille.hauteur - alignement_gagant && c >= alignement_gagant - 1) {
                    if (grille.tableau[l][c] != '_' && grille.tableau[l][c] == grille.tableau[indice_l + 1][indice_c - 1]){
                        // La case du dessus à gauche contient le même jeton que la case actuelle
                        // On incrémente l'alignement
                        indice_l++;
                        indice_c--;
                        jetons_alignes++;
                    } else {
                        jetons_alignes = 1;
                        alignement_en_cours = false;
                    }
                }
            }

            l++;

        } // Fin de parcours des lignes

        c++;

    } while (jetons_alignes != alignement_gagant && c < grille.largeur); // Fin de parcours des colonnes
    // On sort de la boucle s'il y a un alignement gagnant ou qu'on est au bout de la grille

    // Recherche s'il y a un alignement gagnant
    if(jetons_alignes == alignement_gagant && grille.tableau[l - 1][c - 1] == 'O'){
        // On regarde le dernier coup gagnant
        // si c'est un 'O' alors c'est le joueur 1 qui a gagané
        gagnant = 0;
    } else if(jetons_alignes == alignement_gagant && grille.tableau[l - 1][c - 1] == 'X'){
        // On regarde le dernier coup gagnant
        // si c'est un 'X' alors c'est le joueur 2 qui a gagané
        gagnant = 1;
    } else {
        // Pas de gagnant
        gagnant = -1;
    }

    return gagnant;
}

void play(Grid grille, char jeton, int player, char prenom1[], char prenom2[], int c, int size, int nb_jetons, int retrait)
{
    int winner;
    bool ordi;
    char choix;
    char choix_ordi;
    int c_ordi;
    char jeton_sup;


    // Boucle qui fait jouer les joueurs chacun leur tour
    do {
        // Connaître le joueur qui doit jouer
        // Quand nb_jetons est pair alors c'est au joueur 1 de jouer
        // Quand nb_jetons est impair alors c'est au joueur 2 de jouer si player == 0 ou à l'ordi quand player == 1
        if (nb_jetons % 2 == 0) {
            ordi = false;
            jeton = 'O';
            printf("+   C'est a");
            Color(14,0);
            printf(" %s",prenom1);
            Color(15,0);
            printf(" de jouer\n");
        } else if (nb_jetons % 2 != 0) {
            if (player == 0) {
                ordi = false;
                jeton = 'X';
                printf("+   C'est a");
                Color(4,0);
                printf(" %s",prenom2);
                Color(15,0);
                printf(" de jouer\n");
            } else if(player == 1) {
                // Faire jouer l'ordi ou quitter la partie + contrôle
                do {
                    printf("Rentrer O pour faire jouer l'ordinateur ou Q pour quitter: \n");
                    scanf(" %c", &choix_ordi);
                    choix_ordi = toupper(choix_ordi);
                } while (choix_ordi != 'O' && choix_ordi != 'Q');

                ordi = true;
                if (choix_ordi == 'O') {
                    printf("+   C'est a");
                    Color(4,0);
                    printf(" l'ordinateur");
                    Color(15,0);
                    printf(" de jouer\n");
                    retrait = computer(grille, size, retrait);
                    if (retrait == -1) {
                        // L'ordinateur a ajouté un jeton
                        nb_jetons++;
                    } else {
                        // l'odinateur à retiré un jeton
                        nb_jetons--;
                    }
                }

            }
            else if (player ==2) {
                // Le joueur joue contre l'ordinateur en mode facile
                // l'ordinateur ne fait qu'ajouter un jeton aléatoirement dans la grille
                do {
                    printf("Rentrer O pour faire jouer l'ordinateur ou Q pour quitter: \n");
                    scanf(" %c", &choix_ordi);
                    choix_ordi = toupper(choix_ordi);
                } while (choix_ordi != 'O' && choix_ordi != 'Q');
                ordi = true;

                if (choix_ordi == 'O') {
                    printf("+   C'est a l'ordinateur de jouer \n");
                    srand(time(NULL));
                    c_ordi = rand() % grille.hauteur + 1;
                    add_token(grille, c_ordi - 1, 'X', false);
                    nb_jetons++;
                }
            }
        }

        if (ordi == false)
        {
            // Demande à l'utilisateur le choix qu'il souhaite faire
            if(nb_jetons < 2){
                // Si chaque joueur n'a pas au moins joué un jeton on ne peut pas en retirer
                do {
                    printf("    -   Rentrer A si vous vouler ajouter un jeton ou Q si vous voulez quitter: \n");
                    scanf(" %c", &choix);
                    choix = toupper(choix);
                } while (choix != 'A' && choix != 'Q');
            } else {
                do {
                    printf("    -   Rentrer A si vous vouler ajouter un jeton et R si vous voulez retirer un jeton ou Q si\n"
                           "vous voulez quitter et sauvegarder : \n");
                    scanf(" %c", &choix);
                    choix = toupper(choix);
                } while (choix != 'A' && choix != 'R' && choix != 'Q');
            }

            // En fonction du choix fait, lancer la bonne fonction (add_token ou remove_token)
            if(choix == 'R')
            {
                // Retrait d'un jeton
                if(nb_jetons != 0) {
                    do {
                        printf("        /   Dans quelle colonne voulez vous retirer un jeton ?\n");
                        c = saisie_int();
                    } while (c <= 0 || c > grille.largeur);
                    jeton_sup = remove_token(grille, c - 1, false);
                    if (jeton_sup != 'N' ) {
                        // On a pu retirer un jeton, sinon on reboucle sur le joueur en cours
                        nb_jetons--;
                        retrait = c;
                    }
                }
                else{
                    printf("    .   ERREUR, grille vide\n");
                }
            } else if (choix == 'A'){
                // Ajout d'un jeton
                // vérifie que la saisie de la colonne est bien valide (un entier compris entre [0, grille.hauteur])
                // et qu'on a pas rentré la colonne précédamment retirée
                do {
                    printf("        /   Dans quelle colonne voulez vous ajouter un jeton ?\n");
                    c = saisie_int();
                    if (c == retrait){
                        printf(".   Vous ne pouver pas jouer dans cette colonne "
                               "car le joueur precedent a retirer un jeton \n");
                    }
                } while (c == retrait || c <= 0 || c > grille.largeur);

                if (add_token(grille, c-1, jeton, false)) {
                    // On a pu ajouter un jeton, sinon on ne fait rien et on reboucle sur le joueur en cours
                    nb_jetons++;
                    retrait = -1;
                }
            }
        }

        show_grid(grille);
        winner = check_winner(grille,size);

        printf("\n");

    } while(winner == -1 && nb_jetons < (grille.largeur * grille.hauteur) && choix != 'Q' && choix_ordi != 'Q');
    // On sort de la boucle si on a un vainqueur ou qu'on a joué tous les jetons ou qu'on veut quitter

    if(choix == 'Q' || choix_ordi == 'Q'){
        save(grille, nb_jetons, prenom1, prenom2,player,retrait);
    }

    if (winner == 0){
        Color(14, 0);
        printf("=== %s a gagne ===", prenom1);
        Color(15, 0);
    } else if (winner == 1) {
        if (player == 0){
            Color(4, 0);
            printf("=== %s a gagne ===", prenom2);
            Color(15, 0);
        } else {
            Color(4, 0);
            printf("=== L'ordinateur a gagne ===");
            Color(15, 0);
        }
    } else {
        printf("=== Match nul ===");
    }
}

char remove_token(Grid grille, int c, bool test_ordi)
{
    int l = grille.hauteur -1; // j'ai modifié
    char jeton_sup;

    // boucle afin de trouver la premiere case pleine de la colonne
    while(l >= 0 && grille.tableau[l][c] == '_'){
        l--;
    }
    if(l >= 0){
        // Il y a au moins une case de remplie donc on remplace la première case trouvée avec un jeton par une case vide
        jeton_sup = grille.tableau[l][c];
        grille.tableau[l][c] = '_';
    }
    else{
        // On a atteind le bas de la colonne et donc pas de jeton à supprimer
        jeton_sup = 'N';
        if (test_ordi == false) {
            // Ce n'est pas l'ordi qui fait un test donc on peut afficher le message
            printf("ERREUR, colonne vide \n");
        }
    }

    return jeton_sup;  // 1 si on a pu supprimer un jeton, 0 sinon
}

int saisie_int() {

    int i = 0;
    int entier = -1;
    char saisie[10];
    bool is_integer = false;

    // mise en chaine de caractères de ma saisie
    scanf("%s", saisie);
    fflush(stdin);

    // Vérifie si tous les caractères sont compris entre '0' et '9'
    while (saisie[i] != '\0' && saisie[i] >= '0' && saisie[i] <= '9') {
        i++;
        is_integer = true;
    }
    // Si tous les caractère sont des entier alors on les convertis en entier
    // Sinon on retourne -2
    if (saisie[i] == '\0' && is_integer == true) {
        entier = atoi(saisie);
    } else {
        entier = -2;
    }

    return entier;
}
