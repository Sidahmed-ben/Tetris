#ifndef PIECES_H_
#define PIECES_H_


typedef struct piece piece ;
typedef struct forme forme ;

void  test_Remplissage(unsigned int * plat);
void  dessin_piece(piece p1, unsigned int * plat ,int ip);
void  supprime_piece(piece p1, unsigned int * plat ,int ip);  
int   dep_bas_piece(piece p1,int ip_f ,unsigned int  * plat );
int   dep_gauche(piece p1 , int ip_f , unsigned int * plat );
int   dep_droite(piece p1,int ip_f , unsigned int * plat );
int   rotation_possible(piece p1,int ip_f ,unsigned int * plat );
int   rotation( int ip);
piece initialise_piece(void);


struct forme {
    int col;
    int haut_f;
    int larg_f;
    int t_forme[4][4];

};

struct piece {
    int cor_x ;
    int cor_y ;
    forme t_formes[19];
};

#endif