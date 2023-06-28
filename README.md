# Carcassonne
## Environnement

Le code compile et tourne sans problème sur [repl.it](https://replit.com/), qui fonctionne sous un environnement linux.

## Compiler

- Compiler le jeu : `make exec`
- Supprimer les résidus de compilation : `make clean`
- Compiler le jeu et supprimer les résidus de compilation : `make all`
Le fichier compilé est obtenu sous le nom de `carcassonne.out` et peut être renommé.

## Lancement du jeu

Un fichier CSV contenant les tuiles doit être spécifié : `./carcassonne.out "nom_fichier.csv"`  
Si le fichier n'est pas renseigné ou est érroné, le programme se termine avec une erreur. (Une faute de segmentation peut être renvoyée dû au fait que l'existence d'un fichier n'est pas testée par mesure de compatibilité Linux/Windows du programme).  
Le déroulement de la partie s'enchaîne.

## Règles du jeu

Voir les [règles du jeu](https://www.play-in.com/pdf/rules_games/carcassonne_jeu_de_base_-_nouvelle_edition_regles_fr.pdf)."# carcassone" 
