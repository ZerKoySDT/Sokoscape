#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "niveau.h"
#include "evenement.h"

#define OFFSET 11

int getIndiceByPath(int nbTexture, StaticObject* tuiles, const char* path){
	for(int i = 0; i < nbTexture-1; i++){
		if(strcmp((tuiles+i)->path,path) == 0){ 	// Si le chemin de tuiles[i] est égal au chemin en paramètre, renvoie l'indice de la texture
			return i;
		}
	}
	return -1;
}

short int getTuileObject(int width, int height, short int Tilemap[height][width],GameObject* object, int* posTuileObjectX, int* posTuileObjectY){
	SDL_Rect objectCopy = object->destRect;		// Stockage du rectangle de l'objet
	objectCopy.x += OFFSET*2;			// Réajustement de la hitbox de l'objet
	objectCopy.y += OFFSET;
	SDL_Rect TilemapPos = {0,0,64,64};		// Rectangle de placement dans le tableau Tilemap
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			// Si l'objet est dans la tuile Tilemap[i][j] alors on modifie les pointeurs en paramètre et on retourne cet indice.
			if((objectCopy.x >= TilemapPos.x) && objectCopy.x <= (TilemapPos.x + TilemapPos.w) && (objectCopy.y >= TilemapPos.y) && (objectCopy.y <= (TilemapPos.y + TilemapPos.h))){
					*posTuileObjectX = TilemapPos.x;
					*posTuileObjectY = TilemapPos.y;
					return Tilemap[i][j];
			}
			TilemapPos.x += 64;
		}
		TilemapPos.y +=64;
		TilemapPos.x = 0;
	}
	return -1;
}

int CollisionObject(int nbTexture, StaticObject* tuiles, GameObject* object, short int TuileObject, int posTuileObjectX, int posTuileObjectY, int direction){
	if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_haut.png")){		// Si le joueur est sur un mur du haut
		object->destRect.y += OFFSET;			// On annule son déplacement
		return 1;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_gauche.png")){		// Si le joueur est sur un mur de gauche
		object->destRect.x += OFFSET;
		return 1;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_droit.png")){		// Si le joueur est sur un mur de droite
		object->destRect.x -= OFFSET;
		return 1;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_bas.png")){		// Si le joueur est sur un mur du bas
		object->destRect.y -= OFFSET;
		return 1;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_hb.png")){			// Si le joueur est sur un mur vertical
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 1 && object->destRect.x+5 > posTuileObjectX && object->destRect.x < posTuileObjectX+30){
			object->destRect.y += OFFSET;
			return 1;
		} else if(direction == 2 && object->destRect.x+OFFSET > posTuileObjectX+20){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 3 && object->destRect.x+OFFSET > posTuileObjectX){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 4 && object->destRect.x+5 > posTuileObjectX && object->destRect.x < posTuileObjectX+30){
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_gd.png")){	 		// Si le joueur est sur un mur horizontal
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 1 && (object->destRect.y < posTuileObjectY+32)){
			object->destRect.y += OFFSET;
			return 1;
		} else if(direction == 2 && object->destRect.y+5 > posTuileObjectY && object->destRect.y < posTuileObjectY+30){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 3 && object->destRect.y+5 > posTuileObjectY && object->destRect.y < posTuileObjectY+30){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 4 && (object->destRect.y < posTuileObjectY+16)){
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_angle_db.png")){		// Si le joueur est sur un mur angle db
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 2 && (object->destRect.y > posTuileObjectY+16)){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 3 && (object->destRect.y+OFFSET > posTuileObjectY+16)){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 4 && (object->destRect.x+16 > posTuileObjectX+OFFSET) && (object->destRect.y+OFFSET > posTuileObjectY+16)){
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_angle_dh.png")){				// Si le joueur est sur un mur angle dh
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 1 && (object->destRect.x > posTuileObjectX-5) && (object->destRect.y < posTuileObjectY+32)){
			object->destRect.y += OFFSET;
			return 1;
		} else if(direction == 2 && (object->destRect.x > posTuileObjectX+25) && (object->destRect.y < posTuileObjectY + 25)){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 3 && (object->destRect.y < posTuileObjectY+25)){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 4 && (object->destRect.y < posTuileObjectY+25) && (object->destRect.x > posTuileObjectX+25)){
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_angle_gb.png")){				// Si le joueur est sur un mur angle gb
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 2 && (object->destRect.y > posTuileObjectY-5) && (object->destRect.x+16 > posTuileObjectX+16)){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 4 && (object->destRect.x < posTuileObjectX + 36) && (object->destRect.y+OFFSET > posTuileObjectY + 16)){
			object->destRect.y -= OFFSET;
			return 1;
		} else if(direction == 3 && (object->destRect.y > posTuileObjectY +16) && (object->destRect.x+16 > posTuileObjectX + 10)){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 1 && (object->destRect.y > posTuileObjectY + 16) && (object->destRect.x < posTuileObjectX+25)){
			object->destRect.y += OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mur_angle_gh.png")){		   		// Si le joueur est sur un mur angle gh
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 1 && (object->destRect.x < posTuileObjectX + 32)){
			object->destRect.y += OFFSET;
			return 1;
		} else if(direction == 2 && (object->destRect.y < posTuileObjectY + 32) && (object->destRect.x < posTuileObjectX + 32)){
			object->destRect.x += OFFSET;
			return 1;
		} else if(direction == 3 && (object->destRect.y < posTuileObjectY + 32) && (object->destRect.x < posTuileObjectX + 25)){
			object->destRect.x -= OFFSET;
			return 1;
		} else if(direction == 4 && (object->destRect.x < posTuileObjectX + 25) && (object->destRect.y < posTuileObjectY + 25)) {
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mid_bas.png")){						// Si le joueur est sur un mur mid_bas
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 4){
			object->destRect.y -= OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mid_haut.png")){				    // Si le joueur est sur un mur mid_haut
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 1){
			object->destRect.y += OFFSET;
			return 1;
		}
		return 0;
	}  else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/mid_gauche.png")){				// Si le joueur est sur un mur mid_gauche
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 2 && (object->destRect.x < posTuileObjectX + 46)){
			object->destRect.x += OFFSET;
			return 1;
		}
		return 0;
	} else if(TuileObject == getIndiceByPath(nbTexture, tuiles, "../images/sortie_droite.png")){					// Si le joueur est sur la sortie_droite
		// Gestion des collisions par rapport aux directions de déplacement du joueur
		if(direction == 3 && (object->destRect.y+OFFSET < posTuileObjectY+ 12 || object->destRect.y > posTuileObjectY + OFFSET*2)){
			object->destRect.x -= OFFSET;
			return 1;
		}
		return 0;
	}
	return 0;
}

