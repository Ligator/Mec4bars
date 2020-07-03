#include <string.h>
#include <math.h>
#include <iostream>
#include <ctime>

#ifdef __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#endif

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include <string.h>
#include <GL/glui.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float PI=3.141592654;
float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

/** These are the live variables passed into GLUI ***/
int   obj_type = 1;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0;
float light1_intensity = .4;
int   main_window;
int   dialog_window_1;
float scale = 1.0;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
//char *string_list[] = { "Hello World!", "Foo", "Testing...", "Bounding box: on" };
//int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *glui2, *glui3;
GLUI_Spinner    *light0_spinner, *light1_spinner;
GLUI_RadioGroup *radio;
GLUI_Panel      *obj_panel, *file_panel, *openf_panel;
/********** User IDs for callbacks ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
#define ADDTOFILE_ID         300
#define OPENFILE_ID          301
#define SAVEAS_ID            302
#define SAVEFILE_ID          303
#define VFRONTAL_ID          304
#define VRESETXY_ID          305
#define V_ISO_ID               306
#define ADDMEC_ID               307


/********** Miscellaneous global variables **********/

//GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
//GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
//GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

    GLfloat light0_ambient[] =    {0.5f , 0.5f ,0.5f ,1.0f};
    GLfloat light0_diffuse[] =    {1.0f , 1.0f ,1.0f ,1.0f};
    GLfloat light0_specular[] =    {1.0f , 1.0f ,1.0f ,1.0f};
    GLfloat light0_position[] =    {20.0f,100.0f,10.0f,0.0f};

GLfloat light1_ambient[] =  { 0.1f,  0.1f, 0.3f, 1.0f};
GLfloat light1_diffuse[] =  {  .9f,   .6f, 0.0f, 1.0f};
GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

/********** Miscellaneous global variables **********/
#define circle_faces 16
short count=0;
float a=0;
float delta=10.0;
int giro=0;
GLUquadric* qobj;
GLfloat sin_angle[circle_faces+1];
GLfloat cos_angle[circle_faces+1];

float L1=0.5,L2=0.75,L3=1.0,L4=1.0, LE=0.5;
float ang1, ang2, ang3, ang4, angE=1.0;
float ang2_show, ang3_show;
float x=1.0, y=1.0, z=1.0, ang1_ph=1.0, angM=1.0;

struct type_m4bar{                                     // Entradas
     float L1;/////////// longitud barra1                    -
     float L2;         // longitud barra2                    -
     float L3;         // longitud barra3                    -
     float L4;         // longitud barra4                    -
     float LE;         // longitud extensi�n de barra2       -
     float ang1;///////// �ngulo barra1
     float ang2;       // �ngulo barra2
     float ang3;       // �ngulo barra3
     float ang4;       // �ngulo barra4
     float angE;       // �ngulo extensi�n de barra2         -
     float ang2_show;//// �ngulo interno barra2
     float ang3_show;  // �ngulo interno barra3
     float x;//////////// posici�n inicial x barra1          -
     float y;          // posici�n inicial y barra1          -
     float z;          // posici�n inicial x barra1          -
     float ang1_ph;    // fase inicial barra1                -
     float angM;       // inclinaci�n de mecanismo barra4    -
};

//             |_   _   _   _   _  |_   _   _   _   _   |_   _  |_   _   _   _   _  |
#define mec_s  {0.5,0.9,0.7,0.9,0.5,0.0,0.0,0.0,0.0,45.0,0.0,0.0,0.0,0.0,-0.5,0.0,0.0}
#define mec_s2 {0.5,0.9,0.7,0.9,0.5,0.0,0.0,0.0,0.0,90.0,0.0,0.0,0.0,0.0,-1.0,90.0,0.0}
type_m4bar mec[10]={mec_s,mec_s,mec_s2,mec_s,mec_s,mec_s,mec_s,mec_s,mec_s,mec_s};
int mTotal=1, mSelect=0;

/************************* FILE ***************************/
FILE *file;
GLUI_FileBrowser *fb;
GLUI_TextBox *savefilepath;
char file_buffer[100]="  ";

/*********************** Show items ***********************/
int   show_axes = 1;
int   show_angles = 1;
int   show_path=1;
int   show_bars=1;
int   show_gears=1;

