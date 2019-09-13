
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h> 
#include "plano.h"
#include "parede.h"

GLint WIDTH =320;
GLint HEIGHT=240;


GLfloat plano_difusa[]    = { 0.3, 1, 1, 1.0 };
GLfloat plano_especular[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat plano_brilho[]    = { 50.0 };

GLfloat mat_a_difusa[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_brilho[]    = { 50.0 };

GLfloat mat_b_difusa[]    = { 0.7, 0.7, 0.7, 0.5 };
GLfloat mat_b_especular[] = { 1.0, 1.0, 1.0, 0.5 };
GLfloat mat_b_brilho[]    = { 50.0 };

GLfloat posicao_luz0[]    = { 0, 15, 5, 1.0};
GLfloat cor_luz0[]        = { 1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz0_amb[]    = { 0.3, 0.3, 0.3, 1.0};

GLfloat posicao_luz1[]    = { 0.0, 10.0, 5.0, 1.0};
GLfloat cor_luz1[]        = { 0.0, 0.0, 1.0, 1.0};
GLfloat direcao_luz1[]    = { 0.0, -10.0, -5.0, 1.0};



#define QUANT_TEX 1 
unsigned int id_texturas[QUANT_TEX]; 


clock_t tempo[2];
int braco=1;
int bracof=1;

struct cubos{
	float x;
	float y;
	float z;
	bool on;
	float raio;
};

struct Player{
	float x;
	float z;
	bool on;
	bool explo;
};


Player bomba;
Player person;
Player fantasma[4];
const GLfloat vet_borda[4][3] = {{-9,0.5,0},{9,0.5,0},{0,0.5,9},{0,0.5,-9}};
int col;
bool chave=true;
int visao= 15;

cubos blocos[25];
cubos cube[25];
cubos explo[25];

 GLfloat luzCubo_borda[] = { 0.3, 0.3, 0.2, 0 };
 GLfloat luzCubo_interno[] = {0.1,0.1,0.1,1};
 //GLfloat luzCubo[] = {0.55,0.41,0.13,1};
 GLfloat luzBlocos[] = {0.1,0.1,0.1,0.5};
 GLfloat luzBloco2[] = {0.54,0.27,0.07,0.5};
 GLfloat luzCubo[] = {1,0.1,0,0};
 GLfloat luzExplosao[] = { 1, 0.7, 0, 1};
 GLfloat luzEspecular[]={1.0, 0, 0, 1.0};


int colisao(cubos *c,float x,float y,float z){
	float d;
	for(int i=0;i<25;i++){
		if(c[i].on){
		d = sqrt( pow((c[i].x - x),2)  + pow((0.5 - y),2)  + pow((c[i].z - z),2) );
		if(d<(c[i].raio+0.5))
			return i;
		}
	}
	return -1;
}


void reshape(int width, int height){
  WIDTH=width;
  HEIGHT=height;
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0,width/(float)height,1,60.0);
  glMatrixMode(GL_MODELVIEW);
}

void verifica_colisao(){
float d;
	for(int i=0;i<4;i++){
		if(fantasma[i].on){
		d = sqrt( pow((fantasma[i].x - person.x),2)  + pow((0.5 - 0.5),2)  + pow((fantasma[i].z - person.z),2) );
		if(d<1.2){
			 if(!person.explo)
				person.explo=true;
			else
				person.on=false;
			}
		}
	}
}
void personagem(){

  glPushMatrix();                      //personagem
  glTranslatef(person.x,1.5,person.z);
   float j =-0.3;
  for(int i=0;i<4;i++){
	  glPushMatrix();        //barra de vida
	  if(person.explo==true &&( i==2||i==3))
  	      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzCubo_borda);
  	  else
  	  	  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzCubo);
      
	  glTranslatef(j,1.5,0); 	  
      glutSolidCube (0.2);
	    // glutSolidSphere(0.1,20,20);
  	  glPopMatrix(); j+=0.2;
  }
  if(braco==0)
     glRotatef(90,0,1,0);

  glPushMatrix();      //cabeca
  	  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_a_difusa);
  	  glutSolidSphere(0.5,20,20);
  	  glPopMatrix();
  
  glPushMatrix();          //braço direito
      glTranslatef(-0.5,-0.6,0); 
	  glScalef(0.3,0.5,0.3);	  
	  glutSolidCube (1.0);
  glPopMatrix();
  glPushMatrix();          //braço esquerdo
      glTranslatef(0.5,-0.6,0); 
      glScalef(0.3,0.5,0.3);
	  glutSolidCube (1.0);
  glPopMatrix();
  glPushMatrix();        //corpo
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzCubo_borda);
      glTranslatef(0,-0.9,0); 
      glScalef(0.6,1,0.6);  	  
	  glutSolidCube (1.0);
  glPopMatrix();
 
  
