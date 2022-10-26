#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include "niveau.h"

void destroyTextures(StaticObject* tiles, int nbTextures){
  // On parcourt le tableau dynamique de textures
  for(int i = 0; i < nbTextures; i++){
    if((tiles+i)->texture != NULL){
      // Si la texture n'est pas NULL, on la détruit
      SDL_DestroyTexture((tiles+i)->texture);
      free((tiles+i)->path);
    }
  }
}

void destroyObjects(int nbObjects, GameObject* objets){
  // On parcourt le tableau dynamique de textures
  for(int i = 0; i < nbObjects; i++){
    if((objets+i)->texture != NULL){
      // Si la texture n'est pas NULL, on la détruit
      SDL_DestroyTexture((objets+i)->texture);
    }
  }
}

void destroyPlayer(GameObject* player){
  if(player->texture != NULL){
    // Si la texture du joueur n'est pas NULL, on la détruit
    SDL_DestroyTexture(player->texture);
  }
}

void clearRessources(StaticObject* tiles, SDL_Renderer* renderer, SDL_Window* window, GameObject* objects, GameObject* player, int nbTextures, int nbObjects){
  // On détruit les textures
  destroyTextures(tiles, nbTextures);
  // On détruit les GameObjects
  destroyObjects(nbObjects, objects);
  // On détruit le joueur
  destroyPlayer(player);
  // On détruit le rendu
  SDL_DestroyRenderer(renderer);
  // On détruit la fenêtre
  SDL_DestroyWindow(window);
}

void getWidthHeight(FILE* fichier, int* width, int* height){
  int w, h;                               // Variables temporaires pour stocker la largeur et la hauteur dans le fichier
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier

  fgets(ligne,50,fichier);                // On lit le commentaire en début de fichier
  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }
  sscanf(ligne, "LARGEUR:%d\n", &w);      // On récupère la largeur dans le fichier
  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }
  sscanf(ligne, "HAUTEUR:%d\n", &h);      // On récupère la hauteur dans le fichier
  *width = w;                             // On affecte à la variable en paramètre la bonne largeur
  *height = h;                            // On affecte à la variable en paramètre la bonne hauteur
}

StaticObject* loadTextures(FILE* fichier, StaticObject* tiles, SDL_Renderer* renderer, int* nbTextures){
  char* stockChemin;
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier
  char chemin[50];                        // Chaine de caractère pour stocker le chemin de la tuile à charger
  int cpt = 1;                            // Compteur du nombre de cases mémoires à allouer
  int indice;                             // Variable pour l'indice du tableau où placer la prochaine texture (lu dans le fichier)

  // Allocation dynamique d'une case mémoire pour une texture
  tiles = (StaticObject*)malloc(1*sizeof(StaticObject));

  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne alors
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }
  // ligne équivaut à la première tuile.
  // Tant que la ligne n'est pas un \n, c'est à dire qu'on a encore des textures à charger
  while(strcmp(ligne, "\n") != 0){
    sscanf(ligne, "TUILE:%d,%s\n", &indice, chemin); // On récupère l'indice de la tuile, ainsi que le chemin

    // On charge la texture dans la case mémoire d'indice trouvée dans le fichier
    (tiles+indice)->texture = IMG_LoadTexture(renderer, chemin);
    if((tiles+indice)->texture == NULL){
      // S'il y a une erreur, on quitte la fonction on sort
      SDL_Log("Erreur chargement textures > %s", SDL_GetError());
      return NULL;
    }
    stockChemin = chemin;
    (tiles+indice)->path = (char*)malloc(sizeof(char)*100);   // On alloue dynamiquement le chemin dans la structure
    (tiles+indice)->path = memcpy((tiles+indice)->path, chemin, 99);    // Puis on copie le chemin
    (tiles+indice)->indice = indice;
    // On incrémente le compteur du nombre de cases mémoires à allouer
    cpt++;
    // On réalloue cpt cases mémoires au total dans le pointeur de la taille de texture
    tiles = (StaticObject*)realloc(tiles, cpt*sizeof(StaticObject));
    // On lit la prochaine ligne
    fgets(ligne, 50, fichier);
  }
  (tiles+(cpt-1))->texture = NULL;
  (tiles+(cpt-1))->path = NULL;
  *nbTextures = cpt;    // On affecte à la variable en paramètre le bon nombre de textures

  return tiles;   // On retourne le tableau dynamique de textures
}