/************************ Escene **************************/
GLUI_Rotation *view_rot;
GLUI_Translation *trans_xy;
GLUI_Translation *trans_x;
GLUI_Translation *trans_y;
GLUI_Translation *trans_z;
float obj_pos2[] = { 0.0, 0.0, 0.0 };
float view_rotate_FRONTAL[16]       = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
int cal_view_isometric=1;
float view_rotate_USUARIO[16]       = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_rotate_ISOMETRIC[16]     = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
GLdouble view_rotate_ISOMETRIC2[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
int show_all_mec=1;

int mouse_down_left=0, mouse_down_right=0, mouse_left_x0=1, mouse_left_y0=1, mouse_right_x0=1, mouse_right_y0=1;
float rotaX=1.0, rotaY=1.0, trasladaX=1.0, trasladaY=1.0;

/*********************** Materials ************************/
#define MAT_ORO 0
GLfloat oroAmb[4] = { 0.24725f,  0.2245f,  0.0645f,1.0f};
GLfloat oroDif[4] = { 0.34615f,  0.3143f,  0.0903f,1.0f};
GLfloat oroSpe[4] = {0.797357f,0.723991f,0.208006f,1.0f};
GLfloat oroShi = 128.0f;
#define MAT_ESMERALDA 1
GLfloat esmeraldaAmb[4] = { 0.0215f,  0.1745f, 0.0215f,1.0f};
GLfloat esmeraldaDif[4] = {0.07568f, 0.61424f,0.07568f,1.0f};
GLfloat esmeraldaSpe[4] = {  0.633f,0.727811f,  0.633f,1.0f};
GLfloat esmeraldaShi = 76.8f;
#define MAT_CROMO 2
GLfloat cromoAmb[4] = {0.25f    ,    0.25f,    0.25f,0.2f};
GLfloat cromoDif[4] = {0.4f     ,     0.4f,     0.4f,0.2f};
GLfloat cromoSpe[4] = {0.774597f,0.774597f,0.774597f,0.2f};
GLfloat cromoShi = 76.8f;

int En_transparency=1;

/************** Miscellaneous global variables *************/
char encabezado[30]="Sistema de 4 barras.";
int try_version_date[3]={8,11,2050}; // Fecha de caducidad dd/mm/aaaa

int try_version(int dd, int mm, int aaaa){
     time_t tiempo = time(0);
     struct tm *tlocal = localtime(&tiempo);
     tm tVencimiento = {59,59,23,8,10,113,0,0,-1};
     tVencimiento.tm_year=aaaa-1900;
     tVencimiento.tm_mon =mm-1;
     tVencimiento.tm_mday=dd;
     time_t tiempo2 = mktime(&tVencimiento);
/*
     char output[128];
     strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
     printf("tlocal       = %s\n",output);
     strftime(output,128,"%d/%m/%y %H:%M:%S",&tVencimiento);
     printf("tVencimiento = %s\n",output);
*/
     double dif=difftime(tiempo,tiempo2);
     if(dif>0) exit(EXIT_SUCCESS); // tlocal > tVencimiento
     else      printf("\n Te quedan %d d�as de prueba", (int)dif/(-60*60*24)); //tVencimiento >= tlocal
     return((int)dif/(-60*60*24));
}

void initialize(){
      GLfloat angle;
      int i;
      angE=0.0;
      x=0.0, y=0.0, z=0.0, ang1_ph=0.0, angM=0.0;
      mec[0].L1=L1; mec[0].L2=L2; mec[0].L3=L3; mec[0].L4=L4;
      mec[0].LE=LE; mec[0].angE=0.0; mec[0].z=z;
      rotaX=0.0; rotaY=0.0; trasladaX=0.0; trasladaY=0.0;

      for(i=0;i<=circle_faces;i++){
          angle = i * 2.0f * PI / circle_faces;
          //if(i%2==0){
          sin_angle[i]=sin(angle);
          cos_angle[i]=cos(angle);
          //}
          //else{
          //     sin_angle[i]=sin(angle)*1.1;
          //     cos_angle[i]=cos(angle)*1.1;
          //}
          //printf("\n sin(%f) = %f",angle,sin_angle[i]);
          //printf("\n cos(%f) = %f",angle,cos_angle[i]);
      }
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    
    //printf("\n Mecanismo 1 \nL1=%.2f \nL2=%.2f \nL3=%.2f \nL4=%.2f \nLE=%.2f \nangE=%.2f \nang1=%.2f \nangM=%.2f ", m[0].L1, m[0].L2, m[0].L3, m[0].L4, m[0].LE, m[0].angE, m[0].ang1, m[0].angM);
}

void update_glut_bars(void){
       mec[mSelect].L1=L1;
       mec[mSelect].L2=L2;
       mec[mSelect].L3=L3;
       mec[mSelect].L4=L4;
       mec[mSelect].LE=LE;
       mec[mSelect].x=x;
       mec[mSelect].y=y;
       mec[mSelect].z=z;
       if(ang1_ph<0) ang1_ph+=360;
       mec[mSelect].ang1_ph=ang1_ph;
       if(angE<0) angE+=360;
       mec[mSelect].angE=angE;
       if(angM<0) angM+=360;
       mec[mSelect].angM=angM;
}

void update_glui(void){
      L1=mec[mSelect].L1;
      L2=mec[mSelect].L2;
      L3=mec[mSelect].L3;
      L4=mec[mSelect].L4;
      LE=mec[mSelect].LE;
      x=mec[mSelect].x;
      y=mec[mSelect].y;
      z=mec[mSelect].z;
      ang1_ph=mec[mSelect].ang1_ph;
      angE=mec[mSelect].angE;
      angM=mec[mSelect].angM;
      glui->sync_live();
 }
 
void delete_mec_funtion(void){
   if(mTotal>1){
     if(mSelect!=9)
          for(int i=mSelect; i<mTotal; i++)
                mec[i]=mec[i+1];
     mTotal--;
     if(mSelect!=0)mSelect--;
     glutPostRedisplay();
   }
   update_glui();
}
void add_mec_funtion(void){
    if(mTotal<10){
       mec[mTotal].L1 = mec[mSelect].L1;
       mec[mTotal].L2 = mec[mSelect].L2;
       mec[mTotal].L3 = mec[mSelect].L3;
       mec[mTotal].L4 = mec[mSelect].L4;
       mec[mTotal].LE = mec[mSelect].LE;
       mec[mTotal].x = mec[mSelect].x;
       mec[mTotal].y = mec[mSelect].y;
       mec[mTotal].z = mec[mSelect].z+0.5;
       mec[mTotal].ang1_ph = mec[mSelect].ang1_ph;
       mec[mTotal].angE = mec[mSelect].angE;
       mec[mTotal].angM = mec[mSelect].angM;
       mSelect = mTotal;
       mTotal++;
       update_glui();
       glutPostRedisplay();
    }
}
/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control ){
  if ( control == LIGHT0_ENABLED_ID ) {
    if ( light0_enabled ) {
      glEnable( GL_LIGHT0 );
      light0_spinner->enable();
    }
    else {
      glDisable( GL_LIGHT0 );
      light0_spinner->disable();
    }
  }
  else if ( control == LIGHT1_ENABLED_ID ) {
    if ( light1_enabled ) {
      glEnable( GL_LIGHT1 );
      light1_spinner->enable();
    }
    else {
      glDisable( GL_LIGHT1 );
      light1_spinner->disable();
    }
  }
  else if ( control == LIGHT0_INTENSITY_ID ) {
    float v[] = {
      light0_diffuse[0],  light0_diffuse[1],
      light0_diffuse[2],  light0_diffuse[3] };
    
    v[0] *= light0_intensity;
    v[1] *= light0_intensity;
    v[2] *= light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
  }
  else if ( control == LIGHT1_INTENSITY_ID ) {
    float v[] = {
      light1_diffuse[0],  light1_diffuse[1],
      light1_diffuse[2],  light1_diffuse[3] };
    
    v[0] *= light1_intensity;
    v[1] *= light1_intensity;
    v[2] *= light1_intensity;

    glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
  }
  else if ( control == OPENFILE_ID ){
    std::string file_name;
    FILE *file2;
    file_name = " ";
    char file_buffer2[100]="  ";
    int num=1;

    file_name = fb->get_file();
    strcpy(encabezado,file_name.c_str());
    num=strlen(encabezado);
    if(encabezado[num-3]=='y'&&encabezado[num-2]=='e'&&encabezado[num-1]=='e'){
      file2 = fopen(file_name.c_str(),"r");
      if (file2 == NULL)
         printf("Error opening file\n");
      else {
         printf("\n");
         while (feof(file2) == 0){
            fgets(file_buffer2,100,file2);
            if(file_buffer2[0]=='*'){
               fscanf(file2, "%d" ,&num);
               mSelect=num;
               fscanf(file2, "%f %f %f %f" ,&mec[num].L1,&mec[num].L2,&mec[num].L3,&mec[num].L4);
               fscanf(file2, "%f %f" ,&mec[num].LE,&mec[num].angE);
               fscanf(file2, "%f %f %f %f %f" ,&mec[num].x,&mec[num].y,&mec[num].z,&mec[num].angM,&mec[num].ang1_ph);
            }
         }
         mTotal = mSelect+1;
         fclose(file2);
         glutPostRedisplay();
         update_glui();
      }
    }
  }
  else if ( control == ADDTOFILE_ID ){
    //
  }
  else if ( control == SAVEFILE_ID ){
    int file_buffer_len=0;
    strcpy(file_buffer,savefilepath->get_text());
    file_buffer_len=strlen(file_buffer);
    //printf("\nBUFFER %s, GET %s, LEN %d",file_buffer,savefilepath->get_text(),file_buffer_len);
    if(file_buffer_len>0){
         if(file_buffer[file_buffer_len-4]!='.'||file_buffer[file_buffer_len-3]!='y'||file_buffer[file_buffer_len-2]!='e'||file_buffer[file_buffer_len-1]!='e'){
                sprintf(file_buffer,"%s.yee",file_buffer);
                savefilepath->set_text(file_buffer);
         }
         file = fopen(file_buffer,"w");
         for(int i=0; i<mTotal; i++){
                 fprintf(file,"\n*");
                 fprintf(file,"\n%d",i);
                 fprintf(file,"\n%f %f %f %f",mec[i].L1,mec[i].L2,mec[i].L3,mec[i].L4);
                 fprintf(file,"\n%f %f",mec[i].LE,mec[i].angE);
                 fprintf(file,"\n%f %f %f %f %f",mec[i].x,mec[i].y,mec[i].z,mec[i].angM,mec[i].ang1_ph);
         }
         fclose(file);
    }
  }
  else if ( control == SAVEAS_ID ){
    //
  }
  else if(control == 1){
       delta = -delta;
       glui->sync_live();
  }
  else if(control == VFRONTAL_ID){
       view_rot->set_float_array_val( view_rotate_FRONTAL);
       glutPostRedisplay();
  }
  else if(control == V_ISO_ID){
       view_rot->set_float_array_val( view_rotate_ISOMETRIC);
       glutPostRedisplay();
  }
  else if(control == ADDMEC_ID){
    add_mec_funtion();
  }
  else if(control == VRESETXY_ID){
        trans_z->set_x(0.0);
        trans_z->set_y(0.0);
        trans_z->set_z(0.0);
        trans_xy->set_x(0.0);
        trans_xy->set_y(0.0);
        trans_xy->set_z(0.0);
        glui->sync_live();
        glutPostRedisplay();
  }
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y){
  switch(Key)
  {
  case 27: // Bot�n [Esc]
  case 'q':
    exit(0);
    break;
  case '+':
    if(delta<25) delta+=1;
    break;
  case '-':
    if(delta>0) delta-=1;
    break;
  case ' ':
       a=a+delta;
       if(a>=360) a=a-360;
       else if(a<0) a=a+360;
       update_glui();
  break;
  case 'x':
       mSelect++;
       if(mSelect>=mTotal) mSelect=0;
       update_glui();
  break;
  case 127: // Bot�n [Supr]
       delete_mec_funtion();
  break;
  default:
       printf("\n%c %d",Key,Key);
  };
  
  glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value ){
  myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle( void ){
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window )
    glutSetWindow(main_window);

    //GLUI_Master.sync_live_all(); // -- not needed - nothing to sync in this
                                      // application  */

  glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y ){
     if ((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_DOWN)){
        mouse_down_left=1;
        mouse_left_x0=x; mouse_left_y0=y;
     }
     if ((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_UP)){
        mouse_down_left=0;
        obj_pos[0]+=trasladaX;
        obj_pos[1]-=trasladaY;
        trasladaX=0.0;
        trasladaY=0.0;
        trans_xy->set_x(obj_pos[0]);
        trans_xy->set_y(obj_pos[1]);
     }
     if ((button == GLUT_RIGHT_BUTTON) && (button_state == GLUT_DOWN)){
        mouse_down_right=1;
        mouse_right_x0=x; mouse_right_y0=y;
     }
     if ((button == GLUT_RIGHT_BUTTON) && (button_state == GLUT_UP)){
        mouse_down_right=0;
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf( view_rotate );
        glRotated(rotaX,0,1,0);
        glRotated(rotaY,1,0,0);
        glGetDoublev (GL_MODELVIEW_MATRIX, view_rotate_ISOMETRIC2);
        for(int i=0;i<16;i++) view_rotate_USUARIO[i] = (float)view_rotate_ISOMETRIC2[i];
        glPopMatrix();
        rotaX=0.0;
        rotaY=0.0;
        view_rot->set_float_array_val( view_rotate_USUARIO);
        glui->sync_live();
        glutPostRedisplay();
     }
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y ){
     if(mouse_down_left){
          trasladaX=(x-mouse_left_x0)*0.01;
          trasladaY=(y-mouse_left_y0)*0.01;
     }
     if(mouse_down_right){
          rotaX=x-mouse_right_x0;
          rotaY=y-mouse_right_y0;
     }
  glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y ){
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );

  xy_aspect = (float)tw / (float)th;

  glutPostRedisplay();
}


