#ifndef MORPION_H_INCLUDED
#define MORPION_H_INCLUDED
#include <stdbool.h>

#define LARGEUR 9
#define LONGUEUR 9
typedef int *tabNote;


typedef struct CasePlateaux{ // int a = 45 ;   printf("%d",a);  affiche 45  printf("%c",a) affiche '-' ( voir ASCII)
    int player;
    int note;
}Case;

typedef Case **TplateauJeu;

TplateauJeu init_plateau(int longueur , int largeur); // martine vas à la piscine

void affiche_plateau(TplateauJeu Tjeu);
void affiche_note(TplateauJeu p);
void affiche_symbole(int **p,int humain , int ordi);


int** affectationSymb(int humain , int ordinateur, int *symb);
int* generationSymb(int nombres);
int* listeSymb(int **jeuSymb);

bool appartientPlateau( int x , int y );
void init_joueur(int *humainMAX , int *ordinateurMAX);
bool estOrdi( int symb , int **jeuSymb);

void free_all(int *tabsymb , int **OrdiJoueurSymb , TplateauJeu plateau);







#endif // MORPION_H_INCLUDED
