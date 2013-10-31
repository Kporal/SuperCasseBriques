/*
 * blocCB.cpp
 *
 *  Created on: 24 oct. 2013
 */

#include "BlocCB.h"

BlocCB::BlocCB(BlocProps props) {
	properties = props;
}

void BlocCB::changeProps(BlocProps props) {
	properties.couleur = props.couleur;
	properties.points = props.points;
	properties.resist = props.resist;
}

void BlocCB::assignBloc(CaseCB** t) {
	for (int i = 0; i < B_CASES_H * B_CASES_L; i++) {
		tab[i] = t[i];
		// Transformation de la case en brique
		tab[i]->setCaseBrique(properties.couleur);
	}
}

void BlocCB::action() {
	// reduit la resistance de la brique
	properties.resist--;// = properties.resist -1;

	// si la brique n'est pas detruite
	// On affiche que sa resistance a ete reduite
	if (properties.resist > 0) {
		char etat = ' ';
		for (int i = 0; i < B_CASES_H * B_CASES_L; i++) {
			// Affichage de l'etat de la brique
			if (i % 2 == 1)
				etat = 'x';
			else if (properties.resist == 1)
				etat = 'X';
			else
				etat = ' ';

			// Mise à jour de la brique
			tab[i]->setCaseBrique(properties.couleur, etat);
			tab[i]->paint();
		}
	}
	// Sinon, on detruit la brique
	else {
		for (int i = 0; i < B_CASES_H * B_CASES_L; i++) {
			tab[i]->setCaseVide();
			tab[i]->paint();
		}
	}
}