/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */

void draw_axes( float scale ){
  if ( show_axes ){
      glDisable( GL_LIGHTING );

      glPushMatrix();
      glScalef( scale, scale, scale );

      glBegin( GL_LINES );
     
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( .8f, 0.05f, 0.0 );  glVertex3f( 1.0, 0.25f, 0.0 ); /* Letter X */
      glVertex3f( 0.8f, .25f, 0.0 );  glVertex3f( 1.0, 0.05f, 0.0 );
      glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 1.0, 0.0, 0.0 ); /* X axis      */

      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 1.0, 0.0 ); /* Y axis      */

      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 1.0 ); /* Z axis    */
      glEnd();

      glPopMatrix();

      glEnable( GL_LIGHTING );
  }
}

void draw_circle(float ballX, float ballY, float ballZ, float ballRadius){
   glPushMatrix();
   glTranslatef(ballX, ballY, ballZ);  // Translate to (xPos, yPos)
   glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0.0f, 0.0f);       // Center of circle
      for (int i = 0; i <= circle_faces; i++) { // Last vertex same as first vertex
         glVertex2f(cos_angle[i] * ballRadius, sin_angle[i] * ballRadius);
      }
   glEnd();
   glPopMatrix();
}

void draw_disk(float diskX, float diskY, float diskZ, float diskRadius, float grosor){
   glNormal3f(0.0f, 0.0f, 1.0f);
   draw_circle(diskX, diskY, diskZ+grosor, diskRadius);
   glNormal3f(0.0f, 0.0f, -1.0f);
   draw_circle(diskX, diskY, diskZ, diskRadius);
   glPushMatrix();
   glTranslatef(diskX, diskY, diskZ);
   gluCylinder(qobj, diskRadius, diskRadius, grosor, circle_faces, 1);
   glPopMatrix();
}

