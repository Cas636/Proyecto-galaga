/*Galaga, Proyecto Final de Programación Básica.
Presentado por:
Johan Castaño Martinez 20191020029
Santiago Gómez Quintero 20191020006
*/
#include <iostream>
#include <allegro.h>
#include <stdlib.h>
#include <math.h>
#include "inicia.h"
using namespace std;

const int maxdisp = 9, ANCHO = 1300, ALTO = 720, maxEnemies=15;//maximo de disparos permitido,ancho y alto de pantalla
int timeEnem=0, nEnemies=0, fase=1, fila=1, DeadEnem=0, timeFall=1, lifes=3, enemFall;
int timeExp=0, xExp, yExp, FallBee=0, Bee, Esc, timeEsc=1;

struct NAVE{
    int x,y;//coordenadas
    int dir;//dirección
    int ndisparos;//número de disparos efectuados y el numero de disparos que e ven en pantalla
}nav = {650,600,1,0};//variable de la estructura con coordenadas dirección y número de disparos (es cero para que solo cuando se presiona la tecla espacio la variable crezca) quese ven en pantalla

struct MALOS{
    int x,y;//coordenadas
    int xf, yf;
    int dx, dy;
    int frame;//script
    int type;
    bool exist;
}enem[maxEnemies];//variable de la estructura con coordenadas dirección y número de disparos (es cero para que solo cuando se presiona la tecla espacio la variable crezca) quese ven en pantalla

struct Boss{
    bool exist;
    int hits;
}boss;

struct DISPBee{
    int x, y;
    int dy;
    bool exist;
}dispB;

struct DISPEsc{
    int x, y;
    int dx, dy;
    bool exist;
}dispE;

struct Balas{
   int x, y;//coordenadas
   int dx, dy;//incrementos de las variables x,y. si se quiere que el disparo vaya para abajo o para arriba o los lados. esto hace el efecto de movimiento
}disparos[maxdisp];//disparos[maxdisp];//variable,arreglo de la estructura, que son 9 balas como dice la variable de arriba

void pintar_nave(BITMAP *nave,BITMAP *buffer,struct NAVE nav)
{
  masked_blit( nave, buffer, nav.dir, 0, nav.x, nav.y, 90, 97);
}
//Funciones prototipo
void inicio(bool salida, BITMAP *Menu1, BITMAP *Menu2, BITMAP *Menu3, BITMAP *Menu4, BITMAP *Cursor, BITMAP *buffer, int ANCHO, int ALTO,BITMAP *Enem11, BITMAP *Enem21, BITMAP *Enem31, BITMAP *bala, int dsw, int contt, int i, struct NAVE nav, struct Balas disparos[maxdisp], int maxdisp, BITMAP *nube, BITMAP *nave, BITMAP *explosion, BITMAP *aguijon, BITMAP *Jefe1);
void juego(int ANCHO, int ALTO,BITMAP *Enem11, BITMAP *Enem21, BITMAP *Enem31, BITMAP *bala, int dsw, int contt, int i, BITMAP *buffer, struct NAVE nav, struct Balas disparos[maxdisp], int maxdisp, BITMAP *nube, BITMAP *nave, BITMAP *explosion, BITMAP *aguijon, BITMAP *Jefe1);
bool enemScreen();

