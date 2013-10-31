#include "RaquetteCB.h"

RaquetteCB::RaquetteCB() {
}

RaquetteCB::RaquetteCB(int taille, int ligne) {
	init(taille, ligne);
}

void RaquetteCB::init(int taille, int ligne) {
	// Initialiastion de la raquette selon sa ligne et sa taille
	gauche = ((LARGEUR - MENULARGEUR) / 2) - taille / 2;
	droite = ((LARGEUR - MENULARGEUR) / 2) + taille / 2;
	x = ligne;
}

int RaquetteCB::getX() {
	return x;
}
int RaquetteCB::getGauche() {
	return gauche;
}
int RaquetteCB::getDroite() {
	return droite;
}
int RaquetteCB::getTaille() {
	return droite - gauche;
}

void RaquetteCB::movGauche() {
	droite--;
	gauche--;
}

void RaquetteCB::movDroite() {
	droite++;
	gauche++;
}

void RaquetteCB::setGauche(int i) {
	gauche = i;
}

void RaquetteCB::setDroite(int i) {
	gauche = i;
}

void RaquetteCB::setX(int i) {
	x = i;
}
