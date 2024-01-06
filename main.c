#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2\include\SDL2\SDL.h"
#include "SDL2\include\SDL2\SDL_ttf.h"

#include "morpion.h"
#include "ia.h"
#include "save.h"
#include "graph.c"

int main( int argc , char* argv[]){



    int humain, ordi;

    tabNote note = init_tabNote();  // tableau pour savoir la valeur d'un quintuplet X
    init_joueur(&humain , &ordi);

    int* tabsymb = generationSymb(humain+ordi) ;

    int** jeuSymb = affectationSymb(humain,ordi,tabsymb);
    affiche_symbole(jeuSymb,humain,ordi);
    TplateauJeu j = init_plateau(LONGUEUR,LARGEUR);

    morpion_t morpion = init_morpion(&j,jeuSymb,note);
    game_t game = init_game(&morpion);
    LancementVideo(&game);

    free_all(tabsymb,jeuSymb,j);
    return 0;
}