int mouvementCaisse(int nbTexture, StaticObject* tuiles, int nbObjects, GameObject** objects, GameObject* player, int direction, int width, int height, short int Tilemap[height][width]){
	SDL_Rect PlayerR, ObjetR;			// Les deux rectangles stockent les positions du joueur et des caisses
	short int TuileObject;			// Variable pour stocker sur quelle tuile se situe la caisse après son déplacaement
	int posTuileObjectX, posTuileObjectY;		// Variable pour stocker où se situe cette tuile
	PlayerR = player->destRect;		// On stock le rectangle du joueur

	// On parcourt les objets
	for(int i = 0; i < nbObjects; i++){
		// On stock le rectangle de l'objet dont on regarde les collisions pour simplifier les conditions
			ObjetR = (*objects+i)->destRect;

				// Les offset sont gérées par rapport à la taille des caisses et de la texture du player qui a du vide en dessous et au dessus mais pas sur les côtés.
				// On vérifie la direction du joueur, et on fait les déplacement en conséquence
				switch(direction){
					// Si le joueur va vers le haut
					case 1:
						// Si la caisse n'est pas dans un trou ou détruite
						if((*objects+i)->hasCollision == SDL_TRUE){
							// Si le joueur est dans la caisse
							if(PlayerR.x+OFFSET >= ObjetR.x && (PlayerR.x+OFFSET/2 <= ObjetR.x + ObjetR.w) && (PlayerR.y+OFFSET*2) > ObjetR.y && (PlayerR.y+OFFSET*2) < ObjetR.y + ObjetR.h){
								// On déplace la caisse
								(*objects+i)->destRect.y -= OFFSET;
								// Récupération de la tuile de là où se situe la caisse
								TuileObject = getTuileObject(width, height, Tilemap, (*objects+i), &posTuileObjectX, &posTuileObjectY);
								// Si la caisse est en collision, on annule le déplacement du joueur
								if(CollisionObject(nbTexture, tuiles, (*objects+i), TuileObject, posTuileObjectX, posTuileObjectY, direction) == 1){
									player->destRect.y += OFFSET;
									return 1;
								}
							}
						}
						break;
					// Si le joueur va vers la gauche
					case 2:
						// Si la caisse n'est pas dans un trou ou détruite
						if((*objects+i)->hasCollision == SDL_TRUE){
							// Si le joueur est dans la caisse
							if(PlayerR.x >= ObjetR.x && PlayerR.x <= ObjetR.x + ObjetR.w && PlayerR.y+OFFSET*2 < ObjetR.y + ObjetR.h && PlayerR.y+OFFSET*2 > ObjetR.y){
								// On déplace la caisse
								(*objects+i)->destRect.x -= OFFSET;
								// Récupération de la tuile de là où se situe la caisse
								TuileObject = getTuileObject(width, height, Tilemap, (*objects+i), &posTuileObjectX, &posTuileObjectY);
								// Si la caisse est en collision, on annule le déplacement du joueur
								if(CollisionObject(nbTexture, tuiles, (*objects+i), TuileObject, posTuileObjectX, posTuileObjectY, direction) == 1){
									player->destRect.x += OFFSET;
									return 1;
								}
							}
						}
						break;
					// Si le joueur va vers la droite
					case 3:
						// Si la caisse n'est pas dans un trou ou détruite
						if((*objects+i)->hasCollision == SDL_TRUE){
							// Si le joueur est dans la caisse
							if(PlayerR.x+PlayerR.w >= ObjetR.x  && PlayerR.x+OFFSET <= ObjetR.x + ObjetR.w && PlayerR.y+OFFSET*2 < ObjetR.y + ObjetR.h && PlayerR.y+OFFSET*2 > ObjetR.y){
								// On déplace la caisse
								(*objects+i)->destRect.x += OFFSET;
								// Récupération de la tuile de là où se situe la caisse
								TuileObject = getTuileObject(width, height, Tilemap, (*objects+i), &posTuileObjectX, &posTuileObjectY);
								// Si la caisse est en collision, on annule le déplacement du joueur
								if(CollisionObject(nbTexture, tuiles, (*objects+i), TuileObject, posTuileObjectX, posTuileObjectY, direction) == 1){
									player->destRect.x -= OFFSET;
									return 1;
								}
							}
						}
						break;
					// Si le joueur va vers le bas
					case 4:
						// Si la caisse n'est pas dans un trou ou détruite
						if((*objects+i)->hasCollision == SDL_TRUE){
							// Si le joueur est dans la caisse
							if(PlayerR.x+32 >= ObjetR.x && (PlayerR.x+OFFSET/2 <= ObjetR.x + ObjetR.w) && PlayerR.y+OFFSET*2>= ObjetR.y && PlayerR.y+OFFSET < ObjetR.y + ObjetR.h){
								// On déplace la caisse
								(*objects+i)->destRect.y += OFFSET;
								// Récupération de la tuile de là où se situe la caisse
								TuileObject = getTuileObject(width, height, Tilemap, (*objects+i), &posTuileObjectX, &posTuileObjectY);
								if(CollisionObject(nbTexture, tuiles, (*objects+i), TuileObject, posTuileObjectX, posTuileObjectY, direction) == 1){
									player->destRect.y -= OFFSET;
									return 1;
								}
							}
						}
						break;
				}
	}
	return 0;
}

