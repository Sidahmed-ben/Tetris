#include <stdlib.h>
#include <stdio.h>
#include "pieces.h"



piece  initialise_piece(void){ //  stocker toute les pieces dans une variable p de type  piece puis la retournèe
    
                                    // 0 
    piece p = {5,0, { {2, 2 , 2  ,{ {1,1},                //1
                                    {1,1}}},{3,  2 , 3  ,{ {1,0,0,0}, 
                                                           {1,1,1,0}}               //2
                                                                },{3,  3 , 2  ,{ {1,1},
                                                                               {1,0},
                                                                               {1,0},
                                                                               {0,0}}                //3
                                                                                    },{3,  2 , 3  ,{ {1,1,1,0}, 
                                                                                                     {0,0,1,0}}               //4
                                                                                                            },{3,  3 , 2  ,{{0,1},
                                                                                                                           {0,1},
                                                                                                                           {1,1},
                                                                                                                           {0,0}}                //5
                                                                                                                                },{4,  1 , 4  ,{{1,1,1,1}}          //6  
                                                                                                                                                       },{4,  4 , 1  ,{{1},
                                                                                                                                                                     {1},
                                                                                                                                                                     {1},
                                                                                                                                                                     {1}}              //7
                                                                                                                                                                        },{5,  2 , 3  ,{{1,1,0},
                                                                                                                                                                                     {0,1,1}}               //8
                                                                                                                                                                                            }, {5,  3 , 2  ,{{0,1},
                                                                                                                                                                                                            {1,1},
                                                                                                                                                                                                            {1,0}                //9
                                                                                                                                                                                                                }},{6,  2 , 3  ,{{0,1,1},
                                                                                                                                                                                                                               {1,1,0}}               //10
                                                                                                                                                                                                                                      },{6,  3 , 2  ,{{1,0},
                                                                                                                                                                                                                                                    {1,1},
                                                                                                                                                                                                                                                    {0,1}}                //11
                                                                                                                                                                                                                                                            },{7,  2 , 3  ,{{0,1,0},
                                                                                                                                                                                                                                                                          {1,1,1}}              //12
                                                                                                                                                                                                                                                                                  },{7,  3 , 2  ,{{1,0},
                                                                                                                                                                                                                                                                                                {1,1},
                                                                                                                                                                                                                                                                                                {1,0}}               //13
                                                                                                                                                                                                                                                                                                     },{7,  2 , 3  ,{{1,1,1},
                                                                                                                                                                                                                                                                                                                    {0,1,0}}             //14
                                                                                                                                                                                                                                                                                                                          },{7,  3 , 2  ,{{0,1},
                                                                                                                                                                                                                                                                                                                                           {1,1},
                                                                                                                                                                                                                                                                                                                                           {0,1}}                //15
                                                                                                                                                                                                                                                                                                                                                },{8,  2 , 3  ,{{0,0,1,0},
                                                                                                                                                                                                                                                                                                                                                                {1,1,1,0}                 //16
                                                                                                                                                                                                                                                                                                                                                                        }},{8,  3 , 2  ,{{1,0},
                                                                                                                                                                                                                                                                                                                                                                                         {1,0},
                                                                                                                                                                                                                                                                                                                                                                                         {1,1},
                                                                                                                                                                                                                                                                                                                                                                                         {0,0}}                   //17
                                                                                                                                                                                                                                                                                                                                                                                              },{8,  2 , 3  ,{{1,1,1,0}, 
                                                                                                                                                                                                                                                                                                                                                                                                              {1,0,0,0}                 //18
                                                                                                                                                                                                                                                                                                                                                                                                                      }},{8,  3 , 2  ,{{1,1},
                                                                                                                                                                                                                                                                                                                                                                                                                                       {0,1},
                                                                                                                                                                                                                                                                                                                                                                                                                                       {0,1},
                                                                                                                                                                                                                                                                                                                                                                                                                                       {0,0}}  
                                                                                                                                                                                                                                                                                                                                                                                                                                            }}}; 
                                                                                                                                                                                                                                                                                                                                                                                                                        
                                                                                                                                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                                                                                                                      
                                        
                                                                
                                                                
                                                                                                                              
                                                                                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                         
                                                                                                                                                                                                      
                                                                                                                                                                                             
                                                                                                                                                                                             
                                                                                                                                                                                              
    return p;
}



// fonction qui permet d'insérer la piece dans la grille du tetris 
void dessin_piece(piece p1, unsigned int * plat ,int ip){
  int W = 11;
  int i,j;
  for  (i =0 ; i < p1.t_formes[ip].haut_f; i++){
    for (j=0 ; j < p1.t_formes[ip].larg_f; j++){
      if (p1.t_formes[ip].t_forme[i][j] > 0){
        plat[(p1.cor_y + i) * W + (p1.cor_x + j) ]  = p1.t_formes[ip].col;
      }
    }
  }
}


