#include "GrilleCB.h"


	Mutex mutex;

GrilleCB::GrilleCB() {
}

GrilleCB::GrilleCB(EcranV* e) {
	init(e);
}

void GrilleCB::init(EcranV* e) {
	// Initialisation du mutex
	mutex.V();

	scoreNiveau = 0;
	nbBloc = 0;

	//Initialisation de la GrilleCB
	for (int x = 0; x < HAUTEUR; x++) {
		for (int y = 0; y < LARGEUR; y++) {
			tab[x][y].init(x, y);
			tab[x][y].setEcran(e);
			// installation des bords
			if (x == 0 || x == (HAUTEUR - 1) || y == 0 || y == (LARGEUR - 1)
					|| y == (LARGEUR - MENULARGEUR))
				tab[x][y].setCaseBord();
			// installation du trou
			if (x == (HAUTEUR - 1) && y > 0 && y < (LARGEUR - MENULARGEUR))
				tab[x][y].setCaseTrou();
		}
	}

	// TODO : choix de la version
	// Version SANS Blocs
	generateLvl();
	// Version AVEC Blocs
	//generateLv1WithBlocs();
	//

	initRaquette((HAUTEUR - 2), 10); // (le chiffre de la taille de raquette doit être paire sinon un problème de round)
}
;

void GrilleCB::movGaucheRaquette(int gd) {
	if (raq.getGauche() > 1) {
		tab[raq.getX()][raq.getDroite()].setCaseVide();
		tab[raq.getX()][raq.getDroite()].paint();
		raq.movGauche();
		tab[raq.getX()][raq.getGauche()].setCaseBord();
		tab[raq.getX()][raq.getGauche()].paint();
	}
}

void GrilleCB::movDroiteRaquette(int gd) {
	if (raq.getDroite() < (LARGEUR - MENULARGEUR - 1)) {
		tab[raq.getX()][raq.getGauche()].setCaseVide();
		tab[raq.getX()][raq.getGauche()].paint();
		raq.movDroite();
		tab[raq.getX()][raq.getDroite()].setCaseBord();
		tab[raq.getX()][raq.getDroite()].paint();
	}
}

void GrilleCB::initRaquette(int ligne, int length) { // ligne, taille
	raq.init(length, ligne);

	for (int i = 0; i <= length; i++) {
		tab[raq.getX()][raq.getGauche() + i].setCaseBord();
	}
}

CaseCB* GrilleCB::getCase(int x, int y) {
	return &tab[x][y];
}

void GrilleCB::setCaseVide(int x, int y) {
	tab[x][y].setCaseVide();
}

RaquetteCB* GrilleCB::getRaquette() {
	return &raq;
}

int GrilleCB::getNbBlocNiveau() {
	return nbBloc;
}


int GrilleCB::getScore()
{
	return scoreNiveau;
}

void GrilleCB::affiche() {
	for (int x = 0; x < HAUTEUR; x++) {
		for (int y = 0; y < LARGEUR; y++) {
			tab[x][y].paint();
		}
	}
}

/**
 * Generation de niveau SANS blocs
 */
void GrilleCB::generateLvl() {
	for (int x = 0; x < HAUTEUR; x++) {
		for (int y = 0; y < LARGEUR; y++) {
			if (x > 0 && x < 3 && y > 0 && y < LARGEUR - MENULARGEUR) {
				tab[x][y].setCaseBrique(VERT_LEGER);
				nbBloc++;
			}
			if (x > 6 && x < 9 && y > 12 && y < LARGEUR - MENULARGEUR - 12) {
				tab[x][y].setCaseBrique(CYAN_LEGER);
				nbBloc++;
			}
			if (x > 12 && x < 15 && y > 24 && y < LARGEUR - MENULARGEUR - 24) {
				tab[x][y].setCaseBrique(MARRON);
				nbBloc++;
			}
		}
	}
}

/*
 * Generation de niveau AVEC blocs
 */
void GrilleCB::generateLv1WithBlocs()
{
	BlocProps p1;
	p1.couleur = VERT;
	p1.resist = 3;
	p1.points = 100;

	BlocProps p2;
	p2.couleur = CYAN;
	p2.resist = 3;
	p2.points = 200;

	for (int x = 0; x < H_ZONEJEU; x++) {
		for (int y = 0; y < L_ZONEJEU; y++) {
			if ((x%2 == 0 && y%2 == 0)||(x%2 != 0 && y%2 != 0))
			{
				zoneJeu[x][y].changeProps(p1);
				zoneJeu[x][y].assignBloc(getCasesBloc(x,y));
			}
			else
			{
				zoneJeu[x][y].changeProps(p2);
				zoneJeu[x][y].assignBloc(getCasesBloc(x,y));
			}
			nbBloc++;
		}
	}
}

/**
 * Renvois un tableau des cases composant un bloc à partir des coordonnées en haut à droite du bloc.
 */
CaseCB** GrilleCB::getCasesBloc(int x, int y) {
	CaseCB** tmpTab = 0;
	int it = 0;

	for (int i = x * B_CASES_H; i < (x * B_CASES_H) + B_CASES_H; i++)
		for (int j = y * B_CASES_L; j < (y * B_CASES_L) + B_CASES_L; j++) {
			tmpTab[it++] = getCase(i + 1, j + 1);
		}
	return tmpTab;
}

/**
 * Renvois le bloc à partir des coordonnées d'une case.
 */
BlocCB* GrilleCB::getBloc(int x, int y) {
	return &zoneJeu[(x - 1) / B_CASES_H][(y - 1) / B_CASES_L];
}

void GrilleCB::action(int x, int y)
{
	mutex.P();
	// TODO : choix de la version
	// Version SANS Blocs
	getCase(x,y)->action();
	// Version AVEC Blocs
	//getBloc(x,y)->action();
	//

	if (nbBloc > 0)
		nbBloc--;

	scoreNiveau += 100;

	mutex.V();
}
