#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;
using namespace glm;

vector<vec3> vertices;
vector<unsigned> indices;

void add_tri(int a, int b, int c) {
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
}

void add_vert(float x, float y, float z,
			  float, float, float) {
	vertices.push_back({x,y,z});
}

static GLuint vao = -1;
static GLuint vbo = -1;
static GLuint vbo_col = -1;
static GLuint ibo = -1;

#define CS_TRI
// #define ES_TRI
// #define BUNNY

#define VAO

void load_mesh() {
#ifdef BUNNY
	#include "bunny.data"
#elif defined CS_TRI
	add_vert(-1,-1,0,0,0,0);
	add_vert( 1,-1,0,0,0,0);
	add_vert(0,1,0,0,0,0);
#else
	add_vert( 0, 0,-10,0,0,0);
	add_vert(10, 0,-10,0,0,0);
	add_vert( 0,10,-10,0,0,0);
#endif
	add_tri(0,1,2);


	// TODO: Aufgabe 1.3
	// VAO aufsetzen
}

void draw_mesh() {
	// TODO: Aufgabe 1.3
	// VAO rendern
}

