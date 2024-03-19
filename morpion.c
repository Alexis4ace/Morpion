#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "assert.h"
#include "morpion.h"
#include "ia.h"



TplateauJeu init_plateau(int longueur, int largeur){ // init la matrice des cases du plateau
    TplateauJeu plateau = malloc((longueur+2)*sizeof(Case));
    for(int i = 0; i < longueur + 2; i++){
        plateau[i] = malloc((largeur+2)*sizeof(Case));
    }
    for(int x=0;x<largeur+2;x++){
        for(int y=0;y<largeur+2;y++){
            plateau[x][y].note= 0 ;
            plateau[x][y].player = 45 ;
        }
    }
    return plateau;
}

void affiche_plateau(TplateauJeu p){ // affichage graphique
    printf("\n");
    for(int x=1;x<LONGUEUR+1;x++){
        for(int y=1;y<LARGEUR+1;y++){
            printf("%c ", p[y][x].player );
        }
        printf("\n");
    }
}
void affiche_note(TplateauJeu p){ // affichage des note graphique ( uniquement pour aider au develepoment
    printf("\n");
    for(int x=1;x<LONGUEUR+1;x++){
        for(int y=1;y<LARGEUR+1;y++){
            printf(" %d ",(p[y][x].note));
        }
        printf("\n");
    }
}
bool appartientPlateau( int x , int y ){ // test de debordement 
    return x > 0 && x <= LONGUEUR && y <=LARGEUR && y > 0 ;;
}

// test si le joueur est un oridnateur
bool estOrdi( int symb , int **jeuSymb ){
    for(int i = 0 ; i < jeuSymb[1][0] ; i++){

        if(jeuSymb[1][i]==symb)
            return true;
    }
    return false;
}

void affiche_symbole(int **p,int humain , int ordi){
    printf("-------------------------------------\n");
    for( int x = 1; x<humain+1 ;x++ )
            printf("symb hum: %c \n",p[0][x]);

    for( int y = 1 ; y<ordi+1 ; y++)
            printf("symb ordi: %c \n",p[1][y]);
    printf("-----------------------------------\n");



}

//affecte des symboles aux ordis et humains ( entier avec affichage en string en mode console qui devient symbole ASCII )
int** affectationSymb(int humain , int ordinateur , int *symb){
    int** tabSymb = malloc( 2*sizeof(int) );

    tabSymb[0] = malloc( (humain+1)*sizeof(int)  );
    tabSymb[1] = malloc( (ordinateur+1)*sizeof(int) );

    tabSymb[0][0] = humain;
    tabSymb[1][0] = ordinateur;

    for( int x = 0; x<humain ;x++ )
            tabSymb[0][x+1] = symb[x] ;

    for( int y = 0 ; y<ordinateur ; y++)
            tabSymb[1][y+1] = symb[y+humain];

    return tabSymb ;
}


int* generationSymb(int nombres){

    int* symb = malloc(sizeof(int)*nombres) ;

    symb[0] = 88;
    symb[1] = 79;

    int k=1,n = 2 ;

    while(n != nombres){
        if(k < 7 || k >15){
            symb[n] = k;

            n++;
            k++;
        }
        else
            k++;
    }

    return symb;
}

//interface utilisateur pour enregistrer un nombre de joueurs et ordinateurs
void init_joueur(int *humainMAX , int *ordinateurMAX){

    int x=-1,y=-1;
    while(x+y < 2 || x+y > 16){

        printf("Le Nombre de participant joueur et ordinateur ne peut etre superieur a 16 ( et inferieur a 2 evidemment \n");
        x = -1;
        y = -1;
        while(x<0 || x>16 ){
            printf("ENTRER NOMBRE JOUEUR entre 0 et 16 : \n");
            scanf("%d",&x);
        }

        while(y<0 || y>16){
        printf("ENTRER NOMBRE ORDINATEUR entre 0 et 16 : \n");
        scanf("%d",&y);
        }
    }
    *humainMAX = x;
    *ordinateurMAX = y ;
}

// desalocation
void free_all(int *tabsymb , int **OrdiJoueurSymb , TplateauJeu plateau){
    for(int i = 0 ; i < LONGUEUR+2 ; i++)
        free(plateau[i]);
    free(plateau);
    free(tabsymb);
    free(OrdiJoueurSymb[0]);
    free(OrdiJoueurSymb[1]);
    free(OrdiJoueurSymb);
}