int caisseInHoles(int nbTrous, SDL_Rect trous[nbTrous], int nbObjects, GameObject** objects){
	SDL_Rect vide = {0,0,0,0};		// Le rectangle vide (pour faire retirer la collision au trou)
	SDL_Rect ObjetR, TrouR;			// SDL_Rect pour stocker les rectangles de l'objet à regarder et du trou à regarder
	for(int i = 0; i < nbObjects; i++){			// Parcours de notre tableau d'objets
		ObjetR = (*objects+i)->destRect;		// On stock le rectangle de l'objet
		for(int j = 0; j < nbTrous; j++){		// Parcours de notre tableau de trous
			TrouR = trous[j];			// On stock le rectangle du trou
			// Réajustement de la hitbox du trou
			TrouR.y += OFFSET*2;
			TrouR.x += OFFSET*2;
			TrouR.h -= OFFSET*2;
			TrouR.w -= OFFSET*2;
			// Si l'objet a des collisions
			if((*objects+i)->hasCollision == SDL_TRUE){
				// Et que cet objet a une intersection avec le trou
				if(SDL_HasIntersection(&ObjetR, &TrouR) == SDL_TRUE){
					// On retire les collisions, et on déplace la caisse dans le trou
					(*objects+i)->hasCollision = SDL_FALSE;
					(*objects+i)->destRect = trous[j];
					(*objects+i)->destRect.x += 9;
					(*objects+i)->destRect.y += 9;
					// On modifie le trou à vide pour que le joueur ne tombe pas dedans
					trous[j] = vide;
					return 1;
				}
			}
		}
	}
	return 0;
}

