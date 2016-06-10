#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "trackball.h"
#include "game.h"
#include "loadppm.h"
using namespace std;

unsigned int w_win = 640, h_win = 480;
Game game;
Vec3Df origin;
float LightPos[4] = {0, 0, 0, 1};
bool drawBoundingBox = true;
float rotation = 0;
int NbVertX=60, NbVertY=40;
float x_move = 12;
std::vector<GLuint> Texture;
//vertices
std::vector<float> SurfaceVertices3f;
//normals
std::vector<float> SurfaceNormals3f;
//colors
std::vector<float> SurfaceColors3f;
//tex coords
std::vector<float> SurfaceTexCoords2f;
//triangle indices (three successive entries: n1, n2, n3 represent a triangle, each n* is an index representing a vertex.)
std::vector<unsigned int> SurfaceTriangles3ui;
float theta = 0;
void initTexture()
{
	Texture.resize(3);
	Texture[0]=0;
	Texture[1]=0;
	Texture[2]=0;

	
	PPMImage image3("sand.ppm");
	glGenTextures(1, &Texture[2]);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image3.sizeX, image3.sizeY, 
		GL_RGB, GL_UNSIGNED_BYTE, image3.data);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void initSurfaceMesh()
{
	
	//vertices with 3 coordinates
	SurfaceVertices3f.resize(3*2*2*NbVertX*NbVertY);
	//normals with 3 coordinates
	SurfaceNormals3f.resize(3*2*2*NbVertX*NbVertY);
	//texture coords per vertex
	SurfaceTexCoords2f.resize(2*2*2*NbVertX*NbVertY);
	//triangles (2 per default)
	SurfaceTriangles3ui.resize(3*2*NbVertX*NbVertY);
	//per vertex colors 
	SurfaceColors3f.resize(3*2*2*NbVertX*NbVertY);
	
	//vectors defining the plane
	float u[3], v[3], n[3];
	float l;

	for (int i = 0; i < NbVertX; i++) {
		for (int j = 0; j < NbVertY; j++) {
			
			//define coords
			SurfaceVertices3f[0+3*4*(i*NbVertY+j)]=i;
			SurfaceVertices3f[1+3*4*(i*NbVertY+j)]=j;
			SurfaceVertices3f[2+3*4*(i*NbVertY+j)]=cos(i*M_PI/4)*sin(j*M_PI/4);

			SurfaceVertices3f[3+3*4*(i*NbVertY+j)]=i+1;
			SurfaceVertices3f[4+3*4*(i*NbVertY+j)]=j;
			SurfaceVertices3f[5+3*4*(i*NbVertY+j)]=cos((i+1)*M_PI/4)*sin(j*M_PI/4);

			SurfaceVertices3f[6+3*4*(i*NbVertY+j)]=i+1;
			SurfaceVertices3f[7+3*4*(i*NbVertY+j)]=j+1;
			SurfaceVertices3f[8+3*4*(i*NbVertY+j)]=cos((i+1)*M_PI/4)*sin((j+1)*M_PI/4);

			SurfaceVertices3f[9+3*4*(i*NbVertY+j)]=i;
			SurfaceVertices3f[10+3*4*(i*NbVertY+j)]=j+1;
			SurfaceVertices3f[11+3*4*(i*NbVertY+j)]=cos(i*M_PI/4)*sin((j+1)*M_PI/4);
			
			u[0] = 1;
			u[1] = 0;
			u[2] = cos((i+1)*M_PI/4)*sin(j*M_PI/4) - cos(i*M_PI/4)*sin(j*M_PI/4);
			
			v[0] = 0;
			v[1] = 1;
			v[2] = cos(i*M_PI/4)*sin((j+1)*M_PI/4) - cos(i*M_PI/4)*sin(j*M_PI/4);
		
			n[0] = -u[2];
			n[1] = -v[2];
			n[2] = 1;
			
			l = sqrt(1 + u[2]*u[2] + v[2]*v[2]);
			
			n[0] /= l;
			n[1] /= l;
			n[2] /= l;

			//define normals
			SurfaceNormals3f[0+3*4*(i*NbVertY+j)]=n[0];
			SurfaceNormals3f[1+3*4*(i*NbVertY+j)]=n[1];
			SurfaceNormals3f[2+3*4*(i*NbVertY+j)]=n[2];

			SurfaceNormals3f[3+3*4*(i*NbVertY+j)]=n[0];
			SurfaceNormals3f[4+3*4*(i*NbVertY+j)]=n[1];
			SurfaceNormals3f[5+3*4*(i*NbVertY+j)]=n[2];

			SurfaceNormals3f[6+3*4*(i*NbVertY+j)]=n[0];
			SurfaceNormals3f[7+3*4*(i*NbVertY+j)]=n[1];
			SurfaceNormals3f[8+3*4*(i*NbVertY+j)]=n[2];

			SurfaceNormals3f[9+3*4*(i*NbVertY+j)]=n[0];
			SurfaceNormals3f[10+3*4*(i*NbVertY+j)]=n[1];
			SurfaceNormals3f[11+3*4*(i*NbVertY+j)]=n[2];

			//define colors
			//if (cos(i*M_PI) > 0.8) {
				SurfaceColors3f[0+3*4*(i*NbVertY+j)]=0.9;
				SurfaceColors3f[1+3*4*(i*NbVertY+j)]=0.7;
				SurfaceColors3f[2+3*4*(i*NbVertY+j)]=0.4;
				
				SurfaceColors3f[3+3*4*(i*NbVertY+j)]=0.9;
				SurfaceColors3f[4+3*4*(i*NbVertY+j)]=0.7;
				SurfaceColors3f[5+3*4*(i*NbVertY+j)]=0.4;

				SurfaceColors3f[6+3*4*(i*NbVertY+j)]=0.9;
				SurfaceColors3f[7+3*4*(i*NbVertY+j)]=0.7;
				SurfaceColors3f[8+3*4*(i*NbVertY+j)]=0.4;
				
				SurfaceColors3f[9+3*4*(i*NbVertY+j)]=0.9;
				SurfaceColors3f[10+3*4*(i*NbVertY+j)]=0.7;
				SurfaceColors3f[11+3*4*(i*NbVertY+j)]=0.4;

			//define texcoords
			SurfaceTexCoords2f[0+2*4*(i*NbVertY+j)]=0;
			SurfaceTexCoords2f[1+2*4*(i*NbVertY+j)]=0;

			SurfaceTexCoords2f[2+2*4*(i*NbVertY+j)]=1;
			SurfaceTexCoords2f[3+2*4*(i*NbVertY+j)]=0;
			
			SurfaceTexCoords2f[4+2*4*(i*NbVertY+j)]=1;
			SurfaceTexCoords2f[5+2*4*(i*NbVertY+j)]=1;

			SurfaceTexCoords2f[6+2*4*(i*NbVertY+j)]=0;
			SurfaceTexCoords2f[7+2*4*(i*NbVertY+j)]=1;

			
			//define tri indices
			SurfaceTriangles3ui[0+3*2*(i*NbVertY+j)]=0+2*2*(i*NbVertY+j);
			SurfaceTriangles3ui[1+3*2*(i*NbVertY+j)]=1+2*2*(i*NbVertY+j);
			SurfaceTriangles3ui[2+3*2*(i*NbVertY+j)]=2+2*2*(i*NbVertY+j);

			SurfaceTriangles3ui[3+3*2*(i*NbVertY+j)]=2+2*2*(i*NbVertY+j);
			SurfaceTriangles3ui[4+3*2*(i*NbVertY+j)]=3+2*2*(i*NbVertY+j);
			SurfaceTriangles3ui[5+3*2*(i*NbVertY+j)]=0+2*2*(i*NbVertY+j);
		}
	}
}

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
	
	initSurfaceMesh();
	initTexture();
}

