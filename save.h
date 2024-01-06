#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#include "morpion.h"


void ecrit_int_bi(char *name, int *val );
void ecrit_int_txt(char *name, int *val ,bool retourLigne );
void ecrit_char_txt(char *name, char *p);

void load_plateau(char *name, int * joueur , TplateauJeu *j); // init la matrice
void load_joueur(char *name,int ***symb);

void Sauvegarde(TplateauJeu* j , int* joueur ,int*** jeusymb);
void save_game(char *name , TplateauJeu matrice , int joueur);
void save_joueur(int **joueur,char *name);
void save_ban(char *name , int *tab , int taille );
int *load_ban(char *name);


void log_save(char *name_save);
void init_log();
int compte_log();
void delete_log(const char *name);

void affiche_ban(int *tab , int taille );




void menu_save(int *joueur , TplateauJeu *j ,int *** jeusymb);
void Concat_char ( char * nom_fichier , char * extension);
void Save_sans_double( char * nom_fichier , TplateauJeu *j , int *joueur , int ***symb);
void charge_true_save ( char * nom_fichier,int *joueur ,TplateauJeu *j, int ***symb );
void find_true_save(char *nomfichier);


bool isBinaire( char * nom_fichier);
int supp_txt(const char *name );
bool existe_txt(const char *name);

void Rappelle_menu();
char * str_sub( char *s, unsigned int start, unsigned int fin);


#endif // SAVE_H_INCLUDED
