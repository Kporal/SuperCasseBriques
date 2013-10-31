#include "Horloge.h"

/**
 * Variable déclarée dans le fichier tic.cpp servant à compter les tics de l'horloge.
 */

Horloge::Horloge(EcranV* ecr,Timer *timer, int X, int Y, bool startZero) {
	e=ecr;
	t=timer;
	x = X;
	y = Y;
	if(startZero)
	{
		timeZero = timer->getSecondes();
	}
	else
	{
		timeZero = 0;
	}
}

void Horloge::afficheHeure() {
	int temps_secondes=0;
	int minutes=0;
	int temp=0;

	temp=t->getSecondes()-timeZero;

	temps_secondes  = temp % 60;
	if (temps_secondes<10) {e->afficherChiffre(y,x+4,0);e->afficherChiffre(y,x+5,temps_secondes);}
	else e->afficherChiffre(y,x+4,temps_secondes);

	minutes = temp / 60  ;
	e->afficherMot(y,x+2,"::",BLANC);

	if (minutes<10) {e->afficherChiffre(y,x,0);e->afficherChiffre(y,x+1,minutes);}
	else e->afficherChiffre(y,x,minutes);
}
