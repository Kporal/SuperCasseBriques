#include "Map.h"
#include "../../sextant/memoire/op_memoire.h"
#include <Applications/Warcraft/Joueur.h>

Map::Map(EcranV *ecr, Joueur *joueurHumain, Joueur *joueurIA, ClavierV *clavier) :
		ecr(ecr), joueurHumain(joueurHumain), joueurIA(joueurIA), clavier(
				clavier) {
	initialiserSalementLaMap();
	initialiserLesJoueurs();
	for (int i = 1; i < 25; i++) {
		for (int j = 0; j < 70; j++) {
			Couleur prem;
			Couleur ar;
			switch (tab[i][j]) {
			case 176:
				prem = NOIR;
				ar = JAUNE;
				break;
			case 177:
				prem = BLEU;
				ar = BLEU_LEGER;
				break;
			default:
				prem = VERT;
				ar = VERT_LEGER;
				break;
			}
			ecr->afficherCaractere(i, j, prem, ar, (char) tab[i][j]);
		}
	}
}

void Map::run() {
	while (true) {
		action = clavier->getChar();
	}
}

bool Map::recupererOuAttaquer() {
	if (joueurIA->nombreGuerriers() > 4) {
		joueurIA->attaquer();
	}

	for (int i = 0; i < joueurHumain->population; i++) {
		if (joueurHumain->unites[i]->paysan) {
			if (joueurHumain->unites[i]->objetPorte != -1) {
				if (valeurAbsolue(
						joueurHumain->batiments[0]->position->x
								- joueurHumain->unites[i]->position->x) < 2
						&& valeurAbsolue(
								joueurHumain->batiments[0]->position->y
										- joueurHumain->unites[i]->position->y)
								< 2) {
					if (joueurHumain->unites[i]->objetPorte == Map::OR)
						joueurHumain->gold++;
					else
						joueurHumain->bois++;
					joueurHumain->unites[i]->objetPorte = -1;
					joueurHumain->unites[i]->objectif = scanner(
							joueurHumain->batiments[0]->position, 80,
							joueurHumain->unites[i]->objetRecherche);
				}
			} else if (joueurHumain->unites[i]->position->egal(
					joueurHumain->unites[i]->objectif)) {
				joueurHumain->unites[i]->objectif = scanner(
						joueurHumain->batiments[0]->position, 80,
						joueurHumain->unites[i]->objetRecherche);
			} else {
				Position *ressource = scanner(joueurHumain->unites[i]->position,
						1, joueurHumain->unites[i]->objetRecherche);
				if (ressource != 0) {
					if (joueurHumain->unites[i]->objetRecherche == Map::BOIS) {
						tab[ressource->x][ressource->y] = 32;
						effacerPosition(ressource);
					}
					joueurHumain->unites[i]->objetPorte =
							joueurHumain->unites[i]->objetRecherche;
					joueurHumain->unites[i]->objectif =
							joueurHumain->batiments[0]->position;
				}
			}
		}
		Position *positionDeLEnnemi = scanner(joueurHumain->unites[i]->position,
				1, Map::UNITE_IA);
		if (positionDeLEnnemi != 0) {
			if (joueurIA->unitePerdDesPV(positionDeLEnnemi,
					(joueurHumain->unites[i]->paysan) ? 1 : 2))
				effacerPosition(positionDeLEnnemi);
		} else {
			positionDeLEnnemi = scanner(joueurHumain->unites[i]->position, 1,
					Map::BATIMENT_IA);
			if (positionDeLEnnemi != 0) {
				if (joueurIA->batimentPerdDesPV(
						(joueurHumain->unites[i]->paysan) ? 1 : 2))
					return true;
			}
		}
	}

	for (int i = 0; i < joueurIA->population; i++) {
		if (joueurIA->unites[i]->paysan) {
			if (joueurIA->unites[i]->objetPorte != -1) {
				if (valeurAbsolue(
						joueurIA->batiments[0]->position->x
								- joueurIA->unites[i]->position->x) < 2
						&& valeurAbsolue(
								joueurIA->batiments[0]->position->y
										- joueurIA->unites[i]->position->y)
								< 2) {
					if (joueurIA->unites[i]->objetPorte == Map::OR)
						joueurIA->gold++;
					else
						joueurIA->bois++;
					joueurIA->unites[i]->objetPorte = -1;
					joueurIA->unites[i]->objectif = scanner(
							joueurIA->batiments[0]->position, 80,
							joueurIA->unites[i]->objetRecherche);
				}
			} else if (joueurIA->unites[i]->position->egal(
					joueurIA->unites[i]->objectif)) {
				joueurIA->unites[i]->objectif = scanner(
						joueurIA->batiments[0]->position, 80,
						joueurIA->unites[i]->objetRecherche);
			} else {
				Position *ressource = scanner(joueurIA->unites[i]->position, 1,
						joueurIA->unites[i]->objetRecherche);
				if (ressource != 0) {
					if (joueurIA->unites[i]->objetRecherche == Map::BOIS) {
						tab[ressource->x][ressource->y] = 32;
						effacerPosition(ressource);
					}
					joueurIA->unites[i]->objetPorte =
							joueurIA->unites[i]->objetRecherche;
					joueurIA->unites[i]->objectif =
							joueurIA->batiments[0]->position;
				}
			}
		}
		Position *positionDeLEnnemi = scanner(joueurIA->unites[i]->position, 1,
				Map::UNITE_HUMAIN);
		if (positionDeLEnnemi != 0) {
			if (joueurHumain->unitePerdDesPV(positionDeLEnnemi,
					(joueurIA->unites[i]->paysan) ? 1 : 2))
				effacerPosition(positionDeLEnnemi);
		} else {
			positionDeLEnnemi = scanner(joueurIA->unites[i]->position, 1,
					Map::BATIMENT_HUMAIN);
			if (positionDeLEnnemi != 0) {
				if (joueurHumain->batimentPerdDesPV(
						(joueurIA->unites[i]->paysan) ? 1 : 2))
					return true;
			}
		}
	}
	return false;
}

void Map::creerUnites() {
	if (2 * (joueurIA->nombreGuerriers())
			< (joueurIA->nombrePaysanBois() + joueurIA->nombrePaysanOr())) {
		if (joueurIA->gold > 2 && joueurIA->bois > 5) {
			joueurIA->gold -= 3;
			joueurIA->bois -= 6;
			joueurIA->creerGuerrier(
					new Position(joueurIA->batiments[0]->position));
		}
	} else if (2 * joueurIA->nombrePaysanOr() < joueurIA->nombrePaysanBois()) {
		if (joueurIA->gold > 1 && joueurIA->bois > 3) {
			joueurIA->gold -= 2;
			joueurIA->bois -= 4;
			joueurIA->creerPaysan(Map::OR,
					new Position(joueurIA->batiments[0]->position),
					scanner(joueurIA->batiments[0]->position, 80, Map::OR));
		}
	} else {
		if (joueurIA->gold > 1 && joueurIA->bois > 3) {
			joueurIA->gold -= 2;
			joueurIA->bois -= 4;
			joueurIA->creerPaysan(Map::BOIS,
					new Position(joueurIA->batiments[0]->position),
					scanner(joueurIA->batiments[0]->position, 80, Map::BOIS));
		}
	}
}

void Map::deplacer() {
	//afficherMessage("position du peon1 : ");
	ecr->afficherChiffre(25, 0, joueurIA->unites[3]->position->x);
	ecr->afficherChiffre(28, 0, joueurIA->unites[3]->position->y);
	for (int i = 0; i < joueurIA->population; i++) {
		if (joueurIA->unites[i]->objectif->x
				> joueurIA->unites[i]->position->x) {
			effacerPosition(joueurIA->unites[i]->position);
			joueurIA->unites[i]->position->x++;
		} else if (joueurIA->unites[i]->objectif->x
				< joueurIA->unites[i]->position->x) {
			effacerPosition(joueurIA->unites[i]->position);
			joueurIA->unites[i]->position->x--;
		}
		if (joueurIA->unites[i]->objectif->y
				> joueurIA->unites[i]->position->y) {
			effacerPosition(joueurIA->unites[i]->position);
			joueurIA->unites[i]->position->y++;
		} else if (joueurIA->unites[i]->objectif->y
				< joueurIA->unites[i]->position->y) {
			effacerPosition(joueurIA->unites[i]->position);
			joueurIA->unites[i]->position->y--;
		}
	}
	for (int i = 0; i < joueurHumain->population; i++) {
		if (joueurHumain->unites[i]->objectif->x
				> joueurHumain->unites[i]->position->x) {
			effacerPosition(joueurHumain->unites[i]->position);
			joueurHumain->unites[i]->position->x++;
		} else if (joueurHumain->unites[i]->objectif->x
				< joueurHumain->unites[i]->position->x) {
			effacerPosition(joueurHumain->unites[i]->position);
			joueurHumain->unites[i]->position->x--;
		}
		if (joueurHumain->unites[i]->objectif->y
				> joueurHumain->unites[i]->position->y) {
			effacerPosition(joueurHumain->unites[i]->position);
			joueurHumain->unites[i]->position->y++;
		} else if (joueurHumain->unites[i]->objectif->y
				< joueurHumain->unites[i]->position->y) {
			effacerPosition(joueurHumain->unites[i]->position);
			joueurHumain->unites[i]->position->y--;
		}
	}
}

