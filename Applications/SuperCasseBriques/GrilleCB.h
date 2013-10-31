#ifndef GRILLECB_H
#define GRILLECB_H

#include "CaseCB.h"
#include "RaquetteCB.h"
#include "ConfigCB.h"
#include "BlocCB.h"
#include <sextant/Synchronisation/Mutex/Mutex.h>

/**
 * @file GrilleCB.h
 * @class GrilleCB
 * @brief La grille du jeu avec des murs, une raquettes et des balles.
 */
class GrilleCB {

	/*
	 * Tableau des cases du jeu.
	 */
	CaseCB tab[HAUTEUR][LARGEUR];

	/**
	 * Tableau des blocs (non implémentés)
	 */
	BlocCB zoneJeu[H_ZONEJEU][L_ZONEJEU];

	// Récupère les cases d'un bloc
	CaseCB** getCasesBloc(int x, int y);

	/**
	 * Nombre de blocs dans le niveau
	 */
	int nbBloc;
	/*
	 * Score du niveau
	 */
	int scoreNiveau;

public:

	/**
	 * Raquette créée lors de l'initialisation du jeu.
	 */
	RaquetteCB raq;

	/**
	 * Constructeur :
	 * Construit la grille avec les murs, les zones privées, le nombre de verrous associés, les raquettes et les trous
	 * @param Ecran
	 */
	GrilleCB();
	GrilleCB(EcranV *);

	// Getters
	CaseCB* getCase(int x, int y);
	RaquetteCB* getRaquette();
	BlocCB* getBloc(int x, int y);

	/*
	 * Renvois le nombre de blocs restant du niveau
	 */
	int getNbBlocNiveau();

	/*
	 * Renvois le score du niveau
	 */
	int getScore();

	// Setters
	void setCaseVide(int x, int y);

	/**
	 * Initialise la grille
	 */
	void init(EcranV *);

	/**
	 * Affiche la grille
	 */
	void affiche();

	/**
	 * Initialise la raquette
	 * @param y : emplacement y de la raquette
	 * @param lg : taille de la raquette
	 */
	void initRaquette(int ligne, int taille);

	/**
	 * Déplace la raquette vers la gauche
	 */
	void movGaucheRaquette(int gd);

	/**
	 * Déplace la raquette vers la droite
	 */
	void movDroiteRaquette(int gd);

	/**
	 * Construction des niveaux
	 */
	void generateLvl();

	/**
	 * Construction des niveaux avec Blocs
	 */
	void generateLv1WithBlocs();

	/**
	 * Effectue une action sur une case de la grille
	 */
	void action(int, int);

};

#endif
