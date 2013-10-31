#include "CasseBriques.h"

CasseBriques::CasseBriques(EcranV* e, ClavierV* c) {
	ecran = e;
	clavier = c;
}

void CasseBriques::init() {
	laGrille.init(ecran);
	laGrille.affiche();
}

void CasseBriques::start() {
	perdu = false;
	gagne = false;

	// Déclaration de la partie et de la balle
	PartieCB* partie = new PartieCB(&laGrille, ecran);
	BalleCB* balle = new BalleCB(&laGrille, partie);

	do {
		jouer = true;
		rejouer = false;

		// Affiche un message en cas de victoire ou de défaite
		if (perdu) {
			partie->endGame();
			ecran ->afficherMot(Y_NAME, ((LARGEUR - MENULARGEUR) / 2) - 3 ,
					"YOU LOSE !!!", BLANC);
		} else if (gagne){
			partie->endGame();
			ecran ->afficherMot(Y_NAME, ((LARGEUR - MENULARGEUR) / 2) - 3,
					"YOU WIN !!!", BLANC);
		} else { // Sinon, initialise la grille et la partie
			init();
			partie->init();
		}

		// Lancement de la partie
		partie->start("partie CB");

		 // Ecoute des commandes tant que le jeu est en cours
		while (jouer) {
			// Si le joueur a terminé le niveau
			if (laGrille.getNbBlocNiveau() == 0) {
				jouer = false;
				rejouer = true;
				gagne = true;
			}
			char c = clavier->getChar();

			switch (c) {
			case 'w': // Lancement de la balle
				if (!perdu && !gagne) {
					if (!balle->isInit()) {
						balle->init();
						balle->start("Balle CassBriques");
					}
					// Action lorsque la balle tombe dans le trou
					if (!balle->avance()) {		// Si il reste des vies
						if (partie->getVie() > 0) {
							partie->perdreVie();
							balle->init();
							balle->start("Balle CassBriques");
						} else { // Perdu definitivement
							jouer = false;
							rejouer = true;
							perdu = true;
						}
					}
				}
				break;
			case 'a': // Déplacement vers la gauche
				if (!perdu && !gagne)
					laGrille.movGaucheRaquette(RAQ);
				break;
			case 'd': // Déplacement vers la droite
				if (!perdu && !gagne)
					laGrille.movDroiteRaquette(RAQ);
				break;
			case 'r': // Rejouer
				if (!balle->avance()) {
					rejouer = true;
					jouer = false;
					perdu = false;
					gagne = false;
					partie->endGame();
				}
				break;
			case 'p': // Arreter le jeu
				jouer = false;
				break;
			}
		}
	} while (rejouer);
}