void Map::afficher() {
	for (int i = 0; i < joueurHumain->nbBatiment; i++) {
		ecr->afficherCaractere(joueurHumain->batiments[i]->position->x,
				joueurHumain->batiments[i]->position->y, CYAN, VERT_LEGER,
				(char) 127);
	}
	for (int i = 0; i < joueurHumain->population; i++) {
		ecr->afficherCaractere(joueurHumain->unites[i]->position->x,
				joueurHumain->unites[i]->position->y, CYAN, VERT_LEGER,
				(char) (joueurHumain->unites[i]->paysan) ? 1 : 2);
	}
	for (int i = 0; i < joueurIA->nbBatiment; i++) {
		ecr->afficherCaractere(joueurIA->batiments[i]->position->x,
				joueurIA->batiments[i]->position->y, ROUGE, VERT_LEGER,
				(char) 127);
	}
	for (int i = 0; i < joueurIA->population; i++) {
		ecr->afficherCaractere(joueurIA->unites[i]->position->x,
				joueurIA->unites[i]->position->y, ROUGE, VERT_LEGER,
				(char) (joueurIA->unites[i]->paysan) ? 1 : 2);
	}
}

void Map::afficherMessage(const char *aAfficher) {
	ecr->afficherMot(0, 0,
			"                                                                      ");
	ecr->afficherMot(0, 0, aAfficher);
}

Position *Map::scanner(Position *p, int nombreDeCaseMaxAutour,
		int typeRecherche) {
	if (typeRecherche == Map::UNITE_HUMAIN) {
		for (int i = 0; i < joueurHumain->population; i++)
			if (valeurAbsolue(joueurHumain->unites[i]->position->x - p->x)
					<= nombreDeCaseMaxAutour
					&& valeurAbsolue(
							joueurHumain->unites[i]->position->y - p->y)
							<= nombreDeCaseMaxAutour)
				return joueurHumain->unites[i]->position;
	} else if (typeRecherche == Map::BATIMENT_HUMAIN) {
		for (int i = 0; i < joueurHumain->nbBatiment; i++)
			if (valeurAbsolue(joueurHumain->batiments[i]->position->x - p->x)
					<= nombreDeCaseMaxAutour
					&& valeurAbsolue(
							joueurHumain->batiments[i]->position->y - p->y)
							<= nombreDeCaseMaxAutour)
				return joueurHumain->batiments[i]->position;
	} else if (typeRecherche == Map::UNITE_IA) {
		for (int i = 0; i < joueurIA->population; i++)
			if (valeurAbsolue(joueurIA->unites[i]->position->x - p->x)
					<= nombreDeCaseMaxAutour
					&& valeurAbsolue(joueurIA->unites[i]->position->y - p->y)
							<= nombreDeCaseMaxAutour)
				return joueurIA->unites[i]->position;
	} else if (typeRecherche == Map::BATIMENT_IA) {
		for (int i = 0; i < joueurIA->nbBatiment; i++)
			if (valeurAbsolue(joueurIA->batiments[i]->position->x - p->x)
					<= nombreDeCaseMaxAutour
					&& valeurAbsolue(joueurIA->batiments[i]->position->y - p->y)
							<= nombreDeCaseMaxAutour)
				return joueurIA->batiments[i]->position;
	} else if (typeRecherche == Map::BOIS) {
		for (int i = 1; i < nombreDeCaseMaxAutour + 1; i++) {
			int dx = i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 6)
					return new Position(p->x + dx, p->y + dy);
			dx = -i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 6)
					return new Position(p->x + dx, p->y + dy);
			int dy = i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 6)
					return new Position(p->x + dx, p->y + dy);
			dy = -i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 6)
					return new Position(p->x + dx, p->y + dy);
		}
	} else if (typeRecherche == Map::OR) {
		for (int i = 1; i < nombreDeCaseMaxAutour + 1; i++) {
			int dx = -i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 176)
					return new Position(p->x + dx, p->y + dy);
			dx = i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 176)
					return new Position(p->x + dx, p->y + dy);
			int dy = -i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 176)
					return new Position(p->x + dx, p->y + dy);
			dy = i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 176)
					return new Position(p->x + dx, p->y + dy);
		}
	} else if (typeRecherche == Map::CASEVIDE) {
		for (int i = 1; i < nombreDeCaseMaxAutour + 1; i++) {
			int dx = -i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 32)
					return new Position(p->x + dx, p->y + dy);
			dx = i;
			for (int dy = -i; dy <= i; dy++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 32)
					return new Position(p->x + dx, p->y + dy);
			int dy = -i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 32)
					return new Position(p->x + dx, p->y + dy);
			dy = i;
			for (dx = -i; dx <= i; dx++)
				if (estDansLeTableau(p->x + dx, p->y + dy)
						&& tab[p->x + dx][p->y + dy] == 32)
					return new Position(p->x + dx, p->y + dy);
		}
	}
	return 0;
}

bool Map::estDansLeTableau(int x, int y) {
	return (x > 0 && x < 25 && y > -1 && y < 70);
}

int Map::valeurAbsolue(int i) {
	if (i < 0)
		return -i;
	return i;

}

void Map::effacerPosition(Position *p) {
	Couleur prem;
	Couleur ar;
	switch (tab[p->x][p->y]) {
	case 176:
		prem = NOIR;
		ar = JAUNE;
		break;
	case 177:
		prem = BLEU;
		ar = BLEU_LEGER;
		break;
	default:
		prem = VERT;
		ar = VERT_LEGER;
		break;
	}
	ecr->afficherCaractere(p->x, p->y, prem, ar, (char) tab[p->x][p->y]);
}

void Map::initialiserLesJoueurs() {
	Joueur *j = joueurHumain;
	for (int i = 0; i < 2; i++) {
		j->creerPaysan(Map::BOIS, new Position(j->batiments[0]->position),
				scanner(j->batiments[0]->position, 80, Map::BOIS));
		j->creerPaysan(Map::BOIS, new Position(j->batiments[0]->position),
				scanner(j->batiments[0]->position, 80, Map::BOIS));
		j->creerPaysan(Map::OR, new Position(j->batiments[0]->position),
				scanner(j->batiments[0]->position, 80, Map::OR));
		j->creerGuerrier(new Position(j->batiments[0]->position));
		j = joueurIA;
	}
}