int playerInHoles(int nbTrous, SDL_Rect trous[nbTrous], GameObject* player){
	SDL_Rect PlayerR, TrouR;
	PlayerR = player->destRect;		// Stock du rectangle du joueur
	for(int i = 0; i < nbTrous; i++){
		TrouR = trous[i];	// Stock du rectangle du trou[i]
		if(player->hasCollision == SDL_TRUE){		// Si le joueur a des collisions
			// Si le joueur est dans un trou
			if(PlayerR.x+16 > TrouR.x && PlayerR.x + 16 < TrouR.x + TrouR.w && PlayerR.y+OFFSET > TrouR.y && PlayerR.y + OFFSET*2 < TrouR.y + TrouR.h){
				return 1;
			}
		}
	}
	return 0;
}

int caisseInFire(int nbFire, SDL_Rect fire[nbFire], int nbObjects, GameObject** objects){
	SDL_Rect ObjetR, FeuR;
	for(int i = 0; i < nbObjects; i++){
		ObjetR = (*objects+i)->destRect;		// Stock du rectangle de l'objet[i]
		for(int j = 0; j < nbFire; j++){
			FeuR = fire[j];				// Stock du rectangle du fire[j]
			// On réajuste la hitbox du feu
			FeuR.y += OFFSET*2;
			FeuR.x += OFFSET*2;
			FeuR.h -= OFFSET*2;
			FeuR.w -= OFFSET*2;
			// Si l'objet peut avoir des collisions
			if((*objects+i)->hasCollision == SDL_TRUE){
				// Si l'objet est en intersection avec le feu
				if(SDL_HasIntersection(&ObjetR, &FeuR) == SDL_TRUE){
					// On retire les collisions de la caisse
					(*objects+i)->hasCollision = SDL_FALSE;
					// On la téléporte dans le vide pour la faire disparaître
					(*objects+i)->destRect.y = 1500;
					return 1;
				}
			}
		}
	}
	return 0;
}

int playerInFire(int nbFire, SDL_Rect fire[nbFire], GameObject* player){
	SDL_Rect PlayerR, FireR;
	PlayerR = player->destRect;		// Stock des rectangles du joueur
	for(int i = 0; i < nbFire; i++){
		FireR = fire[i];		// Stock du rectangle du feu[i]
		// Si le joueur a des collisions
		if(player->hasCollision == SDL_TRUE){
			// Si le joueur est dans le feu[i]
			if(PlayerR.x+OFFSET > FireR.x && PlayerR.x + OFFSET < FireR.x + FireR.w && PlayerR.y+OFFSET > FireR.y && PlayerR.y + OFFSET*2 < FireR.y + FireR.h){
				return 1;
			}
		}
	}
	return 0;
}

int playerWin(GameObject* player, int posTuileObjectX, int posTuileObjectY){
	// Si le joueur est dans la sortie
	if(player->destRect.x > posTuileObjectX + 16){
		return 1;
	} else {
		return 0;
	}
}

