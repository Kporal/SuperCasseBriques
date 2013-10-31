#include "CaseCB.h"

/**
 * @file CaseCB.cpp
 * Implémentation des méthodes de la classes Case.
 */

void CaseCB::init(int xx, int yy) {
	x = xx;
	y = yy;
	setCaseVide();
}
;

int CaseCB::getEtat() {
	return etat;
}
;

int CaseCB::getEtatOld() {
	return etatold;
}
;

void CaseCB::setEcran(EcranV* e) {
	ecran = e;
}

EcranV* CaseCB::getEcran() {
	return ecran;
}

void CaseCB::setCaseVide() {
	etat = VIDE;
}

void CaseCB::setCaseTrou() {
	etat = TROU;
}

void CaseCB::setCaseBord() {
	etat = BORD;
}

void CaseCB::setCaseBrique(Couleur c, char etatB) {
	etat = BRIQUE;
	couleurBrique = c;
	caractereBrique = etatB;
}

void CaseCB::paint() {
	ecran->afficherCaractere(x, y, couleurPr(), couleurAr(), Caractere());
}

Couleur CaseCB::couleurPr() {
	// Définit la couleur du premier plan à afficher selon l'état de la case
	switch (etat) {
	case VIDE:
		return NOIR;
	case BORD:
		return JAUNE;
	case TROU:
		return NOIR;
	case BALLE:
		return BLANC;
	case BRIQUE:
		return ROUGE;
	default:
		return BLEU_LEGER;
	}
}

Couleur CaseCB::couleurAr() {
	// Définit la couleur du second plan à afficher selon l'état de la case
	switch (etat) {
	case BRIQUE:
		return couleurBrique;
	}
	return GRIS_SOMBRE;
}

char CaseCB::Caractere() {
	// Définit le caractère à afficher selon l'état de la case
	switch (etat) {
	case VIDE:
		return ' ';
	case BORD:
	case TROU:
		return 219;
	case BALLE:
		return '0';
	case BRIQUE:
		return caractereBrique;
	default:
		return ' ';
	}
}

void CaseCB::occupe() {
	etatold = etat;
	etat = BALLE;
	paint();
}

void CaseCB::libere() {
	etat = etatold;
	paint();
}

void CaseCB::action() {
	switch (etat) {
	case BRIQUE:
		etat = VIDE;
		break;
	}
	paint();
}
