#ifndef PARTIECB_H_
#define PARTIECB_H_

#include "GrilleCB.h"
#include <sextant/Activite/Threads.h>
#include <Applications/Horloge/Horloge.h>

extern Timer timer;

const int Y_TIMER = 1;
const int Y_SCORE = 4;
const int Y_LIFES = 7;
const int Y_LEVEL = 10;
const int Y_BRIQUE = 13;
const int Y_NAME = 21;

/**
 * @file PartieCB.h
 * @class PartieCB
 * @brief Classe gerant une partie de casse brique (score, horloge, vies, ...)
 */
class PartieCB: public Threads {
	// Elements communs
	GrilleCB* grille;
	Horloge* horloge;
	EcranV* ecran;

	// Elements specifiques a la partie
	bool enCours; // indique si le jeu est en cours
	int score; // Score de la partie
	int vie_restante; // Vies restante pour la partie

	/**
	 * Méthode privée permettant d'afficher le score
	 */
	void afficherScore();

	/**
	 * Méthode privée permettant d'afficher un texte
	 */
	void afficherText();

	/**
	 * Méthode privée permettant d'afficher les vies
	 */
	void afficherVies();

	/**
	 *Méthode privée permettant d'afficher le nombre de brique restante
	 */
	void afficherBriqueRestante();

public:

	// Constructeur
	PartieCB(GrilleCB* g, EcranV* e) {
		grille = g;
		ecran = e;
	}

	// Getters
	int getVie();

	/**
	 * Méthode run de l'application
	 */
	void run();

	/**
	 * Initialisation de la partie
	 */
	void init();

	/**
	 * Termine la partie
	 */
	void endGame();

	/**
	 * Fait perdre une vie au joueur
	 */
	void perdreVie();

	/**
	 * Vide une ligne du menu
	 */
	void cleanLigneMenu(int ligne);
};

#endif /* PARTIECB_H_ */
