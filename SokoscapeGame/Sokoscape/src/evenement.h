#ifndef EVENEMENT_H
#define EVENEMENT_H

// Prend en paramètre un nombre de textures, le tableau de tuiles, et un chemin
// Renvoie l'indice de la texture si elle est trouvée parmi le tableau
// Renvoie -1 sinon
int getIndiceByPath(int nbTexture, StaticObject* tuiles, const char* path);

// Prend en paramètre la largeur, la hauteur du niveau, le tableau 2D pour le niveau, un pointeur sur le joueur et deux pointeurs sur les entiers.
// Renvoie la valeur de la tuile dans le tableau 2D où se situe le joueur
// Modifie les deux derniers paramètres pour récupérer les positions en x et y de cette tuile
short int getTuileObject(int width, int height, short int Tilemap[height][width], GameObject* player, int* posTuileObjectX, int* posTuileObjectY);

// Prend en paramètre un nombre de textures, le tableau des tuiles, un pointeur sur un objet, la tuile où se situe l'objet après les déplacements
// les positions en X et Y de cette tuile, ainsi que la direction par laquelle il s'est déplacé
// Renvoie 1 si l'objet a eu une collision
// Renvoie 0 sinon
int CollisionObject(int nbTexture, StaticObject* tuiles, GameObject* object, short int TuileObject, int posTuileObjectX, int posTuileObjectY, int direction);

// Prend en paramètres le nombre de textures, la liste des tuiles, un nombre d'objets, un pointeur sur le tableau de ces objets, un pointeur sur le joueur, une direction,
// la largeur et la hauteur du niveau, ainsi que le tableau 2D des tuiles du niveau
// La fonction fait bouger les caisses qui sont en collision avec le joueur
// Elle gère également les collisions des caisses grâce à CollisionObject
int mouvementCaisse(int nbTexture, StaticObject* tuiles, int nbObjects, GameObject** objects, GameObject* player, int direction, int width, int height, short int Tilemap[height][width]);

// Prend en paramètres un nombre de trous, un tableau de rectangle de ces trous, et un pointeur sur le tableau d'objet
// La fonction gère également la suppression de la collision de la caisse, ainsi que son déplacement dans le trou
// Renvoie 1 si une caisse est dans un trou
// Renvoie 0 si aucune caisse est dans aucun trou
int caisseInHoles(int nbTrous, SDL_Rect trous[nbTrous], int nbObjects, GameObject** objects);

// Prend en paramètres un nombre de trous, un tableau de rectangle de ces trous, et un pointeur sur le joueur
// Renvoie 1 si le joueur est dans un trou (GAMEOVER)
// Renvoie 0 si le joueur est dans aucun trou
int playerInHoles(int nbTrous, SDL_Rect trous[nbTrous], GameObject* player);

// Prend en paramètres un nombre de feux, un tableau de rectangle de ces feux, et un pointeur sur le tableau d'objet
// La fonction gère également la "suppression" de la caisse si elle est dans un feu
// Renvoie 1 si une caisse est dans un feu
// Renvoie 0 si aucune caisse est dans aucun feu
int caisseInFire(int nbFire, SDL_Rect fire[nbFire], int nbObjects, GameObject** objects);

// Prend en paramètres un nombre de feux, un tableau de rectangle de ces feux, et un pointeur sur le joueur
// Renvoie 1 si le joueur est dans un feu (GAMEOVER)
// Renvoie 0 si le joueur est dans aucun feux
int playerInFire(int nbFire, SDL_Rect fire[nbFire], GameObject* player);

// Prend en paramètres un pointeur sur un joueur, ainsi que les positions en X et Y
// Renvoie 1 si le joueur est dans la sortie
// Renvoie 0 sinon
int playerWin(GameObject* player, int posTuileObjectX, int posTuileObjectY);

// Prend en paramètres un nombre de flaques, un tableau de rectangles pour les positions des flaques, un nombre d'objet, un pointeur sur un tableau d'objets
// Renvoie l'indice de la caisse dans le tableau si cette caisse est dans une flaque
// Renvoie 0 sinon
int caisseInWater(int nbWater, SDL_Rect water[nbWater], int nbObjects, GameObject** objects);

// Prend en paramètres un nombre de flaques, un tableau de rectangles pour les positions des flaques et un pointeur sur le joueur
// Renvoie 1 si le joueur est en collision avec une flaque
// Renvoie 0 sinon
int playerInWater(int nbWater, SDL_Rect water[nbWater], GameObject* player);

// Prend en paramètres un événement, un nombre d'objets, un pointeur sur le tableau d'objets, un pointeur sur le joueur, la largeur et la hauteur du niveau
// Le tableau 2D pour gérer le niveau, le tableau des textures ainsi qu'un nombre de textures
// Cette fonction renvoie -1 si l'événement est "fermer la fenêtre"
// Renvoie 1 si la direction est vers le hauteur
// Renvoie 2 si la direction est vers la gauche
// Renvoie 3 si la direction est vers la droite
// Renvoie 4 si la direction est vers le bas
// Cette fonction gère les déplacement du joueur des caisses et fait appel aux fonctions des collisions
int getToucheClavier(SDL_Event event, int nbObjects, GameObject** objects, GameObject* player, int width, int height, short int Tilemap[height][width], StaticObject* tuiles, int nbTexture);

#endif