void loadTilemap(FILE* fichier, int width, int height, short int Tilemap[height][width]){
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier
  char* val;                              // pointeur pour stocker chaque caractère (chiffre) du fichier
  int x;                                  // variable pour stocker la valeur à mettre dans le tableau

  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"TAB:\n") != 0){     // Tant que la ligne est égale à \n, donc on est sur un saut de ligne alors
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }
  // Ligne vaut "TAB:\n"
  // On parcourt le tableau 2D du fichier
  for(int i = 0; i < height; i++){
    fgets(ligne,50,fichier);              // On lit la prochaine ligne de chiffres
    val=strtok(ligne, " ");               // On sépare le premier chiffre avec les espaces et on stocke dans val le premier "chiffre"
    for(int j = 0; j < width; j++){
      if(strcmp(val, "\n") != 0){         // Si le chiffre n'est pas un "\n"
        sscanf(val, "%d", &x);            // On transforme le caractère en chiffre
        Tilemap[i][j] = x;                // On affecte le chiffre au tableau passé en paramètre
      }
      val=strtok(NULL, " ");              // On sépare le prochain chiffre
    }
  }
}

void getNumberObjects(FILE* fichier, int* nbObjects){
  int nb;                                 // Entier qui stock le nombre d'objets final
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier
  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }
  // Ligne vaut "NOBJETS:X\n"
  sscanf(ligne,"NOBJETS:%d\n", &nb);      // On récupère dans la variable nb le nombre d'objets dans la ligne
  *nbObjects = nb;                        // On affecte à la variable en paramètre le nombre d'objets
}

GameObject* loadObjects(FILE* fichier, GameObject* objects, SDL_Renderer* renderer){
  int x, y, indice;                       // Entier qui stock les coordonnées x y d'un objet, et son indice dans le tableau de GameObject
  int cpt = 1;                            // Compteur qui compte le nombre de cases allouées dans le tableau de GameObject
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier
  char path[50];                          // Chaine de caractère pour stocker le chemin de la texture
  SDL_Rect rectangle = {0,0,46,46};
  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }

  objects = (GameObject*)malloc(sizeof(GameObject)*1);
  while(strcmp(ligne,"\n") != 0){

    sscanf(ligne, "OBJET:%d,%[^,],%d,%d\n", &indice, path, &x, &y);   // On prend dans indice, la valeur de l'indice dans le tableau
                                                                      // Dans [^,] on prend le chemin (Utilisation des expressions régulières)
                                                                      // Pour éviter de croiser une virgule, sinon on prend même les chiffres dans la chaîne
                                                                      // dans x, on met la coordonnée x de l'objet, pareil dans y
    (objects+indice)->texture = IMG_LoadTexture(renderer, path);      // On charge la texture dans le champ texture de l'objet à l'index du tableau "indice"
    if((objects+indice)->texture == NULL){
      // Si la texture ne se charge pas, on quitte la fonction et la SDL
      SDL_Log("Erreur : Chargement texture objet n°%d > %s", indice, SDL_GetError());
      return NULL;
    }
    // On stock dans les champ posX et posY, les coordonnées X et Y récupérées précédemment
    rectangle.x = x;
    rectangle.y = y;
    (objects+indice)->destRect = rectangle;   // On copie son rectangle de position
    (objects+indice)->hasCollision = SDL_TRUE;    // On met la collision à true pour l'objet
    // On incrémente le compteur du nombre de cases allouées dans le tableau de GameObject
    cpt++;
    // On réalloue la mémoire du pointeur pour pouvoir stocker un nouvel objet
    objects = (GameObject*)realloc(objects, sizeof(GameObject)*cpt);
    fgets(ligne,50,fichier);  // On lit la nouvelle ligne
  }

  return objects;   // On renvoie le pointeur sur les GameObject
}