void draw_gear(float diskX, float diskY, float diskZ, float diskRadius, float grosor){
    if(show_gears){
          draw_disk(diskX, diskY, diskZ, diskRadius, grosor);
    }
}

void draw_bar(float x1, float y1, float z1, float x2, float y2, float z2, float ancho){
   glDisable( GL_LIGHTING );
   glPushMatrix();
   glBegin(GL_LINES);
      glVertex3f( x1, y1, z1);  glVertex3f( x2, y2, z2);
   glEnd();
   glPopMatrix();
   glEnable( GL_LIGHTING );
}

void draw_bar_angle(float x1, float y1, float z1, float largo, float ancho, float grosor){
    if(show_bars){
       draw_disk(x1,y1,z1, ancho/2, grosor);
       draw_disk(x1+largo,y1,z1, ancho/2, grosor);
       glPushMatrix();
       glTranslatef(x1, y1, z1);
       glColor3f(1.0f, 0.0f, 0.0f);
       glBegin(GL_QUADS);
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(  0.0, -ancho/2, grosor);  glVertex3f( largo, -ancho/2, grosor);
          glVertex3f(largo,  ancho/2, grosor);  glVertex3f(   0.0,  ancho/2, grosor);
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(  0.0,  ancho/2, grosor);  glVertex3f( largo,  ancho/2, grosor);
          glVertex3f(largo,  ancho/2,    0.0);  glVertex3f(   0.0,  ancho/2,    0.0);
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(  0.0,  ancho/2,    0.0);  glVertex3f( largo,  ancho/2,    0.0);
          glVertex3f(largo, -ancho/2,    0.0);  glVertex3f(   0.0, -ancho/2,    0.0);
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(  0.0, -ancho/2,    0.0);  glVertex3f( largo, -ancho/2,    0.0);
          glVertex3f(largo, -ancho/2, grosor);  glVertex3f(   0.0, -ancho/2, grosor);
       glEnd();
       glColor3f(0.0f, 0.0f, 0.0f);
       glutSolidSphere(0.01,16,16);
       glTranslatef(0.0, 0.0, grosor);
       glutSolidSphere(0.01,16,16);
       glTranslatef(largo, 0.0, 0.0);
       glutSolidSphere(0.01,16,16);
       glTranslatef(0.0, 0.0, -grosor);
       glutSolidSphere(0.01,16,16);

       glPopMatrix();
    }
}

