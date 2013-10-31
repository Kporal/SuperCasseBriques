#include "SuperPong.h"

SuperPong::SuperPong(EcranV* e, ClavierV* c){
	ecran=e;
	clavier=c;
}

void SuperPong::init() {
	laGrille.init(ecran);
	laGrille.affiche();
}

void SuperPong::init(EcranV* e,ClavierV* c) {
	ecran=e;
	clavier=c;
	laGrille.init(e);
	laGrille.affiche();
}


void SuperPong::start() {
	bool jouer=true;
	Balle *balle = new Balle(&laGrille);

	//Modification de la routine d'interruption du keyboard
//	irq_set_routine(IRQ_KEYBOARD, keyboard_handler_jeu);

	//Initialisation de la grille
	init();
	balle->start("Balle pong");

	while (jouer){
		char c=clavier->getChar();
				switch(c){
				case 's' :
					laGrille.monterRaquette(GAUCHE);
					break;
				case 'x' :
					laGrille.descendreRaquette(GAUCHE);
					break;
				case 'j' :
					laGrille.monterRaquette(DROITE);
					break;
				case 'n' :
					laGrille.descendreRaquette(DROITE);
					break;
				case 'q' :
					jouer=false;
				}
	}
}
