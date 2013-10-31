#ifndef RANDOMCB_H
#define RANDOMCB_H

/**
 * @file RandomCB.h
 * @class RandomCB
 * @brief Classe permettant de choisir un entier al�atoirement.
 */

class RandomCB {
	unsigned long int random(void);

public:
	/**
	 * G�n�ration d'un entier al�atoire
	 */
	int nextInt(int);

	/**
	 * G�n�ration d'un booleen al�atoire
	 */
	bool nextBool(void);
};

#endif
