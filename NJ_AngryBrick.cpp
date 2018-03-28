//---------------------------------------
// Program: NJ_AngryBrick.cpp
// Purpose: CSCE 4813 Project 3
// Author:  Noemi Johnson
// Date:    3/5/18
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
# include <iostream>
using namespace std; 

// Constants
#define ROTATE 1
#define TRANSLATE 1

#define RADIUS 1000

#define SIZE 200
#define COUNT 10
#define SLOW 0.995


//Global Variables
// Transformation variables 
int xangle = 0;
int yangle = 0;
int zangle = 0;
float Radius[COUNT];
float Point_x1; //starting point and ending point on the x-axis
float Point_y1; //starting point and ending point on the y-axis
float Point_z1; //starting point and ending point on the z-axis


float Px[COUNT];
float Py[COUNT];
float Pz[COUNT];


float X[SIZE+1][SIZE+1];
float Y[SIZE+1][SIZE+1];
float Z[SIZE+1][SIZE+1];

float Vx[COUNT];
float Vy[COUNT];
float Vz[COUNT];

float Gravity =  0.3;
float Bounce = -0.9;

int mode = TRANSLATE;

bool throw_it = false;
bool it_bounced = false;

//-----------------------------------------
// Angry Brick Initial Function
//-----------------------------------------
void init_brick()
{
  //this is to draw the brick surface
    int i, j;
	 
 	for (i = 0; i <= SIZE; i++)
   {
      float angle1 = 2 * i * M_PI / SIZE;
      for (j = 0; j <= SIZE; j++)
      {
         float angle2 = j * M_PI / SIZE;
         X[i][j] = cos(angle1) * sin(angle2);
         Y[i][j] = sin(angle1) * sin(angle2);
         Z[i][j] = cos(angle2);
      }
   }
}


