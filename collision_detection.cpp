#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "enemy.h"
#include "trackball.h"

unsigned int w_win = 600, h_win = 600;

Enemy enemy;
bool drawBoundingBox = true;
float LightPos[4] = {1,1,0.4,1};

void init()
{
    glDisable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

	//int MatSpec [4] = {1,1,1,1};
 //   glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
 //   glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);


    // Enable Depth test
    glEnable( GL_DEPTH_TEST );
	
	//glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
	//Draw frontfacing polygons as filled
    glPolygonMode(GL_FRONT,GL_FILL);
	//draw backfacing polygons as outlined
    glPolygonMode(GL_BACK,GL_LINE);
	glShadeModel(GL_SMOOTH);
	//mesh.loadMesh("David.obj");
	glEnable(GL_LIGHTING);
}

void drawUnitFace(float red, float green, float blue)
{
	//1) draw a unit quad in the x,y plane oriented along the z axis
	//2) make sure the orientation of the vertices is positive (counterclock wise)
	//3) What happens if the order is inversed?
	glColor3f(red, green, blue);
	glNormal3f(0,0,-1);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 0, 0);
	glEnd();
}

void drawUnitCube(float red, float green, float blue)
{
	//1) draw a cube using your function drawUnitFace
	//rely on glTranslate, glRotate, glPushMatrix, and glPopMatrix
	//the latter two influence the model matrix, as seen during the course.
	//glPushMatrix stores the current matrix and puts a copy on 
	//the top of a stack.
	//glPopMatrix pops the top matrix on the stack
	
	//bottom
	drawUnitFace(red, green, blue);
	glPushMatrix();
	
	//right
	glTranslatef(1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue);
	
	//top
	glTranslatef(1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue);
	
	//left
	glTranslatef(1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawUnitFace(red, green, blue);
	
	glPopMatrix();
	
	//behind
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -1);
	drawUnitFace(red, green, blue);
	glPopMatrix();
	
	//front
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -1, 0);
	drawUnitFace(red, green, blue);
	glPopMatrix();
}

void drawLight()
{	
	glPushMatrix();
	glTranslatef(LightPos[0], LightPos[1], LightPos[2]);
	glDisable(GL_LIGHTING);
	//glTranslatef(0.5, 0.5 , 0.5);
	glColor3f(1, 1, 0);
	glutSolidSphere(0.5, 100, 100);
	//FIXME there should be possible to save the color in the matrix and then to push/pop the previous setting
	glColor3f(1, 1, 1); 
	glEnable(GL_LIGHTING);
	glPopMatrix();

}
//take keyboard input into account
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
    {
	case 27:     // ESC
        exit(0);
	case 'L':
		//turn lighting on
		glEnable(GL_LIGHTING);
		break;
	case 'l':
		//turn lighting off
		glDisable(GL_LIGHTING);
		break;
	case 'b':
		drawBoundingBox = !drawBoundingBox;
		break;
	case 'w':
		LightPos[0] += 0.1;
		break;
	case 's':
		LightPos[0] -= 0.1;
		break;
	case 'a':
		LightPos[1] -= 0.1;
		break;
	case 'd':
		LightPos[1] += 0.1;
		break;
    }
}

void display(void)
{
	glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
	drawLight();
	enemy.draw();
	if (drawBoundingBox) {
		enemy.drawBoundingBox();
	}
	//drawUnitCube(1, 1, 1);
}

void displayInternal(void)
{
    // Effacer tout
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z
    

    glLoadIdentity();  // repere camera

    tbVisuTransform(); // origine et orientation de la scene

    display();    

    glutSwapBuffers();
    glutPostRedisplay();
}

// pour changement de taille ou desiconification
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
    gluPerspective (50, (float)w/h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // couches du framebuffer utilisees par l'application
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // position et taille de la fenetre
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(w_win, h_win);
    glutCreateWindow(argv[0]);

    init();
	
    // Initialize viewpoint
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    //tbInitTransform();     
    //tbHelp();
         
    

	// cablage des callback
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(displayInternal);
    glutMouseFunc(tbMouseFunc);    // traqueboule utilise la souris
    glutMotionFunc(tbMotionFunc);  // traqueboule utilise la souris
    //glutIdleFunc(animate);

    // lancement de la boucle principale
    glutMainLoop();
    
    return 0;  // instruction jamais exécutée
}
