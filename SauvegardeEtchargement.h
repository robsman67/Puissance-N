//
// Created by coco on 02/06/2021.
//

#ifndef IFB_PROJET_SAUVEGARDEETCHARGEMENT_H
#define IFB_PROJET_SAUVEGARDEETCHARGEMENT_H
#include <stdio.h>
#include "Grille.h"


/**
 * Supprime le caractère de retour (\n) à la fin dune chaine
 * @param chaine La chaine a laquelle on doit supprimer le retour
 */
void del_return(char* chaine);


/**
 * Charge la partie enregistrée
 * @param fichier le fichier d'enregistrement
 * @param grille la grille
 */
void load(FILE *fichier, Grid *grille, char prenom1[100], char prenom2[100], int *player, int *retrait, int *nb_jetons, int *size);


/**
 * Faire jouer l'ordinateur
 * @param grille la grille
 * @param size l'alignement gagnant
 * @param retrait le numéro de colonne où le joueur précédent à enlevé un jeton
 */
void save (Grid grille, int nb_jetons, char prenom1[], char prenom2[],int player, int retrait);

#endif //IFB_PROJET_SAUVEGARDEETCHARGEMENT_H