//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2, 2, -2, 2, -2, 2);
  glEnable(GL_DEPTH_TEST); 

	init_brick();
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   
 	Point_x1 = x; 
	Point_y1 = y;
   glutPostRedisplay();
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
	
 Bounce /= 0.9;
   
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Update brick position
      Px[i] += Vx[i]/RADIUS;
      Py[i] -= Vy[i]/RADIUS;
      Pz[i] += Vz[i]/RADIUS;

      // Update brick velocity
      Vx[i] += Bounce; 
      Vy[i] += Gravity;
      Vz[i] += Bounce; 
 
   
       // Bounce off walls
      if (Px[i] > ((RADIUS/2) - Radius[i])) 
	    {
			Px[i] = ((RADIUS/2) - Radius[i]); Vx[i] *= Bounce;     
		}
	   
      if (Py[i] > ((RADIUS/2) - Radius[i]))
		  
         {
			 Py[i] = ((RADIUS) - Radius[i]); Vy[i] *= Bounce;   
		 }
      if (Pz[i] > ((RADIUS/2) - Radius[i])) 
		  
         {
			 Pz[i] =((RADIUS/2) -Radius[i]); Vz[i] *= Bounce;
		 	
		 }
      if (Px[i] < ((-RADIUS/2)-Radius[i]) )
		 
         {
		    Px[i] = ((-RADIUS/2) + Radius[i]); Vx[i] *= Bounce; 
			 	
		 }
	   
      if (Py[i] < ((-RADIUS/2) + Radius[i]) )
		
         {
			Py[i] = ((-RADIUS/2)+ Radius[i]) ; Vy[i] *= Bounce; 		 
			  
		 }
		
	   
      if (Pz[i] < ((-RADIUS) + Radius[i] ) ) 	  	
         {
			 Pz[i] = ((-RADIUS) + Radius[i] ) ; Vz[i] *= Bounce;		
		 }

   }
   glutPostRedisplay();

} 

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
	
   // Move bouncing brick
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Update brick position
      Px[i] += Vx[i]+5 ;
      Py[i] += Vy[i]+5 ;
      Pz[i] += Vz[i]+5 ;

      // Update brick velocity
      Vy[i] -= Gravity;
 
      // Bounce off walls
      if (Px[i] > ((RADIUS/2) - Radius[i])) 
	    {
			Px[i] = ((RADIUS/2) - Radius[i]) ; Vx[i]   *= Bounce; 	      
		}
	   
      if (Py[i] > ((RADIUS/2) - Radius[i]))
		  
         {
			 Py[i] = ((RADIUS/2) - Radius[i]) ; Vy[i] *= Bounce; 	      
		 }
      if (Pz[i] > ((RADIUS/2) - Radius[i])) 		  
         {
			 Pz[i] =((RADIUS/2) -Radius[i])  ; Vz[i] *= Bounce;		 	
		 }
      if (Px[i] < ((-RADIUS/2)-Radius[i]) )	 
         {
		    Px[i] = ((-RADIUS/2) + Radius[i])   ; Vx[i] *= Bounce; 	
		 }
	   
      if (Py[i] < ((-RADIUS/2) + Radius[i]) )
	    {
		  Py[i] = ((-RADIUS/2) + Radius[i])  ; Vy[i] *= Bounce; 	 		  
		}
		 
      if (Pz[i] < ((-RADIUS/2) + Radius[i]) ) 	  		
         {
			 Pz[i] = ((-RADIUS/2) + Radius[i] ) ; Vz[i] *= Bounce;		
		 }
   }
	
   glutTimerFunc(4, timer, 0);
   glutPostRedisplay();

}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
  // Handle mouse down
   static int xdown, ydown;
   if (state == GLUT_DOWN)
   {
      xdown = x;
      ydown = y;
   }


   // Handle TRANSLATE and Left Mouse Button Pressed
   if ((mode == TRANSLATE) && ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN))
   {
	  throw_it = false;
	  Point_x1 += (x - xdown);
      Point_y1-= (y - ydown);
	  xangle -= (y - ydown);
      yangle += (x - xdown);
      zangle = 0;
	   
      glutPostRedisplay();
   
   }
	 // Handle TRANSLATE and Left Mouse Button Released
   if ((mode == TRANSLATE) && ( button == GLUT_LEFT_BUTTON && state == GLUT_UP))
   {
	  throw_it = true;
	  Point_x1 +=(x - xdown); 
      Point_y1-= (y - ydown);
	  xangle += (y - ydown);
      yangle += (x - xdown);
      zangle = 0;
	  
      glutPostRedisplay();
   
   }
 
}

//---------------------------------------
// Function to draw 3D brick
//---------------------------------------

