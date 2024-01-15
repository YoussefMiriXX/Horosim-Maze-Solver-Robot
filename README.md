## <p align="center"><ins>Horosim-Maze-Solver-Robot
##### Superviseur:Prof Sébastien Rothhut
###### Auteur: Youssef Miri && Daniel Wartski
### <ins> Algorithme utilisé:

 
 Le programme va etre divisé en deux parties:La première est de trouver la case Noir en suivant toujours un mur puis priotirizer le chemin de la gauche.
 
 L'algorithme utilisé s'appelle le `Wall follower Algorithm` dont le principe est de toujours suivre un mur et passer à la gauche donc c'est le `Left Hand Rule`.

 La règle de la main gauche est une stratégie couramment utilisée pour résoudre les labyrinthes. L'idée de base est de garder votre main gauche (ou droite, selon la préférence) contre le mur du labyrinthe et de la suivre de manière cohérente jusqu'à ce que vous atteigniez la sortie. Voici un guide étape par étape en utilisant la règle de la main gauche :

    Choisissez un Point de Départ : Commencez à n'importe quel point le long du mur extérieur du labyrinthe.

    Suivez le Mur avec la Main Gauche : Gardez votre main gauche contre le mur (à gauche ou à droite, selon votre préférence) et commencez à marcher.

    Suivez le Mur : Continuez à marcher et suivez le mur avec votre main gauche. Ne détachez jamais votre main du mur.

    Tournez aux Coins : Lorsque vous rencontrez une jonction ou une intersection, tournez toujours à gauche (ou à droite, selon la main que vous avez choisie).

    Continuez à Explorer : Continuez à naviguer dans le labyrinthe, en tournant à gauche à chaque intersection. Si vous atteignez un cul-de-sac, faites demi-tour et continuez à suivre le mur.

    Atteignez la case Noire: Finalement, en suivant de manière cohérente la règle de la main gauche, on atteind la case noire du labyrinthe.


#### <ins>Contraintes:

En employant, juste l'algorithme du `Hand Rule` pour revenir à la case rouge ça risque parfois que le Robot entre toujours dans une loop et ne sort jamais c'est pour cela on a ajouté la fonction aléatoire(), qui sert pour sortir de la boucle en suivant toujours un mur mais pas forcement aller ver la gauche. 

#### <ins>On aimerai avoir en plus:

- la possibilité de créer unr matrice, qui sauvegarde tous mouvements du robots puis faire le chemin inverse.
  Cette solution semble efficace pour eviter le retour aléatoire. 
  
- Employer un correcteur PID, pour avoir des rotations plus précis selon les cas possibles.