void Map::creation() {
	switch (action) {
	case 'g':
		if (joueurHumain->gold > 2 && joueurHumain->bois > 5) {
			joueurHumain->gold -= 3;
			joueurHumain->bois -= 6;
			joueurHumain->creerGuerrier(
					new Position(joueurHumain->batiments[0]->position));
		}
		break;
	case 'b':
		if (joueurHumain->gold > 1 && joueurHumain->bois > 3) {
			joueurHumain->gold -= 2;
			joueurHumain->bois -= 4;
			joueurHumain->creerPaysan(Map::BOIS,
					new Position(joueurHumain->batiments[0]->position),
					scanner(joueurHumain->batiments[0]->position, 80,
							Map::BOIS));
		}
		break;
	case 'o':
		if (joueurHumain->gold > 1 && joueurHumain->bois > 3) {
			joueurHumain->gold -= 2;
			joueurHumain->bois -= 4;
			joueurHumain->creerPaysan(Map::OR,
					new Position(joueurHumain->batiments[0]->position),
					scanner(joueurHumain->batiments[0]->position, 80, Map::OR));
		}
		break;
	case 'a':
		joueurHumain->attaquerHumain();
		break;
	}
	action = ' ';
}
void Map::initialiserSalementLaMap() {

	//remplissage de la 1ere ligne
	tab[1][0] = 6;
	tab[1][1] = 6;
	tab[1][2] = 6;
	tab[1][3] = 6;
	tab[1][4] = 6;
	tab[1][5] = 6;
	tab[1][6] = 6;
	tab[1][7] = 6;
	tab[1][8] = 6;
	tab[1][9] = 6;
	tab[1][10] = 6;
	tab[1][11] = 6;
	tab[1][12] = 6;
	tab[1][13] = 6;
	tab[1][14] = 6;
	tab[1][15] = 6;
	tab[1][16] = 6;
	tab[1][17] = 6;
	tab[1][18] = 6;
	tab[1][19] = 6;
	tab[1][20] = 6;
	tab[1][21] = 6;
	tab[1][22] = 6;
	tab[1][23] = 6;
	tab[1][24] = 6;
	tab[1][25] = 6;
	tab[1][26] = 6;
	tab[1][27] = 6;
	tab[1][28] = 32;
	tab[1][29] = 177;
	tab[1][30] = 177;
	tab[1][31] = 32;
	tab[1][32] = 6;
	tab[1][33] = 6;
	tab[1][34] = 6;
	tab[1][35] = 6;
	tab[1][36] = 6;
	tab[1][37] = 6;
	tab[1][38] = 6;
	tab[1][39] = 6;
	tab[1][40] = 6;
	tab[1][41] = 6;
	tab[1][42] = 6;
	tab[1][43] = 6;
	tab[1][44] = 6;
	tab[1][45] = 6;
	tab[1][46] = 6;
	tab[1][47] = 6;
	tab[1][48] = 6;
	tab[1][49] = 6;
	tab[1][50] = 6;
	tab[1][51] = 6;
	tab[1][52] = 6;
	tab[1][53] = 6;
	tab[1][54] = 6;
	tab[1][55] = 6;
	tab[1][56] = 6;
	tab[1][57] = 6;
	tab[1][58] = 6;
	tab[1][59] = 6;
	tab[1][60] = 6;
	tab[1][61] = 6;
	tab[1][62] = 6;
	tab[1][63] = 6;
	tab[1][64] = 6;
	tab[1][65] = 6;
	tab[1][66] = 6;
	tab[1][67] = 6;
	tab[1][68] = 6;
	tab[1][69] = 6;
	tab[1][70] = 32;
	tab[1][71] = 32;
	tab[1][72] = 32;
	tab[1][73] = 32;
	tab[1][74] = 32;
	tab[1][75] = 32;
	tab[1][76] = 32;
	tab[1][77] = 32;
	tab[1][78] = 32;
	tab[1][79] = 32;

	//remplissage de la 2eme ligne
	tab[2][0] = 6;
	tab[2][1] = 6;
	tab[2][2] = 6;
	tab[2][3] = 6;
	tab[2][4] = 6;
	tab[2][5] = 176;
	tab[2][6] = 176;
	tab[2][7] = 32;
	tab[2][8] = 32;
	tab[2][9] = 32;
	tab[2][10] = 32;
	tab[2][11] = 32;
	tab[2][12] = 32;
	tab[2][13] = 32;
	tab[2][14] = 32;
	tab[2][15] = 32;
	tab[2][16] = 32;
	tab[2][17] = 32;
	tab[2][18] = 32;
	tab[2][19] = 32;
	tab[2][20] = 32;
	tab[2][21] = 32;
	tab[2][22] = 32;
	tab[2][23] = 32;
	tab[2][24] = 32;
	tab[2][25] = 32;
	tab[2][26] = 32;
	tab[2][27] = 32;
	tab[2][28] = 32;
	tab[2][29] = 177;
	tab[2][30] = 177;
	tab[2][31] = 32;
	tab[2][32] = 32;
	tab[2][33] = 32;
	tab[2][34] = 32;
	tab[2][35] = 32;
	tab[2][36] = 32;
	tab[2][37] = 32;
	tab[2][38] = 32;
	tab[2][39] = 32;
	tab[2][40] = 32;
	tab[2][41] = 32;
	tab[2][42] = 32;
	tab[2][43] = 32;
	tab[2][44] = 32;
	tab[2][45] = 32;
	tab[2][46] = 32;
	tab[2][47] = 32;
	tab[2][48] = 32;
	tab[2][49] = 32;
	tab[2][50] = 32;
	tab[2][51] = 32;
	tab[2][52] = 32;
	tab[2][53] = 32;
	tab[2][54] = 32;
	tab[2][55] = 32;
	tab[2][56] = 32;
	tab[2][57] = 32;
	tab[2][58] = 32;
	tab[2][59] = 32;
	tab[2][60] = 32;
	tab[2][61] = 32;
	tab[2][62] = 32;
	tab[2][63] = 176;
	tab[2][64] = 176;
	tab[2][65] = 6;
	tab[2][66] = 6;
	tab[2][67] = 6;
	tab[2][68] = 6;
	tab[2][69] = 6;
	tab[2][70] = 32;
	tab[2][71] = 32;
	tab[2][72] = 32;
	tab[2][73] = 32;
	tab[2][74] = 32;
	tab[2][75] = 32;
	tab[2][76] = 32;
	tab[2][77] = 32;
	tab[2][78] = 32;
	tab[2][79] = 32;

	//remplissage 3e ligne
	tab[3][0] = 6;
	tab[3][1] = 6;
	tab[3][2] = 6;
	tab[3][3] = 6;
	tab[3][4] = 6;
	tab[3][5] = 176;
	tab[3][6] = 176;
	tab[3][7] = 32;
	tab[3][8] = 32;
	tab[3][9] = 32;
	tab[3][10] = 32;
	tab[3][11] = 32;
	tab[3][12] = 32;
	tab[3][13] = 32;
	tab[3][14] = 32;
	tab[3][15] = 32;
	tab[3][16] = 32;
	tab[3][17] = 32;
	tab[3][18] = 32;
	tab[3][19] = 32;
	tab[3][20] = 32;
	tab[3][21] = 32;
	tab[3][22] = 32;
	tab[3][23] = 32;
	tab[3][24] = 32;
	tab[3][25] = 32;
	tab[3][26] = 32;
	tab[3][27] = 32;
	tab[3][28] = 32;
	tab[3][29] = 177;
	tab[3][30] = 177;
	tab[3][31] = 32;
	tab[3][32] = 32;
	tab[3][33] = 32;
	tab[3][34] = 32;
	tab[3][35] = 32;
	tab[3][36] = 32;
	tab[3][37] = 32;
	tab[3][38] = 32;
	tab[3][39] = 32;
	tab[3][40] = 32;
	tab[3][41] = 32;
	tab[3][42] = 32;
	tab[3][43] = 32;
	tab[3][44] = 32;
	tab[3][45] = 32;
	tab[3][46] = 32;
	tab[3][47] = 32;
	tab[3][48] = 32;
	tab[3][49] = 32;
	tab[3][50] = 32;
	tab[3][51] = 32;
	tab[3][52] = 32;
	tab[3][53] = 32;
	tab[3][54] = 32;
	tab[3][55] = 32;
	tab[3][56] = 32;
	tab[3][57] = 32;
	tab[3][58] = 32;
	tab[3][59] = 32;
	tab[3][60] = 32;
	tab[3][61] = 32;
	tab[3][62] = 32;
	tab[3][63] = 176;
	tab[3][64] = 176;
	tab[3][65] = 6;
	tab[3][66] = 6;
	tab[3][67] = 6;
	tab[3][68] = 6;
	tab[3][69] = 6;
	tab[3][70] = 32;
	tab[3][71] = 32;
	tab[3][72] = 32;
	tab[3][73] = 32;
	tab[3][74] = 32;
	tab[3][75] = 32;
	tab[3][76] = 32;
	tab[3][77] = 32;
	tab[3][78] = 32;
	tab[3][79] = 32;

	//remplissage 4e ligne
	tab[4][0] = 6;
	tab[4][1] = 6;
	tab[4][2] = 6;
	tab[4][3] = 6;
	tab[4][4] = 6;
	tab[4][5] = 32;
	tab[4][6] = 32;
	tab[4][7] = 32;
	tab[4][8] = 32;
	tab[4][9] = 32;
	tab[4][10] = 32;
	tab[4][11] = 32;
	tab[4][12] = 32;
	tab[4][13] = 32;
	tab[4][14] = 32;
	tab[4][15] = 32;
	tab[4][16] = 32;
	tab[4][17] = 32;
	tab[4][18] = 32;
	tab[4][19] = 32;
	tab[4][20] = 32;
	tab[4][21] = 32;
	tab[4][22] = 32;
	tab[4][23] = 32;
	tab[4][24] = 32;
	tab[4][25] = 32;
	tab[4][26] = 32;
	tab[4][27] = 32;
	tab[4][28] = 32;
	tab[4][29] = 32;
	tab[4][30] = 32;
	tab[4][31] = 32;
	tab[4][32] = 32;
	tab[4][33] = 32;
	tab[4][34] = 32;
	tab[4][35] = 32;
	tab[4][36] = 32;
	tab[4][37] = 32;
	tab[4][38] = 32;
	tab[4][39] = 32;
	tab[4][40] = 32;
	tab[4][41] = 32;
	tab[4][42] = 32;
	tab[4][43] = 32;
	tab[4][44] = 32;
	tab[4][45] = 32;
	tab[4][46] = 32;
	tab[4][47] = 32;
	tab[4][48] = 32;
	tab[4][49] = 32;
	tab[4][50] = 32;
	tab[4][51] = 32;
	tab[4][52] = 32;
	tab[4][53] = 32;
	tab[4][54] = 32;
	tab[4][55] = 32;
	tab[4][56] = 32;
	tab[4][57] = 32;
	tab[4][58] = 32;
	tab[4][59] = 32;
	tab[4][60] = 32;
	tab[4][61] = 32;
	tab[4][62] = 32;
	tab[4][63] = 32;
	tab[4][64] = 32;
	tab[4][65] = 6;
	tab[4][66] = 6;
	tab[4][67] = 6;
	tab[4][68] = 6;
	tab[4][69] = 6;
	tab[4][70] = 32;
	tab[4][71] = 32;
	tab[4][72] = 32;
	tab[4][73] = 32;
	tab[4][74] = 32;
	tab[4][75] = 32;
	tab[4][76] = 32;
	tab[4][77] = 32;
	tab[4][78] = 32;
	tab[4][79] = 32;

	//replissage 5e ligne
	tab[5][0] = 6;
	tab[5][1] = 6;
	tab[5][2] = 6;
	tab[5][3] = 6;
	tab[5][4] = 32;
	tab[5][5] = 32;
	tab[5][6] = 32;
	tab[5][7] = 32;
	tab[5][8] = 32;
	tab[5][9] = 32;
	tab[5][10] = 32;
	tab[5][11] = 32;
	tab[5][12] = 32;
	tab[5][13] = 32;
	tab[5][14] = 32;
	tab[5][15] = 32;
	tab[5][16] = 32;
	tab[5][17] = 32;
	tab[5][18] = 32;
	tab[5][19] = 32;
	tab[5][20] = 32;
	tab[5][21] = 32;
	tab[5][22] = 32;
	tab[5][23] = 32;
	tab[5][24] = 32;
	tab[5][25] = 32;
	tab[5][26] = 32;
	tab[5][27] = 32;
	tab[5][28] = 32;
	tab[5][29] = 177;
	tab[5][30] = 177;
	tab[5][31] = 32;
	tab[5][32] = 32;
	tab[5][33] = 32;
	tab[5][34] = 32;
	tab[5][35] = 32;
	tab[5][36] = 32;
	tab[5][37] = 32;
	tab[5][38] = 32;
	tab[5][39] = 32;
	tab[5][40] = 32;
	tab[5][41] = 32;
	tab[5][42] = 32;
	tab[5][43] = 32;
	tab[5][44] = 32;
	tab[5][45] = 32;
	tab[5][46] = 32;
	tab[5][47] = 32;
	tab[5][48] = 32;
	tab[5][49] = 32;
	tab[5][50] = 32;
	tab[5][51] = 32;
	tab[5][52] = 32;
	tab[5][53] = 32;
	tab[5][54] = 32;
	tab[5][55] = 32;
	tab[5][56] = 32;
	tab[5][57] = 32;
	tab[5][58] = 32;
	tab[5][59] = 32;
	tab[5][60] = 32;
	tab[5][61] = 32;
	tab[5][62] = 32;
	tab[5][63] = 32;
	tab[5][64] = 32;
	tab[5][65] = 6;
	tab[5][66] = 6;
	tab[5][67] = 6;
	tab[5][68] = 6;
	tab[5][69] = 6;

	//6e ligne
	tab[6][0] = 6;
	tab[6][1] = 6;
	tab[6][2] = 6;
	tab[6][3] = 32;
	tab[6][4] = 32;
	tab[6][5] = 32;
	tab[6][6] = 32;
	tab[6][7] = 32;
	tab[6][8] = 32;
	tab[6][9] = 32;
	tab[6][10] = 32;
	tab[6][11] = 32;
	tab[6][12] = 32;
	tab[6][13] = 32;
	tab[6][14] = 32;
	tab[6][15] = 32;
	tab[6][16] = 32;
	tab[6][17] = 32;
	tab[6][18] = 32;
	tab[6][19] = 32;
	tab[6][20] = 32;
	tab[6][21] = 32;
	tab[6][22] = 32;
	tab[6][23] = 32;
	tab[6][24] = 32;
	tab[6][25] = 32;
	tab[6][26] = 32;
	tab[6][27] = 32;
	tab[6][28] = 32;
	tab[6][29] = 32;
	tab[6][30] = 177;
	tab[6][31] = 177;
	tab[6][32] = 32;
	tab[6][33] = 32;
	tab[6][34] = 32;
	tab[6][35] = 32;
	tab[6][36] = 32;
	tab[6][37] = 32;
	tab[6][38] = 32;
	tab[6][39] = 32;
	tab[6][40] = 32;
	tab[6][41] = 32;
	tab[6][42] = 32;
	tab[6][43] = 32;
	tab[6][44] = 32;
	tab[6][45] = 32;
	tab[6][46] = 32;
	tab[6][47] = 32;
	tab[6][48] = 32;
	tab[6][49] = 32;
	tab[6][50] = 32;
	tab[6][51] = 32;
	tab[6][52] = 32;
	tab[6][53] = 32;
	tab[6][54] = 32;
	tab[6][55] = 32;
	tab[6][56] = 32;
	tab[6][57] = 32;
	tab[6][58] = 32;
	tab[6][59] = 32;
	tab[6][60] = 32;
	tab[6][61] = 32;
	tab[6][62] = 32;
	tab[6][63] = 32;
	tab[6][64] = 32;
	tab[6][65] = 6;
	tab[6][66] = 6;
	tab[6][67] = 6;
	tab[6][68] = 6;
	tab[6][69] = 6;

	//7e ligne
	tab[7][0] = 6;
	tab[7][1] = 6;
	tab[7][2] = 32;
	tab[7][3] = 32;
	tab[7][4] = 32;
	tab[7][5] = 32;
	tab[7][6] = 32;
	tab[7][7] = 32;
	tab[7][8] = 32;
	tab[7][9] = 32;
	tab[7][10] = 32;
	tab[7][11] = 32;
	tab[7][12] = 32;
	tab[7][13] = 32;
	tab[7][14] = 32;
	tab[7][15] = 32;
	tab[7][16] = 32;
	tab[7][17] = 32;
	tab[7][18] = 32;
	tab[7][19] = 32;
	tab[7][20] = 32;
	tab[7][21] = 32;
	tab[7][22] = 32;
	tab[7][23] = 32;
	tab[7][24] = 32;
	tab[7][25] = 32;
	tab[7][26] = 32;
	tab[7][27] = 32;
	tab[7][28] = 32;
	tab[7][29] = 32;
	tab[7][30] = 32;
	tab[7][31] = 177;
	tab[7][32] = 177;
	tab[7][33] = 32;
	tab[7][34] = 32;
	tab[7][35] = 32;
	tab[7][36] = 32;
	tab[7][37] = 32;
	tab[7][38] = 32;
	tab[7][39] = 32;
	tab[7][40] = 32;
	tab[7][41] = 32;
	tab[7][42] = 32;
	tab[7][43] = 32;
	tab[7][44] = 32;
	tab[7][45] = 32;
	tab[7][46] = 32;
	tab[7][47] = 32;
	tab[7][48] = 32;
	tab[7][49] = 32;
	tab[7][50] = 32;
	tab[7][51] = 32;
	tab[7][52] = 32;
	tab[7][53] = 32;
	tab[7][54] = 32;
	tab[7][55] = 32;
	tab[7][56] = 32;
	tab[7][57] = 32;
	tab[7][58] = 32;
	tab[7][59] = 32;
	tab[7][60] = 32;
	tab[7][61] = 32;
	tab[7][62] = 32;
	tab[7][63] = 32;
	tab[7][64] = 32;
	tab[7][65] = 32;
	tab[7][66] = 6;
	tab[7][67] = 6;
	tab[7][68] = 6;
	tab[7][69] = 6;

	//8e ligne
	tab[8][0] = 6;
	tab[8][1] = 32;
	tab[8][2] = 32;
	tab[8][3] = 32;
	tab[8][4] = 32;
	tab[8][5] = 32;
	tab[8][6] = 32;
	tab[8][7] = 32;
	tab[8][8] = 32;
	tab[8][9] = 32;
	tab[8][10] = 32;
	tab[8][11] = 32;
	tab[8][12] = 32;
	tab[8][13] = 32;
	tab[8][14] = 32;
	tab[8][15] = 32;
	tab[8][16] = 32;
	tab[8][17] = 32;
	tab[8][18] = 32;
	tab[8][19] = 32;
	tab[8][20] = 32;
	tab[8][21] = 32;
	tab[8][22] = 32;
	tab[8][23] = 32;
	tab[8][24] = 32;
	tab[8][25] = 32;
	tab[8][26] = 32;
	tab[8][27] = 32;
	tab[8][28] = 32;
	tab[8][29] = 32;
	tab[8][30] = 32;
	tab[8][31] = 32;
	tab[8][32] = 177;
	tab[8][33] = 177;
	tab[8][34] = 32;
	tab[8][35] = 32;
	tab[8][36] = 32;
	tab[8][37] = 32;
	tab[8][38] = 32;
	tab[8][39] = 32;
	tab[8][40] = 32;
	tab[8][41] = 32;
	tab[8][42] = 32;
	tab[8][43] = 32;
	tab[8][44] = 32;
	tab[8][45] = 32;
	tab[8][46] = 32;
	tab[8][47] = 32;
	tab[8][48] = 32;
	tab[8][49] = 32;
	tab[8][50] = 32;
	tab[8][51] = 32;
	tab[8][52] = 32;
	tab[8][53] = 32;
	tab[8][54] = 32;
	tab[8][55] = 32;
	tab[8][56] = 32;
	tab[8][57] = 32;
	tab[8][58] = 32;
	tab[8][59] = 32;
	tab[8][60] = 32;
	tab[8][61] = 32;
	tab[8][62] = 32;
	tab[8][63] = 32;
	tab[8][64] = 32;
	tab[8][65] = 32;
	tab[8][66] = 32;
	tab[8][67] = 6;
	tab[8][68] = 6;
	tab[8][69] = 6;

	//9e ligne
	tab[9][0] = 6;
	tab[9][1] = 32;
	tab[9][2] = 32;
	tab[9][3] = 32;
	tab[9][4] = 32;
	tab[9][5] = 32;
	tab[9][6] = 32;
	tab[9][7] = 32;
	tab[9][8] = 32;
	tab[9][9] = 32;
	tab[9][10] = 32;
	tab[9][11] = 32;
	tab[9][12] = 32;
	tab[9][13] = 32;
	tab[9][14] = 32;
	tab[9][15] = 32;
	tab[9][16] = 32;
	tab[9][17] = 32;
	tab[9][18] = 32;
	tab[9][19] = 32;
	tab[9][20] = 32;
	tab[9][21] = 32;
	tab[9][22] = 32;
	tab[9][23] = 32;
	tab[9][24] = 32;
	tab[9][25] = 32;
	tab[9][26] = 32;
	tab[9][27] = 32;
	tab[9][28] = 32;
	tab[9][29] = 32;
	tab[9][30] = 32;
	tab[9][31] = 32;
	tab[9][32] = 32;
	tab[9][33] = 177;
	tab[9][34] = 177;
	tab[9][35] = 32;
	tab[9][36] = 32;
	tab[9][37] = 32;
	tab[9][38] = 32;
	tab[9][39] = 32;
	tab[9][40] = 32;
	tab[9][41] = 32;
	tab[9][42] = 32;
	tab[9][43] = 32;
	tab[9][44] = 32;
	tab[9][45] = 32;
	tab[9][46] = 32;
	tab[9][47] = 32;
	tab[9][48] = 32;
	tab[9][49] = 32;
	tab[9][50] = 32;
	tab[9][51] = 32;
	tab[9][52] = 32;
	tab[9][53] = 32;
	tab[9][54] = 32;
	tab[9][55] = 32;
	tab[9][56] = 32;
	tab[9][57] = 32;
	tab[9][58] = 32;
	tab[9][59] = 32;
	tab[9][60] = 32;
	tab[9][61] = 32;
	tab[9][62] = 32;
	tab[9][63] = 32;
	tab[9][64] = 32;
	tab[9][65] = 32;
	tab[9][66] = 32;
	tab[9][67] = 32;
	tab[9][68] = 6;
	tab[9][69] = 6;

	//1Oe ligne
	tab[10][0] = 6;
	tab[10][1] = 32;
	tab[10][2] = 32;
	tab[10][3] = 32;
	tab[10][4] = 32;
	tab[10][5] = 32;
	tab[10][6] = 32;
	tab[10][7] = 32;
	tab[10][8] = 32;
	tab[10][9] = 32;
	tab[10][10] = 32;
	tab[10][11] = 32;
	tab[10][12] = 32;
	tab[10][13] = 32;
	tab[10][14] = 32;
	tab[10][15] = 32;
	tab[10][16] = 32;
	tab[10][17] = 6;
	tab[10][18] = 32;
	tab[10][19] = 32;
	tab[10][20] = 32;
	tab[10][21] = 32;
	tab[10][22] = 32;
	tab[10][23] = 32;
	tab[10][24] = 32;
	tab[10][25] = 32;
	tab[10][26] = 32;
	tab[10][27] = 32;
	tab[10][28] = 32;
	tab[10][29] = 32;
	tab[10][30] = 32;
	tab[10][31] = 32;
	tab[10][32] = 32;
	tab[10][33] = 32;
	tab[10][34] = 177;
	tab[10][35] = 177;
	tab[10][36] = 32;
	tab[10][37] = 32;
	tab[10][38] = 32;
	tab[10][39] = 32;
	tab[10][40] = 32;
	tab[10][41] = 32;
	tab[10][42] = 32;
	tab[10][43] = 32;
	tab[10][44] = 32;
	tab[10][45] = 32;
	tab[10][46] = 32;
	tab[10][47] = 32;
	tab[10][48] = 32;
	tab[10][49] = 32;
	tab[10][50] = 32;
	tab[10][51] = 32;
	tab[10][52] = 32;
	tab[10][53] = 32;
	tab[10][54] = 32;
	tab[10][55] = 6;
	tab[10][56] = 32;
	tab[10][57] = 32;
	tab[10][58] = 32;
	tab[10][59] = 32;
	tab[10][60] = 32;
	tab[10][61] = 32;
	tab[10][62] = 32;
	tab[10][63] = 32;
	tab[10][64] = 32;
	tab[10][65] = 32;
	tab[10][66] = 32;
	tab[10][67] = 32;
	tab[10][68] = 32;
	tab[10][69] = 6;

	//11e ligne
	tab[11][0] = 6;
	tab[11][1] = 32;
	tab[11][2] = 32;
	tab[11][3] = 32;
	tab[11][4] = 32;
	tab[11][5] = 32;
	tab[11][6] = 32;
	tab[11][7] = 32;
	tab[11][8] = 32;
	tab[11][9] = 32;
	tab[11][10] = 32;
	tab[11][11] = 32;
	tab[11][12] = 32;
	tab[11][13] = 32;
	tab[11][14] = 32;
	tab[11][15] = 32;
	tab[11][16] = 6;
	tab[11][17] = 6;
	tab[11][18] = 6;
	tab[11][19] = 32;
	tab[11][20] = 32;
	tab[11][21] = 32;
	tab[11][22] = 32;
	tab[11][23] = 32;
	tab[11][24] = 32;
	tab[11][25] = 32;
	tab[11][26] = 32;
	tab[11][27] = 32;
	tab[11][28] = 32;
	tab[11][29] = 32;
	tab[11][30] = 32;
	tab[11][31] = 32;
	tab[11][32] = 32;
	tab[11][33] = 32;
	tab[11][34] = 32;
	tab[11][35] = 177;
	tab[11][36] = 177;
	tab[11][37] = 32;
	tab[11][38] = 32;
	tab[11][39] = 32;
	tab[11][40] = 32;
	tab[11][41] = 32;
	tab[11][42] = 32;
	tab[11][43] = 32;
	tab[11][44] = 32;
	tab[11][45] = 32;
	tab[11][46] = 32;
	tab[11][47] = 32;
	tab[11][48] = 32;
	tab[11][49] = 32;
	tab[11][50] = 32;
	tab[11][51] = 32;
	tab[11][52] = 32;
	tab[11][53] = 32;
	tab[11][54] = 6;
	tab[11][55] = 6;
	tab[11][56] = 6;
	tab[11][57] = 32;
	tab[11][58] = 32;
	tab[11][59] = 32;
	tab[11][60] = 32;
	tab[11][61] = 32;
	tab[11][62] = 32;
	tab[11][63] = 32;
	tab[11][64] = 32;
	tab[11][65] = 32;
	tab[11][66] = 32;
	tab[11][67] = 32;
	tab[11][68] = 32;
	tab[11][69] = 6;

	//12e ligne
	tab[12][0] = 6;
	tab[12][1] = 32;
	tab[12][2] = 32;
	tab[12][3] = 32;
	tab[12][4] = 32;
	tab[12][5] = 32;
	tab[12][6] = 32;
	tab[12][7] = 32;
	tab[12][8] = 32;
	tab[12][9] = 32;
	tab[12][10] = 32;
	tab[12][11] = 32;
	tab[12][12] = 32;
	tab[12][13] = 32;
	tab[12][14] = 32;
	tab[12][15] = 6;
	tab[12][16] = 6;
	tab[12][17] = 6;
	tab[12][18] = 6;
	tab[12][19] = 6;
	tab[12][20] = 32;
	tab[12][21] = 32;
	tab[12][22] = 32;
	tab[12][23] = 32;
	tab[12][24] = 32;
	tab[12][25] = 32;
	tab[12][26] = 32;
	tab[12][27] = 32;
	tab[12][28] = 32;
	tab[12][29] = 32;
	tab[12][30] = 32;
	tab[12][31] = 32;
	tab[12][32] = 32;
	tab[12][33] = 32;
	tab[12][34] = 32;
	tab[12][35] = 32;
	tab[12][36] = 177;
	tab[12][37] = 177;
	tab[12][38] = 32;
	tab[12][39] = 32;
	tab[12][40] = 32;
	tab[12][41] = 32;
	tab[12][42] = 32;
	tab[12][43] = 32;
	tab[12][44] = 32;
	tab[12][45] = 32;
	tab[12][46] = 32;
	tab[12][47] = 32;
	tab[12][48] = 32;
	tab[12][49] = 32;
	tab[12][50] = 32;
	tab[12][51] = 32;
	tab[12][52] = 32;
	tab[12][53] = 6;
	tab[12][54] = 6;
	tab[12][55] = 6;
	tab[12][56] = 6;
	tab[12][57] = 6;
	tab[12][58] = 32;
	tab[12][59] = 32;
	tab[12][60] = 32;
	tab[12][61] = 32;
	tab[12][62] = 32;
	tab[12][63] = 32;
	tab[12][64] = 32;
	tab[12][65] = 32;
	tab[12][66] = 32;
	tab[12][67] = 32;
	tab[12][68] = 32;
	tab[12][69] = 6;

	//13e ligne
	tab[13][0] = 6;
	tab[13][1] = 32;
	tab[13][2] = 32;
	tab[13][3] = 32;
	tab[13][4] = 32;
	tab[13][5] = 32;
	tab[13][6] = 32;
	tab[13][7] = 32;
	tab[13][8] = 32;
	tab[13][9] = 32;
	tab[13][10] = 32;
	tab[13][11] = 32;
	tab[13][12] = 32;
	tab[13][13] = 32;
	tab[13][14] = 6;
	tab[13][15] = 6;
	tab[13][16] = 6;
	tab[13][17] = 6;
	tab[13][18] = 6;
	tab[13][19] = 6;
	tab[13][20] = 6;
	tab[13][21] = 32;
	tab[13][22] = 32;
	tab[13][23] = 32;
	tab[13][24] = 32;
	tab[13][25] = 32;
	tab[13][26] = 32;
	tab[13][27] = 32;
	tab[13][28] = 32;
	tab[13][29] = 32;
	tab[13][30] = 32;
	tab[13][31] = 32;
	tab[13][32] = 32;
	tab[13][33] = 32;
	tab[13][34] = 32;
	tab[13][35] = 32;
	tab[13][36] = 32;
	tab[13][37] = 32;
	tab[13][38] = 32;
	tab[13][39] = 32;
	tab[13][40] = 32;
	tab[13][41] = 32;
	tab[13][42] = 32;
	tab[13][43] = 32;
	tab[13][44] = 32;
	tab[13][45] = 32;
	tab[13][46] = 32;
	tab[13][47] = 32;
	tab[13][48] = 32;
	tab[13][49] = 32;
	tab[13][50] = 32;
	tab[13][51] = 32;
	tab[13][52] = 6;
	tab[13][53] = 6;
	tab[13][54] = 6;
	tab[13][55] = 6;
	tab[13][56] = 6;
	tab[13][57] = 6;
	tab[13][58] = 6;
	tab[13][59] = 32;
	tab[13][60] = 32;
	tab[13][61] = 32;
	tab[13][62] = 32;
	tab[13][63] = 32;
	tab[13][64] = 32;
	tab[13][65] = 32;
	tab[13][66] = 32;
	tab[13][67] = 32;
	tab[13][68] = 32;
	tab[13][69] = 6;

	//14e ligne
	tab[14][0] = 6;
	tab[14][1] = 32;
	tab[14][2] = 32;
	tab[14][3] = 32;
	tab[14][4] = 32;
	tab[14][5] = 32;
	tab[14][6] = 32;
	tab[14][7] = 32;
	tab[14][8] = 32;
	tab[14][9] = 32;
	tab[14][10] = 32;
	tab[14][11] = 32;
	tab[14][12] = 32;
	tab[14][13] = 32;
	tab[14][14] = 32;
	tab[14][15] = 6;
	tab[14][16] = 6;
	tab[14][17] = 6;
	tab[14][18] = 6;
	tab[14][19] = 6;
	tab[14][20] = 32;
	tab[14][21] = 32;
	tab[14][22] = 32;
	tab[14][23] = 32;
	tab[14][24] = 32;
	tab[14][25] = 32;
	tab[14][26] = 32;
	tab[14][27] = 32;
	tab[14][28] = 32;
	tab[14][29] = 32;
	tab[14][30] = 32;
	tab[14][31] = 32;
	tab[14][32] = 32;
	tab[14][33] = 32;
	tab[14][34] = 32;
	tab[14][35] = 32;
	tab[14][36] = 177;
	tab[14][37] = 177;
	tab[14][38] = 32;
	tab[14][39] = 32;
	tab[14][40] = 32;
	tab[14][41] = 32;
	tab[14][42] = 32;
	tab[14][43] = 32;
	tab[14][44] = 32;
	tab[14][45] = 32;
	tab[14][46] = 32;
	tab[14][47] = 32;
	tab[14][48] = 32;
	tab[14][49] = 32;
	tab[14][50] = 32;
	tab[14][51] = 32;
	tab[14][52] = 32;
	tab[14][53] = 6;
	tab[14][54] = 6;
	tab[14][55] = 6;
	tab[14][56] = 6;
	tab[14][57] = 6;
	tab[14][58] = 32;
	tab[14][59] = 32;
	tab[14][60] = 32;
	tab[14][61] = 32;
	tab[14][62] = 32;
	tab[14][63] = 32;
	tab[14][64] = 32;
	tab[14][65] = 32;
	tab[14][66] = 32;
	tab[14][67] = 32;
	tab[14][68] = 32;
	tab[14][69] = 6;

	//15e ligne
	tab[15][0] = 6;
	tab[15][1] = 32;
	tab[15][2] = 32;
	tab[15][3] = 32;
	tab[15][4] = 32;
	tab[15][5] = 32;
	tab[15][6] = 32;
	tab[15][7] = 32;
	tab[15][8] = 32;
	tab[15][9] = 32;
	tab[15][10] = 32;
	tab[15][11] = 32;
	tab[15][12] = 32;
	tab[15][13] = 32;
	tab[15][14] = 32;
	tab[15][15] = 32;
	tab[15][16] = 6;
	tab[15][17] = 6;
	tab[15][18] = 6;
	tab[15][19] = 32;
	tab[15][20] = 32;
	tab[15][21] = 32;
	tab[15][22] = 32;
	tab[15][23] = 32;
	tab[15][24] = 32;
	tab[15][25] = 32;
	tab[15][26] = 32;
	tab[15][27] = 32;
	tab[15][28] = 32;
	tab[15][29] = 32;
	tab[15][30] = 32;
	tab[15][31] = 32;
	tab[15][32] = 32;
	tab[15][33] = 32;
	tab[15][34] = 32;
	tab[15][35] = 32;
	tab[15][36] = 32;
	tab[15][37] = 177;
	tab[15][38] = 177;
	tab[15][39] = 32;
	tab[15][40] = 32;
	tab[15][41] = 32;
	tab[15][42] = 32;
	tab[15][43] = 32;
	tab[15][44] = 32;
	tab[15][45] = 32;
	tab[15][46] = 32;
	tab[15][47] = 32;
	tab[15][48] = 32;
	tab[15][49] = 32;
	tab[15][50] = 32;
	tab[15][51] = 32;
	tab[15][52] = 32;
	tab[15][53] = 32;
	tab[15][54] = 6;
	tab[15][55] = 6;
	tab[15][56] = 6;
	tab[15][57] = 32;
	tab[15][58] = 32;
	tab[15][59] = 32;
	tab[15][60] = 32;
	tab[15][61] = 32;
	tab[15][62] = 32;
	tab[15][63] = 32;
	tab[15][64] = 32;
	tab[15][65] = 32;
	tab[15][66] = 32;
	tab[15][67] = 32;
	tab[15][68] = 32;
	tab[15][69] = 6;

	//16e ligne
	tab[16][0] = 6;
	tab[16][1] = 32;
	tab[16][2] = 32;
	tab[16][3] = 32;
	tab[16][4] = 32;
	tab[16][5] = 32;
	tab[16][6] = 32;
	tab[16][7] = 32;
	tab[16][8] = 32;
	tab[16][9] = 32;
	tab[16][10] = 32;
	tab[16][11] = 32;
	tab[16][12] = 32;
	tab[16][13] = 32;
	tab[16][14] = 32;
	tab[16][15] = 32;
	tab[16][16] = 32;
	tab[16][17] = 6;
	tab[16][18] = 32;
	tab[16][19] = 32;
	tab[16][20] = 32;
	tab[16][21] = 32;
	tab[16][22] = 32;
	tab[16][23] = 32;
	tab[16][24] = 32;
	tab[16][25] = 32;
	tab[16][26] = 32;
	tab[16][27] = 32;
	tab[16][28] = 32;
	tab[16][29] = 32;
	tab[16][30] = 32;
	tab[16][31] = 32;
	tab[16][32] = 32;
	tab[16][33] = 32;
	tab[16][34] = 32;
	tab[16][35] = 32;
	tab[16][36] = 32;
	tab[16][37] = 32;
	tab[16][38] = 177;
	tab[16][39] = 177;
	tab[16][40] = 32;
	tab[16][41] = 32;
	tab[16][42] = 32;
	tab[16][43] = 32;
	tab[16][44] = 32;
	tab[16][45] = 32;
	tab[16][46] = 32;
	tab[16][47] = 32;
	tab[16][48] = 32;
	tab[16][49] = 32;
	tab[16][50] = 32;
	tab[16][51] = 32;
	tab[16][52] = 32;
	tab[16][53] = 32;
	tab[16][54] = 32;
	tab[16][55] = 6;
	tab[16][56] = 32;
	tab[16][57] = 32;
	tab[16][58] = 32;
	tab[16][59] = 32;
	tab[16][60] = 32;
	tab[16][61] = 32;
	tab[16][62] = 32;
	tab[16][63] = 32;
	tab[16][64] = 32;
	tab[16][65] = 32;
	tab[16][66] = 32;
	tab[16][67] = 32;
	tab[16][68] = 32;
	tab[16][69] = 6;

	//17e ligne
	tab[17][0] = 6;
	tab[17][1] = 6;
	tab[17][2] = 32;
	tab[17][3] = 32;
	tab[17][4] = 32;
	tab[17][5] = 32;
	tab[17][6] = 32;
	tab[17][7] = 32;
	tab[17][8] = 32;
	tab[17][9] = 32;
	tab[17][10] = 32;
	tab[17][11] = 32;
	tab[17][12] = 32;
	tab[17][13] = 32;
	tab[17][14] = 32;
	tab[17][15] = 32;
	tab[17][16] = 32;
	tab[17][17] = 32;
	tab[17][18] = 32;
	tab[17][19] = 32;
	tab[17][20] = 32;
	tab[17][21] = 32;
	tab[17][22] = 32;
	tab[17][23] = 32;
	tab[17][24] = 32;
	tab[17][25] = 32;
	tab[17][26] = 32;
	tab[17][27] = 32;
	tab[17][28] = 32;
	tab[17][29] = 32;
	tab[17][30] = 32;
	tab[17][31] = 32;
	tab[17][32] = 32;
	tab[17][33] = 32;
	tab[17][34] = 32;
	tab[17][35] = 32;
	tab[17][36] = 32;
	tab[17][37] = 32;
	tab[17][38] = 32;
	tab[17][39] = 177;
	tab[17][40] = 177;
	tab[17][41] = 32;
	tab[17][42] = 32;
	tab[17][43] = 32;
	tab[17][44] = 32;
	tab[17][45] = 32;
	tab[17][46] = 32;
	tab[17][47] = 32;
	tab[17][48] = 32;
	tab[17][49] = 32;
	tab[17][50] = 32;
	tab[17][51] = 32;
	tab[17][52] = 32;
	tab[17][53] = 32;
	tab[17][54] = 32;
	tab[17][55] = 32;
	tab[17][56] = 32;
	tab[17][57] = 32;
	tab[17][58] = 32;
	tab[17][59] = 32;
	tab[17][60] = 32;
	tab[17][61] = 32;
	tab[17][62] = 32;
	tab[17][63] = 32;
	tab[17][64] = 32;
	tab[17][65] = 32;
	tab[17][66] = 32;
	tab[17][67] = 32;
	tab[17][68] = 6;
	tab[17][69] = 6;

	//18e ligne
	tab[18][0] = 6;
	tab[18][1] = 6;
	tab[18][2] = 6;
	tab[18][3] = 32;
	tab[18][4] = 32;
	tab[18][5] = 32;
	tab[18][6] = 32;
	tab[18][7] = 32;
	tab[18][8] = 32;
	tab[18][9] = 32;
	tab[18][10] = 32;
	tab[18][11] = 32;
	tab[18][12] = 32;
	tab[18][13] = 32;
	tab[18][14] = 32;
	tab[18][15] = 32;
	tab[18][16] = 32;
	tab[18][17] = 32;
	tab[18][18] = 32;
	tab[18][19] = 32;
	tab[18][20] = 32;
	tab[18][21] = 32;
	tab[18][22] = 32;
	tab[18][23] = 32;
	tab[18][24] = 32;
	tab[18][25] = 32;
	tab[18][26] = 32;
	tab[18][27] = 32;
	tab[18][28] = 32;
	tab[18][29] = 32;
	tab[18][30] = 32;
	tab[18][31] = 32;
	tab[18][32] = 32;
	tab[18][33] = 32;
	tab[18][34] = 32;
	tab[18][35] = 32;
	tab[18][36] = 32;
	tab[18][37] = 32;
	tab[18][38] = 32;
	tab[18][39] = 32;
	tab[18][40] = 177;
	tab[18][41] = 177;
	tab[18][42] = 32;
	tab[18][43] = 32;
	tab[18][44] = 32;
	tab[18][45] = 32;
	tab[18][46] = 32;
	tab[18][47] = 32;
	tab[18][48] = 32;
	tab[18][49] = 32;
	tab[18][50] = 32;
	tab[18][51] = 32;
	tab[18][52] = 32;
	tab[18][53] = 32;
	tab[18][54] = 32;
	tab[18][55] = 32;
	tab[18][56] = 32;
	tab[18][57] = 32;
	tab[18][58] = 32;
	tab[18][59] = 32;
	tab[18][60] = 32;
	tab[18][61] = 32;
	tab[18][62] = 32;
	tab[18][63] = 32;
	tab[18][64] = 32;
	tab[18][65] = 32;
	tab[18][66] = 32;
	tab[18][67] = 6;
	tab[18][68] = 6;
	tab[18][69] = 6;

	//19e ligne
	tab[19][0] = 6;
	tab[19][1] = 6;
	tab[19][2] = 6;
	tab[19][3] = 6;
	tab[19][4] = 32;
	tab[19][5] = 32;
	tab[19][6] = 32;
	tab[19][7] = 32;
	tab[19][8] = 32;
	tab[19][9] = 32;
	tab[19][10] = 32;
	tab[19][11] = 32;
	tab[19][12] = 32;
	tab[19][13] = 32;
	tab[19][14] = 32;
	tab[19][15] = 32;
	tab[19][16] = 32;
	tab[19][17] = 32;
	tab[19][18] = 32;
	tab[19][19] = 32;
	tab[19][20] = 32;
	tab[19][21] = 32;
	tab[19][22] = 32;
	tab[19][23] = 32;
	tab[19][24] = 32;
	tab[19][25] = 32;
	tab[19][26] = 32;
	tab[19][27] = 32;
	tab[19][28] = 32;
	tab[19][29] = 32;
	tab[19][30] = 32;
	tab[19][31] = 32;
	tab[19][32] = 32;
	tab[19][33] = 32;
	tab[19][34] = 32;
	tab[19][35] = 32;
	tab[19][36] = 32;
	tab[19][37] = 32;
	tab[19][38] = 32;
	tab[19][39] = 32;
	tab[19][40] = 32;
	tab[19][41] = 177;
	tab[19][42] = 177;
	tab[19][43] = 32;
	tab[19][44] = 32;
	tab[19][45] = 32;
	tab[19][46] = 32;
	tab[19][47] = 32;
	tab[19][48] = 32;
	tab[19][49] = 32;
	tab[19][50] = 32;
	tab[19][51] = 32;
	tab[19][52] = 32;
	tab[19][53] = 32;
	tab[19][54] = 32;
	tab[19][55] = 32;
	tab[19][56] = 32;
	tab[19][57] = 32;
	tab[19][58] = 32;
	tab[19][59] = 32;
	tab[19][60] = 32;
	tab[19][61] = 32;
	tab[19][62] = 32;
	tab[19][63] = 32;
	tab[19][64] = 32;
	tab[19][65] = 32;
	tab[19][66] = 6;
	tab[19][67] = 6;
	tab[19][68] = 6;
	tab[19][69] = 6;

	//20e ligne
	tab[20][0] = 6;
	tab[20][1] = 6;
	tab[20][2] = 6;
	tab[20][3] = 6;
	tab[20][4] = 6;
	tab[20][5] = 32;
	tab[20][6] = 32;
	tab[20][7] = 32;
	tab[20][8] = 32;
	tab[20][9] = 32;
	tab[20][10] = 32;
	tab[20][11] = 32;
	tab[20][12] = 32;
	tab[20][13] = 32;
	tab[20][14] = 32;
	tab[20][15] = 32;
	tab[20][16] = 32;
	tab[20][17] = 32;
	tab[20][18] = 32;
	tab[20][19] = 32;
	tab[20][20] = 32;
	tab[20][21] = 32;
	tab[20][22] = 32;
	tab[20][23] = 32;
	tab[20][24] = 32;
	tab[20][25] = 32;
	tab[20][26] = 32;
	tab[20][27] = 32;
	tab[20][28] = 32;
	tab[20][29] = 32;
	tab[20][30] = 32;
	tab[20][31] = 32;
	tab[20][32] = 32;
	tab[20][33] = 32;
	tab[20][34] = 32;
	tab[20][35] = 32;
	tab[20][36] = 32;
	tab[20][37] = 32;
	tab[20][38] = 32;
	tab[20][39] = 32;
	tab[20][40] = 32;
	tab[20][41] = 32;
	tab[20][42] = 177;
	tab[20][43] = 177;
	tab[20][44] = 32;
	tab[20][45] = 32;
	tab[20][46] = 32;
	tab[20][47] = 32;
	tab[20][48] = 32;
	tab[20][49] = 32;
	tab[20][50] = 32;
	tab[20][51] = 32;
	tab[20][52] = 32;
	tab[20][53] = 32;
	tab[20][54] = 32;
	tab[20][55] = 32;
	tab[20][56] = 32;
	tab[20][57] = 32;
	tab[20][58] = 32;
	tab[20][59] = 32;
	tab[20][60] = 32;
	tab[20][61] = 32;
	tab[20][62] = 32;
	tab[20][63] = 32;
	tab[20][64] = 32;
	tab[20][65] = 32;
	tab[20][66] = 6;
	tab[20][67] = 6;
	tab[20][68] = 6;
	tab[20][69] = 6;

	//21e ligne
	tab[21][0] = 6;
	tab[21][1] = 6;
	tab[21][2] = 6;
	tab[21][3] = 6;
	tab[21][4] = 6;
	tab[21][5] = 32;
	tab[21][6] = 32;
	tab[21][7] = 32;
	tab[21][8] = 32;
	tab[21][9] = 32;
	tab[21][10] = 32;
	tab[21][11] = 32;
	tab[21][12] = 32;
	tab[21][13] = 32;
	tab[21][14] = 32;
	tab[21][15] = 32;
	tab[21][16] = 32;
	tab[21][17] = 32;
	tab[21][18] = 32;
	tab[21][19] = 32;
	tab[21][20] = 32;
	tab[21][21] = 32;
	tab[21][22] = 32;
	tab[21][23] = 32;
	tab[21][24] = 32;
	tab[21][25] = 32;
	tab[21][26] = 32;
	tab[21][27] = 32;
	tab[21][28] = 32;
	tab[21][29] = 32;
	tab[21][30] = 32;
	tab[21][31] = 32;
	tab[21][32] = 32;
	tab[21][33] = 32;
	tab[21][34] = 32;
	tab[21][35] = 32;
	tab[21][36] = 32;
	tab[21][37] = 32;
	tab[21][38] = 32;
	tab[21][39] = 32;
	tab[21][40] = 32;
	tab[21][41] = 32;
	tab[21][42] = 32;
	tab[21][43] = 177;
	tab[21][44] = 177;
	tab[21][45] = 32;
	tab[21][46] = 32;
	tab[21][47] = 32;
	tab[21][48] = 32;
	tab[21][49] = 32;
	tab[21][50] = 32;
	tab[21][51] = 32;
	tab[21][52] = 32;
	tab[21][53] = 32;
	tab[21][54] = 32;
	tab[21][55] = 32;
	tab[21][56] = 32;
	tab[21][57] = 32;
	tab[21][58] = 32;
	tab[21][59] = 32;
	tab[21][60] = 32;
	tab[21][61] = 32;
	tab[21][62] = 32;
	tab[21][63] = 32;
	tab[21][64] = 32;
	tab[21][65] = 6;
	tab[21][66] = 6;
	tab[21][67] = 6;
	tab[21][68] = 6;
	tab[21][69] = 6;

	//22e ligne
	tab[22][0] = 6;
	tab[22][1] = 6;
	tab[22][2] = 6;
	tab[22][3] = 6;
	tab[22][4] = 6;
	tab[22][5] = 176;
	tab[22][6] = 176;
	tab[22][7] = 32;
	tab[22][8] = 32;
	tab[22][9] = 32;
	tab[22][10] = 32;
	tab[22][11] = 32;
	tab[22][12] = 32;
	tab[22][13] = 32;
	tab[22][14] = 32;
	tab[22][15] = 32;
	tab[22][16] = 32;
	tab[22][17] = 32;
	tab[22][18] = 32;
	tab[22][19] = 32;
	tab[22][20] = 32;
	tab[22][21] = 32;
	tab[22][22] = 32;
	tab[22][23] = 32;
	tab[22][24] = 32;
	tab[22][25] = 32;
	tab[22][26] = 32;
	tab[22][27] = 32;
	tab[22][28] = 32;
	tab[22][29] = 32;
	tab[22][30] = 32;
	tab[22][31] = 32;
	tab[22][32] = 32;
	tab[22][33] = 32;
	tab[22][34] = 32;
	tab[22][35] = 32;
	tab[22][36] = 32;
	tab[22][37] = 32;
	tab[22][38] = 32;
	tab[22][39] = 32;
	tab[22][40] = 32;
	tab[22][41] = 32;
	tab[22][42] = 32;
	tab[22][43] = 32;
	tab[22][44] = 32;
	tab[22][45] = 32;
	tab[22][46] = 32;
	tab[22][47] = 32;
	tab[22][48] = 32;
	tab[22][49] = 32;
	tab[22][50] = 32;
	tab[22][51] = 32;
	tab[22][52] = 32;
	tab[22][53] = 32;
	tab[22][54] = 32;
	tab[22][55] = 32;
	tab[22][56] = 32;
	tab[22][57] = 32;
	tab[22][58] = 32;
	tab[22][59] = 32;
	tab[22][60] = 32;
	tab[22][61] = 32;
	tab[22][62] = 32;
	tab[22][63] = 176;
	tab[22][64] = 176;
	tab[22][65] = 6;
	tab[22][66] = 6;
	tab[22][67] = 6;
	tab[22][68] = 6;
	tab[22][69] = 6;

	//23e ligne
	tab[23][0] = 6;
	tab[23][1] = 6;
	tab[23][2] = 6;
	tab[23][3] = 6;
	tab[23][4] = 6;
	tab[23][5] = 176;
	tab[23][6] = 176;
	tab[23][7] = 32;
	tab[23][8] = 32;
	tab[23][9] = 32;
	tab[23][10] = 32;
	tab[23][11] = 32;
	tab[23][12] = 32;
	tab[23][13] = 32;
	tab[23][14] = 32;
	tab[23][15] = 32;
	tab[23][16] = 32;
	tab[23][17] = 32;
	tab[23][18] = 32;
	tab[23][19] = 32;
	tab[23][20] = 32;
	tab[23][21] = 32;
	tab[23][22] = 32;
	tab[23][23] = 32;
	tab[23][24] = 32;
	tab[23][25] = 32;
	tab[23][26] = 32;
	tab[23][27] = 32;
	tab[23][28] = 32;
	tab[23][29] = 32;
	tab[23][30] = 32;
	tab[23][31] = 32;
	tab[23][32] = 32;
	tab[23][33] = 32;
	tab[23][34] = 32;
	tab[23][35] = 32;
	tab[23][36] = 32;
	tab[23][37] = 32;
	tab[23][38] = 32;
	tab[23][39] = 32;
	tab[23][40] = 32;
	tab[23][41] = 32;
	tab[23][42] = 32;
	tab[23][43] = 177;
	tab[23][44] = 177;
	tab[23][45] = 32;
	tab[23][46] = 32;
	tab[23][47] = 32;
	tab[23][48] = 32;
	tab[23][49] = 32;
	tab[23][50] = 32;
	tab[23][51] = 32;
	tab[23][52] = 32;
	tab[23][53] = 32;
	tab[23][54] = 32;
	tab[23][55] = 32;
	tab[23][56] = 32;
	tab[23][57] = 32;
	tab[23][58] = 32;
	tab[23][59] = 32;
	tab[23][60] = 32;
	tab[23][61] = 32;
	tab[23][62] = 32;
	tab[23][63] = 176;
	tab[23][64] = 176;
	tab[23][65] = 6;
	tab[23][66] = 6;
	tab[23][67] = 6;
	tab[23][68] = 6;
	tab[23][69] = 6;

	//ligne 24
	tab[24][0] = 6;
	tab[24][1] = 6;
	tab[24][2] = 6;
	tab[24][3] = 6;
	tab[24][4] = 6;
	tab[24][5] = 6;
	tab[24][6] = 6;
	tab[24][7] = 6;
	tab[24][8] = 6;
	tab[24][9] = 6;
	tab[24][10] = 6;
	tab[24][11] = 6;
	tab[24][12] = 6;
	tab[24][13] = 6;
	tab[24][14] = 6;
	tab[24][15] = 6;
	tab[24][16] = 6;
	tab[24][17] = 6;
	tab[24][18] = 6;
	tab[24][19] = 6;
	tab[24][20] = 6;
	tab[24][21] = 6;
	tab[24][22] = 6;
	tab[24][23] = 6;
	tab[24][24] = 6;
	tab[24][25] = 6;
	tab[24][26] = 6;
	tab[24][27] = 6;
	tab[24][28] = 6;
	tab[24][29] = 6;
	tab[24][30] = 6;
	tab[24][31] = 6;
	tab[24][32] = 6;
	tab[24][33] = 6;
	tab[24][34] = 6;
	tab[24][35] = 6;
	tab[24][36] = 6;
	tab[24][37] = 6;
	tab[24][38] = 6;
	tab[24][39] = 6;
	tab[24][40] = 6;
	tab[24][41] = 6;
	tab[24][42] = 32;
	tab[24][43] = 177;
	tab[24][44] = 177;
	tab[24][45] = 32;
	tab[24][46] = 6;
	tab[24][47] = 6;
	tab[24][48] = 6;
	tab[24][49] = 6;
	tab[24][50] = 6;
	tab[24][51] = 6;
	tab[24][52] = 6;
	tab[24][53] = 6;
	tab[24][54] = 6;
	tab[24][55] = 6;
	tab[24][56] = 6;
	tab[24][57] = 6;
	tab[24][58] = 6;
	tab[24][59] = 6;
	tab[24][60] = 6;
	tab[24][61] = 6;
	tab[24][62] = 6;
	tab[24][63] = 6;
	tab[24][64] = 6;
	tab[24][65] = 6;
	tab[24][66] = 6;
	tab[24][67] = 6;
	tab[24][68] = 6;
	tab[24][69] = 6;
}