int main()
{
    inicia_allegro(1300,720);
    inicia_audio(70,70);
    install_mouse();

    int i = 720, dsw = 0, contt = 0;
    bool salida = false;

    BITMAP *Menu1 = load_bitmap("imagenes/Menu1.bmp",NULL);//Cada uno de los map carga el archivo de imagen del juego.
    BITMAP *Menu2 = load_bitmap("imagenes/Menu2.bmp",NULL);
    BITMAP *Menu3 = load_bitmap("imagenes/Menu3.bmp",NULL);
    BITMAP *Menu4 = load_bitmap("imagenes/Menu4.bmp",NULL);
    BITMAP *Cursor = load_bitmap("imagenes/Cursor.bmp",NULL);
    BITMAP *nave = load_bitmap("imagenes/nave.bmp",NULL);
    BITMAP *nube = load_bitmap("imagenes/nube.bmp",NULL);
    BITMAP *bala = load_bitmap("imagenes/bala2.bmp",NULL);
    //ENEMIGOS
    BITMAP *Enem11 = load_bitmap("imagenes/Mariposa.bmp",NULL);
    BITMAP *Enem12 = load_bitmap("imagenes/Mariposa_2.bmp",NULL);
    BITMAP *Enem21 = load_bitmap("imagenes/Bee.bmp",NULL);
    BITMAP *Enem22 = load_bitmap("imagenes/Bee-2.bmp",NULL);
    BITMAP *Enem31 = load_bitmap("imagenes/Escarabajo.bmp",NULL);
    BITMAP *Enem32 = load_bitmap("imagenes/Escarabajo 2.bmp",NULL);
    BITMAP *Jefe1 = load_bitmap("imagenes/Araña.bmp",NULL);
    BITMAP *Jefe2 = load_bitmap("imagenes/Araña 2.bmp",NULL);
    BITMAP *explosion = load_bitmap("imagenes/Explosion.bmp",NULL);
    BITMAP *aguijon = load_bitmap("imagenes/Aguijon.bmp",NULL);
    BITMAP *buffer = create_bitmap(1300,720);

    //variable que controla hasta cuando se ejecuta el menu
    inicio(salida, Menu1, Menu2, Menu3, Menu4, Cursor, buffer, ANCHO, ALTO, Enem11, Enem21, Enem31, bala, dsw, contt, i, nav, disparos, maxdisp, nube, nave, explosion, aguijon, Jefe1);//Menu

    destroy_bitmap(buffer);
    destroy_bitmap(nave);
    destroy_bitmap(nube);
    destroy_bitmap(bala);
    destroy_bitmap(Menu1);
    destroy_bitmap(Menu2);
    destroy_bitmap(Menu3);
    destroy_bitmap(Menu4);
    destroy_bitmap(Cursor);
    destroy_bitmap(Enem11);
    destroy_bitmap(Enem12);
    destroy_bitmap(Enem21);
    destroy_bitmap(Enem22);
    destroy_bitmap(Enem31);
    destroy_bitmap(Enem32);
    destroy_bitmap(Jefe1);
    destroy_bitmap(Jefe2);
    destroy_bitmap(explosion);

    return 0;
}
END_OF_MAIN();

void inicio(bool salida, BITMAP *Menu1, BITMAP *Menu2, BITMAP *Menu3, BITMAP *Menu4, BITMAP *Cursor, BITMAP *buffer, int ANCHO, int ALTO,BITMAP *Enem11, BITMAP *Enem21, BITMAP *Enem31, BITMAP *bala, int dsw, int contt, int i, struct NAVE nav, struct Balas disparos[maxdisp], int maxdisp, BITMAP *nube, BITMAP *nave, BITMAP *explosion, BITMAP *aguijon, BITMAP *Jefe1){
    while (!salida){
        fase=1;
        lifes=3;
        if (mouse_x > 397 && mouse_x <933 && mouse_y > 313 && mouse_y < 405){
            blit(Menu2,buffer,0,0,0,0,1300,720);//cuando el raton esta sobre la palabra juego, cambia al menu 2
            if(mouse_b & 1){//si el mouse esta dando click izquierdo, si fuera derecho seria 2
                juego(ANCHO, ALTO, Enem11, Enem21, Enem31, bala, dsw, contt, i, buffer, nav, disparos, maxdisp, nube, nave, explosion, aguijon, Jefe1);//Juego o primer nivel;//Juego o primer nivel
            }
        }
        else if (mouse_x > 239 && mouse_x < 1085 && mouse_y > 417 && mouse_y < 519){
            blit(Menu3,buffer,0,0,0,0,1300,720);//cuando el raton esta sobre la palabra puntaje, cambia al menu 2
            if(mouse_b & 1){//si el mouse esta dando click izquierdo, si fuera derecho seria 2
                salida = true;//se hace verdad y termina el ciclo
            }
        }
        else if (mouse_x > 417 && mouse_x < 909 && mouse_y > 535 && mouse_y < 636){
            blit(Menu4,buffer,0,0,0,0,1300,720);//cuando el raton esta sobre la palabra salir, cambia al menu 2
            if(mouse_b & 1){//si el mouse esta dando click izquierdo, si fuera derecho seria 2
                exit(0);//funcion que cierra el programa
            }
        }
        else  blit(Menu1,buffer,0,0,0,0,1300,720);

        masked_blit(Cursor,buffer,0,0,mouse_x,mouse_y,22,22);
        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
    }
}

