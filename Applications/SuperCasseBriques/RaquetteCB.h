#ifndef RAQUETTECB_H
#define RAQUETTECB_H

#include "ConfigCB.h"

/**
 * @file RaquetteCB.h
 * @class RaquetteCB
 * @brief Une raquette du jeu pouvant se déplacer horizontalement.
 */

class RaquetteCB {
	int x; // Coordonnée
	int gauche, droite; // gauche et droite de la raquette
public:

	// Constructeurs
	RaquetteCB();
	RaquetteCB(int taille, int ligne);

	/**
	 * Initialisation de la raquette sur la grille
	 */
	void init(int taille, int ligne);

	// Getters
	int getX();
	int getGauche();
	int getDroite();
	int getTaille();

	// Setters
	void setGauche(int);
	void setDroite(int);
	void setX(int);

	/**
	 * Permet de faire déplacer la raquette sur la gauche
	 */
	void movGauche();

	/**
	 * Permet de faire déplacer la raquette sur la droite
	 */
	void movDroite();
};
#endif
