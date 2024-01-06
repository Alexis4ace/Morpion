#include <stdio.h>
#include <stdlib.h>

#include "ia.h"
//-----------------------------------------------------------------------------
//tableau du diapo P33 il est final
tabNote init_tabNote(){
    int taille = 11;
    tabNote tab = malloc(sizeof(int)*taille);
    int nombre = 1;
    tab[0] = 1;
    for(int i=1 ; i < taille ; i++){
        if(i%2 == 1)
            nombre = nombre * 5;
        else
            nombre = nombre * 2;
        tab[i] = nombre ;
    }
    return tab;
}
//------------------------P29 DIAPO--------------------------------------------------------------

bool existeQuintu(int x , int y , int vx , int vy){ // regarde a partir de X et Y et une direction (vx,vy) ex : 1 , 1
                                                //regarde si pour jusqu'a 4 case dans cette direction on depasse pas                                                  // du bord du plateau. tres utile pour plus tard
    int n = 0 ;

    while(n<5){  // tant qu'on est dans le plateau et que n<5

        if(x<=0 || y<= 0 || x>=10 || y>=10) // si en dehors du plateau alors renvoi faux
            return false;
        else{
            x = x+vx;  // passe prochaine case ex : pos 5 , 5 alors on va à 6 ,6  jusqu'a 9,9 (avec l'ex dir au dessus
            y = y+vy;
            n++;
        }
    }
    return true; //renvoi vrai si on est jamais sortis
}

int sommeQuintu( int x , int y, int vx , int vy ,int player , bool *ouvert ,TplateauJeu jeu){ // calcule permet de savoir combien il y a
                                                                    // de croix ou de rond dans un quintuplet ( 5 case)
    int x_t = x , y_t = y;
    int somme = 0;
    while((x_t != x+vx*5 || y_t != y+vy*5) ){    // tant qu'on a pas avancé de 5 case alors

        if( jeu[x_t][y_t].player == player )
            somme++;
        if( jeu[x_t][y_t].player != player && jeu[x_t][y_t].player != 45 )
            *ouvert = false ;
        x_t += vx;  //on avance
        y_t += vy;
    }

    return somme; // retourne le resultat pour la fonction du dessous qui va traduire
}

int noteQuintu( int somme , tabNote tabN , bool ouvert , bool ordinateur){ // donne une note par rapport au tableau P33 du diapo
    int indice ;
    if( ordinateur)
        indice = somme*2 ;
    else if( somme != 0)
        indice = somme*2 -1;
    else
        indice = 0;

    if ( ouvert)
        return tabN[indice];
    else
        return (tabN[indice]*0.2);
}

//-----------------------------DIAPO P30--------------------------------------------------
int noteDir(int x_init , int y_init , int vx , int vy , TplateauJeu jeu , tabNote note,bool *fin ,int player , bool ordinateur){

    int somme,temp,x=x_init,y=y_init;
    int final_note=0;    //cette fct renvoi la somme des valeur des quintuplet dans une direction
    int marche = 0;      //diagonale vers le bas ou diagonale vers le haut ou vertical ou horizontale
    bool ouvert = true ;
    while(marche < 5 ){

        if(existeQuintu(x,y,vx,vy)){

            somme = sommeQuintu(x,y , vx , vy , player, &ouvert,jeu);
            temp = noteQuintu(somme , note , ouvert , ordinateur );

            if(temp == note[NOTE-1] || temp == note[NOTE-2])  // si la valeur d'un quintuplet est égale à 5 symbole identique
                *fin = false;  // alors passe le boolean du main à false
            else
                final_note += temp;

        }
        x -= vx ;
        y -= vy ;
        marche++;
    }
    return final_note; // renvoie la somme
}

int noteCarreau( int x, int y , TplateauJeu jeu , tabNote note,bool *fin, int **tabsymb,int player){
    int final_note = 0;
    bool ordi = estOrdi(player,tabsymb);

    final_note += noteDir(x,y , 1 , 0 , jeu , note,fin,player,ordi);
    final_note += noteDir(x,y , 0 , 1 , jeu , note,fin,player,ordi);
    final_note += noteDir(x,y, 1 , 1 , jeu , note,fin,player,ordi);
    final_note += noteDir(x,y, -1 , 1 , jeu , note,fin,player,ordi);

    return final_note; // renvoi la note final du carrreaux c-a-d la somme de la somme de chacune des directions
}


int* meilleurNote(TplateauJeu jeu){ // trivial

    int *tableau;
    tableau = malloc( sizeof(int)*2 );
    int meilleur= -2147483648 ;

    for(int x=1;x<=LONGUEUR;x++){
        for(int y=1;y<=LARGEUR;y++){

            Case c = jeu[x][y] ;
            if((c.note >= meilleur && c.player == 45 )  ){
                meilleur = c.note;
                tableau[0] = x ;
                tableau[1] =  y ;
            /*if(x == 9 && y == 9  && c.player != 45 && meilleur == -2147483648 )
                return NULL;*/
            }
        }
    }
    return tableau;
}

//--------------------------------------------------------------------------------------------
void noteCarreauAll(TplateauJeu jeu, tabNote note, int x ,int y,bool *fin , int **jSymb){
    for(int i =1 ; i>-2 ; i--){

        for(int o=0 ; o<4 ; o++){  // si on joue en 5 5 (X) alors on modifie tout les note (U)

            if( appartientPlateau( x, y-(4 - o)*i ) )
                jeu[x][y-(4 - o)*i].note = noteCarreau(x,y-(4 - o)*i,jeu,note,fin,jSymb , jeu[x][y].player);               // U - - - U - - - U
                                                                                                        // - U - - U - - U -
            if( appartientPlateau( x-(4 - o)*i , y ) )                                                  // - - U - U - U - -
                jeu[x-(4 - o)*i][y].note = noteCarreau(x-(4 - o)*i,y,jeu,note,fin,jSymb,jeu[x][y].player);               // - - - U U U - - -
                                                                                                        // U U U U X U U U U
            if( appartientPlateau( x-(4-o)*i , y-(4-o)*i) )                                             // - - - U U U - - -
                jeu[x-(4-o)*i][y-(4-o)*i].note = noteCarreau(x-(4-o)*i,y-(4-o)*i,jeu,note,fin,jSymb,jeu[x][y].player);   // - - U - U - U - -
                                                                                                        // - U - - U - - U -
            if( appartientPlateau( x+(4-o)*i, y-(4-o)*i) )                                              // U - - - U - - - U
                jeu[x+(4-o)*i][y-(4-o)*i].note = noteCarreau(x+(4-o)*i,y-(4-o)*i,jeu,note,fin,jSymb,jeu[x][y].player);
        }
        i--;
    }

}


