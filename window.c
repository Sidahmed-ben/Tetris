/*!\file window.c
 * \brief Utilisation du raster "maison" pour visualisation en vue du
 * dessus d'un début de casse briques.
 * \author Farès BELHADJ, amsi@up8.edu
 * \date December 4, 2020.
 */
#include <assert.h>
/* inclusion des entêtes de fonctions de gestion de primitives simples
 * de dessin. La lettre p signifie aussi bien primitive que
 * pédagogique. */
#include <GL4D/gl4dp.h>
/* inclure notre bibliothèque "maison" de rendu */
#include "moteur.h"
#include <time.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include "pieces.h"
#include "audioHelper.h"

/* inclusion des entêtes de fonctions de création et de gestion de
 * fenêtres système ouvrant un   contexte favorable à GL4dummies. Cette
 * partie est dépendante de la bibliothèque SDL2 */
//#include <GL4D/gl4duw_SDL2.h>


extern unsigned int * plateau(int w, int h);
/* protos de fonctions locales (static) */
static void init(void);
static void draw(void);
static void sortie(void);
static void descendre_piece();
static void keydown(int keycode);
Uint32      dep_tete_piece(Uint32 intervalle, void * parametre );


//=====================  Declaration des variables Globales =====================
static      piece   p1 ;
static      int     collision;
static      int     ip;
static      float   vf ,vd;
static      int     iF,iC ,C, F;
static      int     ip_pro;
static      int     stop;
static      GLuint  id2,id3;
//========================================================
/*!\brief une surface représentant un quadrilatère */
static surface_t * _quad = NULL;
static surface_t * _quad_fond = NULL;
/*!\brief une surface représentant une piece */
static surface_t * _piece = NULL;
/*!\brief le plateau */
static unsigned int * _plat = NULL;
/*!\brief la largeur du plateau */
static const int _pW = 11;
/*!\brief la hauteur du plateau */
static const int _pH = 23;
/*!\brief la largeur de la fenêtre */
static const int _wW = 800;
/*!\brief la hauteur de la fenêtre */
static const int _wH = 800;


  vec4 Noire  = {0.4f, 0.4f, 0.5f, 1};
  vec4 b      = {0.1f, 0.3f, 0.5f, 1};
  vec4 c2     = {0.7f, 1.0f, 0.5f, 1};
  vec4 c3     = {1.1f, 0.3f, 0.9f, 1};
  vec4 c4     = {1.0f, 0.8f, 0.3f, 1};
  vec4 c5     = {0.5f, 0.0f, 1.0f, 1};
  vec4 c6     = {0.9f, 0.5f, 0.1f, 1};
  vec4 c7     = {1.0f, 0.4f, 0.5f, 1};
  vec4 c8     = {0.2f, 0.8f, 0.2f, 1};
 

/*!\brief paramètre l'application et lance la boucle infinie. */
int main(int argc, char ** argv) {
  /* tentative de création d'une fenêtre pour GL4Dummies */
  if(!gl4duwCreateWindow(argc, argv, /* args du programme */
			 "Mon moteur de rendu <<Maison>>", /* titre */
			 10, 10, _wW, _wH, /* x, y, largeur, heuteur */
			 GL4DW_SHOWN) /* état visible */) {
    /* ici si échec de la création souvent lié à un problème d'absence
     * de contexte graphique ou d'impossibilité d'ouverture d'un
     * contexte OpenGL (au moins 3.2) */
    return 1;
  }
  /* Pour forcer la désactivation de la synchronisation verticale */
 // SDL_GL_SetSwapInterval(0);
  init();
  /* création d'un screen GL4Dummies (texture dans laquelle nous
   * pouvons dessiner) aux dimensions de la fenêtre */
  gl4dpInitScreen();
  /* mettre en place la fonction de display */
  gl4duwDisplayFunc(draw);
  gl4duwKeyDownFunc(keydown);
  SDL_AddTimer(300,dep_tete_piece,NULL);  
  //ahInitAudio("djf.mp3");
  gl4duwMainLoop();
  return 0;
}

