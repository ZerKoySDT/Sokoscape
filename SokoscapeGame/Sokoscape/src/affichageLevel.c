#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "niveau.h"     // Accès au type GameObject & StaticObject

int afficheLevel(StaticObject* tiles, int width, int height, short int Tilemap[height][width], SDL_Renderer* renderer, int largeurFenetre, int hauteurFenetre){
  short int valeurTile; // Entier qui va stocker la case du tableau à 2 dimensions
  int tailleTuileW,tailleTuileH;
  tailleTuileW = 64;
  tailleTuileH = 64;
  SDL_Rect destRect = {0, 0, tailleTuileW, tailleTuileH};   // Rectangle pour placer les tuiles du niveau sur le rendu

  // On parcourt tout notre tableau à 2 dimensions
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      valeurTile = Tilemap[i][j];   // On stock la valeur actuel du tableau
      // On copie sur le rendu la texture d'index valeurTile dans le tableau dynamique de textures
      if(SDL_RenderCopy(renderer, (tiles+valeurTile)->texture, NULL, &destRect) != 0){
        // Si on a une erreur, on sort de la fonction
        SDL_Log("Erreur copie de la tile en x = %d | y = %d > %s", j, i, SDL_GetError());
        return EXIT_FAILURE;
      }
      destRect.x += tailleTuileW; // On déplace le rectangle pour se décaler
    }
    destRect.x = 0;
    destRect.y += tailleTuileH;
  }
  return EXIT_SUCCESS;
}

int afficheObject(int width, int height,int nbObjects, GameObject* objects, SDL_Renderer* renderer){
  SDL_Rect rectangleDestination = {0,0,(46*width)/16,(46*height)/12}; // Pour redimensionner la taille des objets en fonction du nombre de tuiles
  // On parcourt le tableau de GameObject
  for(int i = 0; i < nbObjects; i++){
    // On change les positions du rectangle en X Y pour correspondre aux coordonnées de l'objet qu'on doit placer sur le rendu
    rectangleDestination.x = (objects+i)->destRect.x;
    rectangleDestination.y = (objects+i)->destRect.y;
    // On copie sur le rendu la texture du GameObject d'index nbObjects dans le tableau dynamique de GameObject
    if(SDL_RenderCopy(renderer, (objects+i)->texture, NULL, &rectangleDestination) != 0){
      // Si on a une erreur, on sort de la fonction
      SDL_Log("Erreur rendu de l'objet n°%d > %s", i, SDL_GetError());
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int afficheJoueur(int width, int height, GameObject* player, SDL_Renderer* renderer){
  SDL_Rect rectangleDestination = {0,0,(32*width)/width,(32*height)/height}; // Pour redimensionner la taille des objets en fonction du nombre de tuiles
  rectangleDestination.x = player->destRect.x;
  rectangleDestination.y = player->destRect.y;
  // On copie sur le rendu la texture du GameObject du joueur
  if(SDL_RenderCopy(renderer, player->texture, NULL, &rectangleDestination) != 0){
    // Si on a une erreur dans le dessin, on sort de la fonction
    SDL_Log("Erreur rendercopy joueur > %s \n", SDL_GetError());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int afficheTimer(TTF_Font* policeTimer, SDL_Color colorTimer, SDL_Renderer* renderer, char timer[6]){
  SDL_Rect positionTimer;
  SDL_Surface* surfaceTimer;
  SDL_Texture* textureTimer;
  positionTimer.x = 450;
  positionTimer.y = -10;
  surfaceTimer = TTF_RenderText_Solid(policeTimer, timer, colorTimer);
  textureTimer = SDL_CreateTextureFromSurface(renderer, surfaceTimer);
  SDL_QueryTexture(textureTimer, NULL, NULL, &positionTimer.w, &positionTimer.h);
  SDL_FreeSurface(surfaceTimer);
  if(SDL_RenderCopy(renderer, textureTimer, NULL, &positionTimer) != 0){
    SDL_Log("ERREUR: SDL_RenderCopy dans la fonction afficheTimer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_DestroyTexture(textureTimer);
  return EXIT_SUCCESS;
}

int afficheMap(int width, int height, short int Tilemap[height][width], SDL_Renderer* renderer, int nbObjects, int nbTexture, StaticObject* tiles, GameObject* objects, GameObject* player, int largeurFenetre, int hauteurFenetre){
  if(afficheLevel(tiles, width, height, Tilemap, renderer, largeurFenetre, hauteurFenetre) == EXIT_FAILURE){
    // Si la fonction échoue, on détruit les éléments alloués grâce à la fonction clearRessources et on quitte la SDL
    return EXIT_FAILURE;
  }
  // Affiche les textures des objets sur le rendu grâce à la fonction afficheObject
  if(afficheObject(width, height, nbObjects, objects, renderer) == EXIT_FAILURE){
    // Si la fonction échoue, on détruit les éléments alloués grâce à la fonction clearRessources et on quitte la SDL
    return EXIT_FAILURE;
  }
  // Affiche la texture du joueur sur le rendu grâce à la fonction afficheJoueur
  if(afficheJoueur(width, height, player, renderer) == EXIT_FAILURE){
    // Si la fonction échoue, on détruit les éléments alloués grâce à la fonction clearRessources et on quitte la SDL
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
