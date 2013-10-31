#ifndef CONFIGCB_H
#define CONFIGCB_H

/***
 * @file ConfigCB.h
 * @brief Fichier de configuration du jeu. 
 ***/

/*** Type des cases ***/
#define VIDE 		0
#define BORD 		10
#define TROU 		20
#define COMPTEUR 	30
#define BALLE 		40
#define BRIQUE      50

/*** Taille du jeu ***/
#define LARGEUR 	80
#define HAUTEUR 	25
#define MENULARGEUR 12
/*** Parametrage des blocs ***/
#define B_CASES_H	2
#define B_CASES_L	3
#define L_ZONEJEU 	(LARGEUR- MENULARGEUR-2)/B_CASES_L
#define H_ZONEJEU	(HAUTEUR-5)/B_CASES_H

/*** Nombre de balles maximum ***/
#define BALLE_MAX 	5

/*** Vitesse des balles ***/
#define VITESSE 	400000

/*** Choix de la raquette ***/
#define RAQ 		1

#endif
