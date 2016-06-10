#include "grid.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

int Grid::isContainedAt(const Vec3Df & pos){
    //returns index that contains the position
    if (pos[0] < origin[0] || pos[0] > origin[0] + size) {
		std::cout << "x is out of bound" << std::endl;
		return -1;
	}
	if (pos[1] < origin[1] || pos[1] > origin[1] + size) {
		std::cout << "y is out of bound" << std::endl;
		return -1;
	}
	if (pos[2] < origin[2] || pos[2] > origin[2] + size) {
		std::cout << "z is out of bound" << std::endl;
		return -1;
	}
	
    unsigned int indice = 0;
    unsigned int i, j, k;
    i = floor((pos[0] - origin[0])/(size/r));
    j = floor((pos[1] - origin[1])/(size/r));
    k = floor((pos[2] - origin[2])/(size/r));
    indice = i + j*r + k*r*r;
    return indice;
}

void Grid::computeRepresentatives()
{
	Vec3Df avg = Vec3Df(0, 0, 0);
	for (CellContent::iterator it = verticesInCell.begin(); it != verticesInCell.end(); it++) {
		for (unsigned int i = 0; i < verticesInCell[it->first].size(); i++) {
			avg += originalVertices[verticesInCell[it->first][i]].p;
		}
		avg /= (float)verticesInCell[it->first].size();
	/*	if (isContainedAt(avg) != it->first) {
			std::cout << "avg" << avg << std::endl;
			std::cout << "index = " << it->first << "avg_index = " << isContainedAt(avg) << std::endl;
		}*/
		representatives[it->first] = Vertex(avg);
		avg = Vec3Df(0,0,0);
	}
}

void Grid::putVertices(const std::vector<Vertex> & vertices){
    //put vertices in the corresponding voxels.
   originalVertices = vertices;
    for (unsigned int i=0;i<vertices.size();i++) {
		int index = isContainedAt(vertices[i].p);
		if (index < 0) {
			std::cout << "Found index out of grid!" << index << std::endl;
			std::cout << "vertex = " << vertices[i].p << std::endl;
			std::cout << "origin = " << origin << " size = " << size << std::endl;
			
		}
		verticesInCell[index].push_back(i);
	}
}

void Grid::drawCell(const Vec3Df & Min,const Vec3Df& Max) {

    const Vec3Df corners[8] =
    {
        Vec3Df(Min[0],Min[1],Min[2]),
        Vec3Df(Max[0],Min[1],Min[2]),
        Vec3Df(Min[0],Max[1],Min[2]),
        Vec3Df(Max[0],Max[1],Min[2]),
        Vec3Df(Min[0],Min[1],Max[2]),
        Vec3Df(Max[0],Min[1],Max[2]),
        Vec3Df(Min[0],Max[1],Max[2]),
        Vec3Df(Max[0],Max[1],Max[2])
    };


    static const unsigned short faceCorners[6][4] =
    {
        { 0,4,6,2 },
        { 5,1,3,7 },
        { 0,1,5,4 },
        { 3,2,6,7 },
        { 0,2,3,1 },
        { 6,4,5,7 }
    };

    glBegin(GL_QUADS);
    for (unsigned short f=0;f<6;++f)
    {
        const unsigned short* face = faceCorners[f];
        for(unsigned int v = 0; v<4; v++)
            glVertex3f(corners[face[v]][0], corners[face[v]][1], corners[face[v]][2]);

    }
    glEnd();

}

void Grid::drawGrid(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //Complete this function by calling draw cell several times.
    float inc = size/r;
    Vec3Df app = origin;
    Vec3Df maxV = Vec3Df(inc, inc, inc);
	for(unsigned int k = 0; k < r; k++) {
		for (unsigned int j = 0; j < r; j++) {
			for (unsigned int i = 0; i < r; i++) {
				drawCell(app, app + maxV);
				app[0] += inc;
			}
			app[0] = origin[0];
			app[1] += inc;
		}
		app[1] = origin[1];
		app[2] += inc;
	}
	
    glPopAttrib();
}

void Grid::drawActualGrid(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(1.0f);
    glColor3f(1.0f,0.0f,0.0f);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //Complete this function by calling draw cell several times.
    float inc = size/r;
    Vec3Df app = origin;
    Vec3Df maxV = Vec3Df(inc, inc, inc);
	for(unsigned int k = 0; k < r; k++) {
		for (unsigned int j = 0; j < r; j++) {
			for (unsigned int i = 0; i < r; i++) {
				if (verticesInCell[i + j*r + k*r*r].size() != 0)
				drawCell(app, app + maxV);
				app[0] += inc;
			}
			app[0] = origin[0];
			app[1] += inc;
		}
		app[1] = origin[1];
		app[2] += inc;
	}
	
    glPopAttrib();
}


