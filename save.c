#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "myassert.h"
#include "morpion.h"
#include "SDL2\include\SDL2\SDL.h"

void ecrit_int_bi(char *name, int *val ){ // sauvegarde un entier si entier != null
    FILE *fd = fopen(name,"ab");                                        // si bool == true alors fait un retour à la ligne

    if( val != NULL)    // on peux donc enregister un entier ou non ( +espace obligatoire )
    {
        fwrite(val,1,sizeof(*val),fd); }
    else
        printf("error ecriture vide ");

    fclose(fd);

}
void ecrit_int_txt(char *name, int *val ,bool retourLigne ){ // sauvegarde un entier si entier != null                                            // si bool == true alors fait un retour à la ligne
    FILE *fd = fopen(name,"a");

    if( val != NULL){    // on peux donc enregister un entier ou non ( +espace obligatoire )
         fprintf(fd,"%d",*val); //ecrit valeur
//        myassert( test > 0  ,"error ecriture un entier ");
        fprintf(fd,"%s"," "); //ecrit un espace
        //myassert(test != 1 ,"error ecriture un espace");
    }
    if(retourLigne){  // et/ou faire un retour a la ligne ou non
        fprintf(fd,"%s","\n");
       // myassert(test != 1,"error retour a la ligne");
    }
    fclose(fd);
}
void ecrit_char_txt(char *name, char *p){
    FILE *fd = fopen(name,"a");
    fprintf(fd,"%s",p);
    fclose(fd);
}

void load_plateau(char *name, int * joueur , TplateauJeu *j){

    for(int n=0 ; n < LARGEUR-2 ; n++) // Fait planter car tu libere puis ajuste apres
       free((*j)[n]);
    free(*j);
    bool binaire = isBinaire(name) ;
    TplateauJeu plateau = malloc((LONGUEUR+2)*sizeof(Case));
    for(int i = 0; i < LONGUEUR + 2; i++){
        plateau[i] = malloc((LARGEUR+2)*sizeof(Case));
    }
    FILE *fd;
    fd = binaire ? fopen(name,"rb") : fopen(name,"rt") ;

    int val;
    binaire ? fread(joueur,sizeof(*joueur),1,fd) : fscanf(fd,"%d",joueur); ;
    for(int x=1;x<LARGEUR+1;x++){
        for(int y=1;y<LARGEUR+1;y++){

            binaire ? fread(&val,sizeof(val),1,fd) : fscanf(fd,"%d",&val);
            plateau[x][y].player= val;

            binaire ? fread(&val,sizeof(val),1,fd) : fscanf(fd,"%d",&val);
            plateau[x][y].note = val ;
        }
    }
    fclose(fd);



    *j=plateau;
}
void load_joueur(char *name,int ***symb){

    int** tabSymb = malloc( 2*sizeof(int) );
    bool binaire = isBinaire(name) ;
    if(binaire)
        printf("binaire\n");
    FILE *fd ;
    char fin[4]="u";

    fd = binaire ? fopen(name,"rb") : fopen(name,"rt") ;

    int pos ;
    pos = ((sizeof(int) * LONGUEUR * LARGEUR ) * 2 ) + sizeof(int)  ;
    if(binaire)
        fseek(fd,pos,SEEK_SET);
    else{
        while( strcmp(fin,"j") != 0){
            fscanf(fd,"%s",fin);
        }
    }


    int val;

    for(int h=0 ; h<2;h++){

        binaire ? fread(&val,sizeof(val),1,fd) : fscanf(fd,"%d",&val);
        tabSymb[h] = malloc( (val+1)*sizeof(int)  );
        tabSymb[h][0] = val;

        for(int i=1 ; i < tabSymb[h][0]+1;i++){
            binaire ? fread(&val,sizeof(val),1,fd) : fscanf(fd,"%d",&val);
            tabSymb[h][i]= val;
        }
    }
    fclose(fd);

    free((*symb)[0]); // Les 2 free font planter
    free((*symb)[1]);
    free(*symb);
    *symb = tabSymb;

}


