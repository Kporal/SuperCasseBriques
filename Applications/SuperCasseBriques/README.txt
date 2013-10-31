==== Super Casse Briques ===

1) Lancement :
Dans le shell, écrire "run CB" pour lancer l'application Casse Brique.


2) Commandes :
Indiquées pour un clavier "Azerty" :
'z' = Lancer la balle
'q' = Deplacer la raquette vers la gauche
'd' = Deplacer la raquette vers la droite
'r' = Relancer une partie
'p' = Arrete le jeu


3) Deroulement du jeu :
Le but du jeu est de detruire tous les blocs, en faisant rebondir la balle dessus.
La partie commence quand la balle est lance.
Quand la balle tombe dans le trou (la zone noire sous la raquette), une vie perdue.
Il faut alors relancer une nouvelle balle.
Lorsque le compteur de vie atteind 0, la partie est perdue.
Si toutes les briques sont detruites, le niveau est gagne.

Il est possible de relancer une partie lorsqu'il y a aucune balle active.


4) Bug connus :
La balle peut sortir de l'ecran ou se coincer dans la zonne du menu.


5) Amelioration possibles :
-> Version avec blocs
Une version avec bloc a commence a etre implemente, mais etant bugge, elle est desactive.
Pour l'activer :
	- aller dans la classe GrilleCB
	- decommenter l'appel aux methodes de la version avec Blocs
	- commenter l'appel des methodes de la version sans Blocs