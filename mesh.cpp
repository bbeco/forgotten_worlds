#include "mesh.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#ifdef WIN32
#define sscanf sscanf_s
#endif
using namespace std;
Mesh::Mesh() {};
/************************************************************
 * Normal calculations
 ************************************************************/
void Mesh::computeVertexNormals () {
    //initialisation des normales des vertex
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n = Vec3Df (0.0, 0.0, 0.0);

    //Sum up neighboring normals
    for (unsigned int i = 0; i < triangles.size (); i++) {
        Vec3Df edge01 = vertices[triangles[i].v[1]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df edge02 = vertices[triangles[i].v[2]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df n = Vec3Df::crossProduct (edge01, edge02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            vertices[triangles[i].v[j]].n += n;
    }

    //Normalize
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n.normalize ();
}

/************************************************************
 * Recenter and adjust the mesh - to ease computations
 ************************************************************/
void Mesh::centerAndScaleToUnit () {
    Vec3Df c;
    for  (unsigned int i = 0; i < vertices.size (); i++)
        c += vertices[i].p;
    c /= vertices.size ();
    float maxD = Vec3Df::distance (vertices[0].p, c);
    for (unsigned int i = 0; i < vertices.size (); i++){
        float m = Vec3Df::distance (vertices[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].p = (vertices[i].p - c) / maxD;
}


/************************************************************
 * draw
 ************************************************************/
void Mesh::drawSmooth(){

    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<triangles.size();++i)
    {
        for(int v = 0; v < 3 ; v++){
            glNormal3f(vertices[triangles[i].v[v]].n[0], vertices[triangles[i].v[v]].n[1], vertices[triangles[i].v[v]].n[2]);
            glVertex3f(vertices[triangles[i].v[v]].p[0], vertices[triangles[i].v[v]].p[1] , vertices[triangles[i].v[v]].p[2]);
        }

    }
    glEnd();
}

void Mesh::draw(Vec3Df color){
	glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<triangles.size();++i)
    {
        Vec3Df edge01 = vertices[triangles[i].v[1]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df edge02 = vertices[triangles[i].v[2]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df n = Vec3Df::crossProduct (edge01, edge02);
        n.normalize ();
        glNormal3f(n[0], n[1], n[2]);
        for(int v = 0; v < 3 ; v++){
            glVertex3f(vertices[triangles[i].v[v]].p[0], vertices[triangles[i].v[v]].p[1] , vertices[triangles[i].v[v]].p[2]);
        }

    }
    glEnd();
}

void Mesh::draw(Vec3Df lightPos, Vec3Df camPos, Vec3Df color){
	glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<triangles.size();++i)
    {
        Vec3Df edge01 = vertices[triangles[i].v[1]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df edge02 = vertices[triangles[i].v[2]].p -  vertices[triangles[i].v[0]].p;
        Vec3Df n = Vec3Df::crossProduct (edge01, edge02);
        n.normalize ();
        glNormal3f(n[0], n[1], n[2]);
        for(int v = 0; v < 3 ; v++){
            Vec3Df diffNorm = camPos + lightPos - 2*vertices[triangles[i].v[v]].p;
            diffNorm.normalize();
            float l = std::pow(Vec3Df::dotProduct(diffNorm, n), 7);
            glColor3f(l*color[0], l*color[1], l*color[2]);
            glVertex3f(vertices[triangles[i].v[v]].p[0], vertices[triangles[i].v[v]].p[1] , vertices[triangles[i].v[v]].p[2]);
        }

    }
    glEnd();
    glEnable(GL_LIGHTING);
}


/************************************************************
 * Load
 ************************************************************/
bool Mesh::loadMesh(const char * filename)
{ //do NOT try to understand this function... it is dirty...

    std::vector<int> vhandles;

    const unsigned int LINE_LEN=256;
    char s[LINE_LEN];
    FILE * in;
#ifdef WIN32
    errno_t error=fopen_s(&in, filename,"r");
    if (error!=0)
#else
        in = fopen(filename,"r");
    if (!(in))
#endif
        return false;

    float x, y, z;

    while(in && !feof(in) && fgets(s, LINE_LEN, in))
    {
        // material file
        // vertex
        if (strncmp(s, "v ", 2) == 0)
        {
            if (sscanf(s, "v %f %f %f", &x, &y, &z))
                vertices.push_back(Vertex(Vec3Df(x,y,z)));
        }
        // face
        else if (strncmp(s, "f ", 2) == 0)
        {
            int component(0), nV(0);
            bool endOfVertex(false);
            char *p0, *p1(s+2); //place behind the "f "

            vhandles.clear();

            while (*p1 == ' ') ++p1; // skip white-spaces

            while (p1)
            {
                p0 = p1;

                // overwrite next separator

                // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
                while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' &&
                       *p1 != ' ' && *p1 != '\0')
                    ++p1;

                // detect end of vertex
                if (*p1 != '/') endOfVertex = true;

                // replace separator by '\0'
                if (*p1 != '\0')
                {
                    *p1 = '\0';
                    p1++; // point to next token
                }

                // detect end of line and break
                if (*p1 == '\0' || *p1 == '\n')
                    p1 = 0;


                // read next vertex component
                if (*p0 != '\0')
                {
                    switch (component)
                    {
                    case 0: // vertex
                        vhandles.push_back(atoi(p0)-1);
                        break;

                    case 1: // texture coord
                        //assert(!vhandles.empty());
                        //assert((unsigned int)(atoi(p0)-1) < texcoords.size());
                        //_bi.set_texcoord(vhandles.back(), texcoords[atoi(p0)-1]);
                        break;

                    case 2: // normal
                        //assert(!vhandles.empty());
                        //assert((unsigned int)(atoi(p0)-1) < normals.size());
                        //_bi.set_normal(vhandles.back(), normals[atoi(p0)-1]);
                        break;
                    }
                }

                ++component;

                if (endOfVertex)
                {
                    component = 0;
                    nV++;
                    endOfVertex = false;
                }
            }


            if (vhandles.size()>3)
            {
                //model is not triangulated, so let us do this on the fly...
                //to have a more uniform mesh, we add randomization
                unsigned int k=(false)?(rand()%vhandles.size()):0;
                for (unsigned int i=0;i<vhandles.size()-2;++i)
                {
                    triangles.push_back(Triangle(vhandles[(k+0)%vhandles.size()],vhandles[(k+i+1)%vhandles.size()],vhandles[(k+i+2)%vhandles.size()]));
                }
            }
            else if (vhandles.size()==3)
            {
                triangles.push_back(Triangle(vhandles[0],vhandles[1],vhandles[2]));
            }
            else
            {
                printf("TriMesh::LOAD: Unexpected number of face vertices (<3). Ignoring face");
            }
        }
        memset(&s, 0, LINE_LEN);
    }
    fclose(in);

    centerAndScaleToUnit ();
    computeVertexNormals();
    return true;
}

/*
 * Scale the mesh
 */
void Mesh::scale(float scale)
{
	for (unsigned int i = 0; i < vertices.size(); i += 1)
	{
		vertices[i].p *= scale;
	}	
}
void Mesh::computeBoundingCube () {
    Vec3Df minPoint, maxPoint;
	minPoint=maxPoint=vertices[0].p;	
    for  (unsigned int i = 1; i < vertices.size (); i++)
    {
		for (int j=0;j<3;++j)
		{
			minPoint[j] = minPoint[j]< (vertices[i].p)[j] ? minPoint[j]:(vertices[i].p)[j];
			maxPoint[j] = maxPoint[j]> (vertices[i].p)[j] ? maxPoint[j]:(vertices[i].p)[j];
		}
	}
	
	//set boundind box origin to minimum corner
	bbOriginSimplify=minPoint;
	
	//compute extent of the mesh
	maxPoint-=minPoint;
	bbEdgeSize=max(max(maxPoint[0],maxPoint[1]),maxPoint[2]);
	
}
Mesh Mesh::simplifyMesh(unsigned int r)
{
	const vector<Vertex> & vertices = this->vertices;
    const vector<Triangle> & triangles = this->triangles;
	vector<Triangle> simplifiedTriangles;
	vector<Vertex> simplifiedVertices;
	std::map<unsigned int, unsigned int > newIndexRemapping;
	Mesh simplified;
	computeBoundingCube();
	Vec3Df tmp = Vec3Df(0.01f, 0.01f, 0.01f);
	grid = Grid(bbOriginSimplify - tmp, bbEdgeSize + 2*0.01, r );
	grid.putVertices(vertices);
	grid.computeRepresentatives();
	
	int count = 0;
    for(RepresentativeList::iterator it = grid.representatives.begin() ; it != grid.representatives.end (); it++, count++){
        newIndexRemapping[(*it).first] = count;
        simplifiedVertices.push_back((*it).second);
    }

	for(unsigned int i = 0; i < triangles.size(); i++){
		Vec3Df a, b, c;
		a = vertices[triangles[i].v[0]].p;
		b = vertices[triangles[i].v[1]].p;
		c = vertices[triangles[i].v[2]].p;
		
		int ind1, ind2, ind3;
		ind1 = grid.isContainedAt(a);
		ind2 = grid.isContainedAt(b);
		ind3 = grid.isContainedAt(c);
		
		if (ind1 != ind2 && ind2 != ind3 && ind1 != ind3) {
			unsigned int newIndeces[3];
		
			newIndeces[0] = newIndexRemapping[ind1];
			newIndeces[1] = newIndexRemapping[ind2];
			newIndeces[2] = newIndexRemapping[ind3];
			
			simplifiedTriangles.push_back(Triangle(newIndeces[0], newIndeces[1], newIndeces[2]));
		}
		
	}
	simplified = Mesh(simplifiedVertices , simplifiedTriangles);
	simplified.computeVertexNormals();
	return simplified;
}


#ifdef WIN32
#undef sscanf 
#endif
