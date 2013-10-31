#ifndef CASECB_H
#define CASECB_H

/**
 * @file CaseCB.h
 * @class CaseCB
 * @brief Une case de la grille de jeu.
 */

#include <drivers/EcranV.h>
#include "ConfigCB.h"

class CaseCB {
	/**
	 * Ecran pour l'affichage de la case
	 */
	EcranV* ecran;

	/**
	 * coordonnées de la case dans la grille.
	 */
	int x, y;

	/**
	 * Etat de la case
	 * Permet de savoir si la case est un trou, un mur, une raquette, une brique ou une balle.
	 */
	int etat;

	/**
	 * Etat de l'ancienne case
	 */
	int etatold;

	/**
	 * Attributs de la brique
	 */
	Couleur couleurBrique;
	char caractereBrique;

	/**
	 * Couleur du premier plan de la case.
	 */
	Couleur couleurPr();

	/**
	 * Couleur de l'arrière plan de la case.
	 */
	Couleur couleurAr();

	/**
	 * Caractère de la case.
	 */
	char Caractere();

public:
	// Setters
	void setEcran(EcranV* e);
	void setCaseVide();
	void setCaseBord();
	void setCaseTrou();
	void setCaseCompt(int);
	void setCaseBrique(Couleur c, char etatB = ' ');

	// Getters
	EcranV* getEcran();
	int getEtat();
	int getEtatOld();

	/**
	 * Permet de connaître le nombre de balles perdues pendant le jeu.
	 */
	int nbBallePerdue(int);

	/**
	 * définit la case x,y comme une case vide.
	 */
	void init(int x, int y);

	/**
	 * Affiche la case.
	 */
	void paint();

	/**
	 * Occupe la case
	 */
	void occupe();

	/**
	 * Libère la case
	 */
	void libere();

	/**
	 * Effectue une action sur la case correspondante
	 */
	void action();
};
#endif