glPopMatrix();
}
void fantasmas(){
  for(int i=0;i<4;i++){         //fantasmas
  	if(fantasma[i].on){
  		
  		glPushMatrix();
  		     glTranslatef(fantasma[i].x,1.5,fantasma[i].z); 
  		     if(bracof==1)
             	 glRotatef(90,0,1,0);
  			 glPushMatrix();      //cabeca
  	  	  	  	  glMaterialfv(GL_FRONT, GL_DIFFUSE, plano_difusa);
  	  	          glutSolidSphere(0.5,20,20);
  	  	      glPopMatrix();
  	  	  	  glPushMatrix();          //braço direito
      	  	  	  glTranslatef(-0.5,-0.6,0); 
	  	  	  	  glScalef(0.3,0.5,0.3);	  
		 		  glutSolidCube (1.0);
  	  	      glPopMatrix();
  	  	  	  glPushMatrix();          //braço esquerdo
      	  	  	  glTranslatef(0.5,-0.6,0); 
      	  	  	  glScalef(0.3,0.5,0.3);
	  	      glutSolidCube (1.0);
  	  	  	  glPopMatrix();
  	  	  	  	  glPushMatrix();        //corpo
      	  	  	  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzCubo_borda);
      	  	  	  glTranslatef(0,-0.9,0); 
      	  	  	  glScalef(0.6,1,0.6);  	  
	  	  	  	  glutSolidCube (1.0);
  	  	      glPopMatrix();
  
			glPopMatrix();
	  }
  }
}

void explosao(){
	float i;
	int j=0;
	if(bomba.explo==false){
 	 tempo[1]=clock();
 	 
 	 if( ((tempo[1]-tempo[0])*1000.0/CLOCKS_PER_SEC)>3000 && bomba.on==true){
 	 	 
 	 	 for(i=0;i<2.5;i+=0.5){
 	 	  
 	 	  glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, luzExplosao);
		  if(colisao(cube,bomba.x+i,0.5,bomba.z)==-1 && bomba.x+i<8){
		  	  glPushMatrix();
  	  	  	  glTranslatef(bomba.x+i,0.5,bomba.z);
  	  	  	  explo[j].x = bomba.x+i;
  	  	  	  explo[j].z = bomba.z;
  	  	  	  explo[j].on = true;
  	  	  	  explo[j].raio = 0.3;
  	  	  	  glutSolidSphere(0.6,20,20); 
          	  glPopMatrix();
          	  col = colisao(blocos,bomba.x+i,0.5,bomba.z);
          	  if(col!=-1){
					blocos[col].on = false;
				}
              j++;
		  }
         
          if(colisao(cube,bomba.x-i,0.5,bomba.z)==-1 && bomba.x-i>-8){
  	  	  	  glPushMatrix();
  	  	  	  glTranslatef(bomba.x-i,0.5,bomba.z);
  	  	  	  explo[j].x = bomba.x-i;
  	  	  	  explo[j].z = bomba.z;
  	  	  	  explo[j].on = true;
  	  	  	  explo[j].raio = 0.3;
  	  	  	  glutSolidSphere(0.6,20,20);
  	  	  	  glPopMatrix();
  	  	  	  col = colisao(blocos,bomba.x-i,0.5,bomba.z);
          	  if(col!=-1){
					blocos[col].on = false;
				}
				j++;
		  }
      
      	  if(colisao(cube,bomba.x,0.5,bomba.z+i)==-1 && bomba.z+i<8){
          	  glPushMatrix();
  	  	  	  glTranslatef(bomba.x,0.5,bomba.z+i);
  	  	  	  explo[j].x = bomba.x;
  	  	  	  explo[j].z = bomba.z+i;
  	  	  	  explo[j].on = true;
  	  	  	  explo[j].raio = 0.3;
  	      	  glutSolidSphere(0.6,20,20);
  	      	  glPopMatrix();
  	  	  	  col = colisao(blocos,bomba.x,0.5 ,bomba.z+i);
          	  if(col!=-1){
					blocos[col].on = false;
				}
				j++;
          }
  	  	
  	  	  if(colisao(cube,bomba.x,0.5,bomba.z-i)==-1 && bomba.z-i>-8){
   	      	  glPushMatrix();
  	      	  glTranslatef(bomba.x,0.5,bomba.z-i);
  	      	  explo[j].x = bomba.x;
  	  	  	  explo[j].z = bomba.z-i;
  	  	  	  explo[j].on = true;
  	  	  	  explo[j].raio = 0.3;
  	      	  glutSolidSphere(0.6,20,20);
  	      	  glPopMatrix(); 
  	  	  	  col = colisao(blocos,bomba.x,0.5 ,bomba.z-i);
          	  if(col!=-1){
					blocos[col].on = false;
				}
				j++;
  	      }
  	      glPopMatrix();
	  }
	    for(int i=0;i<4;i++){        //verifica se colidiu com algum fantasma
			if(colisao(explo,fantasma[i].x,1,fantasma[i].z)!=-1){
				fantasma[i].on = false;
			}
		}
		if(colisao(explo,person.x,1.2,person.z)!=-1){        //verifica se colidiu com o personagem
			if(!person.explo)
				person.explo=true;
			else
				person.on=false;
		}
	 bomba.on=false;
 }}
  
}


