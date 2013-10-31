#ifndef BALLECB_H
#define BALLECB_H

#include "RandomCB.h"
#include "GrilleCB.h"
#include "RaquetteCB.h"
#include "PartieCB.h"
#include <sextant/Activite/Threads.h>

/**
 * @file BalleCB.h
 * @class BalleCB
 * @brief Une balle du casse brique.
 */

class BalleCB: public Threads {

	/**
	 * Grille dans laquelle se trouve la balle
	 */
	GrilleCB* grille;

	/*
	 * Partie dans laquelle la balle est utilisé
	 */
	PartieCB* partie;

	/**
	 * Vitesse de déplacement de la balle
	 */
	int vitesse;

	/**
	 * Position de la balle dans la grille
	 * x =>  position horizontale
	 * y => position verticale
	 */
	int x, y;

	/**
	 * Direction de la balle donnée par le déplacement vertical et horizontal
	 */
	int dx, dy;

	/**
	 * Fait avancer la balle du déplacement dy,dx.
	 */
	void bouger();

	/**
	 * Action effectuée lorsque la balle tombe dans le trou
	 */
	bool actionTrou();

public:
	/**
	 * Constructeur : Ajoute une balle dans la grille et affecte une sémaphore à la balle
	 * @param Grille : la grille de jeu
	 * @param Partie :  partie du casse brique en cours
	 */
	BalleCB(GrilleCB*, PartieCB*);

	/**
	 * Permet de savoir si la balle est toujours en jeu.
	 * @return vrai si la balle est toujours en jeu.
	 */
	bool avance();

	/**
	 * Initialise la balle
	 */
	void init();

	/**
	 * Indique si la balle est initialisée
	 * @return vrai si la balle est initialisée
	 */
	bool isInit();

	/**
	 * Méthode run pour le thread de la balle
	 * Fait avancer la balle tant que le thread est actif
	 */
	void run();

	/**
	 * Effectue l'action requise sur une case
	 */
	void actionCase(int x, int y);
};

#endif