void juego(int ANCHO, int ALTO,BITMAP *Enem11, BITMAP *Enem21, BITMAP *Enem31, BITMAP *bala, int dsw, int contt, int i, BITMAP *buffer, struct NAVE nav, struct Balas disparos[maxdisp], int maxdisp, BITMAP *nube, BITMAP *nave, BITMAP *explosion, BITMAP *aguijon, BITMAP *Jefe1){
    while(!key[KEY_ESC] && (lifes>0 && fase!=4)){
        blit(nube,buffer,0,--i,0,0,1300,720);  if(i == 0) i=720;
        pintar_nave(nave,buffer,nav);

       if(key[KEY_RIGHT] && nav.x <= ANCHO-60)
       { nav.dir = 2; nav.x += 9; }

       else if(key[KEY_LEFT] && nav.x >= 5)
       { nav.dir = 0; nav.x -= 9; }

       else nav.dir = 1;

        //creacion enemigos por fase
        if(timeEnem++ == 0 && nEnemies < maxEnemies && fase!=4){
            if(fase==1){
                if(fila==1 || fila==3)enem[nEnemies].x=0;
                if(fila==2)enem[nEnemies].x=1300;
                enem[nEnemies].xf=((nEnemies%5)*185)+185;
                enem[nEnemies].yf=100*fila;
                enem[nEnemies].y=720;
                enem[nEnemies].dx=5;
                enem[nEnemies].dy=-5;
                enem[nEnemies].frame=0;
                enem[nEnemies].type=1;
                enem[nEnemies].exist=true;
                nEnemies++;
                if(nEnemies%5==0)fila++;
            }
            if(fase==2){
                if(fila==1 || fila==3)enem[nEnemies].x=0, enem[nEnemies].type=1;
                if(fila==2)enem[nEnemies].x=1300, enem[nEnemies].type=2;
                enem[nEnemies].xf=((nEnemies%5)*185)+185;
                enem[nEnemies].yf=100*fila;
                enem[nEnemies].y=720;
                enem[nEnemies].dx=5;
                enem[nEnemies].dy=-5;
                enem[nEnemies].frame=0;
                enem[nEnemies].exist=true;
                nEnemies++;
                if(nEnemies%5==0)fila++;
            }
            if(fase==3){
                if(fila==1)enem[nEnemies].x=0, enem[nEnemies].type=1;
                if(fila==3)enem[nEnemies].x=0, enem[nEnemies].type=3;
                if(fila==2)enem[nEnemies].x=1300, enem[nEnemies].type=2;
                enem[nEnemies].xf=((nEnemies%5)*185)+185;
                enem[nEnemies].yf=100*fila;
                enem[nEnemies].y=720;
                enem[nEnemies].dx=5;
                enem[nEnemies].dy=-5;
                enem[nEnemies].frame=0;
                enem[nEnemies].exist=true;
                nEnemies++;
                if(nEnemies%5==0)fila++;
            }
        }
        if(timeEnem > 50) timeEnem=0;

        if(fase==4 && enemScreen()){
            //Enemigo 1
            enem[0].x=300;
            enem[0].y=300;
            enem[0].xf=300;
            enem[0].yf=300;
            enem[0].type=(rand()%3)+1;
            enem[0].exist=true;

            //Enemigo 2
            enem[1].x=1000;
            enem[1].y=300;
            enem[1].xf=1000;
            enem[1].yf=300;
            enem[1].type=(rand()%3)+1;
            enem[1].exist=true;

            //Enemigo 3
            enem[3].x=620;
            enem[3].y=300;
            enem[3].xf=620;
            enem[3].yf=300;
            enem[3].type=(rand()%3)+1;
            enem[3].exist=true;
        }

        /*if(boss.exist==true){
            masked_blit(Jefe1,buffer,0,0,640,50,75,80);
        }*/

        if(nEnemies != 0){
            for(int c=0;c<nEnemies;c++){
                if(enem[c].exist==true){
                    switch(enem[c].type){
                        case 1: masked_blit(Enem11,buffer,0,0,enem[c].x,enem[c].y,75,80);
                        break;
                        case 2: masked_blit(Enem21,buffer,0,0,enem[c].x,enem[c].y,75,80);
                        break;
                        case 3: masked_blit(Enem31,buffer,0,0,enem[c].x,enem[c].y,75,80);
                        break;
                    }
                    if(enem[c].frame==0)enem[c].frame=1;
                    if(enem[c].frame==1)enem[c].frame=0;
                    if(enem[c].x!=enem[c].xf){
                        if(enem[c].x < enem[c].xf)enem[c].x+=enem[c].dx;
                        if(enem[c].x > enem[c].xf)enem[c].x-=enem[c].dx;
                    }
                    if(enem[c].y!=enem[c].yf){
                        enem[c].y+=enem[c].dy;
                    }
                }
                if(enem[c].y>ALTO){
                    enem[c].exist==false;
                    DeadEnem+=1;
                }
            }
        }

       if (key[KEY_P]){
           for(int c = 0; c < 1; c = c){
               masked_blit(Enem11, buffer, 0, 0, 185, 50, 75, 80);
               rest(7);
               if(key[KEY_P]){
                   c++;
               }
           }
       }

        //Rutina Disparos
        if(key[KEY_SPACE] && dsw == 0){//tecla espacio para que dispare
            if(nav.ndisparos < maxdisp){//rutina para crear balas si esta en el rango permitido de disparos
                nav.ndisparos++;//crece en un disparo
                disparos[nav.ndisparos].x = nav.x+5;//coordenada x del disparo la estructura desparos su indice aumenta entonces el disparo toma la posición de la nave mas 20 pixeles para que quede centrado
                disparos[nav.ndisparos].y = nav.y;//coordenada y toma las mismas coordenadas que la nave
                disparos[nav.ndisparos].dx = 0;//es 0 porque los disparos no tienen ninun desplazamiento horizontal
                disparos[nav.ndisparos].dy = -3;//es -5 porque las coordenadas estan invertidas si quiere que avance hacia arriba entonces le resta
                dsw = 1;
                }
        }
        if(contt++ > 40 ){//la variable contt se revisa y al mismo tiempo que se hace la iteración aumenta para que no se disparen todas las balas al mismo tiempo
            dsw = 0;//cuando entran al bucle se vuelven cero
            contt=0;
        }
        if(nav.ndisparos > 0){//cuan hay un disparo en juego(cuando ya se presiono espacio)
            for(int cont = 1; cont <= nav.ndisparos; cont++){//pintar las balas en juego desde 1 porque empieza a guardar datos de las bals desde 1, menor o igual al número de disparos realizados
            disparos[cont].x += disparos[cont].dx;//coordenadas x de la bala cuando aumenta
            disparos[cont].y += disparos[cont].dy;//coordenadas y de la bala cuando aumenta
            masked_blit(bala, buffer, 0, 0,disparos[cont].x ,disparos[cont].y, 50, 50);//pintar la bala, la función masked blit respeta trasparencia, se imprime esa bala sobre el buffer con las coordenadas anteriores, al final se especifica el tamaño del bitmap bala que es 50x50pixeles las definidas en la imagen

               if(disparos[cont].y < 0){//evalua que las balas salen de pantalla
                disparos[cont] = disparos[nav.ndisparos];//el indice de la bala que esta fuera= al indice de la bala que si esta dentro de pantalla
                nav.ndisparos--;//se esta restando para que ya no se llene la variable y permita seguir disparando
                    if(nav.ndisparos < 0 ) nav.ndisparos = 0;//cuando los n (9) disparos de la nave sea menor a 0 se haga igual a 0 para evitar errores
                }
            }
        }

        //caida enemigos
        do{
            enemFall=rand()%15;
        }while(enem[enemFall].exist==false);
        if(enem[enemFall].x==enem[enemFall].xf && enem[enemFall].y==enem[enemFall].yf){
            if(timeFall++ == 0){
                enem[enemFall].dy=5;
                enem[enemFall].yf=800;
            }
            if(timeFall==500)timeFall=0;
        }

       //verificar colisiones disparo-enemigo
        for(int c1=0;c1<nEnemies; c1++){
            for(int c2=1; c2<nav.ndisparos; c2++){
                if((disparos[c2].x<enem[c1].x+75)&&(enem[c1].x<disparos[c2].x+50)&&(disparos[c2].y<enem[c1].y+80)&&(enem[c1].y<disparos[c2].y+50)){
                    enem[c1].exist=false;
                    disparos[c2].y=-100;
                    xExp=enem[c1].x;
                    yExp=enem[c1].y;
                    enem[c1].y=800;
                    DeadEnem+=1;
                    timeExp=1;
                }
            }
        }
        if(timeExp!=0)
        {
            masked_blit(explosion,buffer,0,0,xExp,yExp,100,88);
            timeExp++;
            if(timeExp==50)timeExp=0;
        }

        //Colisiones nave-enemigo
        for(int c1=0;c1<nEnemies; c1++){
            if((enem[c1].x<nav.x+90)&&(nav.x<enem[c1].x+75)&&(enem[c1].y<nav.y+97)&&(nav.y<enem[c1].y+80)){
                enem[c1].exist=false;
                lifes--;
                xExp=nav.x;
                yExp=nav.y;
                enem[c1].y=800;
                DeadEnem+=1;
                timeExp=1;
            }
        }

        //Cambio de fase
        if(enemScreen() && fase!=4){
            fase++;
            nEnemies=0;
            DeadEnem=0;
            fila=1;
            timeEnem=0;
            if(fase==4)boss.exist=true, boss.hits=0;
        }
        if(fase==4 && boss.hits==25)fase=5;
    blit(buffer,screen,0,0,0,0,1300,720);
    }

}
bool enemScreen()
{
    bool r=true;
    for(int c=0; c<nEnemies; c++)
    {
        if(enem[c].y<=720)r=false;
    }
    return r;
}

