#include "SDL2\include\SDL2\SDL.h"
#include "SDL2\include\SDL2\SDL_ttf.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"
int numero_j(int **symb , int n){
    for(int i=0 ; i<2 ;i++){
        for(int y=1 ; y<symb[i][0]+1 ; y++){
            if( symb[i][y] == n && i ==0)
                return y-1;
            if( symb[i][y] == n && i ==1)
                return y+symb[0][0]-1;
        }
    }
    return -1;
}

bool in_matrice(int x , int y , game_t game){
    int espace = game.pixel*2;
    return x>espace && x < game.board*game.pixel+espace && y>espace && y<game.board*game.pixel+espace;
}
bool in_save(int x , int y){
    return x>=LARG-250 && y>=HAUT-400 && y<= HAUT-400+74 && x<=LARG-250+105;
}

bool in_load(int x , int y){
    return x>=1050 && y>=500 && y<= 650 && x<=1200;
}
bool in_abandon(int x , int y){
    return x>1050 && x<1170 && y > 100 && y<220;
}
void remplir(TplateauJeu j,int **jeusymb,SDL_Renderer *renderer,SDL_Texture *tab[]){
    int joueur;
    for(int x=1 ; x<10 ; x++){
        for(int y=1 ; y<10 ; y++){
            joueur = numero_j(jeusymb , j[x][y].player );
            if( joueur != -1){
                afficheImage(tab[joueur], 50+x*50 , 50+y*50 , renderer);
            }
        }
    }
}
bool est_non_ban(game_t game , int player){

    for(int i = 1 ; i < game.morpion->ban[0] ; i++ ){
        if( game.morpion->ban[i] == player )
            return false;
    }
    return true;
}

void click_game(game_t *game , int clickx , int clicky,SDL_Texture *tab[],SDL_Renderer *renderer){

    if(game->morpion->jeuSymb[0][0] == 0)
        game->bot_turn = true ;
    int posx = clickx/game->pixel ;
    int posy = clicky/game->pixel ;
    int *coord;
    char *name = "Sauvegarde/SAUVEGARDE_b.txt" ;
    char *ban_name = "Sauvegarde/ban_t.txt";

    if(in_save(clickx , clicky) && !(game->bot_turn)){
        printf("save");
        FILE *fd = fopen(name,"w");
        fclose(fd);
        FILE *fd2 = fopen(ban_name,"w");
        fclose(fd2);

        save_game(name, *(game->morpion->jeu) , game->morpion->jeuSymb[0][game->players+1]);
        save_joueur( (game->morpion->jeuSymb),name);
        save_ban( ban_name , game->morpion->ban , (game->morpion->jeuSymb[0][0]) + 1);
    }
    else if(!(game->bot_turn) && in_abandon(clickx,clicky)){
        game->morpion->ban[game->morpion->ban[0]]=game->players;
        game->morpion->ban[0] += 1 ;

    }
    else if(!(game->bot_turn) && in_load(clickx,clicky)){

        init_vue(renderer,*game);
        drawBoard(renderer,*game);
        int joueur_ascii;
        load_plateau(name, &joueur_ascii , game->morpion->jeu);
        game->players = numero_j(game->morpion->jeuSymb , joueur_ascii);
        //load joueur

        load_joueur(name,&(game->morpion->jeuSymb));
        game->morpion->ban = load_ban(ban_name);
        remplir(*(game->morpion->jeu) , game->morpion->jeuSymb , renderer , tab );


    }
    else if(!(game->bot_turn) && in_matrice(clickx,clicky,*game) && game->state && (*(game->morpion->jeu))[posx-1][posy-1].player == 45  && game->morpion->jeuSymb[0][0] > 0   ){//&& (game->morpion->jeuSymb)[0][0] > 0 ){
        if(est_non_ban(*game , game->players)){

        afficheImage(tab[game->players],posx*game->pixel +2, posy*game->pixel +2, renderer);
        game->coup -= 1 ;
        game->state = game->coup <= 0 ? false : true ;
        (*(game->morpion->jeu))[posx-1][posy-1].player = game->morpion->jeuSymb[0][game->players+1] ;
        noteCarreauAll(*(game->morpion->jeu),game->morpion->note,posx-1,posy-1,&(game->state),game->morpion->jeuSymb);
        }
        if(game->players+1 >= (game->morpion->jeuSymb)[0][0]){
            game->players = 0 ;

            game->bot_turn = true ;
        }
        else
            game->players += 1 ;
    }
    else if(game->bot_turn && in_matrice(clickx,clicky,*game) ) {
        while( game->morpion->jeuSymb[1][0] > game->ordinateur && game->state ){
            coord = meilleurNote(*(game->morpion->jeu));

            (*(game->morpion->jeu))[coord[0]][coord[1]].player = game->morpion->jeuSymb[1][game->ordinateur+1] ;
            afficheImage(tab[game->ordinateur+game->morpion->jeuSymb[0][0]],game->pixel+coord[0]*game->pixel +2, game->pixel+coord[1]*game->pixel +2, renderer);
            noteCarreauAll(*(game->morpion->jeu),game->morpion->note,coord[0],coord[1],&(game->state),game->morpion->jeuSymb);
            game->ordinateur += 1 ;
            game->coup -= 1 ;
            if( game->coup <= 0 )
                game->state = false ;
            free(coord);
        }

        game->ordinateur = 0;
        game->bot_turn = (game->morpion->jeuSymb)[0][0] >= 0 ? false : true ;
    }

}

