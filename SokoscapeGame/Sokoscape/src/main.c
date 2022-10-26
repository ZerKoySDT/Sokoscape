#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "niveau.h"
#include "affichageLevel.h"
#include "evenement.h"
#include "chrono.h"

#define LARGEUR_FENETRE 1024  // Largeur de la fenêtre 1024
#define HAUTEUR_FENETRE 768   // Hauteur de la fenêtre 768

int main(int argc, char* argv[]){
  SDL_Window* window = NULL;  // Déclaration de la fenêtre à NULL
  GameObject player;  // Déclaration d'un GameObject pour le joueur
  StaticObject* tiles = NULL;   // Déclaration d'un pointeur sur des textures (qui va servir de tableau dynamique)
  GameObject* objects = NULL;   // Déclaration d'un pointeur sur un GameObject (qui va servir de tableau dynamique)
  SDL_Renderer* renderer = NULL;  // Déclaration du rendu
  int width, height;  // Nombre de tuiles en hauteur et en largeur
  int nbTextures;   // Nombre de cases allouées dynamiquement dans tiles
  int nbObjects;  // Nombre d'objets qui sont dans le fichier "NOBJETS:X"
  int nbTrous;    // Déclaration de la variable contenant le nombre de trous dans le jeu
  int nbFire; // Déclaration d'un entier pour le nombre de feu sur le plateau
  int nbWater;    // Déclaration d'un entier pour le nombre de flaques d'eau sur le plateau
  int posTuileObjectX;    // Déclaration de la variable contenant la position de la tuile en X et Y sur laquelle un GameObject est placé
  int posTuileObjectY;
  int indiceWin;    // Déclaration d'un entier pour savoir l'indice où est le joueur pour détecter la victoire
  int win = 0;      // Déclaration d'un entier savoir si la partie est gagnée ou perdue (perdue par défaut)
  int TuileObject;      // Déclaration d'un entier pour savoir l'indice de la tuile où est la caisse tombée dans une flaque
  int waterCaisse = -1;     // Indice de la caisse tombée dans la flaque (par défaut à -1: aucune caisse.)
  SDL_bool isRunning = SDL_TRUE;    // Déclaration d'un booléen qui correspond à la partie en
  SDL_Event event;      // Déclaration d'un événement
  int chrono = 0;     // Déclaration d'un entier pour compter les secondes lors du compteur
  int direction = -2;     // Déclaration d'un entier pour savoir le résultat de l'événement
                          // -1: fermer la fenêtre
                          // 1: vers le haut
                          // 2: vers la gauche
                          // 3: vers la droite
                          // 4: vers le bas



  // Initialisation de la SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0){    // S'il y a une erreur dans l'initialisation de la SDL, on sort du programme.
    SDL_Log("Erreur - Initialisation SDL > %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  // Initialisation de la librairie SDL_ttf.h pour les textes
  if(TTF_Init() == -1){
    SDL_Log("Erreur d'initialisation de TTF : %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }
  // Création de la fenêtre
  window = SDL_CreateWindow("Sokoscape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_FENETRE, HAUTEUR_FENETRE,0);
  if(window == NULL){ // Si la fenêtre est pas créée, on sort de la SDL
    SDL_Log("Erreur - Création fenêtre > %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  FILE* fichier = NULL;   // Déclaration de notre fichier
  fichier = fopen("plateau.tab", "r");    // Ouverture de notre fichier en mode lecture seule
  if(fichier == NULL){  // Si le fichier ne s'ouvre pas, on détruit la fenêtre et on sort de la SDL
    printf("Erreur - Open_File \n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }
  // On appelle la fonction getWidthHeight qui va lire la largeur et la hauteur dans le fichier et le stocker dans les variables
  // width & height
  getWidthHeight(fichier, &width, &height);

  short int Tilemap[height][width];   // Déclaration du tableau à 2 dimensions de height lignes et width colonnes

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);   // Déclaration du rendu pour la fenêtre
  if(renderer == NULL){   // S'il y a une erreur dans la création du rendu, on sort de la SDL en détruisant la fenêtre
    SDL_Log("Erreur - Création rendu > %s", SDL_GetError());
    SDL_DestroyWindow(window);
    // On quitte la librairie TTF
    IMG_Quit();
    // On quitte la SDL
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Appel de la fonction loadTextures pour charger les textures dans notre double pointeur à partir de notre fichier
  tiles = loadTextures(fichier,tiles, renderer, &nbTextures);
  if(tiles == NULL){  // Si la fonction échoue, on détruit tout ce qui a été alloué grâce à la fonction clearRessources et on quitte la SDL
    clearRessources(tiles, renderer, window, objects, &player, nbTextures, nbObjects);
    // On quitte la librairie TTF
    IMG_Quit();
    // On quitte la SDL
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Appel de la fonction loadTilemap qui va charger notre tableau à deux dimensions pour la carte à partir de notre fichier
  loadTilemap(fichier, width, height, Tilemap);

  // Appel de la fonction getNumberObjects qui va modifier la variable nbObjects à partir de notre fichier
  getNumberObjects(fichier, &nbObjects);

  // Appel de la fonction loadObjects qui va modifier le tableau dynamique objects
  objects = loadObjects(fichier, objects, renderer);
  if(objects == NULL){    // Si la fonction échoue, on détruit les éléments alloués grâce à la fonction clearRessources
    clearRessources(tiles, renderer, window, objects, &player, nbTextures, nbObjects);
    // On quitte la librairie TTF
    IMG_Quit();
    // On quitte la SDL
    SDL_Quit();
    return EXIT_FAILURE;
  }
  // Appel de la fonction loadJoueur qui va charger la texture dans le GameObject
  if(loadJoueur(fichier, &player, renderer) == EXIT_FAILURE){   // Si la fonction échoue, on détruit les éléments alloués grâce à la fonction clearRessources
    clearRessources(tiles, renderer, window, objects, &player, nbTextures, nbObjects);
    // On quitte la librairie TTF
    IMG_Quit();
    // On quitte la SDL
    SDL_Quit();
    return EXIT_FAILURE;
  }
  fclose(fichier);  // On ferme le fichier
  // Affiche le niveau
  if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
    SDL_Log("Erreur: affichage map: %s", SDL_GetError());
    clearRessources(tiles, renderer, window, objects, &player, nbTextures, nbObjects);
    // On quitte la librairie TTF
    IMG_Quit();
    // On quitte la SDL
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Déclaration de la police du texte de gameover & de victoire
  TTF_Font* police;
  // Appel de la fonction OpenFont pour modifier la police et sa taille.
  police = TTF_OpenFont("font/Roboto-Bold.ttf", 80);
  SDL_Color color_gameover = {255,0,0,0};       // Déclaration des couleurs pour les textes de victoire & de gameover
  SDL_Color color_victoire = {0,255,0,0};

  // Déclaration de la surface et de la texture pour supporter le texte Gameover
  SDL_Surface* texte_gameover = TTF_RenderText_Solid(police, "Gameover", color_gameover);
  SDL_Texture* texte_G = SDL_CreateTextureFromSurface(renderer, texte_gameover);

  // Déclaration de la surface et de la texture pour supporter le texte Victoire
  SDL_Surface* texte_victoire = TTF_RenderText_Solid(police, "Victoire !", color_victoire);
  SDL_Texture* texte_V = SDL_CreateTextureFromSurface(renderer, texte_victoire);

  // Destruction des surfaces texte_gameover et texte_victoire
  SDL_FreeSurface(texte_gameover);
  SDL_FreeSurface(texte_victoire);
  // Déclaration du rectangle pour la position du texte Gameover et affectation des valeurs x y
  // Ainsi que w et h grâce à SDL_QueryTexture
  SDL_Rect positionGameover;
  positionGameover.x = 350;
  positionGameover.y = 250;
  SDL_QueryTexture(texte_G, NULL, NULL, &positionGameover.w, &positionGameover.h);
  // Déclaration du rectangle pour la position du texte Victoire et affectation des valeurs x y
  // Ainsi que w et h grâce à SDL_QueryTexture
  SDL_Rect positionVictoire;
  positionVictoire.x = 350;
  positionVictoire.y = 250;
  SDL_QueryTexture(texte_V, NULL, NULL, &positionVictoire.w, &positionVictoire.h);

  // Déclaration de la couleur pour l'affichage du chronomètre
  SDL_Color colorTimer = {0,0,0,0};

  // Déclaration de la police d'écriture pour le chronomètre
  TTF_Font* policeTimer;
  policeTimer = TTF_OpenFont("font/8bitOperatorPlus-Bold.ttf", 50);
  // Déclaration de la chaine de caractères du chronomètre
  char timer[6] = "03:00";


  // Récupération du nombre de trou sur le plateau
  nbTrous = getNumberHoles(getIndiceByPath(nbTextures, tiles, "../images/trou.png"), width, height, Tilemap);
  // Déclaration d'un tableau de rectangle pour tous les trous
  SDL_Rect trous[nbTrous];
  // Récupération des rectangles de positionnement des trous sur le plateau
  loadHoles(getIndiceByPath(nbTextures, tiles, "../images/trou.png"), width, height, Tilemap, nbTrous, trous);

  // Récupération du nombre de feu sur le plateau
  nbFire = getNumberFire(getIndiceByPath(nbTextures, tiles, "../images/feu.png"), width, height, Tilemap);
  // Déclaration d'un tableau de rectangle pour tous les feux du plateau
  SDL_Rect fire[nbFire];
  // Récupération des rectangles de positionnement de tous les feux sur le plateau
  loadFire(getIndiceByPath(nbTextures, tiles, "../images/feu.png"), width, height, Tilemap, nbFire, fire);

  // Récupération du nombre de flaque d'eau sur le plateau
  nbWater = getNumberWater(getIndiceByPath(nbTextures, tiles, "../images/flaque.png"), width, height, Tilemap);
  // Déclaration d'un tableau de rectangle pour toutes les flaques du plateau
  SDL_Rect water[nbWater];
  loadWater(getIndiceByPath(nbTextures, tiles, "../images/flaque.png"), width, height, Tilemap, nbWater, water);


  // Boucle du jeu
  while(isRunning){
    // récupération de la touche clavier (cette fonction gère les déplacements des objets et du joueur)
    direction = getToucheClavier(event, nbObjects, &objects, &player, width, height, Tilemap, tiles, nbTextures);
	   if(direction == -1){
          // Arrêt du jeu en cas de clic sur la croix
		     isRunning = SDL_FALSE;
		     break;
	   }
    // Gestion des caisses dans les trous
    caisseInHoles(nbTrous, trous, nbObjects, &objects);
    // Gestion des caisses dans les feux
    caisseInFire(nbFire, fire, nbObjects, &objects);
    // On nettoie le rendu (pour le chronomètre)
    SDL_RenderClear(renderer);
    // Si il y a une caisse qui touche une flaque d'eau, waterCaisse recevra son indice
    waterCaisse = caisseInWater(nbWater, water, nbObjects, &objects);
    if(waterCaisse != -1){    // S'il existe une caisse en collision avec une flaque d'eau
      if(direction == 1){     // Si la caisse se déplace vers le haut
        (objects+waterCaisse)->destRect.y -= 3;   // On déplace la caisse en haut
        // On regarde où la caisse est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);
        // Tant que la caisse ne rencontre pas un obstacle, on la déplace vers le haut et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, (objects+waterCaisse), TuileObject, posTuileObjectX, posTuileObjectY, direction)==0){
          (objects+waterCaisse)->destRect.y -= 3;
          TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si une caisse est dans un trou, on arrête la glissade
          if(caisseInHoles(nbTrous, trous, nbObjects, &objects)==1){
            (objects+waterCaisse)->destRect.y -= 3;
            break;
          }
        }
        (objects+waterCaisse)->destRect.y += 3;   // On redéplace la caisse à la fin de la glissade

      } else if(direction == 2){     // Si la caisse se déplace vers la gauche
        (objects+waterCaisse)->destRect.x -= 3;   // On déplace la caisse à gauche
        // On regarde où la caisse est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);
        // Tant que la caisse ne rencontre pas un obstacle, on la déplace vers la gauche et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, (objects+waterCaisse), TuileObject, posTuileObjectX, posTuileObjectY, direction)==0){
          (objects+waterCaisse)->destRect.x -= 3;
          TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si une caisse est dans un trou, on arrête la glissade
          if(caisseInHoles(nbTrous, trous, nbObjects, &objects)==1){
            break;
          }
        }
        (objects+waterCaisse)->destRect.x += 3;   // On redéplace la caisse à la fin de la glissade

      } else if(direction == 3){     // Si la caisse se déplace vers la droite
        (objects+waterCaisse)->destRect.x += 3;   // On déplace la caisse à droite
        // On regarde où la caisse est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);
        // Tant que la caisse ne rencontre pas un obstacle, on la déplace vers la droite et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, (objects+waterCaisse), TuileObject, posTuileObjectX, posTuileObjectY, direction)==0){
          (objects+waterCaisse)->destRect.x += 3;
          TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si une caisse est dans un trou, on arrête la glissade
          if(caisseInHoles(nbTrous, trous, nbObjects, &objects)==1){
            (objects+waterCaisse)->destRect.x += 3;
            break;
          }
        }
        (objects+waterCaisse)->destRect.x -= 3;   // On redéplace la caisse à la fin de la glissade

      } else if(direction == 4){     // Si la caisse se déplace vers le bas
        (objects+waterCaisse)->destRect.y += 3;   // On déplace la caisse en bas
        // On regarde où la caisse est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);
        // Tant que la caisse ne rencontre pas un obstacle, on la déplace vers le bas et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, (objects+waterCaisse), TuileObject, posTuileObjectX, posTuileObjectY, direction)==0){
          (objects+waterCaisse)->destRect.y += 3;
          TuileObject = getTuileObject(width, height, Tilemap, (objects+waterCaisse), &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si une caisse est dans un trou, on arrête la glissade
          if(caisseInHoles(nbTrous, trous, nbObjects, &objects)==1){
            (objects+waterCaisse)->destRect.y += 3;
            break;
          }
        }
        (objects+waterCaisse)->destRect.y -= 3;   // On redéplace la caisse à la fin de la glissade
      }
    }

    if(playerInWater(nbWater, water, &player) == 1){    // S'il existe une caisse en collision avec une flaque d'eau
      if(direction == 1){     // Si le joueur se déplace vers le haut
        player.destRect.y -= 3;   // On déplace le joueur vers le haut
        // On regarde où le joueur est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);
        // Tant que le joueur ne rencontre pas un obstacle, on le déplace vers le haut et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, &player, TuileObject, posTuileObjectX, posTuileObjectY, direction)==0){
          player.destRect.y -= 3;
          TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si un joueur est tombé dans le trou pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInHoles(nbTrous, trous, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          // Si un joueur est dans le feu pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInFire(nbFire, fire, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          if(mouvementCaisse(nbTextures, tiles, nbObjects, &objects, &player, direction, width, height, Tilemap)==1){
            break;
          }
        }
        player.destRect.y += 3;   // On redéplace le joueur à la fin de la glissade

      } else if(direction == 2){     // Si le joueur se déplace vers la gauche
        player.destRect.x -= 3;   // On déplace le joueur vers la gauche
        // On regarde où le joueur est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);
        // Tant que le joueur ne rencontre pas un obstacle, on le déplace vers la gauche et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, &player, TuileObject, posTuileObjectX, posTuileObjectY, direction)== 0){
          player.destRect.x -= 3;
          TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si un joueur est tombé dans le trou pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInHoles(nbTrous, trous, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          // Si un joueur est dans le feu pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInFire(nbFire, fire, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          if(mouvementCaisse(nbTextures, tiles, nbObjects, &objects, &player, direction, width, height, Tilemap)==1){
            break;
          }
        }
        player.destRect.x += 3;   // On redéplace le joueur à la fin de la glissade

      } else if(direction == 3){     // Si le joueur se déplace vers le bas
        player.destRect.x += 3;     // On déplace le joueur vers la droite
        // On regarde où le joueur est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);
        // Tant que le joueur ne rencontre pas un obstacle, on le déplace vers la droite et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, &player, TuileObject, posTuileObjectX, posTuileObjectY, direction)== 0){
          player.destRect.x += 3;

          TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si un joueur est tombé dans le trou pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInHoles(nbTrous, trous, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          // Si un joueur est dans le feu pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInFire(nbFire, fire, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          if(mouvementCaisse(nbTextures, tiles, nbObjects, &objects, &player, direction, width, height, Tilemap)==1){
            break;
          }
        }
        player.destRect.x -= 3;   // On redéplace le joueur à la fin de la glissade

      } else if(direction == 4){     // Si le joueur se déplace vers le bas
        player.destRect.y += 3;     // On déplace le joueur vers le bas
        // On regarde où le joueur est sur le plateau
        TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);
        // Tant que le joueur ne rencontre pas un obstacle, on le déplace vers le bas et on recalcule une collision
        while(CollisionObject(nbTextures, tiles, &player, TuileObject, posTuileObjectX, posTuileObjectY, direction)== 0){
          player.destRect.y += 3;
          TuileObject = getTuileObject(width, height, Tilemap, &player, &posTuileObjectX, &posTuileObjectY);

          // On affiche la carte
          if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }
          // On affiche le chronomètre
          if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
            isRunning = SDL_FALSE;
            break;
          }

          SDL_RenderPresent(renderer);
          SDL_Delay(20);

          // Si un joueur est tombé dans le trou pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInHoles(nbTrous, trous, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          // Si un joueur est dans le feu pendant une glissade, on arrête le jeu: GAMEOVER
          if(playerInFire(nbFire, fire, &player) == 1){
            isRunning = SDL_FALSE;
            break;
          }
          if(mouvementCaisse(nbTextures, tiles, nbObjects, &objects, &player, direction, width, height, Tilemap)==1){
            break;
          }

        }
        player.destRect.y -= 3;   // On redéplace le joueur à la fin de la glissade

      }
    }
    // On affiche la carte
    if(afficheMap(width, height, Tilemap, renderer, nbObjects, nbTextures, tiles, objects, &player, LARGEUR_FENETRE, HAUTEUR_FENETRE) == EXIT_FAILURE){
      isRunning = SDL_FALSE;
      break;
    }
    // On affiche le chronomètre
    if(afficheTimer(policeTimer, colorTimer, renderer, timer) == EXIT_FAILURE){
      isRunning = SDL_FALSE;
      break;
    }
    // Si un joueur est tombé dans le trou, le jeu s'arrête
    if(playerInHoles(nbTrous, trous, &player) == 1){
      isRunning = SDL_FALSE;
      break;
    }

    // Si un joueur est tombé dans le feu, le jeu s'arrête
    if(playerInFire(nbFire, fire, &player) == 1){
      isRunning = SDL_FALSE;
      break;
    }

    // On récupère l'indice du joueur de la tuile où le joueur est
    indiceWin = getTuileObject(width, height, Tilemap, &player , &posTuileObjectX, &posTuileObjectY);
    // Si l'indice est égal à la sortie droite, on test si le joueur est
    if(indiceWin == getIndiceByPath(nbTextures, tiles, "../images/sortie_droite.png")){
      // Si le joueur est dans la sortie, fin de partie et win passe à 1
      if(playerWin(&player, posTuileObjectX, posTuileObjectY) == 1){
        isRunning = SDL_FALSE;
        win = 1;
        break;
      }
    }
    // Gestion du chronomètre
    chrono++;   // si le compteur arrive à 40, on retire une seconde
    if(chrono == 45){
      if(changeTime(timer) == 1){
        isRunning = SDL_FALSE;  // Si le chronomètre arrive à 0, le jeu s'arrête
        break;
      }
      chrono = 0;
    }
    // On actualise le rendu
    SDL_RenderPresent(renderer);
    // On fait un délai pour réduire les framerate
    SDL_Delay(20);
  }

  // Si le joueur a perdu, on affiche le texte perdu
  if(!win){
    SDL_RenderCopy(renderer, texte_G, NULL, &positionGameover);
  } else {
  // Sinon on affiche le texte victoire
    SDL_RenderCopy(renderer, texte_V, NULL, &positionVictoire);
  }
  SDL_RenderPresent(renderer);
  // On attend 5 secondes
  SDL_Delay(5000);


  // On détruit les éléments alloués grâce à la fonction clearRessources et on quitte la SDL et la librairie TTF
  TTF_CloseFont(police);
  TTF_CloseFont(policeTimer);
  SDL_DestroyTexture(texte_G);
  SDL_DestroyTexture(texte_V);
  clearRessources(tiles, renderer, window, objects, &player, nbTextures, nbObjects);
  // Si le pointeur du tableau dynamique de textures est différent de NULL
  // On libère la mémoire
  free(tiles);
  // Si le pointeur du tableau dynamique de GameObject est différent de NULL
  // On libère la mémoire
  free(objects);
  // On quitte TTF
  IMG_Quit();
  // On quitte la SDL
  SDL_Quit();
}