void display(void){
 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

 
    glShadeModel(GL_SMOOTH);

  glPushMatrix();   //0

     gluLookAt(0,visao,9,
              0,0,0,
              0.0,1.0,0.0);
  /* propriedades do material do plano */
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plano_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, plano_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plano_brilho);
  

  glDisable(GL_TEXTURE_2D);
  
  if(chave){
  glPushMatrix();glMaterialfv(GL_FRONT, GL_DIFFUSE, luzExplosao);
      glTranslatef(-6.2,0,0);
      glPushMatrix();
  	  	   glRotatef(-90,1,0,0); 
  	  	  glTranslatef(2,3,1);
  	  	  glutSolidTorus(0.08,0.2,50,50);
  	  glPopMatrix();
  	  glPushMatrix();
  	  	  glTranslatef(1.5,1,-3);
  	  	  glScalef(1.4,0.3,0.4);
  	  	  glutSolidCube(0.5);
  	  glPopMatrix();
  	  glPushMatrix();
  	  	  glTranslatef(1.23,1,-3.2);
  	  	  glScalef(0.3,0.3,0.4);
  	  	  glutSolidCube(0.5);
  	  glPopMatrix();
  	  glPushMatrix();
  	  	  glTranslatef(1.5,1,-3.1);
  	  	  glScalef(0.3,0.3,0.4);
  	  	  glutSolidCube(0.5);
  	  glPopMatrix();
  glPopMatrix();}

  
  if(sqrt( pow((person.x - (-4.5)),2)  + pow((0.5 - 0.5),2)  + pow((person.z - (-3)),2) ) < 0.8){
	  chave=false;
  }


  if(!chave){
  glPushMatrix();
         glMaterialfv(GL_FRONT, GL_DIFFUSE, luzEspecular);
  	      glRotatef(90,1,0,0);
  	      glTranslatef(7.5,-3,-0.3);
	 glutSolidCone(0.8,0.3,30.0,10); 
  glPopMatrix();
  }

  
  if(!chave && !fantasma[0].on && !fantasma[1].on && !fantasma[2].on && !fantasma[3].on){
	  if(sqrt( pow((person.x - (7.5)),2)  + pow((0.5 - 0.5),2)  + pow((person.z - (-3)),2) ) < 0.5 && visao<80){
		  visao+=2;
		  printf("YOU WIN!!!\n");
	  }
  }
  
  
  if(bomba.on && person.on){                                    //bomba
	  glPushMatrix();
      glPushMatrix();
  	  	  glMaterialfv(GL_FRONT, GL_DIFFUSE, luzCubo_interno);
  	  	  glTranslatef(bomba.x,0.5,bomba.z);
  	  	  glutSolidSphere(0.5,20,20);
  	  glPopMatrix();
  	  glPushMatrix();
  	  	  glBegin(GL_LINES);
  	  	   glColor3f(0,0,0);
   	   	   glVertex3f(bomba.x, 0.5,bomba.z);
   	   	   glVertex3f(bomba.x-0.7,0.5,bomba.z+0.3);
			glEnd();
  	  glPopMatrix();
  	  glPushMatrix();
  	  	  glMaterialfv(GL_FRONT, GL_DIFFUSE, luzExplosao);
  	  	  glTranslatef(bomba.x-0.7,0.5,bomba.z+0.3);
  	  	  glutSolidSphere(0.2,20,2);
  	  glPopMatrix();
  	  if(bomba.explo==true){
  	  	  tempo[0]=clock();
  	  	  bomba.explo=false;
      }

  	  glPopMatrix();
  }
  
  
    explosao();
 
 
  
  for(int i=0;i<25;i++){          //cubos fixos
  glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzBlocos);
      glTranslatef(cube[i].x,0.5,cube[i].z);   
   	  glutSolidCube (1.5);
  glPopMatrix();
  }
  
 
  for(int i=0;i<25;i++){             //cubos 
  	  if(blocos[i].on){
	  glPushMatrix();
	  	  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzBloco2);
	  	  glTranslatef(blocos[i].x,0.5,blocos[i].z); 
	  	  glutSolidCube (1.5);
	  glPopMatrix();
  }
}

  if(person.on)
  	  personagem();
  else
    printf("GAME OVER\n");
 
  fantasmas();
  
  verifica_colisao();

  glPushMatrix();                                             //bordas
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, luzCubo_borda);
  for(int i=0;i<4;i++){
	  glPushMatrix();
	  	  glTranslatef(vet_borda[i][0],vet_borda[i][1],vet_borda[i][2]);
	  	  if(i<=1)
	  	  	  glScalef(1,1,19);
	      else
	      	  glScalef(19,1,1);
          glutSolidCube (1.0);
	  glPopMatrix();
  }
 glPopMatrix(); 

  
  	  	  //Cria a textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    
 
   
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                 0, GL_RGB, GL_UNSIGNED_BYTE,texture_data);
 

  /* desenha o plano */
  
  glNormal3f(0,1,0);
  glBegin(GL_QUADS);
  glTexCoord2f(-9, -9);glVertex3f(-9,0,9);
  glTexCoord2f(-9, 9); glVertex3f(9,0,9);
  glTexCoord2f(9, 9);  glVertex3f(9,0,-9);
  glTexCoord2f(9, -9); glVertex3f(-9,0,-9);
  glEnd();
  

    //Cria a textura
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                 0, GL_RGB, GL_UNSIGNED_BYTE,texture_data2);
  
    
    glBegin(GL_QUADS);                                //textura das bordas
      glTexCoord2f(-9,9); glVertex3f(-9,1.01,9.5);
      glTexCoord2f(-9,8);glVertex3f(-9,1.01,8.5);
      glTexCoord2f(9,8);glVertex3f(9,1.01,8.5);
      glTexCoord2f(9,9);glVertex3f(9,1.01,9.5);
      glEnd();
	  
	  glBegin(GL_QUAD_STRIP);
      glTexCoord2f(-9,9); glVertex3f(-9.5,1.01,9.5);
      glTexCoord2f(-8,9);glVertex3f(-8.5,1.01,9.5);
      glTexCoord2f(-9,-9);glVertex3f(-9.5,1.01,-9.5);
      glTexCoord2f(-8,-9);glVertex3f(-8.5,1.01,-9.5);
      glEnd();
	  
	  glBegin(GL_QUAD_STRIP);
      glTexCoord2f(-9,-8);glVertex3f(-9,1.01,-8.5);
      glTexCoord2f(-9,9); glVertex3f(-9,1.01,-9.5);
      glTexCoord2f(9,-8);glVertex3f(9.5,1.01,-8.5);
      glTexCoord2f(9,-9);glVertex3f(9.5,1.01,-9.5);
      glEnd();
      
      glBegin(GL_QUAD_STRIP);
      glTexCoord2f(8,9); glVertex3f(8.5,1.01,9.5);
      glTexCoord2f(9,9);glVertex3f(9.5,1.01,9.5);
      glTexCoord2f(8,-9);glVertex3f(8.5,1.01,-9.5);
      glTexCoord2f(9,-9);glVertex3f(9.5,1.01,-9.5);
      glEnd();
  
  glPopMatrix();   //0

  
  glutSwapBuffers();
}