void drawSurface()
{
//This function is complete (!) and will draw the data in the Surface**** arrays.
//You do not need to modify this one.

	for (unsigned int t=0; t<SurfaceTriangles3ui.size();t+=3)
	{
		glBegin(GL_TRIANGLES);
		
		for (int triVertex=0; triVertex<3;++triVertex)
		{
			int vIndex=SurfaceTriangles3ui[t+triVertex];

			glTexCoord2fv(&(SurfaceTexCoords2f[2*vIndex]));
			glNormal3fv(&(SurfaceNormals3f[3*vIndex]));
			glColor3fv(&(SurfaceColors3f[3*vIndex]));
			glVertex3fv(&(SurfaceVertices3f[3*vIndex]));
		}
		glEnd();
	}
}

//take keyboard input into account
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
    {
	case 27:     // ESC
        	exit(0);
	case 'b':
		drawBoundingBox = !drawBoundingBox;
		break;
	case 'f': //flip enemy
		//enemies[1].flip();
		break;
	case 'r': //rotate enemy
		rotation += 45;
		//enemies[1].zRotate(rotation);
		break;
	/* Movements */
	case 'd':
		game.hero.p[0] += 0.1;
		break;
	case 'a':
		game.hero.p[0] -= 0.1;
		break;
	case 's':
		game.hero.p[1] -= 0.1;
		break;
	case 'w':
		game.hero.p[1] += 0.1;
		break;
	case 'j':
		theta += 22.5;
		game.hero.zRotate(theta);
		break;
	case 'k':
		theta -= 22.5;
		game.hero.zRotate(theta); 
		break;
		
    }
}

