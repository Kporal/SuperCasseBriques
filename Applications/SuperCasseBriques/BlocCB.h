/*
 * blocCB.h
 *
 *  Created on: 24 oct. 2013
 */

#ifndef BLOCCB_H_
#define BLOCCB_H_

#include "configCB.h"
#include "CaseCB.h"

/**
 * Proprietes d'un bloc
 */
struct BlocProps {
	/*
	 * Resistance du bloc = nombre de coup avant la destruction du boc
	 */
	int resist;
	/*
	 * Points donnés par le bloc lorsqu'il est detruit
	 */
	int points;
	/*
	 * Couleur du bloc
	 */
	Couleur couleur;
};

/**
 * Bloc de cases, determinant leur comportement (points, resistance, ...)
 */
class BlocCB {
	/*
	 * Tableau de pointeur sur les cases de la grilles composant le bloc
	 */
	CaseCB** tab;
	/*
	 * Proprietes du bloc
	 */
	BlocProps properties;

public:
	/*
	 *  Bloc vide pour l'initialisation avec des proprietes par defaut
	 */
	BlocCB() {
		properties.couleur = BLEU_LEGER;
		properties.points = 100;
		properties.resist = 3;
	}
	/*
	 * Creation d'un bloc avec des proprietes
	 */
	BlocCB(BlocProps props);

	/*
	 * Changement des proprietes du bloc
	 */
	void changeProps(BlocProps props);
	/*
	 * Assignations des cases de la grille au bloc
	 */
	void assignBloc(CaseCB** t);
	/*
	 * Action sur le bloc
	 */
	void action();
};

#endif /* BLOCCB_H_ */
