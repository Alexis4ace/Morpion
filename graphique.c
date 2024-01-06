#include "graphique.h"
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
void LancementVideo(){
    if(SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_Log("ERREUR : Initialisation SDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("KAAL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LARGEUR,HAUTEUR,0);


    if(window == NULL){
        SDL_Log("ERREUR : creation fenetre SDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