// fonction qui permet de supprimer la piece de la grille du tetris
void supprime_piece(piece p1, unsigned int * plat ,int ip){
  int W = 11;
  int i,j;
  for (i =0 ; i < p1.t_formes[ip].haut_f;i++){
      for(j=0 ; j < p1.t_formes[ip].larg_f; j++){
        if (p1.t_formes[ip].t_forme[i][j] > 0){
          plat[(p1.cor_y + i) * W + (p1.cor_x + j) ]  = 0 ;
       } 
      }
    }
}


// fonction qui permet verifier la possibilitè
// du deplacement de la piece vers le bas 
int dep_bas_piece(piece p1,int ip ,unsigned int * plat ){
  int W = 11;
  int h = p1.t_formes[ip].haut_f;
  int l = p1.t_formes[ip].larg_f;
  int i=h-1 ,  j =  0 ;
  do{
    if (p1.t_formes[ip].t_forme[i][j] > 0 ){
      if( plat[ (p1.cor_y + i + 1 ) * W + (p1.cor_x + j) ]  > 0 ){
        return 0;
      }
    ++j; i=h-1;
    }else{i--;}
  }while(j<l);

  return 1;
}



// fonction qui permet verifier la possibilitè
// du deplacement de la piece vers la gauche 
int dep_gauche(piece p1 , int ip , unsigned int * plat ){
 int W = 11;
 int h = p1.t_formes[ip].haut_f;
 int i=0 ,  j = 0;
  do{
  if (p1.t_formes[ip].t_forme[i][j] > 0 ){
    if( plat[ (p1.cor_y + i ) * W + (p1.cor_x + j - 1) ]  > 0 ){
      return 0;
    }
   ++i; j=0;
  }else{j++;}
  }while(i<h);

 return 1; 
}


// fonction qui permet verifier la possibilitè
// du deplacement de la piece vers la droite
int dep_droite(piece p1,int ip , unsigned int * plat ){
   
  int W = 11;
  int h = p1.t_formes[ip].haut_f;
  int l = p1.t_formes[ip].larg_f;
  int i=0 ,  j = l -1 ;
  do{
    if (p1.t_formes[ip].t_forme[i][j] > 0 ){
      if( plat[ (p1.cor_y + i ) * W + (p1.cor_x + j + 1) ] > 0 ){
        return 0;
      }
    ++i; j=l-1;
    }else{j--;}
  }while(i<h);

  return 1; 

}

// fonction qui renvoie l'indice de la piece à tourner  
int rotation(int ip_fonc){
if ( ip_fonc == 0 ){
  return 0;

}else if (ip_fonc == 4){
  return 1 ;

}else if (ip_fonc == 6){
  return  5; 

}else if (ip_fonc == 8){
  return  7;  

}else if (ip_fonc == 10){
  return  9; 

}else if (ip_fonc == 14){
  return  11;

}else if (ip_fonc == 18){
  return  15;

}else {
  return ip_fonc + 1; 
 }
}

// fonction qui permet verifier la possibilitè
// de la rotation de la pièce 
int rotation_possible(piece p1,int ip ,unsigned int * plat ){
   int i ,  j;
   int W   = 11;
   int h   = p1.t_formes[ip].haut_f;
   int l   = p1.t_formes[ip].larg_f;
   int col = p1.t_formes[ip].col ;
    for (i = 0 ; i < h; i++){
    for(j = 0 ; j < l; j++){
      if (p1.t_formes[ip].t_forme[i][j] > 0 ){
        if( plat[ (p1.cor_y + i) * W + (p1.cor_x + j) ] != col  && plat[ (p1.cor_y + i) * W + (p1.cor_x + j) ] > 0 ){
          return 0;
        }
      }
    }
  }
  return 1;  
}



///  test qui permet de vèrifier s'il éxiste des lignes pleinnes
//   pour les supprimer 
void test_Remplissage(unsigned int * plat){
  int W = 11;
  int i,j,rempli = 1,ind_ligne = 21;
  do{ 
    rempli = 1;
    for (j=1 ; j<10; j++ ){
      if ( plat[ (ind_ligne) * W + j ] ==  0 ){
        rempli = 0;
        ind_ligne -= 1;
        break;
      }
    }
    if (rempli){
      for(i=ind_ligne ;i>0; i--){
        for (j=1 ; j<10; j++ ){
          plat[ i * W + j ] = plat[ (i-1) * W + j ];
        }
      }
    }
  }while(ind_ligne > 0);
}


