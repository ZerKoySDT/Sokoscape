#ifndef NIVEAU_H
#define NIVEAU_H

typedef struct GameObject{
  SDL_Texture* texture;
  SDL_Rect destRect;
  SDL_bool hasCollision;
}GameObject;

typedef struct StaticObject{
  SDL_Texture* texture;
  char* path;
  int indice;
}StaticObject;

// Prend en paramètre un tableau dynamique de textures ainsi que le nombre de texture et détruit toutes les textures du tableau
void destroyTextures(StaticObject* tiles, int nbTextures);

// Prend en paramètre le nombre d'objets ainsi qu'un tableau dynamique de GameObject et détruit tous les GameObject du tableau.
void destroyObjects(int nbObjects, GameObject objets[nbObjects]);

// Prend en paramètre un pointeur sur un GameObject pour le joueur et détruit sa texture.
void destroyPlayer(GameObject* player);

// Prend en paramètre tout ce qu'on a alloué dynamiquement, ainsi que le nombre de textures et d'objets, afin de tout détruire et quitter la SDL ensuite.
void clearRessources(StaticObject* tiles, SDL_Renderer* renderer, SDL_Window* window, GameObject* objects, GameObject* player, int nbTextures, int nbObjects);

// Prend en paramètre un fichier, un pointeur sur un entier correspondant au nombre de tuiles en largeur et un pointeur sur un entier correspondant au nombre de tuiles en hauteur
// Cette fonction récupère la largeur et la hauteur dans un fichier
void getWidthHeight(FILE* fichier, int* width, int* height);

// Prend en paramètre un fichier, un tableau dynamique de textures, ainsi qu'un rendu et un pointeur sur un entier qui va correspondre au nombre de textures dans le jeu.
// Cette fonction charge en mémoire les textures grâce à notre fichier
// Cette fonction renvoie le pointeur sur ce tableau dynamique
// Renvoie NULL en cas d'échec
StaticObject* loadTextures(FILE* fichier, StaticObject* tiles, SDL_Renderer* renderer, int* nbTextures);

// Prend en paramètre un fichier, le nombre de tuiles en largeur et en hauteur, et un tableau 2 dimensions de type "short int" de height lignes et width colonnes.
// Cette fonction charge dans le tableau à 2 dimensions le canvas dans le fichier
void loadTilemap(FILE* fichier, int width, int height, short int Tilemap[height][width]);

// Prend en paramètre un fichier, et un pointeur sur un entier qui va correspondre au nombre d'objets dans notre fichier.
// Cette fonction trouve le nombre d'objets à charger en mémoire (OPTIONNEL)
void getNumberObjects(FILE* fichier, int* nbObjects);

// Prend en paramètre un fichier, un tableau dynamique de GameObject ainsi qu'un rendu, et renvoie le tableau alloué dynamiquement de GameObject
// Cette fonction charge en mémoire les objets du texte
// Cette fonction renvoie le pointeur sur le tableau dynamique de GameObject
// Cette fonction renvoie NULL si ça s'est mal passé
GameObject* loadObjects(FILE* fichier, GameObject* objects, SDL_Renderer* renderer);

// Prend en paramètre un fichier, un pointeur sur un GameObject ainsi qu'un rendu
// Renvoie EXIT_SUCCESS si tout s'est bien passé
// Renvoie EXIT_FAILURE en cas d'échec
int loadJoueur(FILE* fichier, GameObject* player, SDL_Renderer* renderer);

// Prend en paramètre un identifiant de la texture pour le trou, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// renvoie le nombre de trous sur la carte
int getNumberHoles(int idTextureTrou, int width, int height, short int Tilemap[height][width]);

// Prend en paramètre un identifiant de la texture pour le trou, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// également le nombre de trous et un tableau de rectangle de trous
// Modifie le tableau de rectangle pour stocker les emplacements des trous
void loadHoles(int idTextureTrou, int width, int height, short int Tilemap[height][width], int nbTrous, SDL_Rect trous[nbTrous]);

// Prend en paramètre un identifiant de la texture pour le feu, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// renvoie le nombre de feux sur la carte
int getNumberFire(int idTextureFire, int width, int height, short int Tilemap[height][width]);

// Prend en paramètre un identifiant de la texture pour le trou, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// également le nombre de feux et un tableau de rectangle de feux
// Modifie le tableau de rectangle pour stocker les emplacements des trous
void loadFire(int idTextureFire, int width, int height, short int Tilemap[height][width], int nbFire, SDL_Rect fire[nbFire]);

// Prend en paramètre un identifiant de la texture pour la flaque, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// renvoie le nombre de flaques sur la carte
int getNumberWater(int idTextureWater, int width, int height, short int Tilemap[height][width]);

// Prend en paramètre un identifiant de la texture pour la flaque, la largeur et la hauteur du tableau 2D pour la carte, et le tableau 2D
// également le nombre de trous et un tableau de rectangle de flaque
// Modifie le tableau de rectangle pour stocker les emplacements des flaques
void loadWater(int idTextureWater, int width, int height, short int Tilemap[height][width], int nbWater, SDL_Rect water[nbWater]);


#endif
