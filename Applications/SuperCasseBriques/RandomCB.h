#ifndef RANDOMCB_H
#define RANDOMCB_H

/**
 * @file RandomCB.h
 * @class RandomCB
 * @brief Classe permettant de choisir un entier aléatoirement.
 */

class RandomCB {
	unsigned long int random(void);

public:
	/**
	 * Génération d'un entier aléatoire
	 */
	int nextInt(int);

	/**
	 * Génération d'un booleen aléatoire
	 */
	bool nextBool(void);
};

#endif
