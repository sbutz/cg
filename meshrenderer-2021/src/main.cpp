#include "mesh.h"
#include "shader.h"
#include "context.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;
using namespace glm;


mat4 perspective_projection_transform(float fovy, float aspect, float n, float f) {
	mat4 P(1);
	// TODO: Aufgabe 1.4
	// OpenGL Projektionsmatrix
	return P;
}

mat4 viewing_transform(const vec3 &pos, const vec3 &dir, const vec3 &up) {
	mat4 V(1);
	// TODO: Aufgabe 1.5
	// OpenGL Viewingmatrix
	return V;
}


int main() {
	ContextParameters params;
	params.gl_major = 3;
	params.gl_minor = 3;
	params.title = "CG'20 mini renderer";
	Context::init(params);

	load_mesh();
	load_shader();

	while (Context::running()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bind_shader();
		float aspect = float(Context::instance().vp_w) / Context::instance().vp_h;
		mat4 P = perspective_projection_transform(65, aspect, 1, 300);
		mat4 V = viewing_transform(vec3(180,0,50), vec3(-1, 0, 0), vec3(0,0,1));
		glUniformMatrix4fv(uniform_location("model"), 1, GL_FALSE, glm::value_ptr(mat4(1)));
		glUniformMatrix4fv(uniform_location("view"), 1, GL_FALSE, glm::value_ptr(V));
		glUniformMatrix4fv(uniform_location("proj"), 1, GL_FALSE, glm::value_ptr(P));
		draw_mesh();
		unbind_shader();

		Context::swap_buffers();
	}

	cout << "All seems to be fine :)" << endl;
	return 0;
}
