#ifndef AFFICHAGELEVEL_H
#define AFFICHAGELEVEL_H



// Prend en paramètre un tableau dynamique de textures, un entier width pour le nombre de tiles en largeur, un entier height pour le nombre de tiles en hauteur, un tableau à 2 dimensions pour la map, un rendu, et la taille de la fenêtre en largeur et en hauteur
// Renvoie EXIT_SUCCESS si la fonction s'est bien passée
// Renvoie EXIT_FAILURE si la fonction s'est mal passée
int afficheLevel(StaticObject* tiles, int width, int height, short int Tilemap[height][width], SDL_Renderer* renderer, int largeurFenetre, int hauteurFenetre);

// Prend en paramètre un entier width pour le nombre de tiles en largeur, un entier height pour le nombre de tiles en hauteur, un nombre d'objets, un tableau dynamique de GameObject, et un rendu
// Renvoie EXIT_SUCCESS si la fonction s'est bien passée
// Renvoie EXIT_FAILURE si la fonction s'est mal passée
int afficheObject(int width, int height,int nbObjects, GameObject* objects, SDL_Renderer* renderer);

// Prend en paramètre un entier width pour le nombre de tiles en largeur, un entier height pour le nombre de tiles en hauteur, un nombre d'objets, un pointeur sur un GameObject pour le joueur, et un rendu
// Cette fonction dessine sur le rendu la texture du joueur
// Renvoie EXIT_SUCCESS si la fonction s'est bien passée
// Renvoie EXIT_FAILURE si la fonction s'est mal passée
int afficheJoueur(int width, int height, GameObject* player, SDL_Renderer* renderer);

// Prend en paramètre une police, une couleur, un rendu et la chaine de caractères du chronomètre
// Cette fonction affiche le chronomètre
// Renvoie EXIT_SUCCESS si la fonction s'est bien passée
// Renvoie EXIT_FAILURE si la fonction s'est bien passée
int afficheTimer(TTF_Font* policeTimer, SDL_Color colorTimer, SDL_Renderer* renderer, char timer[6]);

// Prend en paramètre la largeur la hauteur, le tableau 2D pour construire le niveau, le renderer, le nombre d'objet, le nombre de textures
// Le tableau de textures, le tableau d'objets, un pointeur sur le joueur, la largeur et la hauteur de la fenêtre
// Renvoie EXIT_SUCCESS si la fonction s'est bien passée
// Renvoie EXIT_FAILURE si la fonction s'est mal passée
int afficheMap(int width, int height, short int Tilemap[height][width], SDL_Renderer* renderer, int nbObjects, int nbTexture, StaticObject* tiles, GameObject* objects, GameObject* player, int largeurFenetre, int hauteurFenetre);


#endif