/////////////   initialisation  de nos donnèes   ///////////
void init(void) {
  vf = vd = 0;
  iF = 1;
  F = 1;
  C = 0;
  iC = 1;
  collision = 0;
  p1 = initialise_piece();
  ip_pro = 14;
  stop = 0;
  GLuint id  = getTexFromBMP("images/tex1.bmp");
  id2        = getTexFromBMP("images/fond.bmp");
  id3        = getTexFromBMP("images/grand_fond.bmp");
  /* on créé nos 3 types de surfaces */
  _quad_fond =   mkQuad();
  _quad      =   mkQuad();        /* ça fait 2 triangles        */
  _piece     =   mkCube();        /* ça fait 2x6 triangles      */
  /* on change les couleurs de surfaces */
  
  _quad_fond->dcolor =   Noire;
  _quad     ->dcolor =   Noire; 
  _piece     ->dcolor =   b; 

  /* on leur rajoute la texture  */
  
  setTexId(  _quad_fond, id3);
  setTexId(  _quad,      id2);
  setTexId(  _piece,      id);

  /* on active l'utilisation de la texture pour les surfaces */
  enableSurfaceOption(  _quad_fond, SO_USE_TEXTURE);
  enableSurfaceOption(  _quad, SO_USE_TEXTURE);
  enableSurfaceOption(  _piece, SO_USE_TEXTURE);

  
  /* on active l'ombrage */
  enableSurfaceOption(  _quad_fond, SO_USE_LIGHTING);
  enableSurfaceOption(  _quad, SO_USE_LIGHTING);
  enableSurfaceOption(  _piece, SO_USE_LIGHTING);


  _plat = plateau(_pW, _pH);

  atexit(sortie);
}




//////     brief la fonction appelée à chaque display   ///////
void draw(void) {

  int i, j;
  float mvMat[16], projMat[16], nmv[16];
  ////  effacer l'écran et le buffer de profondeur ////// 
  gl4dpClearScreenWith (RGB(0, 0, 0));
  clearDepth();

  MFRUSTUM(projMat, -0.05f, 0.05f,
	   -0.05f * _wH / _wW ,////  pour garder le ratio /////
	   0.05f * _wH / _wW , //// pour garder le ratio  /////
	   0.1f, 1000.0f);
  
  MIDENTITY(mvMat);
    
    
  if  (F)/////////////////////////   vue en face de la grille  ////////////////////////////////////
    lookAt(mvMat,0 ,55-(vf/3) , -1-(iF*(vf/2))  , 0, 0, 0, 0, 1+(iF*(vf/2)), 0);
    
  else if (C)  // /////////////////////   vue a cotè de la grille  /////////////////////////
    lookAt(mvMat, iC*(-1-(vd/2)) ,55-(vd/3) , 0  , 0, 0, 0, 0, 1, 0);



  ////////////////// utilisation du quad comme fond de la grille  //////////////
  memcpy(nmv, mvMat, sizeof nmv); /* copie mvMat dans nmv */
  translate(nmv, 5, -1 , 0 ); 
  rotate(nmv, -90.0f, 1.0f, 0.0f, 0.0f);
  scale(nmv, 11 , 23, 0.0f);
  transform_n_raster(_quad, nmv, projMat);


  //////////////////utilisation du quad comme fond de la prochaine piece a venir ////////////////
  memcpy(nmv, mvMat, sizeof nmv); /* copie mvMat dans nmv */
  translate(nmv, -18, -1 , 4); 
  rotate(nmv, -90.0f, 1.0f, 0.0f, 0.0f);
  scale(nmv, 8 , 6, 0.0f);
  transform_n_raster(_quad, nmv, projMat);
 

///////////////// utilisation du _quad_fond pour afficher le grand fond  en arrière du jeux ///////////////
  memcpy(nmv, mvMat, sizeof nmv); 
  translate(nmv, 0, -60 , 0 ); 
  rotate(nmv, -90.0f, 1.0f, 0.0f, 0.0f);
  scale(nmv, 60 , 60, 60);
  transform_n_raster(_quad_fond, nmv, projMat); 



  //////////////////  dessin du cadre qui entoure la prochaine piece  ///////////////////////////
    for( i = 0 ; i <= 6 ; i++ ){
      for  ( j = 0 ; j <= 7 ; j++){
      if ( i == 0 || i==6 || j==0 || j == 7){
        memcpy(nmv, mvMat, sizeof nmv);
        translate(nmv, -10 - (2.15*j), 0, 10 - (2.15*i)  );
        transform_n_raster(_piece, nmv, projMat);
      }
    }
  }
    ///////////     appliquer  la couleur à la prochaine piéce  /////////////////// 
   switch(p1.t_formes[ip_pro].col) {
        case  1 :
          _piece  -> dcolor = b;
          break;
        case  2 :
          _piece  -> dcolor = c2;
          break;
        case  3 :
          _piece  -> dcolor = c3;
          break;
        case  4:
          _piece  -> dcolor = c4;
          break;
        case  5:
         _piece  -> dcolor = c5;
          break;
        case  6:
          _piece  -> dcolor = c6;
          break;
        case  7:
          _piece  -> dcolor =c7;
          break;
        case  8:
          _piece  -> dcolor =c8;
          break;
        default:
          break;
      }
  ////////////////    affichage de la prochaine pièce à venir  /////////////////////////////
   for( i = 0 ; i <= 3 ; i++ ){
   for( j = 0 ; j <= 3 ; j++){
      if (p1.t_formes[ip_pro].t_forme[i][j] > 0){
        memcpy(nmv, mvMat, sizeof nmv);
        translate(nmv, -14 - (2.15*j), 0, 6 - (2.15*i)  );
        transform_n_raster(_piece, nmv, projMat);
      }
    }
  }

  ///////////////////   Boucle qui affiche la grille du tetris  ////////////////////////// 
  for(i = (-_pH / 2)+2; i <= _pH / 2; ++i){      
    for(j = -_pW / 2; j <= _pW / 2; ++j) {
      if(_plat[(i + _pH / 2) * _pW + j + _pW / 2] == 0) continue;
       memcpy(nmv, mvMat, sizeof nmv);
       translate(nmv,(_pW / 4) - (2.15 * j)+3 , 0 , (_pH /4) - (2.15 * i) -3  ) ; 
       switch(_plat[((i + _pH / 2)) * _pW + j + _pW / 2]) {
        case  1 :
          _piece  -> dcolor = b;
          break;
        case  2 :
          _piece  -> dcolor = c2;
          break;
        case  3 :
          _piece  -> dcolor = c3;
          break;
        case  4:
          _piece  -> dcolor = c4;
          break;
        case  5:
          _piece  -> dcolor = c5;
          break;
        case  6:
          _piece  -> dcolor = c6;
          break;
        case  7:
          _piece  -> dcolor =c7;
          break;
        case  8:
          _piece  -> dcolor =c8;
          break;
        default:
          break;
        }
        transform_n_raster(_piece, nmv, projMat);
     }
  }
  ////// déclarer qu'on a changé (en bas niveau) des pixels du screen  //////////
  gl4dpScreenHasChanged();
  ///////  fonction permettant de raffraîchir l'ensemble de la fenêtre   /////////
  gl4dpUpdateScreen(NULL);
}


