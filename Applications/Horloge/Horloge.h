#ifndef HORLOGE_H
#define HORLOGE_H

#include <drivers/EcranV.h>
#include <drivers/ClavierV.h>
#include <sextant/types.h>
#include <drivers/timer.h>

/**
 * @file Horloge.h
 * @class Horloge
 * @brief Une horloge comptant de 0 à 9 lors des interruptions du timer.
 */
class Horloge {
	EcranV *e;
	Timer *t;
	int x;
	int y;
	int timeZero;

public :
	/**
	 * @brief horloge comptant d'après les interruptions générerées par l'horloge.
	 * @param e écran pour l'affichage
	 * @param c permet d'utiliser le clavier pour arrêter l'horloge.
	 * @param X position X
	 * @param Y position Y
	 */
	Horloge(EcranV* e,Timer* t, int X, int Y, bool startZero);

	void afficheHeure();
};

#endif