void angle_calc(float ang_actual, int i_mec){
     mec[i_mec].ang1 = ang_actual;

     float d2 = mec[i_mec].L1*mec[i_mec].L1 + mec[i_mec].L4*mec[i_mec].L4 - 2*mec[i_mec].L1*mec[i_mec].L4*cos(mec[i_mec].ang1*PI/180);
     float d = sqrt(d2);
     float ang2_1;
     if(mec[i_mec].ang1==0||mec[i_mec].ang1==360) ang2_1=180;
     else if(mec[i_mec].ang1==180) ang2_1=0;
     else ang2_1 = acos((mec[i_mec].L4*mec[i_mec].L4 - mec[i_mec].L1*mec[i_mec].L1 - d2) / (-2*mec[i_mec].L1*d))*180/PI;
     float ang2_2 = acos((mec[i_mec].L3*mec[i_mec].L3 - mec[i_mec].L2*mec[i_mec].L2 - d2) / (-2*mec[i_mec].L2*d))*180/PI;
     ang3 = acos(((mec[i_mec].L1*mec[i_mec].L1 + mec[i_mec].L4*mec[i_mec].L4 - 2*mec[i_mec].L1*mec[i_mec].L4*cos(mec[i_mec].ang1*PI/180) - mec[i_mec].L2*mec[i_mec].L2 - mec[i_mec].L3*mec[i_mec].L3) / (-2*mec[i_mec].L2*mec[i_mec].L3)))*180/PI;

    
     if(mec[i_mec].ang1>180){
          ang2=180 - (ang2_1-ang2_2);
          mec[i_mec].ang2=ang2;
          ang4 = 360 - mec[i_mec].ang1 + (ang2_1-ang2_2) - ang3;
          mec[i_mec].ang4=ang4;
     }
     else{
          ang2 = 180 + (ang2_1 + ang2_2);
          mec[i_mec].ang2=ang2;
          ang4 = 360 - mec[i_mec].ang1 - (ang2_1 + ang2_2) - ang3;
          mec[i_mec].ang4=ang4;
     }
     ang2_show=ang2-180;
     mec[i_mec].ang2_show=ang2_show;
     ang3_show=ang3;
     mec[i_mec].ang3_show=ang3_show;
     ang3+=180;
     mec[i_mec].ang3=ang3;
}

void draw_path(int i_mec){
     float x[73],y[73];
     float x1,y1,x2,y2,xE,yE;
     int j=0;
     update_glut_bars();
     if( show_path ){
         for(float i=0;i<=360;i+=5){
              angle_calc(i,i_mec);
//              x2 = L1*cos(ang1*PI/180) + L2*cos((ang1+ang2_show-180)*PI/180);
//              y2 = L1*sin(ang1*PI/180) + L2*sin((ang1+ang2_show-180)*PI/180);
//              xE = x2 + LE*cos((ang1+ang2_show-180+angE)*PI/180);
//              yE = y2 + LE*sin((ang1+ang2_show-180+angE)*PI/180);

              x2 = mec[i_mec].L1*cos((mec[i_mec].ang1+mec[i_mec].angM)*PI/180) + mec[i_mec].L2*cos(((mec[i_mec].ang1+mec[i_mec].angM)+mec[i_mec].ang2_show-180)*PI/180);
              y2 = mec[i_mec].L1*sin((mec[i_mec].ang1+mec[i_mec].angM)*PI/180) + mec[i_mec].L2*sin(((mec[i_mec].ang1+mec[i_mec].angM)+mec[i_mec].ang2_show-180)*PI/180);
              xE = x2 + mec[i_mec].LE*cos(((mec[i_mec].ang1+mec[i_mec].angM)+mec[i_mec].ang2_show-180+mec[i_mec].angE)*PI/180);
              yE = y2 + mec[i_mec].LE*sin(((mec[i_mec].ang1+mec[i_mec].angM)+mec[i_mec].ang2_show-180+mec[i_mec].angE)*PI/180);

              x[j]=xE; y[j]=yE; j++;
         }

         glDisable( GL_LIGHTING );
         glPushMatrix();
//         glBegin( GL_POINTS );
         glBegin( GL_LINE_STRIP );
         if(i_mec==mSelect) glColor4f( 1.0, 0.0, 0.0, 1.0 );
         else               glColor4f( 0.0, 0.0, 0.0, 0.2 );
         for(j=0;j<=72;j++) glVertex3f( mec[i_mec].x+x[j], mec[i_mec].y+y[j], mec[i_mec].z+0.03 );
         glEnd();
         for(j=0;j<72;j+=6){
             glPushMatrix();
             glTranslatef(mec[i_mec].x+x[j], mec[i_mec].y+y[j], mec[i_mec].z+0.03);
             glutSolidSphere(0.008,5,5);
             glPopMatrix();
         }
         glPopMatrix();
         glEnable( GL_LIGHTING );
         //printf("\nx[0]=%f y[0]=%f x[35]=%f y[35]=%f x[60]=%f y[60]=%f", x[0], y[0], x[35], y[35], x[60], y[60]);
         //printf("\nL1=%,2f L2=.2%f L3=.2%f L4=.2%f",mec[i_mec].L1,mec[i_mec].L2, mec[i_mec].L3, mec[i_mec].L4);
     }
}