void drawRect(SDL_Renderer *renderer,int x , int y , int largx , int largy){

    SDL_Rect rectangle;
    SDL_Texture *texture = loadImage("./IMG/rouge.bmp",renderer);
    if( SDL_QueryTexture(texture,NULL,NULL,&rectangle.w,&rectangle.h) != 0){
        SDL_Log("ERREUR : creation de texture query charger memoireSDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    rectangle.x = x ;
    rectangle.y = y ;
    rectangle.h = largy;
    rectangle.w = largx;

    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_Log("ERREUR : rendercopy SDL charger ecran> %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_RenderPresent(renderer);

}
void init_vue( SDL_Renderer *renderer , game_t game){



    SDL_Texture *texture = loadImage("./IMG/img.bmp",renderer) ;
    afficheImage(texture,0,0,renderer);
    //texture = loadImage("./IMG/tele.bmp",renderer);
    //afficheImage(texture,LARG-400 ,0,renderer);
    texture = loadImage("./IMG/save.bmp",renderer);
    afficheImage(texture,LARG-250 ,HAUT-400,renderer);
    texture = loadImage("./IMG/upload.bmp",renderer);
    afficheImage(texture,LARG-250 ,HAUT-200,renderer);

    texture = loadImage("./IMG/FF.bmp",renderer);
    afficheImage(texture,1050 ,100,renderer);


    drawBoard(renderer,game);

}

void drawBoard(SDL_Renderer *renderer,game_t game){

    int largeur =  game.pixel*game.board ;
    int hauteur =  game.pixel*game.board;

    for(int x=0 ; x<game.board+1 ; x++){
       drawRect(renderer,2*game.pixel+x*game.pixel,game.pixel*2,5,hauteur+5);
    }
    for(int y=0 ; y<game.board+1 ; y++){
        drawRect(renderer,2*game.pixel,game.pixel*y+game.pixel*2,largeur,5);
    }

}
void drawFullRect(SDL_Surface * surface,int posX, int posY, int width, int length, int R, int G, int B)
{
	SDL_Rect Rect;
	Rect.x = posX;
	Rect.y = posY;
	Rect.w = length-1;
	Rect.h = width-1;

	SDL_FillRect(surface, &Rect, SDL_MapRGB(surface->format, R, G, B));
}
void LancementVideo( game_t *game){



    SDL_Event event;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;






    if(SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_Log("ERREUR : Initialisation SDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("MORPION",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LARG,HAUT,0);


    if(window == NULL){
        SDL_Log("ERREUR : creation fenetre SDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    //SDL_SetRenderDrawColor( renderer, 14, 149, 204, 255 );

    if(renderer == NULL){
        SDL_Log("ERREUR : creation de rendu SDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Texture **tab = tabImg(renderer);

    init_vue(renderer,*game);

    while( game->state != false ){
        while(SDL_PollEvent(&event)){
            switch(event.type){

            case SDL_QUIT:
                game->state = false;

                break;

            case SDL_MOUSEBUTTONDOWN:
                click_game(game, event.button.x , event.button.y , tab , renderer);
                }
        }
    }
   // SDL_Delay(10000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void afficheImage(SDL_Texture *texture,int X,int Y,SDL_Renderer *renderer){

    SDL_Rect rectangle;
    if( SDL_QueryTexture(texture,NULL,NULL,&rectangle.w,&rectangle.h) != 0){
        SDL_Log("ERREUR : creation de texture query charger memoireSDL > %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
    rectangle.x = X;
    rectangle.y = Y;

    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_Log("ERREUR : rendercopy SDL charger ecran> %s \n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_RenderPresent(renderer);

}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer){
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = SDL_LoadBMP(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}
/*
void afficheTxt(char path[],int x , int y , int h , int l,SDL_Renderer *renderer ,SDL_Color couleur){
    TTF_Font* police = NULL;

    police = TTF_OpenFont("./fonts/arial.ttf",25);




    SDL_Surface* texte = TTF_RenderText_Blended(police,path, couleur) ;
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, texte);
    int texW = l;
    int texH = h;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect message = {x,y,texW,texH};

    SDL_RenderCopy(renderer, texture, NULL, &message);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(texte);
     TTF_CloseFont(police);

}*/


SDL_Texture **tabImg(SDL_Renderer *renderer){

    SDL_Texture **tab;
    tab = (SDL_Texture**)malloc(16*sizeof(SDL_Texture**));
    *tab = loadImage("./IMG/joueur/j1.bmp",renderer);
    *(tab+1) = loadImage("./IMG/joueur/j2.bmp",renderer);
    *(tab+2) = loadImage("./IMG/joueur/j3.bmp",renderer);
    *(tab+3) = loadImage("./IMG/joueur/j4.bmp",renderer);
    *(tab+4) = loadImage("./IMG/joueur/j5.bmp",renderer);
    *(tab+5) = loadImage("./IMG/joueur/j6.bmp",renderer);
    *(tab+6)= loadImage("./IMG/joueur/j7.bmp",renderer);
    *(tab+7) = loadImage("./IMG/joueur/j8.bmp",renderer);
    *(tab+8) = loadImage("./IMG/joueur/j9.bmp",renderer);
    *(tab+9) = loadImage("./IMG/joueur/j10.bmp",renderer);
    *(tab+10) = loadImage("./IMG/joueur/j11.bmp",renderer);
    *(tab+11) = loadImage("./IMG/joueur/j12.bmp",renderer);
    *(tab+12)= loadImage("./IMG/joueur/j13.bmp",renderer);
    *(tab+13) = loadImage("./IMG/joueur/j14.bmp",renderer);
    *(tab+14) = loadImage("./IMG/joueur/j15.bmp",renderer);
    *(tab+15) = loadImage("./IMG/joueur/j16.bmp",renderer);

    return tab;
}

int *init_ban(int n){
    int *tab = malloc(sizeof(int)*(n+1) );
    for(int i = 0 ; i<n+1 ; i++){
        tab[i] = 0 ;
    }
    tab[0]=1;

    return tab;
}

morpion_t init_morpion(TplateauJeu *jeu , int **symb , tabNote tnote){
    morpion_t morpion;
    morpion.jeu = jeu ;
    morpion.jeuSymb = symb;
    morpion.note = tnote;
    morpion.ban = init_ban(symb[0][0]);

    return morpion;
}
game_t init_game( morpion_t *m){
    game_t game;
    game.board = 9 ;
    game.pixel = 50;
    game.state =true ;
    game.players = 0;
    game.ordinateur = 0;
    game.morpion = m;
    game.bot_turn = false ;
    game.coup = LONGUEUR * LARGEUR ;
    return game;
}
