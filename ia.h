#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED
#include <stdbool.h>
#include "morpion.h"
#define NOTE 11
typedef int *tabNote;
tabNote init_tabNote();
//diapo p29
bool existeQuintu(int x , int y, int vx , int vy);
int sommeQuintu( int x , int y, int vx , int vy ,int player , bool *ouvert , TplateauJeu jeu);
int noteQuintu( int somme , tabNote tabN , bool ouvert , bool ordinateur);

//diapo p30
int noteDir(int x_init , int y_init , int vx , int vy , TplateauJeu jeu , tabNote note,bool *fin ,int player , bool ordinateur);
int noteCarreau( int x , int y , TplateauJeu jeu , tabNote note,bool *fin , int **tabsymb,int player);
int* meilleurNote(TplateauJeu jeu);

//fonction coder par hugo robert le crackito
void noteCarreauAll(TplateauJeu jeu, tabNote note, int x ,int y,bool *fin,int **jeuSymb);

#endif // IA_H_INCLUDED
