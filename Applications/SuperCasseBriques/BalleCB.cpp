#include "BalleCB.h"

/**
 * @file BalleCB.cpp
 * Implémentation des méthodes de la classes Balle
 */

BalleCB::BalleCB(GrilleCB* laGrille, PartieCB* laPartie) {
	grille = laGrille;
	partie = laPartie;
	vitesse = VITESSE;
}
;

bool BalleCB::isInit() {
	if (y > 0 && x > 0)
		return true;
	return false;
}

void BalleCB::init() {
	RandomCB random;
	// Dépose la balle sur le millieu de la raquette
	y = grille->getRaquette()->getDroite()
			- (grille->getRaquette()->getTaille() / 2);
	x = HAUTEUR - 3;

	// Occuper la case
	grille->getCase(x, y)->occupe();
	// Choisir un deplacement aleatoire (h_d, h_g, b_d, b_g)
	dx = 1;
	if (random.nextBool())
		dx = -dx;
	dy = 1;
	if (random.nextBool())
		dy = -dy;
}

void BalleCB::bouger() {
	//Avancer de (dx, dy) :
	int nx = x + dx;
	int ny = y + dy;
	//Deplacement : informer les cases de la presence ou non de la balle :
	grille->getCase(nx, ny)->occupe();
	grille->getCase(x, y)->libere();
	// Nouvelle position :
	x = nx;
	y = ny;
}

void BalleCB::run() {
	// Fait avancer la balle
	while (avance()) {
	}
}

bool BalleCB::avance() {
	// Sauvegarde de l'origine de la balle
	int backDx = -dx, backDy = -dy;
	// Timer pour l'affichage de la balle
	int baseT = timer.getMsecondes();
	while (timer.getMsecondes() < baseT + 150)
		;

	/**
	 * Gestion des collisions
	 */
	//Gestion des collisions gauche/droite
	switch (grille->getCase(x + dx, y)->getEtat()) {
	case BRIQUE:
		actionCase(x + dx, y);
	case BORD:	// Gestion du rebond
		dx = -dx;
		break;
	case TROU:
		return actionTrou();
	}

	// Gestion des collisions haut/bas
	switch (grille->getCase(x, y + dy)->getEtat()) {
	case BRIQUE:
		actionCase(x, y + dy);
	case BORD:	// Gestion du rebond
		dy = -dy;
		break;
	case TROU:
		return actionTrou();
	}

	// Gestion des collisions diagonales
	switch (grille->getCase(x + dx, y + dy)->getEtat()) {
	case BRIQUE:
		actionCase(x + dx, y + dy);
	case BORD:	// Gestion du rebond
		dx = -dx;
		dy = -dy;
		break;
	}

	// Si le mouvement calcule de la balle l'envoie dans une brique ou un mur
	// on l'envoie a la place en arriere
	if (grille->getCase(x + dx, y + dy)->getEtat() == BRIQUE
			|| grille->getCase(x + dx, y + dy)->getEtat() == BORD) {
		dx = backDx;
		dy = backDy;
	}

	grille->getCase(x, y)->libere();
	grille->getCase(x, y)->paint();
	x = x + dx;
	y = y + dy;
	grille->getCase(x, y)->occupe();
	grille->getCase(x, y)->paint();
	return true;
}

void BalleCB::actionCase(int x, int y) {
	grille->action(x, y);
}

bool BalleCB::actionTrou() {
	// Fait disparaître la balle lorsqu'elle tombe dans le trou
	bouger();
	dx = 0;
	dy = 0;
	grille->getCase(x, y)->libere();
	grille->getCase(x, y)->paint();
	return false;
}
