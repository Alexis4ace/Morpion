#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#define HAUT 700
#define LARG 1300
typedef struct {
    TplateauJeu *jeu;
    tabNote note;
    int **jeuSymb;
    int *ban;
}morpion_t;

typedef struct {
    int players;
    int ordinateur;
    int board;
    int pixel;
    bool state;
    bool bot_turn;
    int coup;
    morpion_t *morpion;
}game_t;

void LancementVideo(game_t *game);
void afficheImage(SDL_Texture *texture,int X,int Y,SDL_Renderer *renderer);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
SDL_Texture **tabImg(SDL_Renderer *renderer);
bool in_matrice(int x , int y , game_t game);
void click_game(game_t *game , int clickx , int clicky,SDL_Texture *tab[],SDL_Renderer *renderer);
void drawRect(SDL_Renderer *renderer,int x , int y , int largx , int largy);
void init_vue( SDL_Renderer *renderer ,  game_t game);
void drawBoard(SDL_Renderer *renderer,game_t game);
void drawFullRect(SDL_Surface * surface,int posX, int posY, int width, int length, int R, int G, int B);
void afficheTxt(char path[],int x , int y , int h , int l,SDL_Renderer *renderer ,SDL_Color couleur);



#endif // GRAPH_H_INCLUDED