void asigna_material(int material, float transparencia){
   switch(material){
      case MAT_ORO:
         oroAmb[3]=transparencia;
         oroDif[3]=transparencia;
         oroSpe[3]=transparencia;
         glMaterialfv(GL_FRONT,   GL_AMBIENT, oroAmb);
         glMaterialfv(GL_FRONT,   GL_DIFFUSE, oroDif);
         glMaterialfv(GL_FRONT,  GL_SPECULAR, oroSpe);
         glMaterialf (GL_FRONT, GL_SHININESS, oroShi);
         break;
      case MAT_ESMERALDA:
         esmeraldaAmb[3]=transparencia;
         esmeraldaDif[3]=transparencia;
         esmeraldaSpe[3]=transparencia;
         glMaterialfv(GL_FRONT,   GL_AMBIENT, esmeraldaAmb);
         glMaterialfv(GL_FRONT,   GL_DIFFUSE, esmeraldaDif);
         glMaterialfv(GL_FRONT,  GL_SPECULAR, esmeraldaSpe);
         glMaterialf (GL_FRONT, GL_SHININESS, esmeraldaShi);
           break;
      case MAT_CROMO:
         cromoAmb[3]=transparencia;
         cromoDif[3]=transparencia;
         cromoSpe[3]=transparencia;
         glMaterialfv(GL_FRONT,   GL_AMBIENT, cromoAmb);
         glMaterialfv(GL_FRONT,   GL_DIFFUSE, cromoDif);
         glMaterialfv(GL_FRONT,  GL_SPECULAR, cromoSpe);
         glMaterialf (GL_FRONT, GL_SHININESS, cromoShi);
           break;
      case 3:
           break;
      default: break;
   }
}

void draw_m4bar(int i_mec){
  glPushMatrix();
  draw_path(i_mec);
  update_glut_bars();
  
  glTranslatef(mec[i_mec].x, mec[i_mec].y, mec[i_mec].z);
  glRotated(mec[i_mec].angM,0,0,1);

  float temp = mec[i_mec].ang1_ph+a;
  if(temp>=360) temp=temp-360;
  angle_calc(temp,i_mec);
  
  if(i_mec==mSelect) asigna_material(MAT_ORO, 1.0);
  else               asigna_material(MAT_ORO, 0.15);

  glPushMatrix();
  glRotated(mec[i_mec].ang1,0,0,1);
  draw_gear(0.0,0.0,-0.1, 0.3, 0.1);
  glPopMatrix();
  
  if(i_mec==mSelect) asigna_material(MAT_ESMERALDA, 1.0);
  else               asigna_material(MAT_CROMO, 0.15);

  glPushMatrix();
  glRotated(mec[i_mec].ang1,0,0,1);
  draw_bar_angle(0.0, 0.0, 0.0, mec[i_mec].L1,0.05,0.03);
  
  glTranslatef(mec[i_mec].L1, 0.0, 0.0);
  glRotated(mec[i_mec].ang2,0,0,1);
  draw_bar_angle(0.0, 0.0, 0.03, mec[i_mec].L2,0.05,0.03);

  glPushMatrix();
  glTranslatef(mec[i_mec].L2, 0.0, 0.0);
  glRotated(mec[i_mec].angE,0,0,1);
  draw_bar_angle(0.0, 0.0, 0.03, mec[i_mec].LE,0.05,0.03);
  glPopMatrix();

  glTranslatef(mec[i_mec].L2, 0.0, 0.0);
  glRotated(mec[i_mec].ang3,0,0,1);
  draw_bar_angle(0.0, 0.0, 0.0, mec[i_mec].L3,0.05,0.03);

  glPopMatrix();
  glPopMatrix();
     
}