void setOrigin()
{
	GLdouble modelview[16]; //modelview matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	GLdouble projection[16]; //projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	GLint view[4]; //the view port
	glGetIntegerv(GL_VIEWPORT, view);
	GLdouble objX, objY, objZ;
	if (gluUnProject(0.0, 0.0, 0.0, modelview, projection, view, &objX, &objY, &objZ) == GLU_FALSE) {
		cout << "Error in gluUnProject()" << endl;
		exit(1);
	}
	origin = Vec3Df((float)objX, (float)objY, (float)objZ);
	cout << "origin = " << origin << endl;
}

void display() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glPushMatrix();
	glTranslatef(-x_move,-2.5,-0.5);
	glRotatef(-45,1,0,0);
	drawSurface();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	glPopAttrib();
}

void displayInternal(void)
{
    // Effacer tout
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z
    

    glLoadIdentity();  // repere camera

    tbVisuTransform(); // origine et orientation de la scene

	//drawAbsoluteCoord();
    	game.display();
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
    //gluOrtho2D(-1.0, 1.0, -1.0,1.0);
    gluPerspective (50, (float)w/h, 1, 15); //XXX setting the biew space
    glMatrixMode(GL_MODELVIEW);
}
int count = 0;
void animate()
{	
//	count++;
//	count = count%2;
//	if(count == 1){
		game.bullets.push_back(game.hero.shoot());
//	}
	vector<Bullet>::iterator it;
	for (it = game.bullets.begin(); it->p[0] > 3.5; it++);
	game.bullets.erase(game.bullets.begin(), it);
	//bool tmp =true;
	for (vector<Bullet>::iterator it = game.bullets.begin(); it != game.bullets.end(); it++) {
		it->update();
	}
	if(x_move <= 50){
		x_move += 0.04;
		
		if (x_move >= 50) {
			game.activateBoss = true;
		}
	} else {
		if (game.boss.p[0] >= 2) {
			game.boss.p[0] -= 0.3;
		} else {
			game.boss.create_boss_hands();
			game.drawArm = true;
		}
	}
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
    tbInitTransform();     
    //tbHelp();
    
	setOrigin();
	game.setScreenOrigin(origin);
         
    

	// cablage des callback
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(displayInternal);
    glutMouseFunc(tbMouseFunc);    // traqueboule utilise la souris
    glutMotionFunc(tbMotionFunc);  // traqueboule utilise la souris
    glutIdleFunc(animate);

    // lancement de la boucle principale
    glutMainLoop();
    
    return 0;  // instruction jamais exécutée
}