int caisseInWater(int nbWater, SDL_Rect water[nbWater], int nbObjects, GameObject** objects){
	SDL_Rect ObjetR, WaterR;		// Stock
		for(int i = 0; i < nbObjects; i++){
			ObjetR = (*objects+i)->destRect;
			for(int j = 0; j < nbWater; j++){
				WaterR = water[j];
				// On décale la hitbox du rectangle de la flaque
				WaterR.x += OFFSET*2;
				WaterR.y += OFFSET*2;
				WaterR.w -= OFFSET*2;
				WaterR.h -= OFFSET*2;
				// Si l'objet peut avoir des collisions
				if((*objects+i)->hasCollision == SDL_TRUE){
					// S'il existe une intersection entre l'objet et la caisse
					if(SDL_HasIntersection(&ObjetR, &WaterR) == SDL_TRUE){
						return i;
					}
				}
			}
	}
	return -1;
}

int playerInWater(int nbWater, SDL_Rect water[nbWater], GameObject* player){
	SDL_Rect PlayerR, WaterR;		// Stock les rectangles du joueur et des flaques
	PlayerR = player->destRect;
	for(int j = 0; j < nbWater; j++){
		WaterR = water[j];
		// On décale la hitbox du rectangle de la flaque
		WaterR.x += OFFSET*2;
		WaterR.y += OFFSET*2;
		WaterR.w -= OFFSET*2;
		WaterR.h -= OFFSET*2;
		if(player->hasCollision == SDL_TRUE){
			// S'il existe une intersection entre l'objet et la caisse
			if(SDL_HasIntersection(&PlayerR, &WaterR) == SDL_TRUE){
				return 1;
			}
		}
	}
	return 0;
}

int getToucheClavier(SDL_Event event, int nbObjects, GameObject** objects, GameObject* player, int width, int height, short int Tilemap[height][width], StaticObject* tuiles, int nbTexture){
	int direction;		// Variable pour la commande clavier exécutée par l'utilisateur
	int posTuileObjectX, posTuileObjectY;			// Variables qui stockent les positions en X et Y de la tuile en question
	short int TuileObject;		// Variable pour stocker l'indice de la tuile sur laquelle est le joueur
	while(SDL_PollEvent(&event)){		// On lit un événement
		switch(event.type){		// On test les événements
			case SDL_WINDOWEVENT:		// Si cet événement est de type SDL_WINDOWEVENT
				if(event.window.event == SDL_WINDOWEVENT_CLOSE){   // et que cet événement est la fermeture de la fenêtre
					return -1;
				}
				break;
			case SDL_KEYUP:		// Si cet événement est de type SDL_KEYUP
				if(event.key.keysym.sym == SDLK_UP){		// Si cet événement est la flèche du haut relâchée
						direction = 1;
						player->destRect.y -= OFFSET;		// On déplace le joueur vers le haut
						mouvementCaisse(nbTexture, tuiles, nbObjects, objects, player, direction, width, height, Tilemap);		// On bouge la caisse s'il y en a une à côté du joueur vers le haut
				} else if(event.key.keysym.sym == SDLK_LEFT){		// Si cet événement est la flèche de gauche relâchée
						direction = 2;
						player->destRect.x -= OFFSET;		// On déplace le joueur vers la gauche relâchée
						mouvementCaisse(nbTexture, tuiles, nbObjects, objects, player, direction, width, height, Tilemap);		// On bouge la caisse s'il y en a une à côté du joueur vers la gauche
				} else if(event.key.keysym.sym == SDLK_RIGHT){		// Si cet événement est la flèche de droite relâchée
						direction = 3;
						player->destRect.x += OFFSET;		// On déplace le joueur vers la droite relâchée
						mouvementCaisse(nbTexture, tuiles, nbObjects, objects, player, direction, width, height, Tilemap);		// On bouge la caisse s'il y en a une à côté du joueur vers la droite
				} else if(event.key.keysym.sym == SDLK_DOWN){		// Si cet événement est la flèche du bas relâchée
						direction = 4;
						player->destRect.y += OFFSET;		// On déplace le joueur vers le bas relâchée
						mouvementCaisse(nbTexture, tuiles, nbObjects, objects, player, direction, width, height, Tilemap);		// On bouge la caisse s'il y en a une à côté du joueur vers le bas
				}
				// On stock la tuile sur laquelle est le joueur
				TuileObject = getTuileObject(width, height, Tilemap, player, &posTuileObjectX, &posTuileObjectY);
				// On gère les collisions du joueur en fonction de ctte tuile
				CollisionObject(nbTexture, tuiles, player, TuileObject, posTuileObjectX, posTuileObjectY, direction);
				break;
		}
	}
	return direction;
}