void draw_angles_text(int i_mec){
  if ( show_angles ) {
   
    glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glColor3ub( 0, 0, 0 );

    int i;
    char texto[30]="texto123";
    glRasterPos2i( 5, 20 );
    sprintf(texto, "angle 1: %f", mec[i_mec].ang1);
    for( i=0; i<(int)strlen( texto ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, texto[i] );
    glRasterPos2i( 5, 15 );
    sprintf(texto, "angle 2: %f", mec[i_mec].ang2_show);
    for( i=0; i<(int)strlen( texto ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, texto[i] );
    glRasterPos2i( 5, 10 );
    sprintf(texto, "angle 3: %f", mec[i_mec].ang3_show);
    for( i=0; i<(int)strlen( texto ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, texto[i] );
    glRasterPos2i( 5, 5 );
    sprintf(texto, "angle 4: %f", mec[i_mec].ang4);
    for( i=0; i<(int)strlen( texto ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, texto[i] );
  }
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void ){
     
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

  glMatrixMode( GL_MODELVIEW );

  glLoadIdentity();
  glMultMatrixf( lights_rotation );
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
  glLoadIdentity();
  glTranslatef( -0.4, -0.4, -2.7f );
//  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );
  glTranslatef( obj_pos[0]+trasladaX, obj_pos[1]-trasladaY, -obj_pos[2] );
  glMultMatrixf( view_rotate );
  glRotated(rotaX,0,1,0);
  glRotated(rotaY,1,0,0);
  
  glTranslatef( -mec[mSelect].x, -mec[mSelect].y, -mec[mSelect].z );

  if(cal_view_isometric){
     glPushMatrix();
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glRotated(30.0,1,0,0);
     glRotated(50.0,0,1,0);
     glGetDoublev (GL_MODELVIEW_MATRIX, view_rotate_ISOMETRIC2);
     for(int i=0;i<16;i++) view_rotate_ISOMETRIC[i] = (float)view_rotate_ISOMETRIC2[i];
     glPopMatrix();
  }
  cal_view_isometric=0;

  draw_axes(.52f);

  glScalef( scale, scale, scale );


////////////////////////////////////////////////////////////////////////////////
  
  if(En_transparency){
      glEnable(GL_BLEND);
      glEnable( GL_LINE_SMOOTH );
      glEnable( GL_POLYGON_SMOOTH );
  }
  else{
      glDisable(GL_BLEND);
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POLYGON_SMOOTH );
  }
  
  int i_mec=mSelect;
  
  draw_m4bar(mSelect);
  if(show_all_mec){
       for(i_mec=0;i_mec<mTotal;i_mec++){
            draw_m4bar(i_mec);
       }
  }
  draw_m4bar(mSelect);
//  else draw_m4bar(mSelect);

  draw_angles_text(mSelect);
  glEnable( GL_LIGHTING );
  glutSwapBuffers();
}

/**************************************** main() ********************/

int main(int argc, char* argv[]){

  int days = try_version(try_version_date[0],try_version_date[1],try_version_date[2]);

  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( 800, 600 );
  
  initialize();
  char title[200];
  sprintf(title,"UTM Research - %s (Free %d days trial)", encabezado, days);
  
  main_window = glutCreateWindow( title );
  glutDisplayFunc( myGlutDisplay );
  GLUI_Master.set_glutReshapeFunc( myGlutReshape );
  GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( myGlutMouse );
  glutMotionFunc( myGlutMotion );

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  glEnable(GL_LIGHTING);
  glEnable( GL_NORMALIZE );

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR,light0_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  
  glEnable(GL_COLOR_MATERIAL);    //Se activa lo materiales de color
  glColorMaterial(GL_BACK,GL_AMBIENT_AND_DIFFUSE);  //Se desean de tipo ambiente y difusi�n (tambien incluyen specular


  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );


////////////////////////////////////////////////////////////////////////////////
  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

  /*** Create the side subwindow ***/
  glui = GLUI_Master.create_glui_subwindow( main_window,
                        GLUI_SUBWINDOW_RIGHT );
                        
  //new GLUI_StaticText( glui, "" );

////////////////////////////////////////////////////////////////////////////////
  openf_panel = new GLUI_Rollout(glui, "Open file", false );
//  GLUI_Button *bOpen = new GLUI_Button( file_panel, "Open file", OPENFILE_ID, control_cb );
  fb = new GLUI_FileBrowser(openf_panel, "", false, OPENFILE_ID, control_cb);
  fb->set_h(180);
//  fb -> set_w(150);
//  fb->hide();
//  GLUI_Button *bAdd  = new GLUI_Button( openf_panel, "Add to file...", ADDTOFILE_ID, control_cb );
//  bAdd  -> set_alignment( GLUI_ALIGN_RIGHT );


////////////////////////////////////////////////////////////////////////////////
  /*** Disable/Enable buttons ***/
  file_panel = new GLUI_Rollout(glui, "Save", false );

  //GLUI_Button *bOpen = new GLUI_Button( file_panel, "Open file", OPENFILE_ID, control_cb );
  savefilepath = new GLUI_TextBox(file_panel,false);
  savefilepath -> set_h(25);
  savefilepath -> set_w(150);
  GLUI_Button *bSave   = new GLUI_Button( file_panel, "Save", SAVEFILE_ID, control_cb );
//  GLUI_Button *bSaveAs = new GLUI_Button( file_panel, "Save As...", SAVEAS_ID, control_cb );
  //bOpen -> set_alignment( GLUI_ALIGN_LEFT );
  //bOpen -> set_w(80);
  bSave   -> set_alignment( GLUI_ALIGN_RIGHT );
//  bSaveAs -> set_alignment( GLUI_ALIGN_LEFT );
  //glui->add_separator_to_panel( file_panel );
  //new GLUI_StaticText( glui, "" );

////////////////////////////////////////////////////////////////////////////

  obj_panel = new GLUI_Rollout(glui, "Properties", true );

  GLUI_Spinner *scale_spinner = new GLUI_Spinner( obj_panel, "Scale:", &scale);
  scale_spinner->set_float_limits( .2f, 4.0 );
  scale_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  
  GLUI_Spinner *spinner_angA = new GLUI_Spinner( obj_panel, "main angle:", &a);
  spinner_angA->set_float_limits( 0.0, 360.0);
  spinner_angA->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Checkbox *checkbox_giro = new GLUI_Checkbox( obj_panel, "reverse rotation", &giro, 1, control_cb );
  checkbox_giro->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_tt = new GLUI_Spinner( obj_panel, "DeltaAngle:", &delta);
  spinner_tt->set_float_limits( -90.0, 90.0);
  spinner_tt->set_alignment( GLUI_ALIGN_RIGHT );

  glui->add_separator_to_panel( obj_panel );
  
  GLUI_Spinner *spinner_L1 = new GLUI_Spinner( obj_panel, "L1:", &L1);
  spinner_L1->set_float_limits( 0.1, 1.0);
  spinner_L1->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_L2 = new GLUI_Spinner( obj_panel, "L2:", &L2);
  spinner_L2->set_float_limits( 0.1, 2.0);
  spinner_L2->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_L3 = new GLUI_Spinner( obj_panel, "L3:", &L3);
  spinner_L3->set_float_limits( 0.1, 2.0);
  spinner_L3->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_L4 = new GLUI_Spinner( obj_panel, "L4:", &L4);
  spinner_L4->set_float_limits( 0.1, 1.0);
  spinner_L4->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_ang1_ph = new GLUI_Spinner( obj_panel, "phase angle:", &ang1_ph);
  spinner_ang1_ph->set_float_limits( -360.0, 360.0);
  spinner_ang1_ph->set_alignment( GLUI_ALIGN_RIGHT );

  glui->add_separator_to_panel( obj_panel );

  GLUI_Spinner *spinner_LE = new GLUI_Spinner( obj_panel, "LE:", &LE);
  spinner_LE->set_float_limits( 0.0, 2.0);
  spinner_LE->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_angE = new GLUI_Spinner( obj_panel, "angleE:", &angE);
  spinner_angE->set_float_limits( -360.0, 360.0);
  spinner_angE->set_alignment( GLUI_ALIGN_RIGHT );
  
  glui->add_separator_to_panel( obj_panel );

  GLUI_Spinner *spinner_posx = new GLUI_Spinner( obj_panel, "position x:", &x);
  spinner_posx->set_float_limits( -10.0, 10.0);
  spinner_posx->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_posy = new GLUI_Spinner( obj_panel, "position y:", &y);
  spinner_posy->set_float_limits( -10.0, 10.0);
  spinner_posy->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_posz = new GLUI_Spinner( obj_panel, "position z:", &z);
  spinner_posz->set_float_limits( -10.0, 10.0);
  spinner_posz->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Spinner *spinner_angM = new GLUI_Spinner( obj_panel, "base angle:", &angM);
  spinner_angM->set_float_limits( -360.0, 360.0);
  spinner_angM->set_alignment( GLUI_ALIGN_RIGHT );

  /********************************************************************************/
  
  /******** Add some controls for lights ********/

  GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Lights", false );

  GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Light 1" );
  GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Light 2" );

  new GLUI_Checkbox( light0, "Enabled", &light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb );
  light0_spinner =
    new GLUI_Spinner( light0, "Intensity:",
                      &light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb );
  light0_spinner->set_float_limits( 0.0, 1.0 );
  GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  new GLUI_Checkbox( light1, "Enabled", &light1_enabled,
                     LIGHT1_ENABLED_ID, control_cb );
  light1_spinner =
    new GLUI_Spinner( light1, "Intensity:",
                      &light1_intensity, LIGHT1_INTENSITY_ID,
                      control_cb );
  light1_spinner->set_float_limits( 0.0, 1.0 );
  sb = new GLUI_Scrollbar( light1, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);

////////////////////////////////////////////////////////////////////////////////
  /*** Add another rollout ***/
  GLUI_Rollout *options = new GLUI_Rollout(glui, "Show items", true );
  new GLUI_Checkbox( options, "Draw paths",  &show_path );
  new GLUI_Checkbox( options, "Draw bars",   &show_bars );
  new GLUI_Checkbox( options, "Draw gears",  &show_gears );
  new GLUI_Checkbox( options, "Draw axes",   &show_axes );
  new GLUI_Checkbox( options, "Draw angles", &show_angles );
  
////////////////////////////////////////////////////////////////////////////////

  //new GLUI_StaticText( glui, "" );
  GLUI_Button *bQuit = new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );
  //bQuit -> set_alignment( GLUI_ALIGN_LEFT );

////////////////////////////////////////////////////////////////////////////////

  /**** Add listbox ****/
  //new GLUI_StaticText( glui, "" );
  //GLUI_Listbox *list = new GLUI_Listbox( glui, "Text:", &curr_string );
  //int i;
  //for( i=0; i<4; i++ )
    //list->add_item( i, string_list[i] );



////////////////////////////////////////////////////////////////////////////////
  /**** Link windows to GLUI, and register idle callback ******/
  
  glui->set_main_gfx_window( main_window );

////////////////////////////////////////////////////////////////////////////////
  /*** Create the bottom subwindow ***/
  glui2 = GLUI_Master.create_glui_subwindow( main_window,
                                             GLUI_SUBWINDOW_BOTTOM );
  glui2->set_main_gfx_window( main_window );

  view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
  view_rot->set_spin( 1.0 );

  new GLUI_Column( glui2, false );
  GLUI_Rotation *lights_rot = new GLUI_Rotation(glui2, "Light", lights_rotation );
  lights_rot->set_spin( .82 );
  
  new GLUI_Column( glui2, false );
  trans_xy = new GLUI_Translation(glui2, "Objects XY", GLUI_TRANSLATION_XY, obj_pos );
  trans_xy->set_speed( .005 );

  new GLUI_Column( glui2, false );
  trans_z = new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
  trans_z->set_speed( .005 );
  
  new GLUI_Column( glui2, false );
  GLUI_Button *bVFrontal = new GLUI_Button( glui2, "Front view", VFRONTAL_ID, control_cb );
  GLUI_Button *bVIsometric = new GLUI_Button( glui2, "Isometric view", V_ISO_ID, control_cb );
  GLUI_Button *bResetXY = new GLUI_Button( glui2, "Reset XYZ", VRESETXY_ID, control_cb );
  
  new GLUI_Column( glui2, true );
  GLUI_Button *bAddMec = new GLUI_Button( glui2, "Add mechanism", ADDMEC_ID, control_cb );
  new GLUI_Checkbox( glui2, "Show all mechanisms",  &show_all_mec , true);
  new GLUI_Checkbox( glui2, "Enable transparency",  &En_transparency , true);

  //dialog_window_1 = glutCreateWindow( "Subventama" );
//  glui3 = GLUI_Master.create_glui( "ventana", 1 );
  //glui3->set_main_gfx_window( main_window );


#if 0
  /**** We register the idle callback with GLUI, *not* with GLUT ****/
  GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

  /**** Regular GLUT main loop ****/
  
  glutMainLoop();

  return EXIT_SUCCESS;
}