int loadJoueur(FILE* fichier, GameObject* player, SDL_Renderer* renderer){
  int x, y;                               // Variable x y pour les positions du joueur
  char ligne[50];                         // Chaine de caractère pour stocker la ligne actuelle du fichier
  char chemin[50];                        // Chaine de caractère pour stocker le chemin de la texture du joueur
  SDL_Rect rectangle = {0,0,46,46};

  fgets(ligne,50,fichier);                // On lit la prochaine ligne
  while(strcmp(ligne,"\n") == 0){         // Tant que la ligne est égale à \n, donc on est sur un saut de ligne
    fgets(ligne, 50, fichier);            // On capture la prochaine ligne
  }

  sscanf(ligne, "JOUEUR:%[^,],%d,%d\n", chemin, &x, &y);  // On prend dans indice, la valeur de l'indice dans le tableau
                                                          // Dans [^,] on prend le chemin (Utilisation des expressions régulières)
                                                          // Pour éviter de croiser une virgule, sinon on prend même les chiffres dans la chaîne
                                                          // dans x, on met la coordonnée x de l'objet, pareil dans y
  player->texture = IMG_LoadTexture(renderer, chemin);  // On affecte au champ texture du GameObject du joueur la texture dont le chemin est stocké dans "chemin"
  if(player->texture == NULL){
    // Si le chargement de la texture a échoué, on sort de la fonction et on sort de la SDL
    SDL_Log("Erreur chargement joueur > %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  // On affecte aux champs posX et posY du GameObject du joueur, les positions x y récupérées précédemment
  rectangle.x = x;
  rectangle.y = y;
  player->destRect = rectangle;
  player->hasCollision = SDL_TRUE;
  return EXIT_SUCCESS;
}

int getNumberHoles(int idTextureTrou, int width, int height, short int Tilemap[height][width]){
  int nbTrous = 0;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureTrou){     // Si la tuile qu'on parcourt est de type "trou", on incrémente cpt
        nbTrous++;
      }
    }
  }
  return nbTrous;   // Renvoie le nombre de trous du plateau de jeu
}

void loadHoles(int idTextureTrou, int width, int height, short int Tilemap[height][width], int nbTrous, SDL_Rect trous[nbTrous]){
  int cpt = 0;
  SDL_Rect rectangleTilemap = {0, 0, 64, 64};
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureTrou){ // Si la tuile qu'on parcourt est de type "trou"
        trous[cpt] = rectangleTilemap;    // On stock son rectangle
        cpt++;
      }
      rectangleTilemap.x += 64;       // On décale notre rectangle en x pour la colonne
    }
    rectangleTilemap.x = 0;           // A chaque fin de ligne on remet x à 0
    rectangleTilemap.y += 64;         // A chaque fin de ligne on incrémente y de 64;
  }
}

int getNumberFire(int idTextureFire, int width, int height, short int Tilemap[height][width]){
  int nbFire = 0;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureFire){     // Si la tuile qu'on parcourt est de type "feu", on incrémente cpt
        nbFire++;
      }
    }
  }
  return nbFire;
}

void loadFire(int idTextureFire, int width, int height, short int Tilemap[height][width], int nbFire, SDL_Rect fire[nbFire]){
  int cpt = 0;
  SDL_Rect rectangleTilemap = {0, 0, 64, 64};
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureFire){ // Si la tuile qu'on parcourt est de type "feu"
        fire[cpt] = rectangleTilemap;     // On stock son rectangle
        cpt++;
      }
      rectangleTilemap.x += 64;
    }
    rectangleTilemap.x = 0;
    rectangleTilemap.y += 64;
  }
}

int getNumberWater(int idTextureWater, int width, int height, short int Tilemap[height][width]){
  int nbWater = 0;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureWater){       // Si la tuile qu'on parcourt est de type "flaque", on incrémente cpt
        nbWater++;
      }
    }
  }
  return nbWater;   // Renvoie le nombre de flaques du plateau de jeu
}

void loadWater(int idTextureWater, int width, int height, short int Tilemap[height][width], int nbWater, SDL_Rect water[nbWater]){
  int cpt = 0;
  SDL_Rect rectangleTilemap = {0, 0, 64, 64};
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(Tilemap[i][j] == idTextureWater){    // Si la tuile qu'on parcourt est de type "flaque"
        water[cpt] = rectangleTilemap;        // On stock son rectangle
        cpt++;
      }
      rectangleTilemap.x += 64;
    }
    rectangleTilemap.x = 0;
    rectangleTilemap.y += 64;
  }
}