void special(int key, int x, int y){
  switch (key) {
  case GLUT_KEY_F1:
		person.on=true;
		person.explo=false;
		glutPostRedisplay();
		break; 
  }
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'd':
  	if(braco!=0) braco=0;
      if(colisao(cube,person.x+0.3,0.5,person.z)==-1  && colisao(blocos,person.x+0.3,0.5,person.z)==-1 && person.x<8 ){
          if(person.x >bomba.x-1 && person.x<bomba.x-0.5 && bomba.on && sqrt( pow((person.x-0.3 - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((person.z - bomba.z),2) ) < 1)
             break;
          else		
	         person.x+=0.3;

	 }
    break;

   case 'a':
   	if(braco!=0) braco=0;

		if(colisao(cube,person.x-0.3,0.5,person.z)==-1 && colisao(blocos,person.x-0.3,0.5,person.z)==-1 && person.x>-8){
			if(person.x<bomba.x+1 && person.x>bomba.x+0.5 && bomba.on && sqrt( pow((person.x+0.3 - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((person.z - bomba.z),2) ) < 1)
          	    break;
		    else		
		        person.x-=0.3;
	        
	}
    break;

   case 's':
   	if(braco!=1) braco=1;
	
		if(colisao(cube,person.x,0.5,person.z+0.3)==-1 && colisao(blocos,person.x,0.5,person.z+0.3)==-1  && person.z<8){
			if(person.z>bomba.z-1 && person.z<bomba.z-0.5 && bomba.on && sqrt( pow((person.x - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((person.z-0.3 - bomba.z),2) ) < 1)
		    	break;
		    else
				person.z+=0.3;
	        
	}
    break;

   case 'w':
   	if(braco!=1) braco=1;
		if(colisao(cube,person.x,0.5,person.z-0.3)==-1 && colisao(blocos,person.x,0.5,person.z-0.3)==-1 && person.z>-8){
			if(person.z<bomba.z+1 && person.z>bomba.z+0.5 && bomba.on && sqrt( pow((person.x - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((person.z+0.3 - bomba.z),2) ) < 1)
            	break;
		    else 
				person.z-=0.3;
	        
	  }
    break;
 
  case 13:
  	if(!bomba.on){
  	    bomba.on=true;
  	    bomba.explo=true;
  	    bomba.x = person.x;
  	    bomba.z = person.z;
    }
    break; 	
  
	}
    glutPostRedisplay();
 }

void init(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
  
float vetorx[25]={-3,-1.5,-4.5,-1.5,-1.5,1.5,6,7.5,-7.5,-4.5,-3,1.5,6,4.5,-4.5,-3,0,1.5,6,-4.5,-7.5,-1.5,4.5,-4.5,1.5};
float vetorz[25]={7.5,7.5,6,6,4.5,4.5,4.5,4.5,1.5,1.5,1.5,1.5,1.5,0,-1.5,-1.5,-1.5,-1.5,-1.5,-3,-4.5,-4.5,-4.5,-6,-6};
  
  
   bomba.on=false;
   bomba.explo=false;
   person.x = -6;
   person.z = 7.5;
   person.on = true;
   float x=-6;
   float z=-6;
   for(int i=0;i<25;i++){          //cubos 
   	
      blocos[i].x = vetorx[i];
	  blocos[i].z = vetorz[i];
	  blocos[i].on = true;
	  blocos[i].raio = 0.75;
   	
	  cube[i].x = x;
	  cube[i].on = true;
	  cube[i].z = z; 
	  cube[i].raio = 0.75;
      if(z>=6){ z=-6; x+=3; }
	  else{ z+=3; }
  	  }
   fantasma[0].x = 4.5;
   fantasma[0].z = 7.5;
   fantasma[1].x = 0;
   fantasma[1].z = -4.5;
   fantasma[2].x = -4.5;
   fantasma[2].z = -4.5;
   fantasma[3].x = -1.5;
   fantasma[3].z = 1.5;
   for(int i=0;i<4;i++){
	   fantasma[i].on = true;
   }

  
   /* Prepara a textura */
    glGenTextures(QUANT_TEX, id_texturas); //gera nomes identificadores de texturas
    glBindTexture(GL_TEXTURE_2D, id_texturas[0]); //Ativa a textura atual
  
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
}


int cont=1;
int m1=0,m2=1,m3=2,m4=3;
void movimento(int *r,int i){
	
	if(*r==0  ){
		  if (fantasma[i].x<7.5 && colisao(cube,fantasma[i].x+0.2,0.5,fantasma[i].z)==-1&&colisao(blocos,fantasma[i].x+0.2,0.5,fantasma[i].z)==-1 ){
              if(bomba.on && sqrt( pow((fantasma[i].x+0.2 - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((fantasma[i].z - bomba.z),2) ) < 0.5){
  	                  *r=rand()%4; 
   	          }else 
                   fantasma[i].x+=0.2; 
			 
          }else
  	          *r=rand()%4;
			
	  	  if(bracof!=0) bracof=0;
	    
        
	  }
    if(*r==1  ){
  		if(fantasma[i].x>-8&&colisao(cube,fantasma[i].x-0.2,0.5,fantasma[i].z)==-1&&colisao(blocos,fantasma[i].x-0.2,0.5,fantasma[i].z)==-1 ){
  		   if(bomba.on && sqrt( pow((fantasma[i].x-0.2 - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((fantasma[i].z - bomba.z),2) ) < 0.5){
  	               *r=rand()%4; 
           }else
                fantasma[i].x-=0.2; 
				 
        }else
            *r=rand()%4;	

	       if(bracof!=0) bracof=0;
     }  
	  
    if(*r==2 ) {
    	  if(fantasma[i].z<7.5 && colisao(cube,fantasma[i].x,0.5,fantasma[i].z+0.2)==-1 && colisao(blocos,fantasma[i].x,0.5,fantasma[i].z+0.2)==-1){
  	  	      if(bomba.on && sqrt( pow((fantasma[i].x - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((fantasma[i].z+0.2 - bomba.z),2) ) < 0.5){
  	                  *r=rand()%4; 
   	          }else
                   fantasma[i].z+=0.2; 

           }else
		   	   *r=rand()%4;
	       
        if(bracof!=1) bracof=1;

	  }
    if(*r==3  ){
    	  if (fantasma[i].z>-8 && colisao(cube,fantasma[i].x,0.5,fantasma[i].z-0.2)==-1&&colisao(blocos,fantasma[i].x,0.5,fantasma[i].z-0.2)==-1){
  	  	      if(bomba.on && sqrt( pow((fantasma[i].x - bomba.x),2)  + pow((0.5 - 0.5),2)  + pow((fantasma[i].z-0.2 - bomba.z),2) ) < 0.5){
  	                  *r=rand()%4; 
   	          }else
                   fantasma[i].z-=0.2; 
		  
         }else
             *r=rand()%4;
             
         if(bracof!=1) bracof=1;
		 
	  }
//	  verifica_colisao();

}
void timer_callback(int value){
	 
     
    if(cont<20){
    	cont+=1;
	}else{
	    	m1 = rand()%4;
		    m2 = rand()%4;
		    m3 = rand()%4;
		    m4 = rand()%4;
			cont=1;
	}
  	
  	
  	 movimento(&m1,0);
  	 movimento(&m2,1);
     movimento(&m3,2);
     movimento(&m4,3);

	glutPostRedisplay();
    glutTimerFunc(200,timer_callback, 1);

}

int main(int argc,char **argv){
  glutInitWindowPosition(100,100);
  glutInitWindowSize(1024,512);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
  
  

  if(!glutCreateWindow("Bomberman")) {
    fprintf(stderr,"Error opening a window.\n");
    exit(-1);
  }

  init();
  glutTimerFunc(5,timer_callback,5);
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);


  glutMainLoop();
  return(0);
}