void brick(float midx, float midy, float midz, float size)
{

   // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

   // Draw 6 faces
   glBegin(GL_POLYGON);					 
   	glColor3f(1.0, 0.0, 0.0);		      glVertex3f(ax, ay, az);
   	glColor3f( 1.0,  0.0,  0.0);         glVertex3f(bx, by, bz);
	glColor3f( 1.0,  0.5, 0.0);          glVertex3f(cx, cy, cz);
	glColor3f( 1.0, 1.0, 0.0 );           glVertex3f(dx, dy, dz);
   								
   glEnd();

   glBegin(GL_POLYGON);	
   glColor3f(0.0, 1.0, 0.0);			glVertex3f(ax, ay, az);
   	glColor3f( 1.0,  0.0,  0.0);         glVertex3f(dx, dy, dz);
	glColor3f( 1.0,  0.5, 0.0);   		glVertex3f(hx, hy, hz);
	glColor3f( 1.0, 1.0, 0.0 );         glVertex3f(ex, ey, ez);
	glEnd();

   glBegin(GL_POLYGON);
   glColor3f(0.0, 0.0, 1.0);
   	glColor3f( 1.0,  0.0, 0.0);     glVertex3f(ax, ay, az);
	glColor3f( 1.0,  0.5, 0.0);		glVertex3f(ex, ey, ez);
	glColor3f( 1.0, 1.0, 0.0 );		glVertex3f(fx, fy, fz);				
	glColor3f( 1.0, 0.5, 1.0 );		glVertex3f(bx, by, bz);	
   	glEnd();

    glBegin(GL_POLYGON);
   glColor3f(0.0, 1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(fx, fy, fz);
   glVertex3f(ex, ey, ez);
   glVertex3f(hx, hy, hz);
   glEnd();

    glBegin(GL_POLYGON);
   	glColor3f(1.0,0.5,0.0 );
   glVertex3f(gx, gy, gz);
   glVertex3f(cx, cy, cz);
   glVertex3f(bx, by, bz);
   glVertex3f(fx, fy, fz);
   glEnd();

	glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);   	glVertex3f(gx, gy, gz);
   	glColor3f( 1.0, 0.5,  0.0);	   	glVertex3f(hx, hy, hz);					
   	glColor3f( 1.0, 0.5,  0.0); 	glVertex3f(cx, cy, cz);
   	glColor3f( 1.0, 0.5, 1.0 );		glVertex3f(dx, dy, dz);
  
   glEnd();		
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
	glTranslatef(Point_x1/500 , Point_y1/500, Point_z1/500);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);
	

   brick(0.3, 0.3, 0.3, 0.6);
	
  int i;
   for (i = 0; i < COUNT; i++)
   {
	    // Draw ball at current location
      brick(Px[i], Py[i], Pz[i], Radius[i]);
   }

	if (throw_it)
	{
		it_bounced = false;
		
		 Point_x1 += ((Point_x1)*0.3f);
	     Point_y1 += ((Point_y1)*0.05f);
		glTranslatef(Point_x1/500 , Point_y1/500, Point_z1/500);
		
		glutTimerFunc(4, timer, 0);
		glRotatef(xangle, 1.0, 0.0, 0.0);
   		glRotatef(yangle, 0.0, 1.0, 0.0);
  		glRotatef(zangle, 0.0, 0.0, 1.0);
		
	 	glutPostRedisplay();


		if (Point_x1 > 500 || Point_x1 <-500)
		{ 
			throw_it = !throw_it;
			it_bounced = true;
			Point_x1 += ((Point_x1)*0.3);
			Point_y1 += ((Point_y1)*0.05);
			glTranslatef(Point_x1/500 , Point_y1/500, Point_z1/500);
			glutTimerFunc(4, timer, 0);
			glRotatef(xangle, 1.0, 0.0, 0.0);
			glRotatef(yangle, 0.0, 1.0, 0.0);
			glRotatef(zangle, 0.0, 0.0, 1.0);
			glutPostRedisplay();
		}
	}	
	if(it_bounced)
	{
		xangle =+ xangle*1;	
		yangle -= yangle*1;
		zangle =+ yangle*1;
		Point_y1 -= Point_y1*0.7;
		glTranslatef(Point_x1/500 , Point_y1/500, Point_z1/500);
		glutTimerFunc(4, timer, 0);	
		glRotatef(xangle, 1.0, 0.0, 0.0);
   		glRotatef(yangle, 0.0, 1.0, 0.0);
  		glRotatef(zangle, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		
		if ( Point_y1 < -500 || Point_y1 > 400)
		{
			it_bounced = !it_bounced;
			
			xangle =+ xangle*1;	
			yangle += yangle*1;
			zangle =+ yangle*1;
			Point_y1 += 8.7;
			Point_x1 -= 5.8;
			glTranslatef(Point_x1/500 , Point_y1/500, Point_z1/500);
			glutTimerFunc(5, timer, 0);
			glRotatef(xangle, 1.0, 0.0, 0.0);
   			glRotatef(yangle, 0.0, 1.0, 0.0);
  			glRotatef(zangle, 0.0, 0.0, 1.0);
		
			glutPostRedisplay();
		}
 
	}
	

	if (xangle > 360.0f)  
	{
		xangle -= 360.0f; 
	}
		   
	 glutSwapBuffers(); 
	 glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("AngryBrick");
	init();
	// Specify callback function
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutTimerFunc(4, timer, 0);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
   glutMainLoop();
   return 0;
}
