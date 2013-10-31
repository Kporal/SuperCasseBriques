#ifndef CASSEBRIQUES_H
#define CASSEBRIQUES_H

#include <drivers/ClavierV.h>
#include <drivers/EcranV.h>
#include <sextant/Activite/Threads.h>
#include "ConfigCB.h"
#include "GrilleCB.h"
#include "BalleCB.h"
#include "PartieCB.h"

/**
 * @file CasseBriques.h
 * @class CasseBriques
 * @brief Le jeu Casse-Brique avec une raquettes, une ou plusieurs balles et de jolies couleurs.
 */

extern Timer timer;

class CasseBriques: public Threads {

	/**
	 * Grille de l'application
	 */
	GrilleCB laGrille;

	/**
	 * Permet la gestion du clavier dans l'application
	 */
	ClavierV *clavier;

	/**
	 * Permet la gestion de l'ecran dans l'application
	 */
	EcranV *ecran;

	/**
	 * Booleen indiquant si la partie est en cours
	 */
	bool jouer;

	/**
	 * Booleen indiquand si la partie peut être rejouée
	 */
	bool rejouer;

	/**
	 * Booleen indiquand si la partie est perdu
	 */
	bool perdu;

	/**
	 * Booleen indiquand si la partie est gagnée
	 */
	bool gagne;

public:
	/**
	 * Constructeur : Créer le jeu casse brique
	 * @param EcranV : ecran utilisé par le jeu
	 * @param ClavierV :  clavier utilisé par le jeu
	 */
	CasseBriques(EcranV *e, ClavierV *c);

	/**
	 * Démarre l'application casse briques
	 */
	void start();

	/**
	 * Initialise la grille de l'application
	 */
	void init();

	/**
	 * Initialise la grille de l'application
	 */
	void init(EcranV*, ClavierV*);
};

#endif
