#include "PartieCB.h"

void PartieCB::run() {
	// Initialisation de l'horloge
	horloge = new Horloge(ecran, &timer, LARGEUR - (MENULARGEUR / 2) - 3, 2,
			true);

	// Mise à jour du menu tant que le jeu est en cours
	while (enCours) {
		horloge->afficheHeure();
		afficherScore();
		afficherBriqueRestante();
	}
}

void PartieCB::init() {
	// Initialisation de variables et du menu
	enCours = true;
	score = 0;
	vie_restante = 3;
	afficherText();
	afficherScore();
	afficherVies();
}

void PartieCB::afficherText() {
	// Mise en place du menu
	ecran->afficherMot(Y_TIMER, LARGEUR - (MENULARGEUR / 2) - 5, "*  Timer *",
			BLANC);
	ecran->afficherMot(Y_SCORE, LARGEUR - (MENULARGEUR / 2) - 5, "*  Score *",
			BLANC);
	ecran->afficherMot(Y_LIFES, LARGEUR - (MENULARGEUR / 2) - 5, "*  Lifes *",
			BLANC);
	ecran->afficherMot(Y_LEVEL, LARGEUR - (MENULARGEUR / 2) - 5, "*  Level *",
			BLANC);
	ecran->afficherMot(Y_BRIQUE, LARGEUR - (MENULARGEUR / 2) - 5, "* Brique *",
			BLANC);
	ecran->afficherMot(Y_NAME, LARGEUR - (MENULARGEUR / 2) - 5, "=  BRICK =",
			BLANC);
	ecran->afficherMot(Y_NAME + 1, LARGEUR - (MENULARGEUR / 2) - 5,
			"  BREAKER ", BLANC);
	ecran->afficherMot(Y_NAME + 2, LARGEUR - (MENULARGEUR / 2) - 5,
			"=   1.0  =", BLANC);

}
void PartieCB::afficherScore() {
	ecran -> afficherChiffre(Y_SCORE + 1, LARGEUR - MENULARGEUR + 1,
			grille->getScore());
}

void PartieCB::afficherVies() {
	for (int i = 0; i < vie_restante; i++) {
		ecran -> afficherCaractere(Y_LIFES + 1, LARGEUR - MENULARGEUR + 1 + i,
				BLANC, VERT, 2);
	}
}

void PartieCB::cleanLigneMenu(int ligne) {
	for (int i = 0; i < (MENULARGEUR - 2); i++) {
		ecran -> afficherCaractere(ligne, LARGEUR - MENULARGEUR + 1 + i, BLANC,
				GRIS_SOMBRE, ' ');
	}
}

int PartieCB::getVie() {
	return vie_restante;
}

void PartieCB::afficherBriqueRestante() {
	// Affiche le nombre de briques restantes dans le niveau
	int nb = grille->getNbBlocNiveau();
	if (nb < 100)
		ecran->afficherCaractere(Y_BRIQUE + 1, LARGEUR - MENULARGEUR + 3,
				BLANC, GRIS_SOMBRE, ' ');
	if (nb < 10)
		ecran->afficherCaractere(Y_BRIQUE + 1, LARGEUR - MENULARGEUR + 2,
				BLANC, GRIS_SOMBRE, ' ');

	ecran->afficherChiffre(Y_BRIQUE + 1, LARGEUR - MENULARGEUR + 1, nb);
}

void PartieCB::perdreVie() {
	--vie_restante;
	cleanLigneMenu(Y_LIFES + 1);
	afficherVies();
}

void PartieCB::endGame() {
	enCours = false;
}