void save_joueur(int **joueur,char *name){
    int val;
    bool binaire = isBinaire(name);
    for(int h =0 ; h < 2 ; h++){
        if(!binaire)
            ecrit_int_txt(name,NULL,true);
        for(int o = 0 ; o < joueur[h][0]+1 ; o++){
            val = joueur[h][o];
            binaire ? ecrit_int_bi(name,&val) : ecrit_int_txt(name,&val,false);
        }
    }
}
void save_game(char *name , TplateauJeu matrice , int joueur){
    int val;
    bool binaire = isBinaire(name);

    binaire ? ecrit_int_bi(name,&joueur) : ecrit_int_txt(name,&joueur,true);

    for( int ligne = 1 ; ligne < LONGUEUR+1 ; ligne++){
        for( int colonne = 1 ; colonne < LARGEUR+1 ; colonne++){

            val = matrice[ligne][colonne].player;

            binaire ? ecrit_int_bi(name,&val) :  ecrit_int_txt(name,&val,false);
            val = matrice[ligne][colonne].note;

            binaire ? ecrit_int_bi(name,&val) :  ecrit_int_txt(name,&val,false);
        }
    }
    if(!binaire)
            ecrit_char_txt(name,"j");
    log_save(name);

}
void save_ban(char *name , int *tab , int taille ){
    bool binaire = isBinaire(name);
    binaire ? ecrit_int_bi(name,&taille) : ecrit_int_txt(name,&taille,true);
    for(int i=0 ; i<taille ; i++)
        binaire ? ecrit_int_bi(name,&tab[i]) : ecrit_int_txt(name,&tab[i],false);

}

int *load_ban(char *name){
    FILE *fd ;

    int taille;
    bool binaire = isBinaire(name);
    fd = binaire ? fopen(name,"rb") : fopen(name,"rt") ;
    binaire ? fread(&taille,sizeof(taille),1,fd) : fscanf(fd,"%d",&taille);

    int *tab=malloc(sizeof(int)*taille);
    for(int i=0 ; i<taille ; i++)
        binaire ? fread(&tab[i],sizeof(tab[i]),1,fd) : fscanf(fd,"%d",&tab[i]);

    return tab;

}

void affiche_ban(int *tab , int taille ){
    for(int i = 0 ; i<taille ; i++)
        printf("|  %d  ",tab[i]);
}

bool existe_txt(const char *name){
    FILE *fd = fopen(name,"rt");
    if(fd == NULL){
        fclose(fd);
        return false;
    }
    else{
        fclose(fd);
        return true;
    }
}
int supp_txt(const char *name ){

    if (existe_txt(name)){
        remove(name);
        return 0;
    }
    else
        return -1;
}
char* existe_log(char *txt){

    FILE *fd = fopen("log.txt","r");
    char *oppose = malloc(sizeof(char));
    strcpy(oppose,txt);
    oppose[strlen(oppose)-5] = isBinaire(txt) ? 't' : 'b' ;

    char t[200];
    while ( fscanf(fd,"%s",t) != EOF ){
        if(strcmp(txt,t)==0 )
            return txt;
        if(strcmp(oppose,t)==0)
            return oppose;
    }
    return NULL;
}

void log_save(char *name_save){

    FILE *fd = fopen("log.txt","a");
    myassert(fd != NULL,"erreur ouverture fichier log");

    fprintf(fd,"%s", "\n");
    //myassert(test != 1,"error ecriture retour a ligne");

    fprintf(fd,"%s",name_save);
    //myassert(test != 1,"error ecriture chaine dans log");
    fclose(fd);

}
int compte_log(){
    FILE *fd = fopen("log.txt","rt");
    myassert(fd != NULL , "erruer ouverture log.txt");
    int n=0;
    char texte[30];
    while(fscanf(fd,"%s",texte) != EOF){
        n++;
    }
    fclose(fd);
    return n;
}


void delete_log(const char *name){

    printf("\nDELETE LOG chaine : %s \n",name);

    FILE *fd = fopen("log.txt","rt");
    int taille = compte_log();
    char tab_char[taille][50];
    for(int i = 0 ; i < taille ; i++){
        fscanf(fd,"%s",tab_char[i]);
    }
    fclose(fd);



    fd = fopen("log.txt","w");
    for(int i = 0 ; i < taille ; i++){
        if(strcmp(tab_char[i],name) !=0  ){
            fprintf(fd,"%s",tab_char[i]);
            fprintf(fd,"%s","\n");
        }
    }
    supp_txt(name);
    fclose(fd);
}
void affiche_log(){

    FILE *fd = fopen("log.txt","rt");
    if(fd == NULL)
        printf("ERROR OUVERTURE FICHIER \n");
    char texte[200];
    char * nom_log = NULL;
    while(fscanf(fd,"%s",texte) != EOF ){
        nom_log = str_sub(texte,11, strlen(texte)-7);
        printf("%s \n",nom_log);
    }
    printf("\n");
    fclose(fd);
    free(nom_log);
}

bool isBinaire( char * nom_fichier){
    int pos = strlen(nom_fichier);
    return nom_fichier[pos-5] == 'b' ? true : false ;

}