///////////  prototype de la fonction qui permet d'appeler 
//////////   la fonction descendre_piece chaque 300 ms
Uint32 dep_tete_piece(Uint32 intervalle, void * parametre ){
  descendre_piece();
  return intervalle ;
}


//////////////// fonction qui permet de gèrer le deplacement automatique  /////////////////////////
////////////////       de la piece vers le bas chaque 300 ms          ////////////////////////////

void descendre_piece(){
 int j;
 if (!stop){
   if ( dep_bas_piece(p1,ip,_plat) ) { ///////  verifier la collision avec le prochain element de la grille  /////////
    supprime_piece(p1,_plat,ip);  ///////    supprimer la trace de la piece dans  la grille     ///////////// 
    p1.cor_y += 1;               ////////    Avancer la piece vers  la prochaine ligne de la grille     //////////
    dessin_piece(p1, _plat, ip); ////////   réinscrir la piece dans la nouvelle(prochaine) ligne de la grille     ////////
    }else{
    ////////////       verification si les pieces  depasses  la limite     /////////////////// 
    ///////////////         (3ème ligne de la grille du  tetris )        ////////////////////
    for(j=1 ; j<10;j++){
      if ((_plat[( 2 ) * _pW + j ] > 0)){
        fprintf(stderr , "//////////////////////////////////////////////////// \n ");
        fprintf(stderr , "//////////////////////////////////////////////////// \n ");
        fprintf(stderr , "///////////////                        //////////////// \n ");
        fprintf(stderr , "///////////////      P E R D U         ////////////// \n ");
        fprintf(stderr , "///////////////                        //////////////// \n ");
        fprintf(stderr , "///////////////  PRESS 'R' TO RESTART  //////////////// \n ");
        fprintf(stderr , "//////////////////////////////////////////////////// \n ");
        fprintf(stderr , "//////////////////////////////////////////////////// \n ");
        stop = 1 ;
        break;
      }
    }
    test_Remplissage(_plat);  /////  tester s'il existe des lignes  remplies pour les effacer ////////
    p1.cor_x = 5;
    p1.cor_y = 0;
    ip = ip_pro ;     
    ip_pro = (int)(rand() / (double)RAND_MAX * (19 - 1));
    srand(time(NULL)^ip_pro);    // pour que le rand dans la prochaine fois donne des valeur alèatoire diferente de celle donnè avant 
                                //(a chaque fois quand exècute le programe il nous donne des valeurs diffèrentes du programme prècèdent)
    }
  }
}