void menu_save(int *joueur , TplateauJeu *j ,int *** jeusymb){

    char nom_fichier[200];
    char reponse[200];

    Rappelle_menu();
    scanf("%s",reponse);

    if (  strcmp(reponse,"S") == 0)
        Sauvegarde(j,joueur,jeusymb);
    else if (  strcmp(reponse,"C") == 0){
        // rajouter le concate /Sauvegarde
        printf("Donner le nom de la Sauvegarde  a charger? \n");
        affiche_log();
        scanf("%s",nom_fichier);
        charge_true_save ( nom_fichier, joueur , j, jeusymb );
    }
    else if (  strcmp(reponse,"D") == 0){
         printf("Souhaitez-vous supprime une sauvegarde? \n");
         affiche_log();
         scanf("%s",nom_fichier);
         find_true_save( nom_fichier);
    }
    else { printf("Vous_allez passe a la suite \n");}
}

void Sauvegarde(TplateauJeu* j , int* joueur ,int*** jeusymb){
    char nom_fichier[200];
    char reponse[10];
    bool marche = true ;

    printf("Donner un Nom a votre fichier de Sauvegarde ? \n");
    scanf("%s",nom_fichier);
    printf("Voulez-vous sauvegarder votre partie en Binaire ou format Txt ? \nTapez : BINAIRE || TEXT \n");

    while ( marche ){
    scanf("%s",reponse);
        if (  strcmp(reponse,"BINAIRE") == 0 ||  strcmp(reponse,"B") == 0 ){

            Concat_char(nom_fichier,"_b.txt");
            Save_sans_double(nom_fichier , j , joueur, jeusymb);
            marche = false ;
        }
        else if ( strcmp(reponse,"TEXT") == 0 || strcmp(reponse,"T") == 0){

            Concat_char(nom_fichier,"_t.txt");
            //Lance fonction sauvegarde
            Save_sans_double(nom_fichier , j , joueur,jeusymb);
            marche = false ;
        }
        else{printf("Vous-vous etes tromper \n"); } }

}


void Rappelle_menu(){
    printf("Tapez sur n'importe quel touche pour passer ou tour suivant \n");
    printf("Tapez sur S pour cree une sauvegarde/Save \n");
    printf("Tapez sur C pour charger une sauvegarde \n");
    printf("Tapez sur D pour Supprimer/Delete une sauvegarde \n");
    printf("\n");
}

void Concat_char ( char * nom_fichier , char * extension){
    char name[2000]  =  "Sauvegarde/\0" ;
    strcat(name,nom_fichier);
    strcat(name,extension);
    strcpy(nom_fichier,name);
}


void Save_sans_double( char * nom_fichier , TplateauJeu *j , int *joueur , int ***symb){
    char reponse[2000];
    if ( existe_log(nom_fichier) != NULL ){

        printf("Fichier Deja existant-voulez-vous ecraser l'ancienne sauvegarde");
        printf("Tapez  : 0 (oui)|| 1 (non) \n");
        scanf("%s",reponse);
         if ( strcmp(reponse,"0") == 0){
            delete_log(existe_log(nom_fichier));
            save_game(nom_fichier , *j , *joueur);
            save_joueur( *symb,nom_fichier);

            printf("Sauvegarde Effectuer \n");
         }
         else printf("Retour \n");
    }
    else {
        save_game(nom_fichier , *j , *joueur);
        save_joueur( *symb,nom_fichier);
        }
}

 void find_true_save ( char * nom_fichier){
    Concat_char(nom_fichier,"_t.txt");
    if ( existe_txt(nom_fichier)){

        delete_log(nom_fichier);
        printf("Fichier supprimer");
    }
    int pos = strlen(nom_fichier);
    nom_fichier[pos-5] = 'b';
    if ( existe_txt(nom_fichier)){

        delete_log(nom_fichier);
        printf("Fichier supprimer");
    }
}

 void charge_true_save ( char * nom_fichier,int *joueur ,TplateauJeu *j, int ***symb ){
    Concat_char(nom_fichier,"_t.txt");
    if ( existe_txt(nom_fichier)){

        //load_plateau
        load_plateau(nom_fichier, joueur , j);
        //load joueur
        load_joueur(nom_fichier,symb);
        printf("Fichier Charger \n");
    }
    int pos = strlen(nom_fichier);
    nom_fichier[pos-5] = 'b';
    if ( existe_txt(nom_fichier)){

        //load_plateau
        load_plateau(nom_fichier,joueur , j);
        //load joueur
        load_joueur(nom_fichier,symb);
        printf("Fichier Charger \n");
    }
}


char * str_sub( char *s, unsigned int start, unsigned int fin){
   char *new_s = NULL;

   if (s != NULL && start < fin)
   {
      new_s = malloc (sizeof (*new_s) * (fin - start + 2));
      if (new_s != NULL)
      {
         int i;
         for (i = start; i <= fin; i++)
         {
            new_s[i-start] = s[i];
         }
         new_s[i-start] = '\0';
      }
      else
      {
         fprintf (stderr, "Memoire insuffisante\n");
         exit (EXIT_FAILURE);
      }
   }
   return new_s;
}