static void keydown(int keycode) {
  switch(keycode) {
  /////////////     clicker sur eshape pour quitter le jeux   //////////////     
  case GL4DK_ESCAPE:
    exit(0);
    break;
  case GL4DK_LEFT:  ////  deplacement à gauche de la piece ////////
    if( dep_gauche(p1 , ip , _plat)){ 
      supprime_piece(p1, _plat ,ip);
      p1.cor_x -= 1;
      dessin_piece(p1,  _plat, ip);
    }
    break;
  case GL4DK_RIGHT:  ////////  de placement à droite de la piece ////////
    if(dep_droite(p1,ip,_plat)){ 
      supprime_piece(p1, _plat,ip);
      p1.cor_x += 1;
      dessin_piece(p1, _plat, ip);
    }
    break;
  case GL4DK_DOWN:  //////   accelerer la piece vers le bas //////////
    if(dep_bas_piece(p1,ip,_plat)){
      supprime_piece(p1, _plat,ip);
      p1.cor_y += 1; 
      dessin_piece(p1, _plat, ip);
    }
    break;
  case GL4DK_UP:  //////    faire la rotation de la piece    /////////////
    if ( rotation_possible( p1,rotation(ip) ,_plat ) ){  
      supprime_piece(p1, _plat,ip);
      ip = rotation(ip);
      dessin_piece(p1, _plat, ip);
    }
  ///////  Partie qui  CALCUL  l'angle de vue  du face et du cotè      //////////  
    break;
  case GL4DK_b:
   if(F){
    if(vf<60){
     vf += 0.7;
    }}else{
     if(vd<60){
     vd += 0.7; 
    }}
    break;
  case GL4DK_h:
    if(F){
    if(vf>0){ 
     vf -= 0.7;
    }}else{
     if(vd>0){ 
     vd -= 0.7;
    }}
    break;

  ////////////      partie pour CHOISIR  le point  de vue (Cotè , Face)       //////////////////// 
  case GL4DK_f:
    F = 1;
    C = 0;
    break;
  case GL4DK_c:
    F = 0;
    C = 1;
    break;

  /////////////     évènement qui permet de basculer entre la vue de haut et la vue du bas     /////////////
  case GL4DK_k:
    if (iF == 1){
     iF = -1 ;
    }else{
     iF = 1;
    }
    break;

  //////////////     évènement qui permet de basculer entre la vue de gauche et la vue du droite   //////////
  case GL4DK_l:
    if (iC == 1){
     iC = -1 ;
    }else{
     iC = 1;
    }
    break;

  ///// ///    évènement qui permet la rèinitialisation du jeux 
  case GL4DK_r:
   if(stop){
    _plat = plateau(_pW, _pH);
    stop = 0;
   }
    break;
  ///// ///    évènement qui permet de méttre un pause au jeux 
  case GL4DK_p:
   if(stop){
    stop = 0;
   }else{
    stop =  1;
   }
    break;
  default:
    break;
  }
}



/*!\brief à appeler à la sortie du programme. */
void sortie(void) {
  ///////    on libère nos surfaces  
  if(_quad) {
    freeSurface(_quad);
    _quad = NULL;
  }
  if(_piece) {
    freeSurface(_piece);
    _piece = NULL;
  }
   if(_quad_fond) {
    freeSurface(_quad_fond);
     _quad_fond = NULL;
  }
  ////   on libère le plateau  /////// 
  free(_plat);
  //////    libère tous les objets produits par GL4Dummies, ici  ////// 
  //////    principalement les screen     /////// 
  gl4duClean(GL4DU_ALL);
}